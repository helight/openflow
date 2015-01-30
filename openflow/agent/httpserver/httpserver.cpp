// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver interface

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <exception>
#include <openflow.h>
#include "httpserver_common.h"
#include "httpserver.h"
#include "transport/transport_exception.h"

namespace openflow { namespace httpserver {

using namespace common;
using namespace openflow::httpserver::transport;
using namespace openflow::httpserver::application;

void OHttpServer::setHttpserverPath()
{
	char path_buf[kHttpserverPathSize];		
	if(getcwd(path_buf, kHttpserverPathSize) != NULL)
	{
		httpserver_path_.assign(path_buf);
	}
}

std::string OHttpServer::getHttpserverPath() const 
{
	return httpserver_path_;	
}

void OHttpServer::serve()
{

	setHttpserverPath();
	int ret = sock_->listenClient(8080);
	if (ret < 0)
   	{
		sock_->closeSockect();
		exit(0)	;
	}

	sock_->initializeSelect();
	while(!stop_)
	{
		try{
			if (-1 == (sock_->servPoll()))
			{
				GlobalOutput.printf("OHttpServer::serve()::serverPoll()return -1");
				continue;	
			}

			int connect_fd;
			for(int i = 0; i < sock_->getMaxIndex(); i++) 
			{
				if( (connect_fd = (sock_->getClientId()).at(i) ) < 0) 
				{
					continue;	
				}

				if(sock_->fdIsset(connect_fd, sock_->getRset()))
				{
					ssize_t n;
					char buf[kBufferSize];
					if((n = read(connect_fd, buf, kBufferSize)) < 0)
				   	{
						GlobalOutput.printf("OHttpServer::serve()can not read message::");
					} 
					else 
					{
						request_->parseRequest(std::string(buf));
						if(openflow::httpserver::application::HTTP_GET == request_->getRequestMethod()) 
						{

							std::string httpserver_path(getHttpserverPath());
							std::string request_uri = request_->getRequestUri();

							if(!httpserver_path.empty())
							{
								if(!request_uri.empty())
								{
									httpserver_path.append("/");
									httpserver_path.append(request_uri);
								}
								httpserver_path.assign(strip(httpserver_path));
							}

							response_->setResourcePath(httpserver_path);

							std::string send_message;
							send_message.assign(response_->httpResponse());
							ssize_t send_size = static_cast<ssize_t>(send_message.size());
							send_size = write(connect_fd, send_message.c_str(), send_size);

							close(connect_fd);
							sock_->fdClear(connect_fd,sock_->getAllset());
							(sock_->getClientId()).at(i) = -1;
						} 
						else
						{
							GlobalOutput.printf("OHttpServer::serve()::HTTP METHOD is't GET");
							continue;
						}
					}

					if (--(sock_->getNready()) <= 0 )
				   	{
						break;
					}
				}
			}
		} 
		catch(OTransportException &e) 
		{
			std::string errStr = std::string("OHttpServer::serve()") + e.what();
			GlobalOutput(errStr.c_str());
		} 
		catch(std::exception &e) 
		{
			std::string errStr = std::string("OHttpServer::serve()") + e.what();
			GlobalOutput(errStr.c_str());
		} 
		catch (...) 
		{
			std::string errStr = "something unkown error";
			GlobalOutput(errStr.c_str());
		}
	}
}

}}// openflow::httpserver
