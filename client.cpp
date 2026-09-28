#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "DES.h"
using namespace std;

int main (){
    string key = "496E666F5365630A"; //InfoSec

    int sock;
    sockaddr_in server_addr{};

    char buffer[4096];
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        cerr<<"Failed to create socket"<<endl;
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    string server_ip  ="192.168.121.128";

    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0){
        cerr<<"Invalid VM2 IP address"<<endl;
        return 1;
    }

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cerr<<"Failed to connect to the VM2"<<endl;
        return 1;
    }
    cout << "Connected to VM2"<<endl<<endl;

    while (true){
        while (true){ //send mode
            string msg;
            cout<<"VM1: ";
            getline(cin, msg);

            string ct;
            try{
                ct = encryptmsg(msg, key);
            }
            catch (exception& error){
                cerr<<"Encryption failed: "<<error.what()<<endl;
                close(sock);
                return 1;
            }
            cout<<"Ciphertext sent: "<<ct<<endl<<endl;

            send(sock, ct.c_str(), ct.size(), 0);

            if (msg == "EXIT"){
                cout<<"Communication ended"<<endl;
                close(sock);
                return 0;
            }
            if (msg == "CHANGE"){
                cout<<"Turn changed to VM2"<<endl<<endl;
                break;
            }
        }
        while (true){ //receive mode
            memset(buffer, 0, sizeof(buffer));

            int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);

            if (bytes_received <= 0){
                cout<<"VM2 disconnected"<<endl;
                cout<<"Communication ended"<<endl;
                close(sock);
                return 0;
            }

            buffer[bytes_received] = '\0';

            string received_ct = buffer;

            cout<<"Ciphertext received: "<<received_ct<<endl;

            string pt;
            try{
                pt = decryptmsg(received_ct, key);
            }
            catch (exception& error){
                cerr<<"Decryption failed: "<<error.what()<<endl;
                close(sock);
                return 1;
            }
            cout<<"Message from VM2: "<<pt<<endl;

            if (pt == "EXIT"){
                cout<<"VM2 ended the communication"<<endl;
                close(sock);
                return 0;
            }
            if (pt == "CHANGE"){
                cout<<"Turn changed to VM1"<<endl<<endl;
                break;
            }
            cout<<endl;
        }
    }
    close(sock);
    return 0;
}