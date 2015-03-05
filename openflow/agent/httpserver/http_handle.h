// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver implemetation of http request
//

#ifndef _OPENFLOW_HTTPREQUEST_H_
#define _OPENFLOW_HTTPREQUEST_H_

#include <string>
#include <cstring>
#include <boost/noncopyable.hpp>

#define SERVER_INFO "http server test 0.1"
namespace openflow{ namespace httpserver{


void http_handle(struct evhttp_request *request, void *arg);

}}//: end namespace openflow::httpserver

#endif//: _OPENFLOW_HTTPREQUEST_H_
