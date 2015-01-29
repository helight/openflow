// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-05                                                                                            
// Description:httpserver interface of transport 


#ifndef _OPENFLOW_SERVER_TRANSPORT_H_
#define _OPENFLOW_SERVER_TRANSPORT_H_

#include <stdint.h>

namespace openflow{ namespace httpserver{ namespace transport{

/*
 *
 */

class OServerTransport
{
public:
	OServerTransport(){}
	virtual ~OServerTransport(){}
	//virtual int32_t listenClient(uint32_t port) = 0;
};

}}}

#endif // #ifndef _OPENFLOW_SERVER_TRANSPORT_H_ ///:~
