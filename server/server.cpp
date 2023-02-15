#include "server.hpp"
#include "client.hpp"
double const &Server::getVersion() const {
    return this->version;
}

void Server::setVersion(double const &version){
    this->version = version;
}

string const &Server::getName() const{
    return this->name;
}

void Server::setName(string const &name){
    this->name = name;
}

string const &Server::getIP() const{
    return this->ip;
}

void Server::setIP(string const &ip){
    this->ip = ip;
}

u_short const &Server::getPort() const{
    return this->port;
}

void Server::setPort(int const &port){
    this->port = htons(port);
}

int const &Server::getMaxQueue() const{
    return this->maxQueue;
}

void Server::setMaxQueue(int const &maxQueue){
    this->maxQueue = maxQueue;
}

SOCKET &Server::getListenSocket(){
    return this->listenSocket;
}

void Server::setListenSocket(SOCKET const &listenSocket){
    this->listenSocket = listenSocket;
}

vector<Client *> &Server::getClients(){
    return this->clients;
}
void Server::setClients(vector<Client *> const clients){
    this->clients = clients;
}

SOCKADDR_IN &Server::getServerAddr(){
    return this->serverAddr;
}

WSADATA &Server::getWSAData(){
    return wsaData;
}

Server::Server(): Server("Test-Server", 1.0, "127.0.0.1", 7070){}

Server::Server(string const &name, double const &version, string const &ip, int const &port){
    this->setVersion(version);
    this->setName(name);
    this->setIP(ip);
    this->setPort(port);
    this->setMaxQueue(5);
}

Server::~Server(){
    this->stop();
}

Server::operator string(){
    return this->getName() + ", v" + to_string(this->getVersion());
}

void Server::start(){
    cout << "Server is starting..." << endl;
    if(WSAStartup(MAKEWORD(2,2),(LPWSADATA)&(this->getWSAData())) != 0){
        cout << "WSAStartup() Error: " << WSAGetLastError() << endl;
        exit(1);
    }

    memset(&serverAddr,0,sizeof(SOCKADDR_IN));

    this->getServerAddr().sin_family = AF_INET;
    this->getServerAddr().sin_port = this->getPort();
    this->getServerAddr().sin_addr.s_addr = inet_addr(this->getIP().c_str());
    this->setListenSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

    if(this->getListenSocket() == INVALID_SOCKET) {
        cout << "listenSocket Error: " << WSAGetLastError()  << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    cout << "Listen socket created..." << endl;

    if(bind(this->getListenSocket(),(SOCKADDR*)&(this->getServerAddr()),sizeof(SOCKADDR_IN)) == SOCKET_ERROR){
        cout << "bind() Error: " << WSAGetLastError()  << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    cout << "IP & Port binded..." << endl;

    if(listen(this->getListenSocket(), this->getMaxQueue()) == SOCKET_ERROR){
        cout << "listen() Error: " << WSAGetLastError()  << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    cout << "Server started and listen now!" << endl;
    cout << "Waiting for clients..." << endl;

    for(;;){
        Client *client = new Client();
        int size = sizeof(client->getSocketInfo());
        
        SOCKET clientSocket = accept(this->getListenSocket(), (sockaddr *)&client->getSocketInfo(), &size);
        cout << "New incoming connection..." << endl;

        if(clientSocket == INVALID_SOCKET){
            cout << "socket accept() Error: " << WSAGetLastError() << endl;
            continue;
        }

        cout << "Socket is valid, starting client thread..." << endl;
        this->getClients().push_back(client);

        client->setSocket(clientSocket);
        client->setWorkThread(new thread(&Client::startWorkThread, client, this));
    }
}


void Server::stop(){
    for(auto it = this->getClients().begin(); it != this->getClients().end(); it++){
        delete *it;
    }

    closesocket(this->getListenSocket());
    WSACleanup();
}