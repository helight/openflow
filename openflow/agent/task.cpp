// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <glog/logging.h>
#include <thrift/TApplicationException.h>
#include "task.h"

namespace openflow { namespace agent {

CTask::CTask(const openflow::task_info& task_info)
    : _task_pid(-1), _task_info(task_info), _task_state(0)
{}

void CTask::start()
{
    LOG(INFO) << "start handling...\n"
        << "task id: " << _task_info.task_id << std::endl
        << "task name: " << _task_info.task_name << std::endl
        << ".............................\n";

    std::string excption_msg = "";

    _task_pid = fork();
    if (-1 == _task_pid)
    {
        excption_msg = "fork " + _task_info.task_name + " error: " + strerror(errno);
        throw apache::thrift::TApplicationException(excption_msg);
    }
    else if (0 == _task_pid)
    {
        exit(task_child_process());
    }

    VLOG(4) << "end of fork process to run this app " <<  _task_info.task_name;
}

int32_t CTask::task_child_process()
{
    return 0;
}

void CTask::stop()
{
}

void CTask::update_state(int32_t state)
{
    _task_state = state;
}

pid_t CTask::get_pid() const
{
    return _task_pid;
}

}}
