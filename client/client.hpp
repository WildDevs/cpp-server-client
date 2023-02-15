
#pragma once
#include <winsock2.h>
#include <string>
#include <ostream>
#include <Ws2tcpip.h>
#include <iostream>
using namespace std;

class Client {
    private:
        SOCKET connectSocket;
        WSADATA wsaData;

    public:
        SOCKET &getConnectSocket();
        void setConnectSocket(SOCKET const &connectSocket);

        WSADATA &getWSAData();

        Client();
        ~Client();

        void connectToServer();
        void sendMessage(string const &message);
        void disconnectFromServer();
};