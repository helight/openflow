// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-25                                                                                            
// Description:httpserver appException implemetation 
//
#include "app_exception.h"

namespace openflow{ namespace httpserver { namespace application{

const char* OAppException::what() const throw()
{
	if(message_.empty())
	{
		switch(type_)	
		{
			case UNKNOWN: 			return "OAppException: Unknown application exception";
			case NOT_OPEN: 			return "OAppException: Application not open";
			case END_OF_FILE: 		return "OAppException: End of file";
			case INTERRUPTED: 		return "OAppException: Interrupted";
			case BAD_ARGS:	 		return "OAppException: Invalid arguments";
			case CORRUPTED_DATA:	return "OAppException: Corrupted Data";
			default:				return "OAppException: Invalid exception type";
		}
	} 
	else
	{
		return message_.c_str();
	}
}

}}}

