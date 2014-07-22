// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#ifndef OPENFLOW_MASTER_HANDLER_H
#define OPENFLOW_MASTER_HANDLER_H
#pragma once

#include "rpc/master/MasterService.h"
#include "blocking_queue.h"

namespace openflow { namespace master {

class CMasterHandler : public MasterServiceIf
{
public:
    CMasterHandler();

    int32_t submit_job(const  int32_t job_id);
    int32_t stop_job(const int32_t id);
    int32_t kill_job(const int32_t id);
    int32_t report_task_state(const int32_t state);

    //threads function.
    //fetch and parse job.
    void process_job_func(void);
    //distribute the tasks of an job to agents.
    void dist_tasks_func(void);

private:
    common::CBlockingQueue<int32_t> _job_ids;  //job id queue.
};

}} // end openflow::master

#endif // OPENFLOW_MASTER_HANDLER_H
