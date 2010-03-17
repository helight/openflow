// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhengeek@163.com>
// Created: 2014-06-08
// Description:
//  Using rpc interface to process job.
#include <time.h>
#include <string>
#include <map>
#include <boost/format.hpp>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "master_handler.h"
#include "job_scheduler.h"
#include "../config.h"

namespace openflow { namespace master {

CMasterHandler::CMasterHandler() {}

int32_t CMasterHandler::submit_job(const int32_t job_id)
{
    //push job id into blocking queue.
    CJobScheduler& job_scheduler =
        boost::serialization::singleton<CJobScheduler>::get_mutable_instance();

    return job_scheduler.submit_job(job_id);
}

int32_t CMasterHandler::stop_job(const int32_t id)
{
    // Your implementation goes here
    return 0;
}

int32_t CMasterHandler::kill_job(const int32_t id)
{
    // Your implementation goes here
    return 0;
}

int32_t CMasterHandler::report_agent_state(const openflow::agent_state &state)
{
    CJobScheduler& job_scheduler =
        boost::serialization::singleton<CJobScheduler>::get_mutable_instance();

    return job_scheduler.report_agent_state(state);
}

int32_t CMasterHandler::report_task_state(const openflow::task_state &state)
{

    CJobScheduler& job_scheduler =
        boost::serialization::singleton<CJobScheduler>::get_mutable_instance();

    return job_scheduler.report_task_state(state);
}

void CMasterHandler::get_current_jobinfo(openflow::execute_jobinfo& _return)
{
    _return.current_jobnum = 1;
    _return.failure_jobnum = 1;
    _return.done_jobnum = 1;
    _return.success_jobnum = 1;
    LOG(INFO) << "exexute...............";
}

void CMasterHandler::get_agent_info(std::vector<openflow::agent_state>& _return)
{
    CJobScheduler& job_scheduler =
        boost::serialization::singleton<CJobScheduler>::get_mutable_instance();
    int32_t ret = job_scheduler.get_agent_state(_return);

    LOG(INFO) << "agent num: " << ret;
}

}} // end openflow::master

