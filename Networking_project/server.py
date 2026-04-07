import sys
import os
import random
import string
import threading
from datetime import datetime
from socket import *

#fixed server IP and port
SERVER_IP = "0.0.0.0"
SERVER_PORT = 12001

#lock for safe multi-threaded access to clients dictionary
clients_lock = threading.Lock()
clients = {}  #username -> socket

#FEATURE 1: Broadcast Function
def broadcast(sender, msg):
    to_remove = []      #clients to remove if sending fails
    #safely access shared clients
    with clients_lock:
        for username, client_socket in clients.items():
            #if username == sender:
                #continue    #skip sender
            try:
                client_socket.sendall((msg + "\n").encode())    #send message
            except:
                to_remove.append(username)  #client to remove

    #remove disconnected clients
    for username in to_remove:
        remove_client(username)

#FEATURE 2: One-to-one private messaging
def send_private(sender, target, msg):
    #safely access shared clients
    with clients_lock:
        client_socket = clients.get(target)    #recieving socket
    if client_socket:
        try:
            client_socket.sendall(f"[{sender} -> you]: {msg}\n".encode())
            return True     #message sent
        except:
            #remove if sending fails
            remove_client(target)
    return False    #message not sent

#HELPER FUNCTION: Remove client on disconnect
def remove_client(username):
    #safely access shared clients
    with clients_lock:
        client_socket = clients.pop(username, None)  #remove client if exists
    if client_socket:
        try:
            client_socket.close()    #close the client's socket
        except:
            pass

        #inform remaining clients about disconnection
        broadcast(None, f"[SERVER] {username} disconnected.")

#THREAD FUNCTION: Handle each client
def client_handler(client_socket, addr):
    #request username from client
    client_socket.send("Enter username: ".encode())
    username = client_socket.recv(1024).decode().strip()
    #close connection if no username provided
    if not username:
        client_socket.close()
        return

    #check uniqueness and add to clients dictionary
    with clients_lock:
        #close connection if username not unique
        if username in clients:
            client_socket.send("USERNAME_TAKEN\n".encode())
            client_socket.close()
            return
        #add to clients dictionary
        clients[username] = client_socket
    print(f"{username} connected from {addr}")

    #inform client of commands
    client_socket.send(f"[SERVER] Welcome {username}! Use @username msg for private messages, /list, /quit.\n".encode())
    #broadcast join message
    broadcast(username, f"[SERVER] {username} joined the chat.")

    #recieve messages from client
    while True:
        data = client_socket.recv(1024)
        if not data:
            break   #client disconnected
        msg = data.decode().strip()
        if not msg:
            continue    #ignore empty messages

        #client requested disconnect
        if msg == "/quit":
            client_socket.send("[SERVER] Goodbye!\n".encode())
            break   #client disconnected
        
        #send list of connected users
        if msg == "/list":
            with clients_lock:
                connected_users = ', '.join(clients.keys())
            client_socket.send(f"[SERVER] Online users: {connected_users}\n".encode())
            continue    #back to waiting for next message

        #FEATURE 2: private message
        if msg.startswith("@"):
            split_msg = msg.split(" ", 1)
            if len(split_msg) < 2:
                client_socket.send("[SERVER] Invalid format. Use: @username message\n".encode())
                continue

            target_user = split_msg[0][1:]
            message = split_msg[1]

            if target_user == username:
                client_socket.send("[SERVER] Cannot send message to yourself.\n".encode())
                continue
            #send direct message
            if send_private(username, target_user, message):
                client_socket.send(f"[you -> {target_user}]: {message}\n".encode())
            else:
                client_socket.send(f"[SERVER] User {target_user} not found.\n".encode())
        else:
            #FEATURE 1: broadcast message
            timestamp = datetime.now().strftime("%H:%M:%S")
            broadcast(username, f"[{timestamp}] [{username}]: {msg}")

    #remove client and close connection
    remove_client(username)
    print(f"{username} disconnected.")

#main server logic
def main():
    #create socket and bind
    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind((SERVER_IP, SERVER_PORT))
    server_socket.listen(5)     #listen for connections

    print(f"[SERVER] Running on {SERVER_IP}:{SERVER_PORT}")

    while True:
        #accept connection
        client_sock, addr = server_socket.accept()

        #start client handler thread
        threading.Thread(target=client_handler, args=(client_sock, addr), daemon=True).start()

if __name__ == "__main__":
    main()
