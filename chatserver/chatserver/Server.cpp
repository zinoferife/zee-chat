//
//  Server.cpp
//  chatserver
//
//  Created by ferife zino  on 21/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#include "Server.hpp"

#ifdef _LIBCPP_FUNCTIONAL
#undef _LIBCPP_FUNCTIONAL
#endif



void catch_erro(int n)
{
    
    printf("the system call was rejected \n");
}


pid_t pp = getpid();

Server::Server()
: sock_fd(0)
{
    // number used as index in to the vector of client fd
    num = 0;
    clino.reserve(10);
    clientfd.reserve(10);
   if( (sock_fd = socket(AF_INET,SOCK_STREAM,0) ) < 0)
   {
       perror("Server constructor : cannot open socekt \n");
   }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TCP_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
   int b =  bind(sock_fd,(sockaddr *)&servaddr, sizeof(servaddr));
    if(b < 0)
    {
        perror("Server contructor : cannot bind to socket \n");
        
    }
    
    //this bind function is acting up, dosent want to let me check for errors

    std::cout << pp << "\n";
    signal(SIGINT, catch_erro);
    
    
    printf("printing..\n");
}




int Server::waitforconnections()
{
    
    printf("listening....\n");
    
    std::cout  << "printing..."  << std::endl;
    
    if(listen(sock_fd, 5) < 0)
    {
        perror("Server constructor : listen error \n");
        return (-1);
    }
    
    for(;;)
    {
        
        //no client number 0
        clino[num] = num + 1;
    
        printf("waiting for conncetions.. \n");
        clilen = sizeof(cliaddr);
        if((curfd =  accept(sock_fd, (sockaddr *)&cliaddr, &clilen)) < 0)
        {
            perror("Waitforconncetions : cannot accept conncetions \n");
            return (-1);
        }
        
        clientfd[clino[num]] = curfd;
        clisocktoaddr[clientfd[clino[num]]] = cliaddr;
        
        
        if(clientfd[num] < 0)
        {
            perror("waitforconnections: accept error \n");
            return (-1);
        }
        
       // char buff[] = "zino is on your computer";
        //write(fd_,buff, sizeof(buff));
        
        //fork child
            if(do_service() < 0)
                {
                    perror("waitforconnections: cannot do service, !!FATAL ERROR!! \n");
                    _exit(-1);
                }
                  //if i am going to use kids then they must be some interprocessing communication
        num++;
    
    }
    \
    
    

    return (0);
}


int Server::do_service()
{

while(true)
{
    recv_message();
    //this cast may not be correct to fix, when revising the code
    type =  (Type)recvmess.header.types;
    switch (type) {
        case BROADCAST:
            do_broadcast(&recvmess);
            break;
        case CHAT:
            do_chat(&recvmess);
            break;
        case REQUST:
            do_request(&recvmess);
            break;
        case EXIT:
            do_exit(&recvmess);
            break;
        case MESSAGE:
            do_message(&recvmess);
            break;
        default:
            perror("do_service: no valid type \n");
            return (-1);
    }
 }
    return(0);
}

void Server::do_exit(struct message *mess)
{
    //clean up and remove the client from the chat;
    int fd;
    clino[mess->header.clino - 1] = 0;
    fd = clientfd[mess->header.clino];
    clisocktoaddr.erase(mess->header.clino);
    close(fd);
    do_broadcast(mess);
}


void Server::do_broadcast(struct message *mess)
{
    size_t size = clientfd.size();
    size_t n; int i;
    while(size != 0)
    {
        n = write(clientfd[i], (char*)mess,sizeof(*mess));
        
        if(n != sizeof(*mess))
        {
            perror("do_broadcast : didnt write complete message");
        }
        
        i++;
        size--;
    }

}

void Server::do_chat(struct message *mess)
{
    
    // any extra code would be placed here ....
    do_broadcast(mess);
    
}


void Server::do_request(struct message *mess)
{
    // 1, extract the name from the header
    // 2, place the naem in the map memeber
    // 3, send a reply to the client saying it has been added
    // if the server is to be expanded then this function would be the heart the server
    
    Header header = mess->header;
    std::string name = header.name;
    clinames[clino[num]] = name; // on the fact that the first message is all was request
    
    //buld the reply message
    message * mess_ = new message;
    mess_->header.types = REQUST;
    mess_->header.size = 0;
    mess_->header.clino = clino[num];
    
    std::string buff = "you have been added";
    mess_->mbuff = buff;
    
     size_t n = write(clientfd[mess_->header.clino], mess_, sizeof(*mess_));
    if(n != sizeof(*mess_))
    {
        perror("do_request : cannot send reply on request");
    }
    
    delete mess_;
}

void Server::do_message(struct message *mess)
{
    std::string retrunstring;
    std::map<int,std::string>::iterator iter;
    
    std::cout << "< " << mess->header.name << " > " << mess->mbuff << std::endl;
    
    for(iter = clinames.begin(); iter!= clinames.end(); iter++)
    {
        if(iter->second == mess->header.peer)
        break;
    }
    
    int clino_ = iter->first;
    if(clino_ == 0)
    {
        retrunstring = " !!! sorry but the client you want to talk to is offline !!!";
        sendmess.mbuff = retrunstring;
        curfd = clientfd[mess->header.clino];
        send_message();
    }
        
        
    curfd = clientfd[mess->header.clino];
    peerfd = clientfd[clino_];
    
    if(peerfd)
    {
        message *mess_ = new message;
    
        mess_->header.types = MESSAGE;
        mess->mbuff = mess_->mbuff;
        write(peerfd,mess_, sizeof(mess_));
    
        delete mess_;
    }
}

Server::~Server()
{
    close(sock_fd);
}


void Server::send_message()
{
    size_t wr;
    if(curfd != 0)
    {
        if((wr = write(curfd, &sendmess, sizeof(sendmess))) != sizeof(sendmess))
        {
            err = WRITE;
            perror("send_message : cannot send message");
        }
    }
}

void Server::recv_message()
{
    size_t rn;
    if(curfd != 0)
    {
        if((rn = read(curfd, &recvmess, sizeof(recvmess))) < sizeof(recvmess))
        {
            err = READ;
            perror("recv_message : cannot recvive message");
        }
    }
}
