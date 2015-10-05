//
//  jasl.cpp
//  jasl
//
//  Created by Ben Jones on 10/03/2015.
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#include "CommandInterpretor.hpp"
#include "VarCache.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

int main (int argc , char *argv[])
{

    /* initialize random seed: */
    srand (time(NULL));

    namespace ll = jasl;
    ll::CommandInterpretor ci;

    if(argc <= 1) {
        std::cout<<"Error: please provide a jasl script"<<std::endl;
        exit(0);
    }  

    // Store how many arguments there are
    // in special reserved integer, 'argsCount'
    ll::VarCache::setInt("argsCount", (argc - 2));

    // store any command line arguments that should also be interpreted
    if(argc >= 2) {
        for(int i = 2; i < argc; ++i) {
            ll::VarCache::args.push_back(std::string(argv[i]));
        }
    }

    // parse input file
    auto functions = ci.parseCommandFile(argv[1]);
    for(auto &f : functions) {
        if(f.name == "start") {
            (void)ci.interpretFunc(f, std::cout);
            break;
        }
    }
}
