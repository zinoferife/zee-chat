//
//  main.cpp
//  chatserver
//
//  Created by ferife zino  on 21/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#include <iostream>
#include "Server.hpp"
#include "client.hpp"
int main(int argc, const char * argv[]) {
 
    Server server;
    server.waitforconnections();
    
    return 0;
}

