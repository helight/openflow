// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/serialization/singleton.hpp>
#include <glog/logging.h>
#include <dbhelp_factory.h>
#include "../config.h"
#include "agent_client.h"
#include "task.h"

namespace openflow { namespace master {

CTask::CTask(const openflow::task_info* task_info) : _is_finished(false)
{
    _task_info = *task_info;
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

void CTask::set_state(const openflow::task_state &state)
{
    _task_state = state.task_status;
    _ret_msg = state.task_result;
    _is_finished = true;
}

bool CTask::is_finished()
{
    return _is_finished;
}

void CTask::on_job_finished()
{
    common::CSqliteHelp& db_help =
        boost::serialization::singleton<common::CSqliteHelp>::get_mutable_instance();

     std::string sql = boost::str(boost::format(
             "UPDATE  TaskState SET task_status='%d',task_result='%s' "
             "where job_id='%d' and task_id='%d';")
         % _task_state % _ret_msg % _task_info.job_id % _task_info.task_id);

     if(!db_help.update(sql))
     {
         LOG(ERROR) << "execut inert task sql error: " << sql;
     }
}

std::string CTask::get_uuid()
{
    return _task_info.uuid;
}

}} //enf namespace openflow::master
