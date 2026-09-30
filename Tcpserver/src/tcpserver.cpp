#include "tcpserver.h"

tcpserver::tcpserver():opt(1){
	serverfh = socket(AF_INET, SOCK_STREAM, 0);
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;


    if(setsockopt(serverfh,SOL_SOCKET,SO_REUSEADDR| SO_REUSEPORT,&opt,sizeof(opt))){
        spdlog::debug("Socket set option failed ");
    }

    bind(serverfh, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        spdlog::debug("epoll_create failed ");
    }

    spdlog::info("Server socket is listening");
    //listen(serverfh, SOMAXCONN);
    listen(serverfh,5);

   epollthread = std::thread(&tcpserver::epollLoop,this);
}

tcpserver::~tcpserver(){
}

void tcpserver::handleRecv(int fd){
    char buffer[1024];
    memset(buffer,'\0',1024);

    int bytesRead =  recv(fd,buffer,1024,0);

    if(bytesRead == 0){
            spdlog::info("Recv recived for closing the socket , number of bytes read {}",bytesRead);
            epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
            close(fd);
            return;
    }else if(bytesRead > 0){
            spdlog::info("Client data recieved : {}",buffer);
            std::string output;
            char outputBuffer[1024];
            FILE *ptr;
            ptr = popen(buffer,"r");

            if(ptr == NULL){
                spdlog::info("Linux command failed ");
            }else{
                spdlog::info("Linux command Sucess ");
            }

            while(fgets(outputBuffer,1024,ptr)){
                output += outputBuffer;
            }

            if(output.size() < 1){
                output = "Command failed ";
            }
            
            spdlog::info("Command output {} ",output.c_str());
            send(fd,output.c_str(),output.size(),0);
            spdlog::info("message send succefully -> {} ",output);
            return;
    }else if(bytesRead < 0){
            spdlog::info("Recv recived for negative data , number of bytes read {}",bytesRead);
            if(errno == EWOULDBLOCK || errno == EAGAIN){
                spdlog::info("No problem with the data , but resource is not available for the time being");
                return;
            }else{
                spdlog::info("Issue with data recieved, closing the fd");
                epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
                close(fd);
                return;
            }
            spdlog::info("Recv recived with errno {}",errno);
    } 
}
void tcpserver::handleSend(int fd){
        p.push( [&](int id){ send(fd, SetDataClient().c_str(), strlen(SetDataClient().c_str()), 0);} );
}

void tcpserver::epollLoop(){
    struct epoll_event events[MAX_EVENTS];
    while(true){
        int event_count = epoll_wait(epollfd,events,MAX_EVENTS,-1); // is a blocking call 
        spdlog::info("Epoll event recieved {}",event_count);
        for(int i=0;i<event_count;++i){
            int fd  = events[i].data.fd;
            if(events[i].events & EPOLLIN){
                spdlog::info("Epoll in event recieved ");
                handleRecv(events[i].data.fd);
            }else if(events[i].events & EPOLLOUT){
                spdlog::info("Epoll out event recieved ");
            }else{
                spdlog::info("Epoll in edge event recieved");
            }
        }
    }
}

int tcpserver::getServerfh(){return serverfh;}

std::string tcpserver::SetDataClient(){
    std::string body = "Hello from my TCP server!";

    std::string response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: " + std::to_string(body.size()) + "\r\n"
    "Connection: close\r\n"
    "\r\n" +
    body;
    spdlog::info("Server message send ");
    return response;
}

bool tcpserver::setSocketNonBlocking(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        spdlog::error("fcntl F_GETFL failed: {}", strerror(errno));
        return false;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        spdlog::error("fcntl F_SETFL failed: {}", strerror(errno));
        return false;
    }
    return true;
}

void tcpserver::start(){
	while(true){
        spdlog::info("Client connection recieved with thread id ");
		int clientSocket= accept(serverfh, nullptr, nullptr);

        if(!setSocketNonBlocking(clientSocket)){
            spdlog::info("Client fd setting to non blocking failed ");
        }

        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLET; //ET - edge triggers
        ev.data.fd = clientSocket;
        epoll_ctl(epollfd,EPOLL_CTL_ADD,clientSocket,&ev);

	}
}