// Copyright (c) 2014, OpenFlow, HTTP SERVER                                                                      
// Author:  David<daijinwei41@gmail.com>                                                                          
// Created: 2014-12-25                                                                                            
// Description:httpserver app interface
//
#ifndef _OPENFLOW_APP_H_
#define _OPENFLOW_APP_H_

namespace openflow { namespace httpserver { namespace application{

/**
 * The base class OApp
 */
class OApp
{
 public:
  OApp(){}
  virtual ~OApp(){}
};

}}} //openflow::httpserver::application

#endif	//: #ifndef _OPENFLOW_APP_H_ ///:~
