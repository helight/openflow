// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver implemetation of http request
//

#include <string.h>
#include <string>
#include <event.h>
#include <evhttp.h>
#include <boost/noncopyable.hpp>
#include "http_handle.h"
#include "http_resource.h"
#include "http_common.h"

namespace openflow{ namespace httpserver{

void http_handle(struct evhttp_request *request, void *arg)
{
    struct evbuffer *buffer = evbuffer_new();    
    
    // Get the request URI
    const char *uri = NULL;
    uri = evhttp_request_uri(request);
    evbuffer_add_printf(buffer, "uri = %s\n", uri);

    // Get http server resource
    std::string resource_path = strip(strip(get_http_resource_path()) + "/" + uri);
    int fd;
    if(0 == (fd = access(resource_path.c_str(),F_OK))){
        evbuffer_add_printf(buffer, "Task context = %s\n", 
                            get_http_resources(resource_path).c_str());
    }else{
        std::string context = "The task file not exists";
        evbuffer_add_printf(buffer, "Task context = %s\n", context.c_str());
    }
   
    // Set http response header
    evhttp_add_header(request->output_headers, "Server", SERVER_INFO);
    evhttp_add_header(request->output_headers, "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(request->output_headers, "Connection", "close");

    evhttp_send_reply(request, HTTP_OK, "OK", buffer);
    evbuffer_free(buffer);
}

}}//: end namespace openflow::httpserver::aplication
