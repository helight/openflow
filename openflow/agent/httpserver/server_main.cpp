// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  	David<daijinwei41@gmail.com>                                                                          
// Created: 	2014-11-25                                                                                            
// Modified: 	2015-02-03                                                                                            
// Description: httpserver main


#include <stdio.h>
#include "httpserver.h"
#include "application/http_response.h"

using namespace openflow::httpserver;
using namespace openflow::httpserver::transport;
using namespace openflow::httpserver::application;

const std::string kUsage = "Usage: httpserver [start|stop|restart]\r\n"	\
							"--Options--:\r\n"							\
							"\tstart\tstart httpserver\r\n"				\
							"\tstop\tstop httpserver\r\n"				\
							"\trestart\trestart httpserver\r\n"			\
							"\tpause\tpause httpserver\r\n"				\
							"\tresume\tresume httpserver\r\n";

void display_usage()
{
	printf("%s\n", kUsage.c_str());
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	if(argc <= 1)
	{
		display_usage();
		return -1;	
	}

	// listening port
	int port = 8080;

	boost::shared_ptr<OServerSocket> sock(new OServerSocket(port));

	// get request
	boost::shared_ptr<ORequestFactory> request_factory(new OHttpRequestFactory());
	boost::shared_ptr<ORequest> request = request_factory->createORequest();

	// get response
	boost::shared_ptr<OResponseFactory> response_factory(new OHttpResponseFactory());
	boost::shared_ptr<OResponse> response = response_factory->createOResponse();

	// main serve
	OHttpServer server(sock, request, response);
	server.serve();

	return 0;
}
