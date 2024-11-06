import customtkinter

customtkinter.set_appearance_mode("system")
customtkinter.set_default_color_theme("dark-blue")

var = 0
global counter

def increment():
    global var
    var = var + 1
    counter.configure(text = var)

root =  customtkinter.CTk()
root.geometry("300x300")

frame = customtkinter.CTkFrame(master=root)

label = customtkinter.CTkLabel(master=frame, text="My GUI", font=("Tahome", 24))

counter = customtkinter.CTkLabel(master=frame, text=var, font=("Tahoma", 24))
counter.pack()

button = customtkinter.CTkButton(master = frame, text="press me", command=increment)

frame.pack(pady = 20, padx = 60, fill = "both", expand = True)
label.pack(pady = 10, padx = 20)
button.pack(pady=12, padx=20)
root.mainloop()
