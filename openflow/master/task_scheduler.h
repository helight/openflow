// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#ifndef OPENFLOW_MASTER_TASK_SCHEDULER_H
#define OPENFLOW_MASTER_TASK_SCHEDULER_H
#pragma once
#include <list>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "rpc/master/MasterService.h"
#include "task.h"

namespace openflow { namespace master {
/**
 * What to do:
 * 1. assign task to corresponding agent;
 */
class CTaskScheduler{
public:
    CTaskScheduler(const int32_t job_id);
    ~CTaskScheduler();

    bool start_scheduler();
    void scheduler_thread();
    void monitor_thread();

    bool is_finished();

private:
    void on_job_finished();


private:
    int32_t _job_id;
    bool _is_finished;
    bool _is_scheduled;
    mutable boost::mutex _mutex;
    std::list<std::list<openflow::task_info*>*> _job_tasks;
    std::map<std::string, CTask*> _run_task_queue;

    boost::scoped_ptr<boost::thread> _scheduler_thread;
    boost::scoped_ptr<boost::thread> _monitor_thread;
};
}} //end namespace openflow::master
#endif // MASTER_CORE_H
