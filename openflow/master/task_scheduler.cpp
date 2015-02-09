// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>
// Created: 2014-06-29
// Description:
//
#include <boost/format.hpp>
#include <glog/logging.h>
#include <boost/serialization/singleton.hpp>
#include <boost/algorithm/string.hpp>
#include <thrift/transport/TTransportException.h>
#include <boost/algorithm/string.hpp>
#include "../config.h"
#include "task_scheduler.h"
#include "agent_client.h"

namespace openflow { namespace master {

void CTaskScheduler::print_job(const int32_t job_id)
{
}

void CTaskScheduler::print_tasks(const int32_t job_id)
{
}

int CTaskScheduler::exec_job(const int32_t job_id, std::vector<openflow::task_info>& tasks)
{
    openflow::task_info task;

    for(std::vector<openflow::task_info>::iterator it = tasks.begin(); it != tasks.end(); it++)
    {
        try
        {
            boost::trim(it->nodes);
            task.name = it->name;
            task.description = it->description;
            task.nodes = it->nodes;
            task.cmd = it->cmd;
            task.task_name = it->task_name;
            task.task_id = it->task_id;

            LOG(INFO) << "Task name: " << task.name << " Task description: " << task.description
                << " Task nodes: " << task.nodes << " Task command: " << task.cmd
                << " task task_name" <<task.task_name << " task task_id" <<task.task_id << " over";

            LOG(INFO) << "connect to agent success";
            CAgentClient agent((it->nodes).c_str(),openflow::OPENFLOW_AGENT_HANDLER_PORT);
            agent.execute_task(task);
        }
        catch(apache::thrift::TException &e)
        {
            LOG(ERROR) << e.what();
            return -1;
        }
    }
    LOG(INFO)<<"for execute end";
    return 1;
}

}} //enf namespace openflow::master
