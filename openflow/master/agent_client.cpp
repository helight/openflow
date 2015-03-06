// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//
#include <glog/logging.h>
#include "agent_client.h"

namespace openflow { namespace master {

//连接客户端要进行的初始化
CAgentClient::CAgentClient(const std::string& host, uint16_t port)
{
	//客户端操作类的初始化
	_agent = new common::CThriftClientHelper<openflow::agent::AgentServiceClient>(host, port);
	_agent->connect();
}

//关闭到客户端的连接
CAgentClient::~CAgentClient()
{
	_agent->close(); //显示调用close方法;
	delete _agent;
}

//发送任务
int32_t CAgentClient::execute_task(const openflow::task_info& task)
{
    LOG(INFO) << "send task to agent";
	return _agent->get()->execute_task(task);
}

}}
