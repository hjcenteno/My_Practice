#!/usr/bin/env python3
"""
Live plot of the STM32 + MPU6050 attitude stream (lightweight version).

Frame format (matches client_transmit on the MCU):
    0xAA | length | payload[length] | checksum
    checksum = 0xAA ^ length ^ payload[0] ^ ... ^ payload[length-1]

Payloads used here (little-endian floats):
    28 bytes -> cal_mpu6050_t : accX accY accZ temp gyroX gyroY gyroZ
     8 bytes -> orientation   : roll pitch   (radians)
Other lengths (whoAmI, raw struct) are ignored.

Performance notes:
  * The x axis is "seconds ago" (-window .. 0), so the axes never change and
    matplotlib can blit: only the 6 lines are redrawn each frame, not the
    whole figure (ticks, labels, legends, grid).
  * Y limits are fixed (see --angle-range / --rate-range) instead of
    autoscaling every frame.
  * Redraw rate is --fps (default 10); serial data is still read and logged
    in full between redraws.

Usage:
    python3 attitude_plot.py
    python3 attitude_plot.py --port /dev/ttyACM1 --baud 115200 --window 30
    python3 attitude_plot.py --csv run3.csv --fps 15 --angle-range 180
"""
import argparse
import csv
import math
import struct
import time
from collections import deque

import matplotlib.pyplot as plt
import numpy as np
import serial
from matplotlib.animation import FuncAnimation

START = 0xAA
CAL_LEN = 28
ANGLES_LEN = 8


def parse_frames(buf: bytearray):
    """Remove every complete, checksum-valid frame from buf and return the payloads."""
    frames = []
    while True:
        i = buf.find(bytes([START]))
        if i < 0:
            buf.clear()
            break
        del buf[:i]                      # drop junk before the start byte
        if len(buf) < 2:
            break
        n = buf[1]
        if len(buf) < n + 3:             # wait for the rest of the frame
            break
        payload = bytes(buf[2:2 + n])
        chk = START ^ n
        for b in payload:
            chk ^= b
        if chk == buf[2 + n]:
            frames.append(payload)
            del buf[:n + 3]
        else:
            del buf[0]                   # 0xAA inside a payload; resync
    return frames


def main():
    ap = argparse.ArgumentParser(description="Live roll/pitch plot")
    ap.add_argument("--port", default="/dev/ttyACM0")
    ap.add_argument("--baud", type=int, default=9600)
    ap.add_argument("--window", type=float, default=20.0, help="seconds of history shown")
    ap.add_argument("--fps", type=float, default=10.0, help="plot redraws per second")
    ap.add_argument("--angle-range", type=float, default=90.0, help="+/- deg for roll/pitch axes")
    ap.add_argument("--rate-range", type=float, default=250.0, help="+/- deg/s for gyro axis")
    ap.add_argument("--max-rate", type=float, default=500.0,
                    help="highest expected sample rate (Hz), sizes the history buffer")
    ap.add_argument("--csv", help="optional file to log samples to")
    args = ap.parse_args()

    ser = serial.Serial(args.port, args.baud, timeout=0)
    ser.reset_input_buffer()             # drop packets buffered before we started
    buf = bytearray()
    t0 = time.monotonic()

    log_file = None
    log = None
    if args.csv:
        log_file = open(args.csv, "w", newline="")
        log = csv.writer(log_file)
        log.writerow(["t_s", "roll_deg", "pitch_deg", "acc_roll_deg", "acc_pitch_deg",
                      "gyroX_dps", "gyroY_dps", "accX_g", "accY_g", "accZ_g"])

    # Only keep what fits in the visible window.
    maxlen = int(args.window * args.max_rate) + 1
    t = deque(maxlen=maxlen)
    roll, pitch = deque(maxlen=maxlen), deque(maxlen=maxlen)
    acc_roll, acc_pitch = deque(maxlen=maxlen), deque(maxlen=maxlen)
    gx, gy = deque(maxlen=maxlen), deque(maxlen=maxlen)
    latest_cal = None                    # cal packet arrives just before the angles packet

    fig, (ax_r, ax_p, ax_g) = plt.subplots(3, 1, sharex=True, figsize=(10, 8))
    fig.suptitle("MPU6050 complementary filter")

    l_roll, = ax_r.plot([], [], label="filtered roll", animated=True)
    l_aroll, = ax_r.plot([], [], label="accel-only roll", alpha=0.5, animated=True)
    ax_r.set_ylabel("roll (deg)")
    ax_r.set_ylim(-args.angle_range, args.angle_range)

    l_pitch, = ax_p.plot([], [], label="filtered pitch", animated=True)
    l_apitch, = ax_p.plot([], [], label="accel-only pitch", alpha=0.5, animated=True)
    ax_p.set_ylabel("pitch (deg)")
    ax_p.set_ylim(-args.angle_range, args.angle_range)

    l_gx, = ax_g.plot([], [], label="gyroX (roll rate)", animated=True)
    l_gy, = ax_g.plot([], [], label="gyroY (pitch rate)", animated=True)
    ax_g.set_ylabel("rate (deg/s)")
    ax_g.set_ylim(-args.rate_range, args.rate_range)
    ax_g.set_xlabel("seconds ago")
    ax_g.set_xlim(-args.window, 0.0)

    for a in (ax_r, ax_p, ax_g):
        a.grid(True, alpha=0.3)
        a.legend(loc="upper left", fontsize=8)

    lines = (l_roll, l_aroll, l_pitch, l_apitch, l_gx, l_gy)
    series = (roll, acc_roll, pitch, acc_pitch, gx, gy)

    def update(_frame):
        nonlocal latest_cal
        waiting = ser.in_waiting
        if waiting:
            buf.extend(ser.read(waiting))

        for payload in parse_frames(buf):
            if len(payload) == CAL_LEN:
                latest_cal = struct.unpack("<7f", payload)
            elif len(payload) == ANGLES_LEN and latest_cal is not None:
                r, p = struct.unpack("<2f", payload)
                axg, ayg, azg, _temp, gxd, gyd, _gzd = latest_cal
                # same formulas as calculate_roll / calculate_pitch on the MCU
                ar = math.atan2(ayg, math.sqrt(axg * axg + azg * azg))
                ap_ = math.atan2(-axg, math.sqrt(ayg * ayg + azg * azg))

                now = time.monotonic() - t0
                t.append(now)
                roll.append(math.degrees(r))
                pitch.append(math.degrees(p))
                acc_roll.append(math.degrees(ar))
                acc_pitch.append(math.degrees(ap_))
                gx.append(gxd)
                gy.append(gyd)

                if log:
                    log.writerow([f"{now:.3f}", roll[-1], pitch[-1], acc_roll[-1],
                                  acc_pitch[-1], gxd, gyd, axg, ayg, azg])

        if t:
            # x = seconds ago, so the axes stay fixed and blitting works
            x = np.fromiter(t, float, len(t)) - (time.monotonic() - t0)
            for line, data in zip(lines, series):
                line.set_data(x, np.fromiter(data, float, len(data)))
        return lines

    anim = FuncAnimation(fig, update, interval=1000.0 / args.fps,  # noqa: F841
                         blit=True, cache_frame_data=False)
    try:
        plt.tight_layout()
        plt.show()
    finally:
        ser.close()
        if log_file:
            log_file.close()


if __name__ == "__main__":
    main()