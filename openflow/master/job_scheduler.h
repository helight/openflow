#ifndef OPENFLOW_MASTER_JOB_SCHEDULER_H
#define OPENFLOW_MASTER_JOB_SCHEDULER_H
#pragma once
#include <iostream>
#include <vector>
#include <boost/unordered_map.hpp>
#include "blocking_queue.h"
#include "rpc/master/MasterService.h"
namespace openflow { namespace master {


class CJobScheduler{
public:
    CJobScheduler() {}

    void scheduler_thread();
    void clean_thread();

    int32_t submit_job(const int32_t job_id);
    int32_t report_task_state(const openflow::task_state &state);

private:
    common::CBlockingQueue<int32_t> _wait_jobs_queue;  //job id queue.
};
}} //end namespace openflow::master
#endif // MASTER_CORE_H
