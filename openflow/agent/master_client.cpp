// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//
#include <glog/logging.h>
#include "master_client.h"

namespace openflow { namespace master {

//连接客户端要进行的初始化
CMasterClient::CMasterClient(const std::string& host, uint16_t port)
{
	//客户端操作类的初始化
	_master = new common::CThriftClientHelper<openflow::master::MasterServiceClient>(host, port);
	_master->connect();
}

//关闭到客户端的连接
CMasterClient::~CMasterClient()
{
	_master->close(); //显示调用close方法;
	delete _master;
}

// 发送任务
int32_t CMasterClient::report_task_state(const openflow::task_state& state)
{
	return _master->get()->report_task_state(state);
}

int32_t CMasterClient::report_agent_state(const openflow::agent_state &state)
{
    return _master->get()->report_agent_state(state);
}

}}
