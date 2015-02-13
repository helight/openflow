#ifndef OPENFLOW_MASTER_TASK_H
#define OPENFLOW_MASTER_TASK_H
#pragma once
#include "rpc/master/MasterService.h"
namespace openflow { namespace master {
/**
 * What to do:
 * 1. assign task to corresponding agent;
 */
class CTask{
public:
    CTask(const openflow::task_info* task_info);
    ~CTask();

    bool start();
    bool is_finished();
    int32_t get_task_id();
    std::string get_uuid();

private:
    void on_job_finished();

private:
    int32_t _task_id;
    openflow::task_info _task_info;
    bool _is_finished;
    bool _is_scheduled;
};
}} //end namespace openflow::master
#endif // OPENFLOW_MASTER_TASK_H
