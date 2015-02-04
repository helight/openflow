// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <glog/logging.h>
#include <thrift/TApplicationException.h>
#include "../config.h"
#include "task_execute.h"

DECLARE_int32(max_tasks_num);
namespace openflow { namespace agent {

int32_t CTaskExecute::start_task(const openflow::task_info& task_info)
{
    LOG(INFO) << "start handling...\n"
        << "task id: " << task_info.task_id << std::endl
        << "task name: " << task_info.task_name << std::endl
        << ".............................\n";

    if (over_max_tasks_num())
    {
        LOG(WARNING) << "over max num tasks task id:" << task_info.task_name;
        return AGENT_TASK_OVER_MAX;
    }

    CTask *task =  new CTask(task_info);

    // run this task
    try
    {
        ++_all_task_count;
        task->start();
    }
    catch (apache::thrift::TException &ex)
    {
        LOG(ERROR) << "task start error " << ex.what();
        throw ex;
    }

    _pid_tasks.insert(std::make_pair(task->get_pid(), task));
    return 0;
}

bool CTaskExecute::over_max_tasks_num()
{
    return _pid_tasks.size() >= (uint32_t)FLAGS_max_tasks_num;
}

void CTaskExecute::update_task_state(int32_t task_pid, int32_t state)
{
}

void CTaskExecute::delete_task_by_pid(int32_t task_pid)
{
}

void CTaskExecute::report_heart_beat_thread()
{
    while (true)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

        // @todo report to master
    }
}

}} // end openflow agent
