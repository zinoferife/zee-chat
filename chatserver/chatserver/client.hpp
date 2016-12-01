//
//  client.hpp
//  chatserver
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
    Client();
    ~Client();
    
    void connecting();
    
private:
    int sock_fd;
    int clino;
    std::string message;
    std::string name;
    struct sockaddr_in6 servaddr;

};


#endif /* client_hpp */
