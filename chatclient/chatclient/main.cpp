//
//  main.cpp
//  chatclient
//
//  Created by ferife zino  on 24/11/2016.
//  Copyright © 2016 practice. All rights reserved.
//

#include <iostream>
#include <vector>
#include "client.hpp"
void  d_error(int err)
{

    printf("pipe error, %d \n",err);
}


int main(int argc, const char * argv[]) {
  
    signal(SIGPIPE, d_error);
    Client client(argc, argv);
    
    (*d_error)(3);
    printf("%p \n",d_error);
    
    
    return 0;
}
