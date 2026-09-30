#include "tcpserver.h"


void handle_signal(int signalNumber){
	std::cout<<"just got the signal , with signal number "<<signalNumber<<std::endl;
}


int main(int argc, char const *argv[])
{
	tcpserver server;
	server.start();
	return 0;
}