#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include "spdlog/spdlog.h"
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>


#define BUFFER_SIZE 1024
class tcpclient
{
	int Clientfh;
	sockaddr_in serverAddress;
public:
	tcpclient();
	~tcpclient();
	void start();
};

#endif //__TCPCLIENT_H__//
