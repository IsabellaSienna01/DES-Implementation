# Data Encryption Standard (DES) Implementation


This project is an implementation of the **Data Encryption Standard (DES)** algorithm in C++ combined with **two-way communication between two virtual machines using TCP sockets**.

VM1 is used as the **client** and VM2 is used as the **server**. Before a message is sent, the message is encrypted using DES. The receiver will receive the ciphertext and decrypt it back into the original message.

Both VM1 and VM2 use the same DES key, so the key does not need to be sent through the network.

---

## Project Structure

```text
.
├── client.cpp
├── server.cpp
├── DES.cpp
└── DES.h
```

- `client.cpp`  
  Used by VM1 as the client. It connects to VM2, encrypts messages before sending them, receives ciphertext, and decrypts received messages.

- `server.cpp`  
  Used by VM2 as the server. It listens on port `8080`, accepts the connection from VM1, receives encrypted messages, decrypts them, and sends encrypted replies.

- `DES.cpp`  
  Contains the DES implementation, including key generation, permutation, S-Box, encryption, decryption, and padding.

- `DES.h`  
  Contains the DES function declarations used by the client and server.

---

## Compilation

Compile the server:

```bash
g++ server.cpp DES.cpp -o server
```

Compile the client:

```bash
g++ client.cpp DES.cpp -o client
```

Make sure `DES.cpp` and `DES.h` are in the same directory as `client.cpp` and `server.cpp`.

---

## Commands

### Check VM IP Address

To check the IP address of the virtual machine:

```bash
hostname -I
```

or:

```bash
ip addr
```

### Check Connection Between VMs

From VM1, check whether VM2 can be reached:

```bash
ping <VM2_IP_ADDRESS>
```

Example:

```bash
ping 192.168.121.128
```

### Set Server IP Address

Update the VM2 IP address in `client.cpp` if needed:

```cpp
string server_ip = "192.168.121.128";
```

### Run the Program

Run the server on VM2 first:

```bash
./server
```

Then run the client on VM1:

```bash
./client
```

---

## Special Commands

There are two commands that can be used during the communication:

### `CHANGE`

`CHANGE` is used to switch the sender.

For example, initially VM1 is the sender:

```text
VM1: Hello VM2
VM1: How are you?
VM1: CHANGE
```

After `CHANGE` is sent, VM1 changes to receive mode and VM2 can start sending messages.

Example on VM2:

```text
Message from VM1: Hello VM2
Message from VM1: How are you?
Message from VM1: CHANGE

Turn changed to VM2

VM2: Hello VM1
VM2: I'm fine
```

If VM2 wants to give the sending turn back to VM1:

```text
VM2: CHANGE
```

Then VM1 will become the sender again.

Example:

```text
VM1 sends messages
        ↓
     CHANGE
        ↓
VM2 sends messages
        ↓
     CHANGE
        ↓
VM1 sends messages again
```

### `EXIT`

`EXIT` is used to end the communication.

For example:

```text
VM1: EXIT
```

After the message is sent, the connection will be closed.

The same command can also be used by VM2:

```text
VM2: EXIT
```

When either VM sends `EXIT`, the communication between the client and server ends.