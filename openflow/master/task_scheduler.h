#ifndef OPENFLOW_MASTER_TASK_SCHEDULER_H
#define OPENFLOW_MASTER_TASK_SCHEDULER_H
#pragma once
#include <iostream>
#include <vector>
#include <boost/unordered_map.hpp>
#include "rpc/master/MasterService.h"
namespace openflow { namespace master {
/**
 * What to do:
 * 1. assign task to corresponding agent;
 */
class CTaskScheduler{
public:
    CTaskScheduler() {}
    ~CTaskScheduler() {}

    int exec_job(const int32_t job_id, std::vector<openflow::task_info>& tasks);

    void print_job(const int32_t job_id);
    void print_tasks(const int32_t job_id);
};
}} //end namespace openflow::master
#endif // MASTER_CORE_H
