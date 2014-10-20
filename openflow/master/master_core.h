#ifndef MASTER_CORE_H
#define MASTER_CORE_H
#pragma once
#include <iostream>
#include <vector>
#include <boost/unordered_map.hpp>
#include "rpc/master/MasterService.h"

namespace openflow { namespace master {
//FIXME:struct StTask needs to be removed later.
//FIXME:struct StTask needs to remove change to task_info
struct StTask
{
    std::string name;
    std::string description;
    std::string nodes;
    std::string cmd;
};
/**
 * What to do:
 * 1. read job from DB by job ID;
 * 2. parse job xml into tasks;
 * 3. assign task to corresponding agent;
 */
class CMasterCore{
public:
    CMasterCore()
    {}
    ~CMasterCore()
    {}

    bool fetch_job(const int32_t job_id);
    //FIXME: Now just print out tasks.
    bool parse_job(const int32_t job_id);
    void print_job(const int32_t job_id);
    void print_tasks(const int32_t job_id);
    int exec_job(const int32_t job_id);

private:
    typedef boost::unordered_map<int32_t, openflow::job_info> id_job_map;
    //FIXME: xml_desc double stored in both _jobs and _tasks.
    id_job_map _jobs;
//   openflow::task_info task;
    //FIXME: should keep tasks in orignal sequence.
    typedef std::map<int32_t, std::vector<openflow::task_info> > id_tasks_map;
    id_tasks_map _tasks;
};
}} //end namespace openflow::master
#endif // MASTER_CORE_H
