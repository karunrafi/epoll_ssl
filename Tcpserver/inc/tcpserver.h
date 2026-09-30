#ifndef  __TCP__SERVER__HH__
#define  __TCP__SERVER__HH__


#include "spdlog/spdlog.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include <csignal>
#include <ctpl.h>
#include <thread>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_EVENTS 10
#define MAX_THREAD 10

class tcpserver
{
	int serverfh,opt,epollfd;
	sockaddr_in serverAddress;
	struct epoll_event ev, events[MAX_EVENTS];
	ctpl::thread_pool p{MAX_THREAD};
	void epollLoop();
	bool setSocketNonBlocking(int socket);
	std::thread epollthread;
	void handleRecv(int fd);
	void handleSend(int fd);
public:
	int getServerfh();

	tcpserver();
	~tcpserver();

	void start();

	std::string SetDataClient();
	
};


#endif  // __TCP__SERVER__HH__//