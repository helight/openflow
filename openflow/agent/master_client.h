// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-13
// Description: 客户端操作类的二次封装
//

#ifndef OPENFLOW_AGENT_MASTER_CLIENT_H
#define OPENFLOW_AGENT_MASTER_CLIENT_H
#pragma once

#include <thrift_helper.h>
#include "../config.h"
#include "rpc/master/MasterService.h"

namespace openflow { namespace master {

class CMasterClient
{
public:
    // 初始化
    CMasterClient(const std::string& host, uint16_t port);
    ~CMasterClient();
    //向客户端提交task
    int32_t report_task_state(const openflow::task_state& state);
private:
    //传递给客户端的task
    common::CThriftClientHelper<openflow::master::MasterServiceClient> *_master;
};
}} //end namespace
#endif
