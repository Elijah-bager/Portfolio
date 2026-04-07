import sys
import os
import random
import string
import threading
from socket import *
import tkinter as tk
from tkinter import scrolledtext, simpledialog

SERVER_IP = "172.20.10.2"
SERVER_PORT = 12001

#create TCP client Socket
client_socket = socket(AF_INET, SOCK_STREAM)

#connect to the server using the server's IP address and port
client_socket.connect((SERVER_IP, SERVER_PORT))

#tkinter GUI setup
root = tk.Tk()
root.title("Python Chat Client")
root.geometry("600x450")
root.minsize(500, 350)

#send username
username = simpledialog.askstring("Username", "Enter your username:", parent=root)
client_socket.send(username.encode())
root.title(f"Python Chat - {username}")

#styles and fonts used
font_main = ("Segoe UI", 11)
font_small = ("Segoe UI", 10)
font_server = ("Segoe UI", 10, "bold")

#chat display area
frame_chat = tk.Frame(root)
frame_chat.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

chat_area = scrolledtext.ScrolledText(
    frame_chat,
    wrap=tk.WORD,
    state="disabled",
    font=font_main,
    bg="#1e1e1e",
    fg="white",
    insertbackground="white"
)
chat_area.pack(fill=tk.BOTH, expand=True)

#tag styles for colors
chat_area.tag_config("server", foreground="#4ea5ff", font=font_server)
chat_area.tag_config("you", foreground="#98ff98")
chat_area.tag_config("other", foreground="white")

#bottom frame for entry + send button
bottom_frame = tk.Frame(root)
bottom_frame.pack(fill=tk.X, padx=10, pady=(0,10))

#entry box (expands horizontally)
msg_entry = tk.Entry(bottom_frame, font=font_main)
msg_entry.pack(side=tk.LEFT, expand=True, fill=tk.X, padx=(0, 10))
msg_entry.focus()

#send button
def send_msg():
    msg = msg_entry.get().strip()
    if msg:
        client_socket.send(msg.encode())
        msg_entry.delete(0, tk.END)
        if msg == "/quit":
            root.destroy()
            
send_button = tk.Button(bottom_frame, text="Send", font=font_small, command=send_msg, width=10)
send_button.pack(side=tk.RIGHT)

#recieve messages from the server and display received messages to the user
def continuously_receive_messages():
    while True:
        try:
            msg = client_socket.recv(1024)
            if not msg:
                chat_area.configure(state="normal")
                chat_area.insert(tk.END, "Server closed connection.")
                chat_area.configure(state="disabled")
                break
            
            #determine tag (server, others)
            decoded = msg.decode()
            if decoded.startswith("[SERVER]"):
                tag = "server"
            elif "-> you" in decoded:
                tag = "other"
            else:
                tag = "other"

            chat_area.configure(state="normal")
            chat_area.insert(tk.END, msg.decode(), tag)
            chat_area.configure(state="disabled")
            chat_area.yview(tk.END)
        except:
            break
    client_socket.close()

#start a background thread to continuously recieve messages
threading.Thread(target=continuously_receive_messages, daemon=True).start()

#bind Enter key to send message
root.bind('<Return>', lambda event: send_msg())

#start the GUI loop
root.mainloop()
