// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:  David<daijinwei41@gmail.com>
// Created: 2014-11-25
// Description:httpserver implemetation 
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "http_common.h"

namespace openflow{ namespace httpserver{

int file_type(const char *file_path)
{
	enum FileType	file_type;
	struct stat	buf;
	if (lstat(file_path, &buf) < 0) 
	{
		return 0;
	}
		
	if(S_ISREG(buf.st_mode))
	{
		file_type = S_REG;	
	} 
	else if(S_ISDIR(buf.st_mode))
	{
		file_type = S_DIR;	
	} 
	else if(S_ISCHR(buf.st_mode))
	{
		file_type = S_CHR;	
	} 
	else if(S_ISBLK(buf.st_mode))
	{ 
		file_type = S_BLK;	
	} 
	else if(S_ISFIFO(buf.st_mode))
	{
		file_type = S_FIFO;	
	} 
	else if(S_ISLNK(buf.st_mode))
	{
		file_type = S_LNK;	
	}
   	else if(S_ISSOCK(buf.st_mode))
	{ 
		file_type = S_SOCK;	
	} 
	else 
	{
		file_type = UNKNOWN;
	}

	return file_type;
}

std::string strip(const std::string &input_string)
{
	std::string output_string;
	if(!input_string.empty()) 
	{
		std::string::size_type  start = input_string.find_first_not_of(kStrWhiteSpace);
		if(start != std::string::npos) 
		{
			std::string::size_type end = input_string.find_last_not_of(kStrWhiteSpace);
			output_string = input_string.substr(start, (end - start +1));
		}
	}

	return output_string;
}

//
OOutput GlobalOutput;

void OOutput::perror(const char *message, int error_copy)
{
	std::string out = message + strerror_s(error_copy);
	f_(out.c_str());
}

std::string OOutput::strerror_s(int errno_copy)
{
	char error_buf[1024] = {'\0'};
    char* ebuf = strerror_r(errno_copy, error_buf, sizeof(error_buf));
	return std::string(ebuf);
}

void OOutput::errorTimeWrapper(const char* msg)
{
    time_t now;
	// store the time string
    char dbgtime[26];
    time(&now);
	dbgtime[24] = '\0';
    // convert time_t to a string
	ctime_r(&now, dbgtime);
    fprintf(stderr,"Openflow: %s%s\n", dbgtime, msg);
}

void OOutput::printf(const char *message, ...)
{
	const int STACK_BUFFER_SIZE = 256;
    char *buffer = (char *)malloc(STACK_BUFFER_SIZE);
    if (buffer != NULL) 
	{
		memset(buffer, 0, STACK_BUFFER_SIZE);
	  	va_list arg_ptr;
	  	va_start(arg_ptr, message);

	  	int ret = vsnprintf(buffer, STACK_BUFFER_SIZE, message, arg_ptr);
	  	va_end(arg_ptr);
	  	if ( -1 != ret) 
		{
	   		f_(buffer);
	  	}

	  	free(buffer);
	} 
	else
	{
		return;
 	}
}

}}//: namespace
