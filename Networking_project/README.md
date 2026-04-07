# Networking Project - Python Chat Application

A multi-client chat application built with Python sockets and Tkinter GUI. Supports broadcast messaging, private messaging, and real-time user management.

## Project Overview

This is a client-server chat application that allows multiple users to connect to a central server and communicate with each other. It features:

- **Broadcast Messaging**: Send messages to all connected users
- **Private Messaging**: Send direct messages to specific users using `@username` syntax
- **User Management**: View online users and handle unique usernames
- **Multi-threading**: Server handles multiple clients concurrently
- **GUI Interface**: Clean Tkinter-based chat interface for clients
- **Thread-Safe Operations**: Uses locks for safe concurrent access to shared resources

## Project Structure

```
Networking_project/
├── server.py      # Server application that manages connections and messages
├── client.py      # Client GUI application for users to chat
└── README.md      # Project documentation
```

## Requirements

- Python 3.6+
- Tkinter (usually included with Python)
- Standard library modules: `socket`, `threading`, `datetime`

## Installation

1. Clone or download the project:
```bash
git clone <repository-url>
cd Networking_project
```

2. No external dependencies needed - uses only Python standard library!

## How to Use

### Starting the Server

On the machine that will host the server:

```bash
python server.py
```

You should see:
```
[SERVER] Running on 0.0.0.0:12001
```

### Finding the Server's IP Address

Before running the client, find the server's IP address on your network:

**On macOS/Linux:**
```bash
ifconfig | grep inet
```

**On Windows:**
```bash
ipconfig
```

Look for the local IP address (usually `192.168.x.x`, `10.0.x.x`, or `172.x.x.x`)

### Connecting Clients

1. Edit `client.py` and change the `SERVER_IP` variable to match the server's IP address:
```python
SERVER_IP = "192.168.1.100"  # Replace with actual server IP
```

2. Run the client on any machine on the same network:
```bash
python client.py
```

3. Enter a username when prompted (must be unique)

4. Start chatting!

## Commands

Once connected, users can:

| Command | Description | Example |
|---------|-------------|---------|
| Regular message | Broadcast to all users | `Hello everyone!` |
| `@username message` | Send private message | `@alice Hello Alice!` |
| `/list` | View all online users | `/list` |
| `/quit` | Disconnect from server | `/quit` |

## Features Explained

### Broadcast Messaging
- All messages (except those starting with `@` or `/`) are sent to all connected users
- Messages include timestamp, sender username, and content
- Format: `[HH:MM:SS] [username]: message`

### Private Messaging
- Use `@username message` to send a private message to a specific user
- Only the recipient sees the private message
- Sender receives confirmation of delivery
- If user doesn't exist, you receive an error message

### User Management
- Usernames must be unique - duplicate usernames are rejected
- `/list` command shows all currently online users
- Server broadcasts when users join or disconnect

### Multi-threading
- Server uses one thread per client for concurrent handling
- Thread-safe dictionary access with locks prevents race conditions
- Disconnected clients are automatically removed

## Network Configuration

### For Local Network (Hotspot)

1. **Ensure both devices are on the same network:**
   - Both connected to the same WiFi hotspot, or
   - Both on the same LAN

2. **Find the server's actual IP:**
   - Run `ifconfig` on the server machine
   - Note the IP address (not 127.0.0.1 or 0.0.0.0)
   - This is what goes in the client's `SERVER_IP`

3. **Port forwarding (if needed):**
   - Port 12001 must be accessible
   - May need to configure firewall if behind a router

### Example Setup

**Server machine IP:** `192.168.1.100`
- Run: `python server.py`
- Server listens on all interfaces at port 12001

**Client machine:**
- Edit client.py: `SERVER_IP = "192.168.1.100"`
- Run: `python client.py`
- Successfully connects!

## Troubleshooting

**Connection Refused Error:**
- Verify server is running: `python server.py`
- Check server IP in client matches actual network IP
- Ensure both devices are on same network

**Username Already Taken:**
- Choose a different username
- Only one user per username at a time

**No Messages Received:**
- Check connection status (look at window title)
- Verify username was entered correctly
- Check that other users' messages appear (test with broadcast first)

## Architecture Notes

### Server (`server.py`)
- Binds to `0.0.0.0:12001` (listens on all interfaces)
- Creates thread for each client connection
- Maintains dictionary of connected users (username → socket)
- Uses locks for thread-safe access

### Client (`client.py`)
- GUI built with Tkinter
- Connects to server using configured IP and port
- Runs receiving thread to listen for messages
- Color-coded message display (blue for server, green for own, white for others)

## Potential Improvements

- Add message history/logging
- Implement user authentication/passwords
- Add file transfer capability
- Support for chat rooms/channels
- Encryption for messages
- User status indicators (typing, idle, etc.)
- Database for user persistence

## License

This project is open source and available for educational use.