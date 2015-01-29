// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                  
// Author:  David<daijinwei41@gmail.com>                                                                      
// Created: 2014-12-25                                                                                        
// Description:httpserver implemetation 
//
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstring>
#include <errno.h>
#include <boost/noncopyable.hpp>
#include <openflow.h>
#include "http_response.h"
#include "app_exception.h"
#include "../httpserver_common.h"

namespace openflow{ namespace httpserver{ namespace application{

void OHttpResponse::httpResponseHeader(char *http_header_buffer,
									   	const std::string& http_version,
										int status_code, 
										const std::string& status_string)
{
	if(!http_version.empty() && (!status_string.empty()))
	{
		sprintf(http_header_buffer, HTTP_HEAD_FORMAT, http_version.c_str(), 
										status_code, status_string.c_str());
		if(strlen(http_header_buffer) < 0) 
		{
			GlobalOutput.printf("httpResponseHeader() not enough space");
		}
	}
}

std::string OHttpResponse::httpResponseError()
{
	char http_header_buffer[HTTP_HEAD_BUFFER];
	sprintf(http_header_buffer, HTTP_HEAD_FORMAT, HTTP_VERSION.c_str(), 
								HTTP_NOTFOUND, HTTP_NOTFOUND_STR.c_str());
	return std::string(http_header_buffer);
}

std::string OHttpResponse::httpResponseOk()
{
	char http_header_buffer[HTTP_HEAD_BUFFER];
	sprintf(http_header_buffer, HTTP_HEAD_FORMAT, HTTP_VERSION.c_str(), 
								HTTP_OK, HTTP_OK_STR.c_str());
	return std::string(http_header_buffer);
}


std::string OHttpResponse::httpResponseBody(const std::string http_resource_path)
{
	std::string http_body;
	std::string http_body_message;

	if(!http_resource_path.empty()) 
	{
		http_body_message = getHttpResources(http_resource_path);
	}

	const char *http_title = "openflow httpserver";
	const char *http_body_head = "Welcome to openflow Http server!";

	int http_body_len = strlen(http_body_message.c_str()) + HTTP_BODY_FORMAT_BUFFER;

	char buf[http_body_len];
	sprintf(buf,HTTP_INDEX_HTML_FORMAT, http_title, http_body_head, http_body_message.c_str());
	http_body.assign(buf);

	return http_body;
}

std::string OHttpResponse::getHttpResources(const std::string resource_path)
{
	std::string resource_contents;
	std::string http_resource_path = resource_path;

	if (http_resource_path.empty()) 
	{
		GlobalOutput.printf("getHttpResources() uri path is empty");
		return resource_contents;
	}

	int ret =  file_type(http_resource_path.c_str());

	if(!((S_REG == ret) || (S_LNK == ret))) 
	{
		GlobalOutput.printf("getHttpResources() uri path is not a regular or link file");
		return resource_contents;
	}

	if (0 == access(http_resource_path.c_str(), R_OK)) 
	{
		std::ifstream in(http_resource_path.c_str(), std::ios::in);
		if(in) 
		{
			std::istreambuf_iterator<char> beg(in), end;
			std::string data(beg, end);
			resource_contents.assign(data);
			in.close();
		} 
		else 
		{
			GlobalOutput.printf("getHttpResources(): %s can not open", http_resource_path.c_str());
		}
	} 
	else 
	{
		std::cout << "file not exitst" << std::endl;
		GlobalOutput.printf("getHttpResources(): %s can not access", http_resource_path.c_str());
	}

	return resource_contents;
}


// abs 
void OHttpResponse::setResourcePath(const std::string http_request_uri)
{
	if(!http_request_uri.empty()) 
	{
		http_resource_path_ = http_request_uri;
	}
}

std::string OHttpResponse::getResourcePath() const
{
	return http_resource_path_;
}

std::string OHttpResponse::httpResponse()
{
	std::string http_message;
	std::string http_head;
	
	int ret;
	const char *str_path = http_resource_path_.c_str();

	ret = file_type(str_path);
	if(!((S_REG == ret) || (S_LNK == ret))) 
	{
		http_head = httpResponseError();
		http_message.assign(http_head);
		GlobalOutput.printf("httpResponse()::file_type():  file is not regular or link file");
		return http_message;
	}

#if 0
	if ( 0 != (ret = access(str_path, F_OK))) 
	{
		http_head = httpResponseError();
		http_message.assign(http_head);
		GlobalOutput.printf("httpResponse()::access(): not a file");
		return http_message;
	} 
#endif

	if ( 0 != (ret = access(str_path, R_OK))) 
	{
		http_head = httpResponseError();
		http_message.assign(http_head);
		GlobalOutput.printf("httpResponse()::access(): can not read file");
		return http_message;
	} 

	http_head = httpResponseOk();
	http_message.assign(http_head);
	std::string http_body = httpResponseBody(getResourcePath().c_str());
	http_message.append(http_body);
	
	return http_message;
}

}}} ///:~
