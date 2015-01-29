// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author: 	David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver interface

#ifndef _OPENFLOW_SERVER_H_
#define _OPENFLOW_SERVER_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "transport/server_socket.h"
#include "application/http_request.h"
#include "application/http_response.h"

namespace openflow { namespace httpserver {

using namespace openflow::httpserver::transport;
using namespace openflow::httpserver::application;

class OServer : boost::noncopyable
{
 public:
   // Default constructor
   OServer(){}

   // Destructor
   virtual ~OServer(){}
   virtual void serve() = 0;
   virtual void stop(){}
   virtual void run()
   {
     serve();
   }

   boost::shared_ptr<OServerSocket> getSock() 
   {
     return sock_;
   }

   boost::shared_ptr<ORequest> getRequest()
   {
     return request_;
   }

   boost::shared_ptr<OResponse> getResponse() 
   {
     return response_;
   }

   //                                                                                                        
   void setSock(boost::shared_ptr<OServerSocket> sock)
   {
     sock_ = sock;
   }

   void setRequest(boost::shared_ptr<ORequest> request)
   {
     request_ = request;
   }

   void setResponse(boost::shared_ptr<OResponse> response) 
   {
     response_ = response;
   }

 protected:
   OServer(boost::shared_ptr<OServerSocket> sock,
     const boost::shared_ptr<ORequest>  &request,
	 const boost::shared_ptr<OResponse> &response):
   	 sock_(sock),
     request_(request),
     response_(response){}

   	 boost::shared_ptr<OServerSocket>    	sock_;
	 boost::shared_ptr<ORequest>     		request_;
	 boost::shared_ptr<OResponse>    		response_;
};

}}// openflow::httpserver

#endif //: #ifndef _OPENFLOW_SERVER_H_ ///:~
