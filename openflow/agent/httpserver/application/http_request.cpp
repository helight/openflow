//Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                  
// Author:  David<daijinwei41@gmail.com>                                                                      
// Created: 2014-12-25                                                                                        
// Description:httpserver implemetation 
// :
#include <openflow.h>
#include "http_request.h"
#include "app_exception.h"
#include "../httpserver_common.h"

namespace openflow{ namespace httpserver{ namespace application{

void OHttpRequest::parseRequest(const std::string& http_request_str) 
{
	// Parse the http method
	std::string str = http_request_str;
	std::string::size_type  pos;

	if((pos = str.find("GET")) != std::string::npos)
	{
		http_request_method_ = HTTP_GET;	
	} 
	else if((pos = str.find("HEAD")) != std::string::npos)
	{
		http_request_method_ = HTTP_HEAD;	
	}
   	else if((pos = str.find("POST")) != std::string::npos)
	{
		http_request_method_ = HTTP_POST;	
	} 
	else if((pos = str.find("PUT")) != std::string::npos)
	{
		http_request_method_ = HTTP_PUT;	
	} 
	else if((pos = str.find("DELETE")) != std::string::npos)
	{
		http_request_method_ = HTTP_DELETE;	
	}
   	else if((pos = str.find("TRACE")) != std::string::npos)
	{
		http_request_method_ = HTTP_TRACE;	
	}
   	else if((pos = str.find("CONNECT")) != std::string::npos)
	{
		http_request_method_ = HTTP_CONNECT;	
	} 
	else 
	{
		http_request_method_ = HTTP_UNKNOWN;	
		GlobalOutput.printf("OHttpRequest::parseHttpRequest() can not find the HTTP method");
	}

	// Parse the http uri and http version
	std::string::size_type  pos_end;
	if((pos = str.find('/')) != std::string::npos) 
	{
		if((pos_end = str.find("HTTP")) != std::string::npos) 
		{
			//: /uri, find uri
			http_request_uri_.assign(strip(str.substr(pos + 1, (pos_end - pos -1 -1))));

			if((pos = str.find("\r\n")) != std::string::npos) 
			{
				http_request_version_ = str.substr(pos_end, (pos -  pos_end + 1));
			} 
			else 
			{
				GlobalOutput.printf("OHttpRequest::parseHttpRequest() can  not find the CRLF");
			}
		} 
		else 
		{
			GlobalOutput.printf("OHttpRequest::parseHttpRequest() can not find the HTTP version");
		}
	} 
	else 
	{
		GlobalOutput.printf("OHttpRequest::parseHttpRequest() can  not find the absolutely uri");
	}

	// Parse the request host
	if((pos = str.find("Host")) != std::string::npos) 
	{
		if((pos_end = str.find("Connection")) != std::string::npos) 
		{
			http_request_host_  = str.substr(pos + 6, (pos_end - (pos + 6)));
		}
	   	else
	   	{
			GlobalOutput.printf("OHttpRequest::parseHttpRequest() can not find the Connection");
		}

	} 
	else 
	{
		GlobalOutput.printf("OHttpRequest::parseHttpRequest() can not find the Host");
	}
}


// Get http args
METHOD OHttpRequest::getRequestMethod() const 
{
	return http_request_method_;
}

std::string OHttpRequest::getRequestUri()const
{
	return http_request_uri_;
}

std::string OHttpRequest::getRequestVersion()const
{
	return http_request_version_;
}

std::string OHttpRequest::getRequestHost()const
{
	return http_request_host_;
}

}}}// namespace 

