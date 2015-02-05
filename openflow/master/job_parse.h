#ifndef OPENFLOW_MASTER_JOB_PARSE_H
#define OPENFLOW_MASTER_JOB_PARSE_H
#pragma once
#include <iostream>
#include <vector>
#include <boost/unordered_map.hpp>
#include "rpc/master/MasterService.h"
namespace openflow { namespace master {
/**
 * What to do:
 * 1. read job from DB by job ID;
 * 2. parse job xml into tasks;
 * 3. assign task to corresponding agent;
 */
class CJobParse
{
public:
    CJobParse() {}

public:
    //FIXME: Now just print out tasks.
    bool parse_job(const int32_t job_id, std::vector<openflow::task_info>& job_tasks);
    void print_job(const int32_t job_id);
    void print_tasks(const int32_t job_id);

private:
    bool fetch_job(const int32_t job_id, openflow::job_info& info);
};

}} //end namespace openflow::master
#endif // OPENFLOW_MASTER_JOB_PARSE_H
