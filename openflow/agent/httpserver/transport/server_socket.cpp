// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-05                                                                                            
// Description:httpserver implemetation 
//
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openflow.h>
#include "server_socket.h"
#include "transport_exception.h"

namespace openflow{ namespace httpserver { namespace transport{

OServerSocket::OServerSocket(){}

OServerSocket::OServerSocket(int32_t port): 
	port_(port),
	backlog_(5),
	reuse_addr_(0),
	tcp_recv_buffer_(0),
	tcp_send_buffer_(0),
	recv_timeout_(0),
	send_timeout_(0)
	{
		client_id_.assign(FD_SETSIZE, -1);
		setReuseAddr(1);
	}

OServerSocket::OServerSocket(int32_t port, uint32_t backlog, uint32_t tcp_recv_buff, 
				uint32_t tcp_send_buff, uint32_t recv_timeout,  uint32_t send_timeout):
	port_(port),
	backlog_(backlog),
	reuse_addr_(0),
	tcp_recv_buffer_(tcp_recv_buff),
	tcp_send_buffer_(tcp_send_buff),
	recv_timeout_(recv_timeout),
	send_timeout_(send_timeout)
	{
		client_id_.assign(FD_SETSIZE, -1);
	}

OServerSocket::~OServerSocket()
{
	closeSockect();
}

void OServerSocket::setPort(int32_t port) 
{
	port_ = port;   
}

void OServerSocket::setMaxIndex(int32_t index) 
{
	max_index_ = index; 
}

void OServerSocket::setBacklog(uint32_t max_listen_queue)
{
	backlog_ = max_listen_queue;     
}

void OServerSocket::setReuseAddr(int32_t value)
{
	reuse_addr_ = value;
}

void OServerSocket::setTcpRecvBuffer(uint32_t buffer_size)
{
	tcp_recv_buffer_ = buffer_size;
}

void OServerSocket::setTcpSendBuffer(uint32_t buffer_size)
{
	tcp_send_buffer_ = buffer_size;
}

void OServerSocket::setRecvTimeout(uint32_t timeout)
{
	recv_timeout_ = timeout;
}

void OServerSocket::setSendTimeout(uint32_t timeout)
{
	send_timeout_ = timeout;
}

int32_t OServerSocket::getPort()const
{
	return port_;   
}

int32_t OServerSocket::getMaxIndex()const 
{
	 return max_index_;  
}

uint32_t OServerSocket::getBacklog()const
{
    return backlog_;    
}

int32_t OServerSocket::getSockfd()const
{
	return listen_fd_; 
}	

// fd
void OServerSocket::fdZero(fd_set &fdset) 
{
    FD_ZERO(&fdset);
}

void OServerSocket::fdSet(SockectFd fd, fd_set &fdset) 
{
	FD_SET(fd, &fdset);  
}

bool OServerSocket::fdIsset(SockectFd fd, fd_set fdset)
{
	return FD_ISSET(fd, &fdset);
}

void OServerSocket::fdClear(SockectFd fd, fd_set &fdset)
{
	 FD_CLR(fd, &fdset);
}

fd_set& OServerSocket::getRset() 
{
	return rset_;   
}

fd_set& OServerSocket::getAllset() 
{
	return allset_; 
}

std::vector<int>& OServerSocket::getClientId()
{
	return client_id_;
}

int32_t OServerSocket::setSockOpt(SockectFd &sockfd)
{
	// Set tcp to prevent 2MSL delay on aceept
	if(0 < reuse_addr_) 
	{
		socklen_t len = sizeof(int);
		if (-1 == setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, cast_sockopt(&reuse_addr_), len))
		{
			int32_t errno_num = errno;
			GlobalOutput.perror("OServerSocket::setSockOpt()setpotsock SO_REUSEADDR", errno_num);
			closeSockect();
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set SO_REUSEADDR", errno_num);
			return -1;
		}
	}

	// Set tcp buffer size
	if( 0 < tcp_recv_buffer_) 
	{
		if (-1 == setsockopt(sockfd,SOL_SOCKET, SO_RCVBUF, cast_sockopt(&tcp_recv_buffer_), sizeof(int)))
		{
			int32_t errno_num = errno;
			GlobalOutput.perror("OServerSocket::setSockOpt()setpotsock SO_RCVBUF", errno_num);
			closeSockect();
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set SO_RCVBUF", errno_num);
			return -1;
		}
	}

	if( 0 < tcp_send_buffer_) 
	{
		if (-1 == setsockopt(sockfd,SOL_SOCKET, SO_SNDBUF, cast_sockopt(&tcp_send_buffer_), sizeof(int)))
		{
			int32_t errno_num = errno;
			GlobalOutput.perror("OServerSocket::setSockOpt()setpotsock SO_SNDBUF", errno_num);
			closeSockect();
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set SO_SNDBUF", errno_num);
			return -1;
		}
	}
	// Set timeout
	if( 0 < recv_timeout_) 
	{
		if (-1 == setsockopt(sockfd,SOL_SOCKET, SO_RCVTIMEO, cast_sockopt(&recv_timeout_), sizeof(int)))
		{
			int32_t errno_num = errno;
			GlobalOutput.perror("OServerSocket::setSockOpt()setpotsock SO_RCVTIMEO", errno_num);
			closeSockect();
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set SO_RCVTIMEO", errno_num);
			return -1;
		}
	}

	if( 0 < send_timeout_)
   	{
		if (-1 == setsockopt(sockfd,SOL_SOCKET, SO_SNDTIMEO, cast_sockopt(&send_timeout_), sizeof(int)))
		{
			int32_t errno_num = errno;
			GlobalOutput.perror("OServerSocket::setSockOpt()setpotsock SO_SNDTIMEO", errno_num);
			closeSockect();
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set SO_SNDTIMEO", errno_num);
			return -1;
		}
	}

	// Don't have TIME_WAIT, don't want to block on class to close
	struct linger ling = {0, 0};
	if (-1 == setsockopt(sockfd,SOL_SOCKET, SO_LINGER, cast_sockopt(&ling), sizeof(struct linger)))
	{
		int32_t errno_num = errno;
		GlobalOutput.perror("OServerSocket::setSockOpt()setpotsock SO_SNDTIMEO", errno_num);
		closeSockect();
		throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set SO_LINGER", errno_num);
		return -1;
	}

	return 0;
}

int32_t OServerSocket::listenClient(uint32_t port)
{
	char port_buf[sizeof("65535") + 1];
	sprintf(port_buf, "%d", port);

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;	// Open socket passive
	hints.ai_family = AF_UNSPEC;					// Allow IPV4 or IPV6
	hints.ai_socktype = SOCK_STREAM;				// TCP stream
	hints.ai_protocol = 0;							// Support any protocol
	hints.ai_canonname = NULL;						// The canonname is NULL
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	int32_t ret = getaddrinfo(NULL, port_buf, &hints, &result);
	if (0 != ret) 
	{
		int32_t errno_num = ret;
		GlobalOutput.printf("getaddrinfo %d", errno_num);
		throw OTransportException(OTransportException::NOT_OPEN,
				"Can not use get hostaddrinfo from getaddrinfo()", errno_num);
		return -1;
	}
	
	/**
	 * getaddrinfo return a list of address struct,
	 * so we must get the right address we need
	 */
	for (rp = result; rp != NULL; rp = result->ai_next) 
	{
		// We need IPV6, support dual protocol stack, recive ipv4 or ipv6 
		if (rp->ai_family == AF_INET6)
		{
			break;	
		}
	}

	listen_fd_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
	if (-1 == listen_fd_) 
	{
		int32_t errno_num = listen_fd_;
		GlobalOutput.perror("OServerSocket::listenClient()socket()", errno_num);
		throw OTransportException(OTransportException::NOT_OPEN,
							"Can not create a socket", errno_num);
		return -1;
	}

	ret = setSockOpt(listen_fd_);
	if (-1 == ret) 
	{
		int32_t errno_num = listen_fd_;
		GlobalOutput.perror("OServerSocket::listenClient()socket()", errno_num);
		throw OTransportException(OTransportException::NOT_OPEN,
								"Can not setsockopt", errno_num);
		return -1;
	}

	ret = bind(listen_fd_, rp->ai_addr, rp->ai_addrlen);	
	if (-1 == ret)
   	{
		int32_t errno_num = ret;
		GlobalOutput.perror("OServerSocket::listenClient()bind()", errno_num);
		closeSockect();
		throw OTransportException(OTransportException::NOT_OPEN,
							"Can not bind a socket", errno_num);
		return -1;
	}

	freeaddrinfo(result);			// No longer need

	ret = listen(listen_fd_, backlog_);
	if ( -1 == ret) 
	{
		int32_t errno_num = ret;
		GlobalOutput.perror("OServerSocket::listenClient()listen()", errno_num);
		closeSockect();
		throw OTransportException(OTransportException::NOT_OPEN,
							"Can not listen socket file description", errno_num);
	}
	return 0;
}

int OServerSocket::acceptClient() 
{
	struct sockaddr_in client_addr;
	socklen_t clientLen = sizeof(struct sockaddr_in);

	connect_fd_ = accept(listen_fd_, (struct sockaddr *)&client_addr, &clientLen);	
	if ( -1 == connect_fd_) 
	{
		// In order to deal with RST data package
		if((EWOULDBLOCK == errno) 
			| (ECONNABORTED == errno)
			| (EPROTO == errno) 
			| (EINTR == errno))
	   	{	

			int32_t errno_num = -1;
			GlobalOutput.perror("OServerSocket::acceptClient() return errno ", errno_num);
			return -1;
		}
	}

	return connect_fd_;
}

void OServerSocket::initializeSelect()
{
	// init allset
	FD_ZERO(&allset_);
	FD_SET(listen_fd_, &allset_);

	// set max_fd_
	max_fd_ = listen_fd_;
}

int OServerSocket::servPoll()
{
	rset_ = allset_;
	nready_ = select(max_fd_ + 1, &rset_, NULL, NULL, NULL);

	if (FD_ISSET(listen_fd_, &rset_))
   	{
		// Set listen fd non-blocking, to deal with the RST which client send;
		int32_t flags;
		if ((flags = fcntl(listen_fd_, F_GETFL, 0)) < 0) 
		{
			int32_t errno_num = flags;
			GlobalOutput.perror("OServerSocket::listenClient()fcntl()F_GETFL:",errno_num);
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not get file description", errno_num);
			return -1;
		}

		flags |= O_NONBLOCK;
		if ((flags = (fcntl(listen_fd_, F_SETFL, flags))) < 0)
		{
			int32_t errno_num = flags;
			GlobalOutput.perror("OServerSocket::listenClient()fcntl()SET listen fd non-blocking:", errno_num);
			throw OTransportException(OTransportException::NOT_OPEN,
							"Can not set file description", errno_num);
			return -1;
		}

		connect_fd_ = acceptClient();
		if ( -1 == connect_fd_) 
		{
			int32_t errno_num = -1;
			GlobalOutput.perror("OServerSocket::servPoll() acceptClient return error ", errno_num);
			return -1;
		}

		int32_t i = 0;
		for (i = 0; i < FD_SETSIZE; i++) 
		{
			if(0 > client_id_.at(i))
		   	{
				client_id_.at(i) = connect_fd_;	
				break;
			}
		}

		if (i == FD_SETSIZE) 
		{
			GlobalOutput.printf("OServerSocket::servPoll()fd_set too many client");
			return -1;
		}

		FD_SET(connect_fd_, &allset_);		// Add new descriptor to set

		if( connect_fd_ > max_fd_)
	   	{
			max_fd_ = connect_fd_;	
		}

		if (i > max_index_)
	   	{
			max_index_ = i;	
		}

		if(--nready_ <= 0) 
		{
			GlobalOutput.printf("OServerSocket::servPoll() no more readable description");
			return -1;	
		}
	} 
	return 0;
}

void OServerSocket::closeSockect() 
{
	close(listen_fd_);
}

}}}///: openflow::httpserver::transport
