#include "client.hpp"
#include "server.hpp"
const SOCKET &Client::getSocket() const{
    return this->socket;
}
void Client::setSocket(SOCKET const &socket){
    this->socket = socket;
}

const sockaddr_in &Client::getSocketInfo(){
    return this->socketInfo;
}

const string &Client::getName() const{
    return this->name;
}

void Client::setName(string const &name){
    this->name = name;    
}

const string &Client::getIP() const{
    return this->ip;
}

void Client::setIP(string const &ip){
    this->ip = ip;
}

const u_short &Client::getPort() const{
    return this->port;
}

void Client::setPort(int const &port){
    this->port = htons(port);
}

thread *Client::getWorkThread(){
    return this->workThread;
}

void Client::setWorkThread(thread *workThread){
    this->workThread = workThread;
}

Client::Client(): Client(0){}

Client::Client(SOCKET const &socket){
    this->setSocket(socket);
    this->setName("");
    this->setIP("0.0.0.0");
    this->setPort(0);
}

Client::~Client(){
    closesocket(this->getSocket());
    this->getWorkThread()->join();
    delete this->getWorkThread();
}

Client::operator string(){
    return this->getIP() + " (" + to_string(this->getPort()) + ")";
}

void Client::startWorkThread(Server *server){
    char *clientIpAddress = inet_ntoa(this->getSocketInfo().sin_addr);
    int clientPort = ntohs(this->getSocketInfo().sin_port); 

    if(strcmp("0", clientIpAddress) == 0 || clientPort == 0){
        cout << "No valid ip address and/or port! Disconnecting..." << endl;
        closesocket(this->getSocket());
        return;
    }

    this->setIP(clientIpAddress);
    this->setPort(clientPort);

    cout << "[New Connection] " << clientIpAddress << " (" << clientPort << ")" << endl;
    do {
        char dataBuffer[MSG_BUFFER_SIZE];
        memset((void *)dataBuffer, 0, MSG_BUFFER_SIZE);
        int result = recv(this->getSocket(), dataBuffer, sizeof(dataBuffer), 0);
        if (result > 0) {
            cout << "content of dataBuffer: " << dataBuffer << endl;
            Data *data = new Data(dataBuffer);

            if(strcmp(data->getCmd(), "su") == 0){
                string name = data->getContext();
                this->setName(name);
            }

            if(strcmp(this->getName().c_str(), "") != 0){
                if(strcmp(data->getContext(), "") != 0){
                    if(strcmp(data->getCmd(), "msg") == 0){
                        for(Client *client : server->getClients()){
                            if(client->getSocket() != this->getSocket()){
                                if(client->getSocket() != SOCKET_ERROR && client->getSocket() != INVALID_SOCKET){
                                    string msg = this->getName() + ": " + data->getContext();
                                    int sendResult = send(client->getSocket(), msg.c_str(), msg.size(), 0);
                                    if(sendResult == SOCKET_ERROR){
                                        cout << "send() Error: " << WSAGetLastError() << endl;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    string msg = "err#Empty message.";
                    int sendResult = send(this->getSocket(), msg.c_str(), msg.size(), 0);
                    if(sendResult == SOCKET_ERROR){
                        cout << "send() Error: " << WSAGetLastError() << endl;
                    }
                }
            } else {
                string msg = "err#Set a nickname first before sending any messages.";
                int sendResult = send(this->getSocket(), msg.c_str(), msg.size(), 0);
                if(sendResult == SOCKET_ERROR){
                    cout << "send() Error: " << WSAGetLastError() << endl;
                }
            }
        } else  {
            switch(WSAGetLastError()){
                case WSAECONNRESET: {
                    cout << "Connection reset by peer." << endl; break;
                }
                case WSAECONNABORTED: {
                    cout << "Software caused connection abort." << endl; break;
                }
                case WSAESHUTDOWN: {
                    cout << "The client socket has been shut down." << endl; break;
                }
                case WSAETIMEDOUT: {
                    cout << "The connection has been dropped because of a network failure or because the peer system failed to respond." << endl; break;
                }
            }

            closesocket(this->getSocket());
            this->setSocket(SOCKET_ERROR);
        }
    } while (this->getSocket() != INVALID_SOCKET && this->getSocket() != SOCKET_ERROR);

    closesocket(this->getSocket());
}