// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#ifndef OPENFLOW_AGENT_TASK_MONITOR_H
#define OPENFLOW_AGENT_TASK_MONITOR_H
#pragma once

namespace openflow { namespace agent {

class CTaskMonitor
{
public:
	CTaskMonitor();

public:
    void signal_thread();

private:
    int wait_signal() const;
    void on_executor_terminated();
    void on_task_end(pid_t task_pid, int exited_code);

private:
    volatile bool _stop;
};

} } //openflow::gent
#endif
