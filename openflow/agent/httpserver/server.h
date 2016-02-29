// Copyright (c) 2014, OpenFlow, HTTP SERVER
// Author:      David<daijinwei41@gmail.com>
// Created:     2014-11-25
// Modifidd:    2015-02-25
// Description: Oserver, base class
//

#ifndef _OPENFLOW_SERVER_H_
#define _OPENFLOW_SERVER_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace openflow { namespace httpserver{

/**
 * HttpServerBase class 
 **/
class OServer : boost::noncopyable
{
public:
  // Default constructor 
  OServer(){}

  // Destructor
  virtual ~OServer(){}
  virtual void serve() = 0;
  virtual void stop() = 0;
  virtual void run()
  {
    serve();  
  }
};

}} // end namespace openflow::httpserver

#endif //: #ifndef _OPENFLOW_SERVER_H_ ///:~
