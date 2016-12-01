//
//  inetheader.h
//  chatserver
//
//  Created by ferife zino  on 24/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#ifndef inetheader_h
#define inetheader_h


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include  <string>
#include  <map>
#include <cstdio>
#include <signal.h>
#include <iostream>


#define SERVER_ADDR "2a02:c7d:69e6:3800:c56:d48e:ddb0:68ae"
#define TCP_PORT 9997
#define MAXBUFFLEN 1024
enum Type {BROADCAST = 1,CHAT = 2 ,REQUST =3,EXIT =4 ,MESSAGE =5, WELCOME = 6} ;
enum error { WRITE = 1, READ = 2};
struct Eserver_status
{
    int clifd;
    int errorno;
    std::string errormessage;
};


struct Header
{
    int types;
    int seqno;
    int clino;
    ssize_t size;
    std::string name;
    std::string peer;
};

void print_error_stat(int num);



struct message
{
    
    Header header;
    std::string mbuff;
    
};



#endif /* inetheader_h */
