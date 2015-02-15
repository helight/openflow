// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#include <boost/format.hpp>
#include <boost/serialization/singleton.hpp>
#include <glog/logging.h>
#include "../config.h"
#include "task_scheduler.h"
#include "job_parse.h"

namespace openflow { namespace master {

CTaskScheduler::CTaskScheduler(const int32_t job_id) : _job_id(job_id), _is_finished(false), _is_scheduled(false)
{}

CTaskScheduler::~CTaskScheduler()
{
    if (_is_scheduled)
    {
        LOG(INFO) << "to release task scheduler";
        _scheduler_thread->join();
    }

    on_job_finished();
}

bool CTaskScheduler::start_scheduler()
{
    CJobParse& job_parse = boost::serialization::singleton<CJobParse>::get_mutable_instance();
    //parse job into tasks
    if(false == job_parse.parse_job(_job_id, _job_tasks))
    {
        LOG(ERROR) << "Fail to parse job(" << _job_id << ") into tasks.";
        return false;
    }

    _scheduler_thread.reset(
        new boost::thread(boost::bind(&CTaskScheduler::scheduler_thread, this)));

    return true;
}

void CTaskScheduler::scheduler_thread()
{
    _is_scheduled = true;
    while (!_job_tasks.empty())
    {
        // create task
        std::list<CTask*> wait_task_queue;
        std::list<openflow::task_info*>* task_list = _job_tasks.front();
        for (std::list<openflow::task_info*>::iterator iter = task_list->begin()
             ; iter != task_list->end();)
        {
            openflow::task_info* task_info = *iter;
            CTask *task = new CTask(task_info);
            wait_task_queue.push_back(task);
            delete *iter;
        }
        // schedule task
        while (!wait_task_queue.empty())
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(500));
            std::list<CTask*>::iterator it = wait_task_queue.begin();
            CTask *task = *it;
            if (task->start())
            {
                _run_task_queue[task->get_uuid()] = task;
                wait_task_queue.erase(it++);
            }
            else
            {
                ++it;
            }
        }
        // check is task finished
        while (!_run_task_queue.empty())
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(500));
            {
                boost::mutex::scoped_lock lock(_mutex);
                std::map<std::string, CTask*>::iterator it = _run_task_queue.begin();
                CTask *task = it->second;
                if (task->is_finished())
                {
                    LOG(INFO) << "task finished: ";
                    // @todo update db state
                    _run_task_queue.erase(it++);
                    delete task;
                }
                else
                {
                    ++it;
                }
            }
        }
        _job_tasks.pop_front();
    }

    _is_finished = true;
}

bool CTaskScheduler::update_task_state(const openflow::task_state &state)
{
    bool ret = false;
    boost::mutex::scoped_lock lock(_mutex);
    std::map<std::string, CTask*>::iterator it = _run_task_queue.find(state.uuid);

    if (it != _run_task_queue.end())
    {
        CTask *task = it->second;
        task->set_state(state);
        ret = true;
    }

    return ret;
}

bool CTaskScheduler::is_finished()
{
    return _is_finished;
}

void CTaskScheduler::on_job_finished()
{
    // @todo update db
}

}} //enf namespace openflow::master
