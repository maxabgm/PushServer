#ifndef REMOTE_CLIENT_H
#define REMOTE_CLIENT_H

#include <QDebug>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utility.h"

class Remote_Client
{
public:
    Remote_Client(std::string s_ip);
    bool set_ip(std::string s_ip);
    void send_msg(std::string msg);

private:
    std::string str_ip;
};

#endif // REMOTE_CLIENT_H
