// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-11-25                                                                                            
// Description:httpserver implemetation 
//
#ifndef	_OPENFLOW_HTTPSERVER_COMMON_H_
#define	_OPENFLOW_HTTPSERVER_COMMON_H_

#include <iostream>
#include <string>
#include <exception>

namespace openflow{ namespace httpserver{

// define the white space 
const std::string kStrWhiteSpace = " \t\n\r\v\f";

// Define the file type
enum FileType
{
	UNKNOWN = 0,
	S_REG,
	S_DIR,
	S_CHR,
	S_BLK,
	S_FIFO,
	S_LNK,
	S_SOCK
}; 

/**
 * Get the file type
 * args: the file path
 * return: return file type
 */
int file_type(const char *file_path);

/**
 * Removes white Spaces at the string of the fore and last 
 * args: 	the orignal string
 * return:	the result of string 
 */
std::string strip(const std::string &input_string);

/**
 * Http server output message
 * Log modules
 */
class OOutput 
{
 public:
 OOutput() : f_(&errorTimeWrapper){}
   // Set the function ptr                                                                                   
   inline void setOutputFunction(void (*function)(const char *)) 
   {
     f_ = function;
   }

   // Set thr function prt arguments                                                                         
   inline void operator()(const char *message) 
   {
     f_(message);
   }

   void perror(const char *message, int error_copy);
   inline void perror(const std::string &message, int error_copy)
   {
     perror(message.c_str(), error_copy)     ;
   }

   void printf(const char *message, ...);
   static void errorTimeWrapper(const char *msg);
   static std::string strerror_s(int errno_copy);
 private:
   void (*f_)(const char *);
 };

 extern OOutput GlobalOutput;

/**                                                                                                               
  * Define the openflow exception                                                                                  
  */
class OException : public std::exception
{
 public:
   OException() throw():
   message_(){}
   OException(const std::string& message) throw():
   message_(message){}
   virtual ~OException() throw(){}
   // Overwrite what function                                                                                
   virtual const char *what()const throw()
   {
     if(message_.empty()) 
	 {
       return "Default OException";
     } 
	 else 
	 {
       return message_.c_str();
     }
   }
 protected:
   std::string message_;
};

}}//namespace

#endif //: #ifndef	_OPENFLOW_HTTPSERVER_COMMON_H_ ///:~
