import tkinter as tk
import customtkinter as ctk
import random as rdm

# Setup customtkinter
ctk.set_default_color_theme("dark-blue")
ctk.set_appearance_mode("system")

# Create root of the gui and run it
root = ctk.CTk()
root.geometry("500x450")  # Adjust the initial size of the window

def getLength(value):
    # Update the label with the current slider value
    lengthLabel.configure(text=f"Length: {int(float(value))}")

# Updated generate function using the logic you provided
def generate():
    password_length = int(pwLength.get())
    checkLetters = letters.get()
    checkSpecial = special.get()
    checkNumbers = numbers.get()

    # Ensure at least one option is selected
    if not checkLetters and not checkNumbers and not checkSpecial:
        password_label.configure(text="Please select at least one option.")
        return

    # Generate the password using the provided logic
    password = generatePW(password_length, checkLetters, checkNumbers, checkSpecial)
    password_label.configure(text=password)

# Function to generate password using your updated logic
def generatePW(n, useLetters, useNumbers, useSpecial):
    newPassword = ""
    
    for i in range(n):
        # Create a list of enabled character types
        available_choices = []
        
        if useLetters:
            available_choices.append('letters')
        if useNumbers:
            available_choices.append('numbers')
        if useSpecial:
            available_choices.append('special')
        
        # Randomly select a character type from the enabled options
        choice = rdm.choice(available_choices)
        
        if choice == 'letters':
            if rdm.randint(0, 1) == 0:  # uppercase letters
                currentChar = chr(rdm.randint(65, 90))
            else:  # lowercase letters
                currentChar = chr(rdm.randint(97, 122))

        elif choice == 'numbers':
            currentChar = chr(rdm.randint(48, 57))  # numbers

        elif choice == 'special':
            currentChar = chr(rdm.randint(33, 47))  # special characters
        
        newPassword += currentChar

    return newPassword

def setup(r):
    global lengthLabel, pwLength, password_label
    
    # Set up the main frame of the application
    frame = ctk.CTkFrame(master=root)
    frame.pack(pady=20, padx=20, fill="both", expand=True)  # Allow the frame to expand

    label = ctk.CTkLabel(master=frame, text="Password Generator", font=("Tahoma", 24))
    label.pack(pady=10, padx=12)

    # Slider frame
    sliderFrame = ctk.CTkFrame(master=frame)
    sliderFrame.pack(pady=12, padx=10, fill="x")

    # Add fromLabel to the left side of the slider
    fromLabel = ctk.CTkLabel(master=sliderFrame, text="8", font=("Tahoma", 15))
    fromLabel.grid(row=0, column=0, padx=(0, 10))  # Add padding to right

    # Add the slider in the middle
    pwLength = ctk.CTkSlider(master=sliderFrame, from_=8, to=128, command=getLength)
    pwLength.set(8)  # Set initial slider value
    pwLength.grid(row=0, column=1, sticky="ew")  # Fill horizontally

    # Add toLabel to the right side of the slider
    toLabel = ctk.CTkLabel(master=sliderFrame, text="128", font=("Tahoma", 15))
    toLabel.grid(row=0, column=2, padx=(10, 0))  # Add padding to left

    # Frame to output the length
    lengthFrame = ctk.CTkFrame(master=frame)
    lengthFrame.pack(pady=10)

    lengthLabel = ctk.CTkLabel(master=lengthFrame, text="Length: 8", font=("Tahoma", 12))  # Start with initial length 8
    lengthLabel.pack(side="left")

    # Configure the middle column to expand with the window
    sliderFrame.grid_columnconfigure(1, weight=1)

    # Create a frame for the checkboxes
    checkboxFrame = ctk.CTkFrame(master=frame)
    checkboxFrame.pack(pady=20, padx=10, fill="x")

    # Variables to store the selected password options
    global special, numbers, letters
    letters = tk.BooleanVar(value=True)  # Default is True for letters
    special = tk.BooleanVar(value=False)
    numbers = tk.BooleanVar(value=False)

    # Add checkboxes for password options
    lettersCheckbox = ctk.CTkCheckBox(master=checkboxFrame, text="Letters (A-Z, a-z)", variable=letters)
    lettersCheckbox.grid(row=0, column=2, padx=10, sticky="w")

    specialCharCheckbox = ctk.CTkCheckBox(master=checkboxFrame, text="Special Characters", variable=special)
    specialCharCheckbox.grid(row=0, column=0, padx=10, sticky="w")

    numbersCheckbox = ctk.CTkCheckBox(master=checkboxFrame, text="Numbers", variable=numbers)
    numbersCheckbox.grid(row=0, column=1, padx=10, sticky="w")

    # Make columns in checkbox frame stretch
    checkboxFrame.grid_columnconfigure(0, weight=1)
    checkboxFrame.grid_columnconfigure(1, weight=1)
    checkboxFrame.grid_columnconfigure(2, weight=1)

    # Button to generate the password
    generateButton = ctk.CTkButton(master=frame, text="Generate", command=generate)
    generateButton.pack(pady=10)

    # Label to display the generated password
    global password_label
    password_label = ctk.CTkLabel(master=frame, text="", font=("Tahoma", 16))
    password_label.pack(pady=20)

def main():
    setup(root)
    root.mainloop()

main()
