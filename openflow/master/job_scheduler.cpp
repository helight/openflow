// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/typeof/typeof.hpp>
#include <glog/logging.h>
#include <thrift/transport/TTransportException.h>
#include "../config.h"
#include "job_scheduler.h"
#include "job_parse.h"

namespace openflow { namespace master {

void CJobScheduler::scheduler_thread()
{
    while (true)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        int32_t job_id;
        //get job id from blocking queue.
        _wait_jobs_queue.pop_front(job_id);
        LOG(INFO) << "received a job id: " << job_id;
        if (_run_jobs_queue.find(job_id) != _run_jobs_queue.end())
        {
            LOG(INFO) << "aleardy run this job: " << job_id;
            continue;
        }

        // start to scheduler
        LOG(INFO) << "push job_id into execute queue";
        CTaskScheduler *task_scheduler = new CTaskScheduler(job_id);
        task_scheduler->start_scheduler();
        {
            boost::mutex::scoped_lock lock(_mutex);
            _run_jobs_queue[job_id] = task_scheduler;
        }
    }
}

void CJobScheduler::clean_thread()
{
    LOG(INFO) << "do clean";
    while (true)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        {
            boost::mutex::scoped_lock lock(_mutex);
            std::map<int32_t, CTaskScheduler*>::iterator it = _run_jobs_queue.begin();
            for (; it != _run_jobs_queue.end();)
            {
                CTaskScheduler *task_scheduler = (CTaskScheduler*)it->second;
                if (task_scheduler->is_finished())
                {
                    LOG(INFO) << "this job finished: " << it->first;
                    _run_jobs_queue.erase(it++);
                    delete task_scheduler;
                }
            }
        }
    }
}

int32_t CJobScheduler::submit_job(const int32_t job_id)
{
    //push job id into blocking queue.
    int ret = 0;
    if (!_wait_jobs_queue.push_back(job_id))
        ret = MASTER_JOB_OVER_MAX;

    return ret;
}

int32_t CJobScheduler::report_task_state(const openflow::task_state &state)
{
    int32_t ret = -1;
    boost::mutex::scoped_lock lock(_mutex);
    std::map<int32_t, CTaskScheduler*>::iterator it = _run_jobs_queue.find(state.job_id);
    if (it != _run_jobs_queue.end())
    {
        CTaskScheduler* task_scheduler = it->second;
        if (task_scheduler->update_task_state(state))
        {
            ret = 0;
        }
        else
        {
            LOG(ERROR) << "task state update error";
        }
    }

    return ret;
}

int32_t CJobScheduler::report_agent_state(const openflow::agent_state &state)
{
    std::cout << "receive ok" <<std::endl;

    boost::mutex::scoped_lock lock(_agent_mutex);
    _agent_state[state.ipaddr] = state;

    return 0;
}

int32_t CJobScheduler::get_agent_state(std::vector<openflow::agent_state>& agent_state)
{
    boost::mutex::scoped_lock lock(_agent_mutex);
    std::map<std::string, openflow::agent_state>::iterator it = _agent_state.begin();
    for (; it != _agent_state.end(); ++it)
    {
        agent_state.push_back(it->second);
    }

    return _agent_state.size();
}

}} //enf namespace openflow::master
