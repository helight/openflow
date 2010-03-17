// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#ifndef OPENFLOW_MASTER_JOB_SCHEDULER_H
#define OPENFLOW_MASTER_JOB_SCHEDULER_H
#pragma once
#include <iostream>
#include <vector>
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>
#include "blocking_queue.h"
#include "task_scheduler.h"
#include "rpc/master/MasterService.h"

namespace openflow { namespace master {

class CJobScheduler{
public:
    CJobScheduler() {}

    void scheduler_thread();
    void clean_thread();

    int32_t submit_job(const int32_t job_id);
    int32_t report_task_state(const openflow::task_state &state);
    int32_t report_agent_state(const openflow::agent_state &state);

public:
    int32_t get_agent_state(std::vector<openflow::agent_state>& agent_state);

private:
    mutable boost::mutex _mutex;
    common::CBlockingQueue<int32_t> _wait_jobs_queue;  //job id queue.
    std::map<int32_t, CTaskScheduler*> _run_jobs_queue;

private:
    mutable boost::mutex _agent_mutex;
    std::map<std::string, openflow::agent_state> _agent_state;
};
}} //end namespace openflow::master
#endif // MASTER_CORE_H
