/*
    Author: Henry Centeno
    Description: 
        This header files is to allow for a broad initializing for the server to read/write from ttyACM0 from the mcu.
*/

#ifndef LPUART_SERVER_H
#define LPUART_SERVER_H

#include <inttypes.h>
#include <time.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_PAYLOAD_SIZE 80
#define MAX_PAYLOAD_LENGTH 255
#define lpuart_baud 9600
#define lse_hz 32768
#define start_byte 0xAA

//caller supplied handler since it is the responsibility of the caller to know how to interpret the bytes
typedef void (*lpuart_data_handler)(const uint8_t *data, uint8_t length);

int init_lpuart_server(void); //stuck to a baud of 9600 since the lpuart runs off the lse clock giving a maximum of 9600 for the baud
void close_lpuart_server(int fd); 

//implementation of the state machine for the server side
int run_lpuart_server(int fd, lpuart_data_handler handler); //currently a read only implementation

#endif