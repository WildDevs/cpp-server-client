#include "data.hpp"
char *Data::getBuffer(){
    return this->buffer;
}

void Data::setBuffer(char *buffer){
    this->buffer = buffer;
}

const char *Data::getCmd() const{
    return this->cmd;
}

void Data::setCmd(char *cmd){
    this->cmd = cmd;
}

const char *Data::getContext() const{
    return this->context;
}

void Data::setContext(char *context){
    this->context = context;
}

Data::Data(): Data(new char[0]){}

Data::Data(char *buffer){
    this->setBuffer(buffer);
    this->parse();
}

Data::~Data(){
    delete this->getBuffer();
    delete this->getCmd();
    delete this->getContext();
}

void Data::parse(){
    char *token = strtok(this->getBuffer(), "\x01");

    if(token != NULL){
        char *cmd = new char[strlen(token)];
        strcpy(cmd, token);
        this->setCmd(token);

        if((token = strtok(NULL, "\x01")) != NULL){
            char *context = new char[strlen(token)];
            strcpy(context, token);
            this->setContext(context);
        }
    }
    //delete token;
}