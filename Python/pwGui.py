"""
author 
"""

import tkinter as tk
import customtkinter as ctk
import random as rdm

#setup customtkinter
ctk.set_default_color_theme("dark-blue")
ctk.set_appearance_mode("system")

#create root of the gui and run it
root = ctk.CTk()
root.geometry("500x350")

def getLength(value):
    lengthLabel.configure(text=f"Length: {int(value)}")  # Update the global lengthLabel

def setup(r):
    global lengthLabel  # Declare lengthLabel as global so it can be accessed in getLength
    # set up the gui of the application
    frame = ctk.CTkFrame(master=root)
    frame.pack(pady=20, padx=60, fill="both", expand=True)

    label = ctk.CTkLabel(master=frame, text="Password Generator", font=("Tahoma", 24))
    label.pack(pady=10, padx=12)

    #slider frames for the slider and labels
    sliderFrame = ctk.CTkFrame(master=frame)
    sliderFrame.pack(pady=12, padx=10, fill="x")

    # Add fromLabel to the left side of the slider
    fromLabel = ctk.CTkLabel(master=sliderFrame, text="8", font=("Tahoma", 15))
    fromLabel.grid(row=0, column=0, padx=(0, 10))  # add padding to right

    # Add the slider in the middle
    pwLength = ctk.CTkSlider(master=sliderFrame, from_=8, to=128, command=getLength)
    pwLength.set(8)
    pwLength.grid(row=0, column=1, sticky="ew")  # fill horizontally

    # Add toLabel to the right side of the slider
    toLabel = ctk.CTkLabel(master=sliderFrame, text="128", font=("Tahoma", 15))
    toLabel.grid(row=0, column=2, padx=(10, 0))  # add padding to left

    # Bind the slider to update the length label when moved
    # pwLength.bind("<Motion>", lambda event: getLength(pwLength.get()))

    #frame to output the length
    lengthFrame = ctk.CTkFrame(master=frame)
    lengthFrame.pack(pady=10)

    lengthLabel = ctk.CTkLabel(master=lengthFrame, text="Length: 8", font=("Tahoma", 12))  # Start with initial length 8
    lengthLabel.pack(side="left")

    # Configure the middle column to expand with the window
    sliderFrame.grid_columnconfigure(1, weight=1)

    #create frame for checkboxes (digits, letters, special characters)
    # checkboxFrame


def main():
    setup(root)
    root.mainloop()

main()
