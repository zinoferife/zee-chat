//
//  client.cpp
//  chatserver
//
//  Created by ferife zino  on 24/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#include "client.hpp"


Client::Client()
{
    sock_fd = socket(AF_INET6,SOCK_STREAM,0);
    message = "new member";
    // build server to conncet to
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(TCP_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &servaddr.sin6_addr);
    
  
    connect(sock_fd,(sockaddr *)&servaddr, sizeof(servaddr));
    
    Header header;
    header.types = REQUST;
    header.clino = 0;
    header.seqno = 0;
    header.size = 0;
    header.name = "zino";
    header.peer = "\0";
    
    
    struct message mess;
    mess.header = header;
    
    mess.mbuff = message;
    write(sock_fd, &mess, sizeof(mess));
    

}



Client::~Client()
{
    close(sock_fd);
}