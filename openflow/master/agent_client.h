// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//

#ifndef OPENFLOW_MASTER_MASTER_CLIENT_H
#define OPENFLOW_MASTER_MASTER_CLIENT_H
#pragma once

#include <thrift_helper.h>
#include "../config.h"
#include "rpc/agent/AgentService.h"

namespace openflow { namespace master {

class CAgentClient
{
public:
    CAgentClient(const std::string& host, uint16_t port);
    ~CAgentClient();
    // 向客户端提交task
    int32_t execute_task(const openflow::task_info &task);
private:
    // 传递给客户端的task
    common::CThriftClientHelper<openflow::agent::AgentServiceClient> *_agent;
};
}} //end namespace

#endif // OPENFLOW_MASTER_MASTER_CLIENT_H
