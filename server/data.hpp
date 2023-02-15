#pragma once
#include <string>
#include <Ws2tcpip.h>
using namespace std;
class Data {
    private:
        char *buffer;
        char *cmd;
        char *context;

    public:
        char *getBuffer();
        void setBuffer(char *buffer);
        
        const char *getCmd() const;
        void setCmd(char *cmd);

        const char *getContext() const;
        void setContext(char *context);

        Data();
        Data(char *buffer);
        Data(Data &) = delete;
        Data(Data &&) = delete;
        ~Data();

        void parse();
};