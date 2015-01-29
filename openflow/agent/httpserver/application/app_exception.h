// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-25                                                                                            
// Description:httpserver implemetation 

#ifndef _OPENFLOW_APP_EXCEPTION_H_
#define _OPENFLOW_APP_EXCEPTION_H_

#include <cstring>
#include "../httpserver_common.h"

#define	BUF_SIZE 1024

namespace openflow{ namespace httpserver{ namespace application{

/**
 * Define the app exception
 */
class OAppException : public OException
{	
public:
	/**
	 * Error code for the various types of exception
	 */
	enum OAppExceptionType
	{
		UNKNOWN = 0,
		NOT_OPEN = 1,
		END_OF_FILE = 2,
		INTERRUPTED = 4,
		BAD_ARGS = 5,
		CORRUPTED_DATA = 6
	}; 
	
	/* The default constructor */
	OAppException() throw():
	OException(), 
	type_(UNKNOWN){}

	/* Constructor*/
	OAppException(OAppExceptionType type) throw():
	OException(), 
	type_(type){}

	OAppException(const std::string& message) throw():
	OException(message), 
	type_(UNKNOWN){}

	OAppException(OAppExceptionType type, const std::string& message) throw():
	OException(message), 
	type_(type){}

	OAppException(OAppExceptionType type, const std::string& message, int num) throw():
	OException(message + ":" + strerror_s(num)), 
	type_(type){}

	/* Destructor */
	~OAppException() throw(){}

	OAppExceptionType getType() const 
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
	OAppExceptionType	type_;
};

}}}

#endif //: #ifndef _OPENFLOW_APP_EXCEPTION_H_ ///:~
