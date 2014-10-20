// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//

#ifndef OPENFLOW_MASTER_CONN_H
#define OPENFLOW_MASTER_CONN_H
#pragma once

#include <glog/logging.h>
#include <thrift_helper.h>
#include "../config.h"
#include "rpc/agent/AgentService.h"

namespace openflow { namespace master {


	class CmasterConn
	{
		public:
			//初始化
			CmasterConn(const std::string& host, uint16_t port);
			~CmasterConn();
			//设置传递给客户端的task
			void SetTask(const openflow::task_info& Task);
			//向客户端提交task
		//	bool receive_task(const openflow::task_info& task);
			int32_t execute_task(const openflow::task_info &task);
		private:
			//传递给客户端的task
			openflow::task_info task;
			//客户端操作类
			common::CThriftClientHelper<openflow::agent::AgentServiceClient> *Agent;
			
			
			
	};
   }
}
#endif
