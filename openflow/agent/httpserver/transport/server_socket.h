// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-05                                                                                            
// Description:httpserver implemetation of transport

#ifndef _OPENFLOW_SERVER_SOCKET_H_
#define _OPENFLOW_SERVER_SOCKET_H_

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "server_transport.h"

#define SOCKET_CAST_T	void

/** 
 * type convert
 * like void * convert to char *
 */
template<class T>
inline SOCKET_CAST_T* cast_sockopt(T* v)
{
	return reinterpret_cast<SOCKET_CAST_T*>(v);
}

namespace openflow{ namespace httpserver{ namespace transport{

/**
 * Server socket implation s tcp server.
 * Use a unix socket listen and accept call
 */
class OServerSocket : public OServerTransport, boost::noncopyable
{
public:
	typedef int32_t SockectFd;
	// Construction
	OServerSocket();
	OServerSocket(int32_t port);
	OServerSocket(int32_t port, 
		uint32_t backlog, 
		uint32_t tcp_recv_buff, 
		uint32_t tcp_send_buff, 
		uint32_t recv_timeout, 
		uint32_t send_timeout);

	// Destruction, virtual function
	virtual ~OServerSocket();

	void initialize() 
	{
		client_id_.assign(1000, -1);
	}

	// Set function
	inline void setPort(int32_t port);
	inline void setMaxIndex(int32_t index);
	inline void setBacklog(uint32_t max_listen_queue = 5);
	inline void setSockfd();

	void setReuseAddr(int32_t value);
	inline void setTcpRecvBuffer(uint32_t buffer_size);
	inline void setTcpSendBuffer(uint32_t buffer_size);
	inline void setRecvTimeout(uint32_t timeout);
	inline void setSendTimeout(uint32_t timeout);

	// Get function
	int32_t getPort()const;
	uint32_t getBacklog()const;
	int32_t getMaxIndex()const;
	int32_t getSockfd()const;

	int32_t &getNready(){
		return nready_;	
	}

	// FD_* operator
	void fdZero(fd_set &fdset);
	void fdSet(SockectFd fd, fd_set &fdset);
	void fdClear(SockectFd fd, fd_set &fdset);
	bool fdIsset(SockectFd fd, fd_set fdset);

	fd_set& getRset();
	fd_set& getAllset();
	std::vector<int32_t> &getClientId();

	// 
	//void setValue();
	void initializeSelect();
 	int32_t servPoll();
	//
	int32_t setSockOpt(SockectFd &sockfd);
	int32_t listenClient(uint32_t port);
	int32_t acceptClient();
	void closeSockect();

private:
	int32_t port_;						// tcp server listen port
   	int32_t	max_fd_;					// FDSET max fd
	int32_t max_index_;					// The max number of socketfd
	uint32_t backlog_;					// Listen args, the max numbers of  backlog
	int32_t nready_;					// Select return value
	fd_set rset_;						// Select fd_set
	fd_set allset_;						// Decription set, store listen_fd_, connected fd
	std::vector<int32_t> client_id_;	// Store listen_fd_, connected fd
	SockectFd listen_fd_;				// Socket listening fd
	SockectFd connect_fd_;				// Socket connected fd
	uint32_t reuse_addr_;				// Tcp reuseaddr
	uint32_t tcp_recv_buffer_;			// Tcp recv buffer size
	uint32_t tcp_send_buffer_;			// Tcp send buffer size
	uint32_t recv_timeout_;				// Tcp recv timeout
	uint32_t send_timeout_;				// Tcp send timeout
};

/**                                                                                                           
 * Generic factory class to create a transport server object                                                             
 */
class OServerSocketFactory 
{
 public:
  OServerSocketFactory(){}
  virtual ~OServerSocketFactory(){}
 
  /**                                                                                                        
   * create a  OserverSocket object                           
   */
 
  virtual boost::shared_ptr<OServerSocket> createOTransport()
  {
     return boost::shared_ptr<OServerSocket>(new OServerSocket());
  }
};

}}} // openflow::httpserver::transport

#endif // #ifndef _OPENFLOW_SERVER_SOCKET_H_ ///:~
