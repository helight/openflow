// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#ifndef OPENFLOW_AGENT_TASK_H
#define OPENFLOW_AGENT_TASK_H
#pragma once
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <glog/logging.h>
#include <blocking_queue.h>
#include "agent_handler.h"


namespace openflow { namespace agent {
class CTask
{
public:
	CTask(const openflow::task_info& task_info);

public:
    void start();
    void stop();
    void update_state(int32_t state);
    pid_t get_pid() const;
    int32_t get_job_id() const;
    std::string get_uuid() const;

private:
    int32_t task_child_process();

private:
	pid_t _task_pid;
    openflow::task_info _task_info;
    int32_t _task_state;
};

} } //openflow::gent
#endif
