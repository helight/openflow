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
#include "response.h"

namespace openflow{ namespace httpserver{ namespace application{

/* HTTP status code*/
enum HTTP_STATUS_CODE
{
	HTTP_OK = 200,
	HTTP_NOTFOUND = 404,
	HTTP_BADREQUEST = 400,
	HTTP_NOTIMPLEMENT = 501,
	HTTP_SERVUNAVAIL = 503
}; 

/* Response status readable code*/
const static std::string HTTP_OK_STR("OK");
const static std::string HTTP_NOTFOUND_STR("Not Found");
const static std::string HTTP_BADREQUEST_STR("BAD REQUEST");
const static std::string HTTP_NOTIMPLEMENT_STR("NOT IMPLEMENTED");
const static std::string HTTP_SERVUNAVAIL_STR("SERVER ERROR");


/* http version */
const std::string HTTP_VERSION = "HTTP/1.1";

/* http head buffer size */
const uint32_t HTTP_HEAD_BUFFER = 128;
const uint32_t HTTP_BODY_FORMAT_BUFFER = 256;

/**
 * http response head, the format like this
 * HTTP/1.1 200 OK
 */
#define HTTP_HEAD_FORMAT "%s %d %s\r\nContent-type: text/text\r\n\r\n"


#define HTTP_INDEX_HTML_FORMAT "%s\r\n%s\r\n%s\r\n"

class OHttpResponse : public OResponse
{
public:
	// Default constructor
	OHttpResponse(){}

	// Constructor
	explicit OHttpResponse(std::string message){}

	// Response Http the header
	void httpResponseHeader(char *http_header_buffer,
							const std::string& http_version,
							int status_code, 
							const std::string &status_string);

	// Destructor
    virtual ~OHttpResponse() throw(){}

	// Response http error
	std::string httpResponseError();

	// Response http Ok
	std::string httpResponseOk();

	// Response http body
	std::string httpResponseBody(const std::string http_request_path);

	/**
	 * Accoding to http request uri,
	 * get the http resources form the http server
	 */
	std::string getHttpResources(const std::string http_request_path);

	/**
	 * The implementation of getResourcePath.
	 * return: the path resource path, for example, /home/david/httpserver/taskid
	 */
	virtual std::string getResourcePath() const;

	/**
	 * The implementation of setResourcePath.
	 * args: http_request_uri, the path of resource 
	 */
	virtual void setResourcePath(const std::string http_request_uri);

	/**
	 * The implementation of httpResponse.
	 */
	virtual std::string httpResponse();

private:
	std::string	http_message_;
	std::string	http_resource_path_;
};

/**
 * Generate a OHttpResponse object
 */
class OHttpResponseFactory : public OResponseFactory
{
 public:
   OHttpResponseFactory(){}
   virtual ~OHttpResponseFactory(){}

   /**                                                                                                             
    * Default implementation does nothing ,just returns the request given                                          
    */

   virtual boost::shared_ptr<OResponse> createOResponse()
   {

     return boost::shared_ptr<OResponse>(new OHttpResponse());
   }
};

}}}

#endif//: #ifndef _OPENFLOW_HTTPRESPONSE_H_ ///:~
