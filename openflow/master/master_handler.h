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
#include <map>
#include <time.h>
#include <string>

namespace openflow { namespace master {

class CMasterHandler : public MasterServiceIf
{
public:
    CMasterHandler();

    int32_t submit_job(const int32_t job_id);
    int32_t stop_job(const int32_t id);
    int32_t kill_job(const int32_t id);

    int32_t report_task_state(const openflow::task_state &state);
    int32_t report_agent_state(const openflow::agent_state &state);

    void get_current_jobinfo(openflow::execute_jobinfo& _return);
    void get_agent_info(std::vector<openflow::agent_state>& _return);
};

}} // end openflow::master

#endif // OPENFLOW_MASTER_HANDLER_H
