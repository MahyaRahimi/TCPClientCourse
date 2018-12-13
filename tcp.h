#ifndef TCP_H
#define TCP_H
#include <thread>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
//#include <mutex>
//#include <condition_variable>
//#include <fcntl.h>

class tcp
{
public:
    tcp(){}
    int tcp_connection();

private:
    void listening();
    void transmitting();
    std::thread listener;
    std::thread transmitter;
    int sockfd = 0,n = 0;
    char recvBuff[1024];
    char buffer[256];
    struct sockaddr_in serv_addr;
    int s; //size
};

#endif // TCP_H
