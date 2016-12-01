//
//  Server.hpp
//  chatserver
//
//  Created by ferife zino  on 21/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include "inetheader.h"


static int num;
static error err;
class Server
{
    
public :
    Server();
    ~Server();
    int reciveMessage(Type type);
    int writeMessage(Type type, int who_fd);
    struct sockaddr_in servaddr;
    struct sockaddr_in  cliaddr;
    char buffer[MAXBUFFLEN];
    int waitforconnections();
    int do_service();
    void do_error_stat(struct Eserver_status * Esta);
    void do_broadcast(struct message *mess);
    void do_exit(struct message *mess);
    void do_request(struct message *mess);
    void do_chat(struct message *mess);
    void do_message(struct message *mess);
    void do_welcome(struct message *mess);
    
    void send_message();
    void recv_message();
private:

    int sock_fd;
    socklen_t  clilen;
    std::vector<int> clientfd;
    std::vector<int> clino;
    std::string address;
    std::map<int,sockaddr_in> clisocktoaddr;
    std::map<int, std::string> clinames;
    int curfd;
    int peerfd;
    struct message sendmess;
    struct message recvmess;
    Type type;
        
    
    
    


};




#endif /* Server_hpp */
