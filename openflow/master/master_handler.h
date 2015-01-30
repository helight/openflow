// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#ifndef OPENFLOW_MASTER_HANDLER_H
#define OPENFLOW_MASTER_HANDLER_H
#pragma once

#include <boost/shared_ptr.hpp>
#include "rpc/master/MasterService.h"
#include "blocking_queue.h"
#include "master_core.h"
#include <map>
#include <time.h>
#include <string>

namespace openflow { namespace master {

class CMasterHandler : public MasterServiceIf
{
public:
    CMasterHandler();
    ~CMasterHandler();

    int32_t submit_job(const  int32_t job_id);
    int32_t stop_job(const int32_t id);
    int32_t kill_job(const int32_t id);
    void get_current_jobinfo(openflow::execute_jobinfo& _return);    
    int32_t report_agent_state(const openflow::agent_state &state);
    int32_t report_task_state(const openflow::task_state &state);
    int32_t store_state();
    //threads function.
    //fetch and parse job.
    void process_job_func(void);
    //distribute the tasks of an job to agents.
    void dist_tasks_func(void);

private:
    common::CBlockingQueue<int32_t> _job_ids;  //job id queue.
    common::CBlockingQueue<int32_t> _execute_queue; //execute queue
    boost::shared_ptr<boost::thread> process_job_thread;
    boost::shared_ptr<boost::thread> process_tasks_thread;
    boost::shared_ptr<boost::thread> process_state_thread;
    std::map<std::string,openflow::agent_state> state_map;
    std::map<std::string,time_t> timexpire;
    CMasterCore core; //share tasks in two thread (process_job_func and dist_tasks_func)
};

}} // end openflow::master

#endif // OPENFLOW_MASTER_HANDLER_H
