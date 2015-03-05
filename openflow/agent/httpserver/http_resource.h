// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-12-25
// Description:httpserver implemetation of http response 

#ifndef _OPENFLOW_HTTPRESPONSE_H_
#define _OPENFLOW_HTTPRESPONSE_H_

#include <string>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

namespace openflow{ namespace httpserver{

const int kHttpserverPathSize = 128;
const std::string httpserver_resource_path = "";

/**
 * The httpserver resource path.
 * We can assgin the path, for example /usr/local/httpserver
 */
//std::string httpserver_resource_path = "";

std::string get_http_resource_path();

std::string get_http_resources(const std::string resource_path);

}} //:end namespace openflow httpserver

#endif//: #ifndef _OPENFLOW_HTTPRESPONSE_H_ ///:~
