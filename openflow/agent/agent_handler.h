// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com> helight<helight@zhwen.org>
// Created: 2014-07-03
// Description:
//

#ifndef OPENFLOW_AGENT_HANDLER_H
#define OPENFLOW_AGENT_HANDLER_H
#pragma once

#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "blocking_queue.h"
#include "rpc/agent/AgentService.h"

// Set default value
const uint32_t kForkMax = 10;
const uint32_t kForkCnt = 0;

namespace openflow { namespace agent {

class CAgentHandler : public AgentServiceIf
{
public:
    CAgentHandler();
    ~CAgentHandler();

    int32_t execute_task(const openflow::task_info &task);

    /*agent测试用：显示当前执行的任务*/
    int32_t show_running_task();

    /*中止一个task*/
    int32_t kill_task(const openflow::task_info &task);

    /*上报自身状态信息*/
    int32_t report_status();

};

} }

#endif
