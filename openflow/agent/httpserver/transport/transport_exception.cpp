// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-05                                                                                            
// Description:httpserver transport exception

#include "transport_exception.h"

namespace openflow{ namespace httpserver { namespace transport{

const char* OTransportException::what()const throw()
{
	if(message_.empty())
	{
		switch(type_)	
		{
			case UNKNOWN: 			return "OTransportException: Unknown transport exception";
			case NOT_OPEN: 			return "OTransportException: Transport not open";
			case TIMED_OUT: 		return "OTransportException: Time out";
			case END_OF_FILE: 		return "OTransportException: End of file";
			case INTERRUPTED: 		return "OTransportException: Interrupted";
			case BAD_ARGS:	 		return "OTransportException: Invalid arguments";
			case CORRUPTED_DATA:	return "OTransportException: Corrupted Data";
			case INTERNAL_ERROR:	return "OTransportException: Internal error";
			default:				return "OTransportException: Invalid exception type";
		}
	} 
	else
	{
		return message_.c_str();
	}
}

}}}///:openflow::httpserver::transport

