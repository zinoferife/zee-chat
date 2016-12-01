//
//  client.hpp
//  chatclient
//
//  Created by ferife zino  on 24/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#ifndef client_hpp
#define client_hpp

#include "inetheader.h"

class Client
{
    
public:
    Client(int argc, const char * argv[]);
    ~Client();
    
    void ack(struct message *mess);
    void do_request(struct message *mess);
    int  do_service();
    void do_message();
    void do_broadcast(struct message *mess);
    void do_exit(struct message *mess);
    void connecting(struct message *mess);
    int parse_message();
    int srflag;
    Type type;
private:
    int sock_fd;
    int clino;
    string message;
    vector<string> messagelist;
    string name;
    struct sockaddr_in servaddr;
    struct hostent* host;
    int send_message();
    int recv_message();
    string peername;
    struct message recvmess;
    struct message sendmess;
};


#endif /* client_hpp */
