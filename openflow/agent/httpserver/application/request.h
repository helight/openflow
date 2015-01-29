// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-25                                                                                            
// Description: http request interface

#ifndef _OPENFLOW_REQUEST_H_
#define _OPENFLOW_REQUEST_H_

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace openflow {namespace httpserver { namespace application {

// DEFINE HTTP METHOD
enum METHOD
{
	HTTP_OPTIONS,       
	HTTP_GET,       	// GET method    
	HTTP_HEAD,      	// HEAD method, only retrive the data infomation
	HTTP_POST,      	// POST method, commit the infomation
	HTTP_PUT,      		// PUT method, only retrive the data infomation
	HTTP_DELETE,     
	HTTP_TRACE,       
	HTTP_CONNECT,   
	HTTP_UNKNOWN   
};

/**
 * Generic interface for http request.
 */
class ORequest : boost::noncopyable 
{
public:
	ORequest(){}

	/**
	 * The interface for parseRquest.
	 * str: http request string
	 */
	virtual void parseRequest(const std::string &str) = 0;

	/**
	 * The interface for getRequestMethod.
	 * return: http method
	 */
	virtual METHOD getRequestMethod() const = 0;

	/**
	 * The interface for getRequestUri.
	 * return: http request uri
	 */
	virtual std::string getRequestUri() const = 0;


	/**
	 * The interface for getRequestVersion.
	 * return: http version. For example  HTTP1.1
	 */
	virtual std::string getRequestVersion() const = 0;

	/**
	 * The interface for getRequestHost.
	 * return: request host. For example 192.168.1.1
	 */
	virtual std::string getRequestHost() const = 0;

	// Destruction
	virtual ~ORequest(){}
};

/**
 * Generic factory class to make a request object
 */
class ORequestFactory
{
 public:
  ORequestFactory(){}
  virtual ~ORequestFactory(){}

  /**
   * Default implementation does nothing ,just returns the request given
   */
  virtual boost::shared_ptr<ORequest> createORequest() = 0;

};

}}}// openflow::httpserver::application

#endif // #ifndef _OPENFLOW_REQUEST_H_ ///:~
