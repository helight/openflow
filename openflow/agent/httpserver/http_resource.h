// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:      David<daijinwei41@gmail.com>
// Created:     2014-12-25
// Modified:    2014-01-25
// Description: Define get http resource function

#ifndef _OPENFLOW_HTTPRESOURCE_H_
#define _OPENFLOW_HTTPRESOURCE_H_

#include <string>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

namespace openflow{ namespace httpserver{

/* Define http server resource path string max size */
const int kHttpserverPathSize = 128;

/**
 * The httpserver resource path.
 * We can assgin the path, for example /usr/local/httpserver
 */
const std::string httpserver_resource_path = "";

/* Return http resource path */
std::string get_http_resource_path();

/* Get http resource */
std::string get_http_resources(const std::string resource_path);

}} //:end namespace openflow httpserver

#endif//: #ifndef _OPENFLOW_HTTPRESOURCE_H_ ///:~
