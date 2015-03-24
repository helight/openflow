// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:David<daijinwei41gmailcom>
// Created:     2014-11-25 
// Modified:    2015-02-03 
// Description: httpserver main
//

#include <stdio.h>
#include "httpserver.h"

using namespace openflow::httpserver;

const std::string kUsage = "Usage: httpserver [start|stop|restart]\r\n" \
                            "--Options--:\r\n"                          \
                            "\tstart\tstart httpserver\r\n"             \
                            "\tstop\tstop httpserver\r\n"               \
                            "\trestart\trestart httpserver\r\n"         \
                            "\tpause\tpause httpserver\r\n"             \
                            "\tresume\tresume httpserver\r\n";

void display_usage()
{
    fprintf(stdout,"%s\n", kUsage.c_str());
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        display_usage();
        return -1;  
    }

    boost::shared_ptr<OHttpServer> server(new OHttpServer());
    server->serve();

    return 0;
}
