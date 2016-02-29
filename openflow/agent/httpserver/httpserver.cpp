// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:      David<daijinwei41@gmail.com>
// Created:     2014-11-25
// Modified:    2014-12-25
// Description:httpserver interface

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <exception>
#include <event.h>
#include <evhttp.h>
#include "httpserver.h"
#include "http_handle.h"
#include "http_common.h"

namespace openflow { namespace httpserver {

void OHttpServer::serve()
{
    struct event_base *base;
    struct evhttp *http_server;

    // Create a event
    base = event_base_new();
    if (!base){
        GlobalOutput.printf("OHttpServer::serve() can not create a base event");
        return;
    }

    // Create a http  event
    http_server = evhttp_new(base);
    if(!http_server){
        GlobalOutput.printf("OHttpServer::serve() can not create a http event");
        return; 
    }

    evhttp_set_gencb(http_server, http_handle, NULL);
    evhttp_set_timeout(http_server, kTimeOut);

    // Select listening address and port
    int ret = evhttp_bind_socket(http_server, kHttpAddress.c_str(), kHttpPort);
    if(0 != ret)
    {
        int32_t num = ret;
        GlobalOutput.perror("OHttpServer::serve() can not bind a address to socket",num);
        return;
    }

    // event while
    event_base_dispatch(base);
    evhttp_free(http_server);
    event_base_free(base);
}

}}// end namespace openflow::httpserver ///:~
