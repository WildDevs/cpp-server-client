#ifndef __SERVER__HPP__
#define __SERVER__HPP__

#include <winsock2.h>
#include <string>
#include <ostream>
#include <Ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

class Client;

class Server {
    private:
        double version;
        string name;
        string ip;
        u_short port;
        int maxQueue;
        SOCKET listenSocket;
        SOCKADDR_IN serverAddr;
        WSADATA wsaData;

        vector<Client *> clients;

        void clientThread(Client *client);

    public:
        double const &getVersion() const;
        void setVersion(double const &version);

        string const &getName() const;
        void setName(string const &name);

        string const &getIP() const;
        void setIP(string const &ip);

        u_short const &getPort() const;
        void setPort(int const &port);

        int const &getMaxQueue() const;
        void setMaxQueue(int const &maxQueue);

        SOCKET &getListenSocket();
        void setListenSocket(SOCKET const &listenSocket);

        vector<Client *> &getClients();
        void setClients(vector<Client *> const clients);

        SOCKADDR_IN &getServerAddr();
        WSADATA &getWSAData();

        Server();
        Server(string const &name, double const &version, string const &ip, int const &port);
        ~Server();

        operator string();

        void start();
        void stop();
};

inline ostream &operator<<(ostream &os, Server &s){
    return (os << static_cast<string>(s));
}

#endif  //!__SERVER__H__