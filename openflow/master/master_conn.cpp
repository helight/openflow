// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//
#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "master_conn.h"

namespace openflow { namespace master {

//连接客户端要进行的初始化
CmasterConn::CmasterConn(const std::string& host, uint16_t port)
{
	//客户端操作类的初始化
	Agent = new common::CThriftClientHelper<openflow::agent::AgentServiceClient>(host,port);
	Agent->connect();
}

//关闭到客户端的连接
CmasterConn::~CmasterConn()
{
	Agent->close(); //显示调用close方法;
	delete Agent;
}

//填充发送到客户端的任务
void CmasterConn::SetTask(const openflow::task_info &Task)
{	
	task = Task;
}


//发送任务
/*
bool CmasterConn::receive_task(const openflow::task_info& Task)
{
	return agent->receive_task(task);
}
*/

int32_t CmasterConn::execute_task(const openflow::task_info &Task)
{
	SetTask(Task);
	int32_t ret = (*Agent)->execute_task(task);
	return ret;
}

}}
