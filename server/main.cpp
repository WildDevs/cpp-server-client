#include <iostream>
#include <string>
#include "server.hpp"

using namespace std;

int main(int argc, char** argv){
    Server *s = new Server();
    s->start();
    return 0;
}