// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver implemetation of Oserver
//

#ifndef _OPENFLOW_HTTPSERVER_H_
#define _OPENFLOW_HTTPSERVER_H_

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include "server.h" 

namespace openflow { namespace httpserver{

//using namespace openflow::httpserver::transport;
//using namespace openflow::httpserver::application;

// Define the the path length
const uint32_t kHttpserverPathSize = 128;

// Recive the message, buffer Size
const uint32_t kBufferSize = 4096;

// Set the http recive timeout
const uint32_t kTimeOut = 120;

// Set the http server listening address
const std::string kHttpAddress = "0.0.0.0";

// Set the http http listening port
const uint32_t kHttpPort = 8080;

/**
 * Implementation of Oserver;
 */
class OHttpServer : public OServer 
{
 public:
  // Default constructor 
  OHttpServer(){}

  // Constructor
  virtual ~OHttpServer()
  {
    stop(); 
  }

  virtual void serve();
  virtual void stop()
  {
	stop_ = true;	
  }

protected:
	bool stop_;
};

}}//openflow::httpserver

#endif //: #ifndef _OPENFLOW_HTTPSERVER_H_ ///:~
