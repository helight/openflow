// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#ifndef OPENFLOW_AGENT_TASK_EXECUTE_H
#define OPENFLOW_AGENT_TASK_EXECUTE_H
#pragma once
#include <map>
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <thrift_helper.h>
#include "rpc/master/MasterService.h"
#include "task.h"

namespace openflow { namespace agent {
class CTaskExecute
{
public:
	CTaskExecute();
	~CTaskExecute();
    bool init(const std::string& host, uint16_t port);

	int32_t start_task(const openflow::task_info& task_info);
    int32_t update_task_state(const int32_t task_pid, const int32_t state);
    void delete_task_by_pid(const int32_t task_pid);

    void report_heart_beat_thread();

private:
    bool over_max_tasks_num();

private:
    bool _is_init;
    mutable boost::mutex _mutex;
    uint16_t _port;
    std::string _local_ip;
    std::string _host;
    std::map<pid_t, CTask*> _pid_tasks;
    boost::atomic<uint64_t> _finished_count;
    common::CThriftClientHelper<openflow::master::MasterServiceClient> *_master_client;
};

} } //openflow::gent
#endif
