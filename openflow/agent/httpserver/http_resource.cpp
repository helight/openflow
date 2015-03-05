// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-12-25
// Description:httpserver implemetation 
//

#include <stdio.h>
#include <unistd.h>
#include <iterator>
#include <fstream>
#include <cstring>
#include <errno.h>
#include <boost/noncopyable.hpp>
#include "http_resource.h"
#include "http_common.h"

namespace openflow{ namespace httpserver{ 

std::string get_http_resource_path()
{
    std::string httpserver_path;
    if(!httpserver_resource_path.empty())
    {
        httpserver_path.assign(httpserver_resource_path);
        return httpserver_path;
    }

    char path_buf[kHttpserverPathSize];
    if(getcwd(path_buf, kHttpserverPathSize) != NULL)
    {
        httpserver_path.assign(path_buf);
    }
    return httpserver_path;
}

std::string get_http_resources(const std::string resource_path)
{
	std::string resource_contents;
	std::string http_resource_path = resource_path;

	if (http_resource_path.empty()) 
	{
		GlobalOutput.printf("get_http_resources() http_resource_path is empty");
		return resource_contents;
	}

	int ret =  file_type(http_resource_path.c_str());

	if(!((S_REG == ret) || (S_LNK == ret))) 
	{
        GlobalOutput.printf("get_http_resources(): resource file is not a regular or link file");
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
			GlobalOutput.printf("get_http_resources(): %s can not open", http_resource_path.c_str());
		}
	} 
	else 
	{

		GlobalOutput.printf("get_http_resources(): %s can not access", http_resource_path.c_str());
	}

	return resource_contents;
}

}} ///:~
