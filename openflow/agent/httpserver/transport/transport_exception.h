// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-05                                                                                            
// Description:httpserver transport own exception

#ifndef _OPENFLOW_TRANSPORT_EXCEPTION_H_
#define _OPENFLOW_TRANSPORT_EXCEPTION_H_

#include <cstring>
#include "../httpserver_common.h"
//#include <openflow.h>

#define	BUF_SIZE 1024
namespace openflow{ namespace httpserver{ namespace transport{

class OTransportException : public OException 
{	
public:
	/**
	 * Error code for the various types of exception
	 */
	enum OTransportExceptionType
	{
		UNKNOWN = 0,
		NOT_OPEN = 1,
		END_OF_FILE = 2,
		TIMED_OUT = 3,
		INTERRUPTED = 4,
		BAD_ARGS = 5,
		CORRUPTED_DATA = 6,
		INTERNAL_ERROR = 7	
	}; 
	
	/* the default constructor */
	OTransportException() throw():
	OException(), 
	type_(UNKNOWN){}

	/* constructor*/
	OTransportException(OTransportExceptionType type) throw():
	OException(), 
	type_(type){}

	OTransportException(const std::string& message) throw():
	OException(message), 
	type_(UNKNOWN){}


	OTransportException(OTransportExceptionType type, const std::string& message) throw():
	OException(message), 
	type_(type){}

	OTransportException(OTransportExceptionType type, const std::string& message, int num) throw():
	OException(message + ":" + strerror_s(num)), 
	type_(type){}

	/* destructor */
	~OTransportException() throw(){}

	OTransportExceptionType	getType() const 
	{
		return type_;	
	}

	// like c++ strerror_r
	std::string strerror_s(int copy_num) 
	{
		char buf[BUF_SIZE];
		char* ebuf = strerror_r(copy_num, buf, BUF_SIZE);
		return std::string(ebuf);	
	}

	virtual const char* what() const throw();
protected:
	OTransportExceptionType	type_;
};

}}}// openflow::httpserver::transport

#endif //: #ifndef _OPENFLOW_TRANSPORT_EXCEPTION_H_ ///:~
