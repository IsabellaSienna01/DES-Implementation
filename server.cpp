#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "DES.h"
using namespace std;

int main (){
    string key = "496E666F5365630A"; //InfoSec

    int server_fd;
    int client_socket;

    sockaddr_in address{};

    int addr = sizeof(address);
    char buffer[4096];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0){
        cerr << "Failed to create socket"<<endl;
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&address, addr) < 0){
        cerr<<"Failed to bind socket"<<endl;
        return 1;
    }
    if (listen(server_fd, 3) < 0){
        cerr<<"Failed to listen for connections"<<endl;
        return 1;
    }
    
    cout<<"Waiting connection from VM1"<<endl;

    client_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)& addr);
   
    if (client_socket < 0){
        cerr<<"Failed to accept VM1 connection"<<endl;
        return 1;
    }
    cout<<"VM1 connected successfully"<<endl<<endl;

    while (true){
        while (true){ //receive mode
            memset(buffer, 0, sizeof(buffer));

            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

            if (bytes_received <= 0){
                cout<<"VM1 disconnected"<<endl;
                close(client_socket);
                close(server_fd);
                return 0;
            }

            buffer[bytes_received] = '\0';

            string ct = buffer;

            cout<<"Ciphertext received: "<<ct<<endl;

            string pt;
            try{
                pt = decryptmsg(ct, key);
            }
            catch (exception& error){
                cerr<<"Decryption failed: "<<error.what()<<endl;
                close(client_socket);
                close(server_fd);
                return 1;
            }
            cout<<"Message from VM1: "<<pt<<endl<<endl;

            if (pt == "EXIT"){
                cout<<"VM1 ended the communication"<<endl;
                close(client_socket);
                close(server_fd);
                return 0;
            }
            if (pt == "CHANGE"){
                cout<<"Turn changed to VM2"<<endl<<endl;
                break;
            }
        }

        while (true){ //send mode
            string reply;
            cout<<"VM2: ";
            getline(cin, reply);
                
            string encrypted_reply;
                
            try{
                encrypted_reply = encryptmsg(reply, key);
            }
            catch(exception& error){
                cerr<<"Encryption failed: "<<error.what()<<endl;
                close(client_socket);
                close(server_fd);
                return 1;
            }

            cout<<"Ciphertext sent: "<<encrypted_reply<<endl<<endl;

            send(client_socket, encrypted_reply.c_str(), encrypted_reply.size(), 0);

            if (reply == "EXIT"){
                cout<<"Communication ended"<<endl;
                close(client_socket);
                close(server_fd);
                return 0;
            }
            if (reply == "CHANGE"){
                cout<<"Turn changed to VM1"<<endl<<endl;
                break;
            }
        }
    }

    close(client_socket);
    close(server_fd);
    return 0;
}