/*
 * =====================================================================================
 *
 *       Filename:  agent_conn.cpp
 *        Version:  1.0
 *        Created:  2014年11月27日 16时31分33秒
 *       Revision:  none
 *       Compiler:  g++
 *         Author:  zomoe, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <thrift/transport/TTransportException.h>
#include "agent_conn.h"

namespace openflow { namespace agent {

CagentConn::CagentConn(const std::string& host,\
			uint16_t port)
{
	Master = new common::CThriftClientHelper<openflow::master::MasterServiceClient>(host,port);
	Master->connect();
}

CagentConn::~CagentConn()
{
	Master->close();
	delete Master;
}

void CagentConn::SetInfo(const openflow::agent_state & State)
{
	state = State;
}

int32_t CagentConn::report_status(const openflow::agent_state &State)
{
	SetInfo(State);
	int32_t ret = (*Master)->report_agent_state(state);
}
}} //end namespace
