#include <iostream>
#include <string>
#include "client.hpp"

using namespace std;

int main(int argc, char** argv){ 
    Client *c = new Client();
    c->connectToServer();
    for(;;){
        string msg;
        getline(cin, msg);
        c->sendMessage(msg);
    }
    getchar();
    c->disconnectFromServer();
    return 0;
}