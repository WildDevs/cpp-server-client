#include "client.hpp"


SOCKET &Client::getConnectSocket(){
    return this->connectSocket;
}

void Client::setConnectSocket(SOCKET const &connectSocket){
    this->connectSocket = connectSocket;
}

WSADATA &Client::getWSAData(){
    return wsaData;
}

Client::Client(){
    
}

Client::~Client(){
    if (!WSACleanup() == 0) {
        cout << "WSACleanup failed!" << endl;
    }
}

// Server::operator string(){
//     return this->getName() + ", v" + to_string(this->getVersion());
// }

void Client::connectToServer(){
    if(WSAStartup(MAKEWORD(2,2),(LPWSADATA)&(this->getWSAData())) != 0){
        cout << "WSAStartup() Error: " << WSAGetLastError() << endl;
        exit(1);
    }

    memset(&this->getConnectSocket(),0,sizeof(SOCKADDR_IN));
    this->setConnectSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if (this->getConnectSocket() == INVALID_SOCKET) {
        cout << "socket() Error: " << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }
    SOCKADDR_IN server;
    server.sin_family = AF_INET;
    server.sin_port = htons(7070);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int result = connect(this->getConnectSocket(), (SOCKADDR*)&server, sizeof(SOCKADDR_IN));
    if (result == SOCKET_ERROR) {
        closesocket(this->getConnectSocket());
        this->setConnectSocket(INVALID_SOCKET);
    }
}

void Client::sendMessage(string const &message){
    int result = send(this->getConnectSocket(), message.c_str(), (int)strlen(message.c_str()), 0);
    cout << "Send bytes: " << result << endl;
    
    if (result == SOCKET_ERROR) {
        cout << "send() Error: " << WSAGetLastError() << endl;
        closesocket(this->getConnectSocket());
        WSACleanup();
        return;
    }
}

void Client::disconnectFromServer(){
    WSACleanup();
}