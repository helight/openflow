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

namespace openflow { namespace agent{

class CMasterClient
{
public:
    // 初始化
    CMasterClient();
    ~CMasterClient();

public:
    bool init(const std::string& host, uint16_t port);

    // 向master端上报task状态
    int32_t report_task_state(const openflow::task_state& state);

    // 向master上报心跳
    int32_t report_agent_state(const openflow::agent_state &state);

private:
    bool _is_init;
    uint16_t _port;
    std::string _host;
    common::CThriftClientHelper<openflow::master::MasterServiceClient> *_master;
};
}} //end namespace
#endif
