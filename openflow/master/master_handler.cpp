// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhengeek@163.com>
// Created: 2014-06-08
// Description:
//  Using rpc interface to process job.
#include <glog/logging.h>
#include "master_handler.h"
#include "master_core.h"

namespace openflow { namespace master {

CMasterHandler::CMasterHandler() {}

int32_t CMasterHandler::submit_job(const int32_t job_id) {
    CMasterCore core;

    LOG(INFO) << "received a job.";
    LOG(INFO) << "job id: " << job_id;

    //fetch job from database by job ID
    if( false == core.fetch_job(job_id) )
    {
        LOG(ERROR) << "Fail to fectch job(" << job_id << ") from database.";
        return -1;
    }

    LOG(INFO)  << "print job...";
    core.print_job(job_id);

    //parse job into tasks
    if( false == core.parse_job(job_id) )
    {
        LOG(ERROR) << "Fail to fectch job(" << job_id << ") from database.";
        return -1;
    }
    LOG(INFO) << "print tasks...";
    core.print_tasks(job_id);

    return 0;
}

int32_t CMasterHandler::stop_job(const int32_t id) {
    // Your implementation goes here
    return 0;
}

int32_t CMasterHandler::kill_job(const int32_t id) {
    // Your implementation goes here
    return 0;
}

int32_t CMasterHandler::report_task_state(const int32_t state) {
    // Your implementation goes here
    return 0;
}

}} // end openflow::master

