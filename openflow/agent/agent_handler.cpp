// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com> helight<helight@zhwen.org>
// Created: 2014-07-03
// Description:
// deal with tasks received from master
#include <glog/logging.h>
#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "agent_handler.h"
#include "task_execute.h"

namespace openflow { namespace agent {
CAgentHandler::CAgentHandler() {}

CAgentHandler::~CAgentHandler() {}

int32_t CAgentHandler::execute_task(const openflow::task_info &task)
{
    CTaskExecute& task_excutor =
        boost::serialization::singleton<CTaskExecute>::get_mutable_instance();

    return task_excutor.start_task(task);
}

/*agent测试用：显示当前执行的任务*/
int32_t CAgentHandler::show_running_task()
{
    return 0;
}

/*中止一个task*/
int32_t CAgentHandler::kill_task(const openflow::task_info &task)
{
    return 0;
}

/*上报自身状态信息*/
int32_t CAgentHandler::report_status()
{
    return 0;
}

} }

