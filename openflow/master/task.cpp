// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include "../config.h"
#include "agent_client.h"
#include "task.h"

namespace openflow { namespace master {

CTask::CTask(const openflow::task_info* task_info) : _is_finished(false)
{
    _task_info = *task_info;
    _task_id = _task_info.task_id;
}

CTask::~CTask()
{
    on_job_finished();
}

bool CTask::start()
{
    boost::trim(_task_info.nodes);
    LOG(INFO) << "Task name: " << _task_info.name << " Task description: " << _task_info.description
        << " Task nodes: " << _task_info.nodes << " Task command: " << _task_info.cmd
        << " task task_name" << _task_info.task_name << " task task_id" << _task_info.task_id << " over";
    try
    {
        CAgentClient agent((_task_info.nodes).c_str(),openflow::OPENFLOW_AGENT_HANDLER_PORT);
        int32_t ret = agent.execute_task(_task_info);
        LOG(INFO) << "send task to agent: " << ret;
    }
    catch(apache::thrift::TException &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }
    LOG(INFO)<<"for execute end";
    return true;
}

bool CTask::is_finished()
{
    return _is_finished;
}

void CTask::on_job_finished()
{

}

}} //enf namespace openflow::master
