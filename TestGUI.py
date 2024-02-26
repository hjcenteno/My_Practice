#first attempt at making a gui in python (2023-13-06)
import customtkinter

customtkinter.set_appearance_mode("system")
customtkinter.set_default_color_theme("dark-blue")

root=customtkinter.CTk()
root.geometry("500x350")

onDark=True
print(onDark)

def login():
    print("test")

def changeAppearance():
    global onDark
    if(onDark==True):
        customtkinter.set_appearance_mode("light")
        onDark=False
    else:
        customtkinter.set_appearance_mode("dark")
        onDark=True

#frame of the gui
frame=customtkinter.CTkFrame(master=root) #places frame in root
frame.pack(pady=20, padx=60, fill="both", expand=True) #the pack function actually places the wanted object into the master

label=customtkinter.CTkLabel(master=frame, text="Login System", font=("Tahoma", 24))
label.pack(pady=10, padx=12)

entry_USN=customtkinter.CTkEntry(master=frame, placeholder_text="username")
entry_USN.pack(pady=12, padx=10)

entry_pass=customtkinter.CTkEntry(master=frame, placeholder_text="password", show="*") #typing would show * on the gui
entry_pass.pack(pady=12, padx=10)

button=customtkinter.CTkButton(master=frame, text="login", command=login)
button.pack(pady=12, padx=10)

checkbox=customtkinter.CTkButton(master=frame, text="remember me")
checkbox.pack(pady=12, padx=10)

appearance=customtkinter.CTkButton(master=frame, text="change appearnce", command=changeAppearance)
appearance.pack(pady=12, padx=10)

root.mainloop() #creates the root

