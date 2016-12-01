//
//  client.cpp
//  chatclient
//
//  Created by ferife zino  on 24/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//


#include "client.hpp"
#include <stdlib.h>

Client::Client(int argc , const char *argv[])
: clino(0), srflag(0)
{
    if((sock_fd = socket(AF_INET,SOCK_STREAM,0))< 0)
    {
        printf("Client: cannot open socket %d \n", sock_fd);
    }
    const char str[] = "ferifes-MacBook-Air.local";
    host = gethostbyname(str);
    message = "new member";
    
    in_addr * ptr;
    ptr  = (in_addr *)*host->h_addr_list;
    
    
    
    printf("%s \n",inet_ntoa(*ptr));
    // build server to conncet to
    
    bzero(&servaddr, sizeof(servaddr));
    switch(host->h_addrtype)
    {
        case AF_INET:
    servaddr.sin_family = host->h_addrtype;
    servaddr.sin_port = htons(TCP_PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
            break;
            
        default:
            printf("VERSION 6 NOT SUPPORTED");
    }
            int conn;
    if((conn = connect(sock_fd,(sockaddr *)&servaddr, sizeof(servaddr))) < 0)
    {
        printf("CLient : cannot connect to server at %s \n", inet_ntoa(*ptr));
        return;
        
    }
    
    string list;
 
    
    
    while(true)
    {
        if(clino == 0)
        {
            message = "\0";
        }
        if(do_service() < 0)
        {
            perror("Client :cannot do service");
            return;
        }
        
    }
    
}



Client::~Client()
{
    close(sock_fd);
}


int Client::do_service()
{
ssize_t wr;
struct message mess;
type = (Type)0;

if(clino == 0)
{
    do_request(&sendmess);
    wr = write(sock_fd, &sendmess, sizeof(sendmess));
    if(wr  != sizeof(sendmess))
    {
        perror("do_service : cannot send request to server");
    }
    if(recv_message() < 0)
    {
        perror("cannot recive from server");
    }
    ack(&recvmess);
}
    
//how do i send and rev messages with out blocking
 

    send_message();
    
    do_message();
  
    
    
    return 0;
    

}

void Client::do_request(struct message *mess)
{
    Header header;
    header.types = REQUST;
    header.clino = 0;
    header.seqno = 0;
    header.size = 0;
    header.name = "zino";
    header.peer = "\0";
    mess->header = header;
    name = "zino";
    mess->mbuff = message;
}


void Client::ack(struct message *mess)
{
    
    clino = mess->header.clino;
    std::cout <<  clino << endl;
    std::cout << mess->mbuff << endl;
}



void Client::do_message()
{
    recv_message();
    //parse the recived message;
    switch(recvmess.header.types)
    {
        case MESSAGE:
            cout << recvmess.mbuff;
            break;
        case CHAT:
            cout << recvmess.mbuff;
            break;
    }
    
}


void Client::do_broadcast(struct message *mess)
{
    
}


void Client::do_exit(struct message *mess)
{
    
    
    
}

int Client::parse_message()
{
    if(message == "\0")
    {
        perror("Parse message: cannot parse an empty string");
        return (-1);
    }
    string mm;
    for(int i = 0; i < message.length(); i++)
    {
        if(message[i] == ' ')
        {
            messagelist.push_back(mm);
            mm = "\0";
            
        }else
            mm += message[i];
    }
    
    messagelist.push_back(mm);
    
    return (0);
}




int Client::send_message()
{
    struct Header header;
    size_t wr;
    getline(cin,message);
    parse_message();
    
    if(clino  != 0)
    {
        switch(messagelist[0][0])
        {
            case 'm':
                type = MESSAGE;
                srflag = 2;
                break;
            case 'b':
                type = BROADCAST;
                break;
            case 'c':
                type = CHAT;
                break;
            case 'e':
                type = EXIT;
                break;
            default:
                perror("no valid request");
                return (-1);
        }
    }
    
    
    header.types = type;
    header.size = sizeof(message);
    header.seqno = 0;
    header.clino = clino;
    if(type == CHAT)
    {
        peername = messagelist[1];
        header.peer = peername;
    }
    else
        header.peer = "\0";

    header.name = name;
    sendmess.header = header;
    string mm;
    // this routine assume that the peer name is not in the sentence
    for(int i = 1; i < messagelist.size(); i++)
    {
        mm += messagelist[i] + " ";
    
    }
    
    sendmess.mbuff = mm;
    if(( wr = write(sock_fd, &sendmess, sizeof(sendmess))) != sizeof(sendmess))
    {
        return (-1);
    }
        return (0);
}


//i have to make the client not block on reading, using treads
int Client::recv_message()
{
    size_t rn;
    bzero(&recvmess, sizeof(recvmess));
    if((rn = read(sock_fd, &recvmess, sizeof(recvmess)) < sizeof(recvmess)))
    {
        return (-1);
    }
    if(recvmess.header.types == MESSAGE)
    {
        srflag = 1;
    }
    return (0);
}

