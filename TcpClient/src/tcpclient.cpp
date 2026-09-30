#include <tcpclient.h>


tcpclient::tcpclient():Clientfh(-1){
	Clientfh = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
}

tcpclient::~tcpclient(){
    if(Clientfh != -1){
        close(Clientfh);
    }
}

void tcpclient::start(){
    spdlog::info("Tcp Client started ");
	if(connect(Clientfh, (struct sockaddr*)&serverAddress,sizeof(serverAddress)) != 0){
        spdlog::debug("Tcp client connection failed");
        return;
    }
    

   
    while(true){
        char buffer[1024];
        memset(buffer,'\0',1024);

        std::string command;
        std::cout<<"Enter the linux command "<<std::endl;
        std::getline(std::cin,command);
    	spdlog::info("writing the shh command {}",command);
        send(Clientfh,command.c_str(),command.size(),0);

        recv(Clientfh,buffer,1024,0);
        spdlog::info("Recieved message from server {}",buffer);
    }
}
