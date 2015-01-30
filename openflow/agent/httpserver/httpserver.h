// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver implemetation of Oserver

#ifndef _OPENFLOW_HTTPSERVER_H_
#define _OPENFLOW_HTTPSERVER_H_

#include <boost/shared_ptr.hpp>
#include "transport/server_socket.h"
#include "application/http_request.h"
#include "application/http_response.h"
#include "server.h"

namespace openflow { namespace httpserver{

using namespace openflow::httpserver::transport;
using namespace openflow::httpserver::application;

// Define the the path length
const uint32_t kHttpserverPathSize = 128;

// Recive the message, buffer Size
const uint32_t kBufferSize = 4096;

/**
 * Implementation of Oserver;
 *
 */
class OHttpServer : public OServer 
{
 public:
  // Default constructor 
  OHttpServer(){
  	stop();
  }


  // Constructor
  OHttpServer(
    const boost::shared_ptr<OServerSocket> sock,
	const boost::shared_ptr<ORequest>  &request,
	const boost::shared_ptr<OResponse> &response): 
	OServer(sock, request, response),
   	stop_(false){}

  virtual ~OHttpServer(){}
  virtual void serve();
  virtual void stop()
  {
	stop_ = true;	
  }

  // Destructor
protected:
	bool stop_;
	std::string httpserver_path_;
private:
	void setHttpserverPath();
	std::string getHttpserverPath() const;
};

}}//openflow::httpserver

#endif //: #ifndef _OPENFLOW_HTTPSERVER_H_ ///:~
