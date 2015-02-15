// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <boost/serialization/singleton.hpp>
#include <glog/logging.h>
#include <thrift/TApplicationException.h>
#include <thrift_helper.h>
#include <utils.h>
#include "../config.h"
#include "master_client.h"
#include "task_execute.h"

DECLARE_int32(max_tasks_num);
namespace openflow { namespace agent {

bool CTaskExecute::init()
{
    if (!_is_init)
    {
        _local_ip = common::CUtils::get_local_ip("eth0");
        if (_local_ip.empty())
        {
            LOG(FATAL) << "can't get local ip " << _local_ip;
            return false;
        }
    }

    return true;
}

int32_t CTaskExecute::start_task(const openflow::task_info& task_info)
{
    LOG(INFO) << "start handling task task id: " << task_info.task_id
        << " task name: " << task_info.task_name;

    if (over_max_tasks_num())
    {
        LOG(WARNING) << "over max num tasks task id:" << task_info.task_id
            << " task name: " << task_info.task_name;
        return AGENT_TASK_OVER_MAX;
    }

    CTask *task =  new CTask(task_info);

    // run this task
    try
    {
        task->start();
    }
    catch (apache::thrift::TException &ex)
    {
        LOG(ERROR) << "task start error " << ex.what();
        throw ex;
    }

    {
        boost::mutex::scoped_lock lock(_mutex);
        _pid_tasks.insert(std::make_pair(task->get_pid(), task));
    }

    return 0;
}

bool CTaskExecute::over_max_tasks_num()
{
    boost::mutex::scoped_lock lock(_mutex);
    return _pid_tasks.size() >= (uint32_t)FLAGS_max_tasks_num;
}

int32_t CTaskExecute::update_task_state(const int32_t task_pid, const int32_t state)
{
    int32_t ret = -1;
    LOG(INFO) << "task pid " << task_pid << " state change: " << state;
    boost::mutex::scoped_lock lock(_mutex);
    std::map<pid_t, CTask*>::iterator it = _pid_tasks.find(task_pid);
    if (it != _pid_tasks.end())
    {
        CTask *task = it->second;
        openflow::task_state task_state;
        task_state.uuid = task->get_uuid();
        task_state.job_id = task->get_job_id();
        task_state.task_status = state;

        CMasterClient& master_client =
            boost::serialization::singleton<CMasterClient>::get_mutable_instance();
        ret = master_client.report_task_state(task_state);
    }
    else
    {
        LOG(INFO) << "cann't find this task: " << task_pid;
        ret = 0;
    }

    return ret;
}

void CTaskExecute::delete_task_by_pid(const int32_t task_pid)
{
    LOG(INFO) << "task finished pid: " << task_pid;
    boost::mutex::scoped_lock lock(_mutex);
    std::map<pid_t, CTask*>::iterator it = _pid_tasks.find(task_pid);
    if (it != _pid_tasks.end())
    {
        CTask *task = it->second;
        _pid_tasks.erase(it);
        ++_finished_count;
        delete task;
    }
}

void CTaskExecute::report_heart_beat_thread()
{
    while (true)
    {
        LOG(INFO) << "do report heartbeat to master";
        boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        // @todo report to master
        openflow::agent_state agent_state;
        {
            boost::mutex::scoped_lock lock(_mutex);
            agent_state.running_task_num = _pid_tasks.size();
        }
        agent_state.ipaddr = _local_ip;
        agent_state.finished_task_num = _finished_count;
        agent_state.last_time = time(NULL);

        LOG(INFO) << "running task num: " << agent_state.running_task_num
            << " finished task num: " << agent_state.finished_task_num;

        CMasterClient& master_client =
            boost::serialization::singleton<CMasterClient>::get_mutable_instance();
        master_client.report_agent_state(agent_state);
    }
}

}} // end openflow agent
