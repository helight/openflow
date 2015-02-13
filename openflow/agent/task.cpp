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
    LOG(INFO) << "start task task id: " << _task_info.task_id
        << "task name: " << _task_info.task_name << "...";

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

    LOG(INFO) << "end of fork process to run this app " <<  _task_info.task_name;
}

int32_t CTask::task_child_process()
{
    int32_t fd;

    /*将执行结果保存到以task_id命名的文件中*/
    char id_src[255];
    sprintf(id_src, "%d", _task_info.task_id);
    /*文件路径*/
    char id_des[255] = "../log/";
    strcat(id_des, id_src);
    if( (fd = creat(id_des, 0644)) == -1 )
    {
        LOG(ERROR) << "create log file error: " << id_des;
        return 1;
    }

    /*关闭stdout和stderr，任务执行结果输出到task_id文件中*/
    dup2(fd, 1);
    dup2(fd, 2);
    if (execl("/bin/bash", "bash", "-cx", _task_info.cmd.c_str(), NULL) < 0)
    {
        LOG(ERROR) << "execlp error";
        return 2;
    }

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

int32_t CTask::get_job_id() const
{
    return _task_info.job_id;
}

std::string CTask::get_uuid() const
{
    return _task_info.uuid;
}

}}
