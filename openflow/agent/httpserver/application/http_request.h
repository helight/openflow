// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-11-25                                                                                            
// Description:httpserver implemetation of http request
//
#ifndef _OPENFLOW_HTTPREQUEST_H_
#define _OPENFLOW_HTTPREQUEST_H_

#include <cstring>
#include <boost/noncopyable.hpp>
#include "request.h"

namespace openflow{ namespace httpserver{ namespace application{

class OHttpRequest : public ORequest 
{
public:
	// Construction
	OHttpRequest(){}
    explicit OHttpRequest(METHOD http_method, 
						  const std::string& http_uri = " ") throw(): 
		http_request_method_(http_method),
		http_request_uri_(http_uri){}

	// Destruction
    virtual ~OHttpRequest() throw(){}

	/**
	 * The implementation of parseRquest.
	 * args: http_request_string
	 */
    virtual void parseRequest(const std::string& http_request_string);

	/**
	 * The implementation of getRequestMehod.
	 * return: http method, for example, GET
	 */
    virtual METHOD getRequestMethod() const;

	/**
	 * The implementation of getRequestUri.
	 * return: http request uri, for example /index.php
	 */
	virtual std::string getRequestUri() const;

	/**
	 * The implementation of getRequestUri.
	 * return: http version, for example, HTTP 1.1
	 */
	virtual std::string getRequestVersion() const;

	/**
	 * The implementation of getRequestHost.
	 * return: request host, for example, 192.168.1.1
	 */
	virtual std::string getRequestHost() const;
	
private:
    METHOD			http_request_method_;       	// Request type: GET, HEAD 
    std::string     http_request_uri_;
    std::string     http_request_host_;
    std::string     http_request_version_;
};

/**                                                                                                               
 * Generic factory class to make a request object                                                                 
 */

class OHttpRequestFactory : public ORequestFactory 
{
 public:
   OHttpRequestFactory(){}
   virtual ~OHttpRequestFactory(){}

   /**                                                                                                             
    * Default implementation does nothing ,just returns the request given                                          
    */

    virtual boost::shared_ptr<ORequest> createORequest()
	{
      return boost::shared_ptr<ORequest>(new OHttpRequest());
    }
};

}}}

#endif//: _OPENFLOW_HTTPREQUEST_H_
