#ifndef __CLIENT__HPP__
#define __CLIENT__HPP__

#include <winsock2.h>
#include <string>
#include <ostream>
#include <Ws2tcpip.h>
#include <iostream>
#include <thread>
#include "data.hpp"
#define MSG_BUFFER_SIZE 4096
using namespace std;

class Server;

class Client {
    private:
        SOCKET socket;
        sockaddr_in socketInfo;
        string name;
        string ip;
        u_short port;
        thread *workThread;

    public:
        const SOCKET &getSocket() const;
        void setSocket(SOCKET const &socket);

        const sockaddr_in &getSocketInfo();

        const string &getName() const;
        void setName(string const &name);

        const string &getIP() const;
        void setIP(string const &ip);

        const u_short &getPort() const;
        void setPort(int const &port);

        thread *getWorkThread();
        void setWorkThread(thread *workThread);

        Client();
        Client(const Client&) = delete;
        Client(Client&&) = delete;
        Client(SOCKET const &socket);
        ~Client();

        operator string();
        const Client &operator=(const Client&) = delete;
        const Client &operator=(Client&&) = delete;

        void startWorkThread(Server *server);
};

inline ostream &operator<<(ostream &os, Client &client){
    return (os << static_cast<string>(client));
}

#endif