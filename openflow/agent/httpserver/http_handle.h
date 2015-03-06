// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:      David<daijinwei41@gmail.com>
// Created:     2014-11-25
// Modified:    2015-01-25
// Description: Implemetation of http Handle 
// handle http request, get http resource and response to client
//

#ifndef _OPENFLOW_HTTPHANDLE_H_
#define _OPENFLOW_HTTPHANDLE_H_

#include <string>
#include <cstring>
#include <boost/noncopyable.hpp>

#define SERVER_INFO "http server test 0.1"
namespace openflow{ namespace httpserver{

/**
 * Handle http request, than get httpserver resources 
 * and send resources to http client
 * Args:
 *  request: A http request struct, include request infomation
 *  arg:
 * Return: void
 **/
void http_handle(struct evhttp_request *request, void *arg);

}}//: end namespace openflow::httpserver

#endif//: #ifndef _OPENFLOW_HTTPHANDLE_H_ ///:~
