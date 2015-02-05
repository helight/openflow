// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//
#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <thrift/transport/TTransportException.h>
#include "agent_client.h"

namespace openflow { namespace master {

//连接客户端要进行的初始化
CAgentClient::CAgentClient(const std::string& host, uint16_t port)
{
	//客户端操作类的初始化
	Agent = new common::CThriftClientHelper<openflow::agent::AgentServiceClient>(host, port);
	Agent->connect();
}

//关闭到客户端的连接
CAgentClient::~CAgentClient()
{
	Agent->close(); //显示调用close方法;
	delete Agent;
}

//发送任务
int32_t CAgentClient::execute_task(const openflow::task_info& task)
{
	return (*Agent)->execute_task(task);
}

}}
