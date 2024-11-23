"""
Password Generator GUI
Author@ Henry Centeno
Purpose: Make a gui of Password_Generator.py
"""

import tkinter as tk
import customtkinter as ctk
import random as rdm

#setup customtkinter
ctk.set_default_color_theme("dark-blue")
ctk.set_appearance_mode("system")

#create root of the gui
root = ctk.CTk()
root.geometry("500x450") 

TIMEOUT = 120000 #two minutes/120 milliseconds
inactivityTimeout_id = None #used to cancel previous timer 

#global variables
letters = tk.BooleanVar(value=True)
special = tk.BooleanVar(value=False)
numbers = tk.BooleanVar(value=False)

def reset_timer(event=None):
    global inactivityTimeout_id
    if(inactivityTimeout_id != None):
        root.after_cancel(inactivityTimeout_id) #cancel previous timer to allow for another 2 min

    inactivityTimeout_id = root.after(TIMEOUT, quitGUI) #set new timer

#quit program after inactivity
def quitGUI():
    root.quit()

def getLength(value):
    #update the label with the current slider value
    lengthLabel.configure(text=f"Length: {int(float(value))}")

#get the values of the checkboxes and ensure at least one box is checked
def getCheckBoxValues():
    passwordLength = int(pwLength.get()) 
    checkLetters = letters.get()
    checkSpecial = special.get()
    checkNumbers = numbers.get()

    #ensure at least one option is selected
    if(not checkLetters and not checkNumbers and not checkSpecial):
        passwordLabel.configure(text="Please select at least one option.")
        return

    #generate password from given parameters
    password = generatePW(passwordLength, checkLetters, checkNumbers, checkSpecial)
    passwordLabel.configure(text=password)


#generate a password character by character
def generatePW(length, useLetters, useNumbers, useSpecial):
    newPassword = "" # initialize empty string

    #create a list of enabled character types
    toss = []
    
    if(useLetters):
        toss.append("letters")

    if(useNumbers):
        toss.append("numbers")

    if(useSpecial):
        toss.append("special")
    
    for i in range(length):
        #randomly select a character type from the enabled options
        choice = rdm.choice(toss)
        
        if(choice == "letters"):
            if(rdm.randint(0, 1) == 0):  #uppercase letters
                currentChar = chr(rdm.randint(65, 90))

            else:  #lowercase letters
                currentChar = chr(rdm.randint(97, 122))

        elif(choice == "numbers"):
            currentChar = chr(rdm.randint(48, 57)) #numbers 0 - 9

        elif(choice == "special"): #separated due to ascii table
            sChar = rdm.randint(0, 3) 
            if(sChar == 0):
                currentChar = chr(rdm.randint(33, 47))
            
            if(sChar == 1):
                currentChar = chr(rdm.randint(58, 64))

            if(sChar == 2):
                currentChar = chr(rdm.randint(91, 96))

            if(sChar == 3):
                currentChar = chr(rdm.randint(123, 126))
        
        newPassword += currentChar

    return newPassword

def setup(r):
    #initialize global variables
    global lengthLabel, pwLength, passwordLabel
    
    #set up the main frame of the application
    frame = ctk.CTkFrame(master=root)
    frame.pack(pady=20, padx=20, fill="both", expand=True)  #allow the frame to expand

    label = ctk.CTkLabel(master=frame, text="Password Generator", font=("Tahoma", 24))
    label.pack(pady=10, padx=12)

    #slider frame
    sliderFrame = ctk.CTkFrame(master=frame)
    sliderFrame.pack(pady=12, padx=10, fill="x")

    #add fromLabel to the left side of the slider
    fromLabel = ctk.CTkLabel(master=sliderFrame, text="8", font=("Tahoma", 15))
    fromLabel.grid(row=0, column=0, padx=(0, 10))  #add padding to right

    #add the slider in the middle
    pwLength = ctk.CTkSlider(master=sliderFrame, from_=8, to=128, command=getLength)
    pwLength.set(8)  #set initial slider value
    pwLength.grid(row=0, column=1, sticky="ew")  #fill horizontally

    #add toLabel to the right side of the slider
    toLabel = ctk.CTkLabel(master=sliderFrame, text="128", font=("Tahoma", 15))
    toLabel.grid(row=0, column=2, padx=(10, 0))  #add padding to left

    #frame to output the length
    lengthFrame = ctk.CTkFrame(master=frame)
    lengthFrame.pack(pady=10)

    lengthLabel = ctk.CTkLabel(master=lengthFrame, text="Length: 8", font=("Tahoma", 12))  #start with initial length 8
    lengthLabel.pack(side="left")

    #configure the middle column to expand with the window
    sliderFrame.grid_columnconfigure(1, weight=1)

    #checkboxes frame
    checkboxFrame = ctk.CTkFrame(master=frame)
    checkboxFrame.pack(pady=20, padx=10, fill="x")

    #add checkboxes for password options
    lettersCheckbox = ctk.CTkCheckBox(master=checkboxFrame, text="Letters (A-Z, a-z)", variable=letters)
    lettersCheckbox.grid(row=0, column=0, padx=10, sticky="w")

    specialCharCheckbox = ctk.CTkCheckBox(master=checkboxFrame, text="Special Characters", variable=special)
    specialCharCheckbox.grid(row=0, column=2, padx=10, sticky="w")

    numbersCheckbox = ctk.CTkCheckBox(master=checkboxFrame, text="Numbers", variable=numbers)
    numbersCheckbox.grid(row=0, column=1, padx=10, sticky="w")

    #make columns in checkbox frame stretch
    checkboxFrame.grid_columnconfigure(0, weight=1)
    checkboxFrame.grid_columnconfigure(1, weight=1)
    checkboxFrame.grid_columnconfigure(2, weight=1)

    #button to generate the password
    generateButton = ctk.CTkButton(master=frame, text="Generate", command=getCheckBoxValues)
    generateButton.pack(pady=10)

    #label to display the generated password
    passwordLabel = ctk.CTkLabel(master=frame, text="", font=("Tahoma", 16), wraplength=420, anchor="center") 
    passwordLabel.pack(pady=20)

    #bind events to reset the inactivity timer on user activity
    root.bind("<Any-KeyPress>", reset_timer)
    root.bind("<Any-Button>", reset_timer)
    root.bind("<Motion>", reset_timer)

def main():
    setup(root)
    reset_timer()
    root.mainloop()

main()