// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-25                                                                                            
// Description:httpserver response interface 

#ifndef _OPENFLOW_RESPONSE_H_
#define _OPENFLOW_RESPONSE_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <string>

namespace openflow {namespace httpserver { namespace application {

/**
 * Generic interface for a method of request.
 */

class OResponse : boost::noncopyable 
{
public:
	// Constructor
	OResponse(){}
	// The interface of setResourcePath
	virtual void setResourcePath(const std::string http_request_uri) = 0;

	// The interface of getResourcePath
	virtual std::string getResourcePath() const = 0;

	// The interface of httpResponse
	virtual std::string httpResponse() = 0;

	// Destructor
	virtual ~OResponse(){}
};

/**
 * Generic factory class to make a request object
 */
class OResponseFactory
{
 public:
  OResponseFactory(){}
  virtual ~OResponseFactory(){}
  /**
   * Default implementation does nothing ,just returns the request given
   */

  virtual boost::shared_ptr<OResponse> createOResponse() = 0;
};

}}}// openflow::httpserver::application

#endif // #ifndef _OPENFLOW_RESPONSE_H_ ///:~
