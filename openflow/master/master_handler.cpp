// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhengeek@163.com>
// Created: 2014-06-08
// Description:
//  Using rpc interface to process job.
#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "master_handler.h"
#include "master_core.h"
#include "master_conn.h"

namespace openflow { namespace master {

CMasterHandler::CMasterHandler() {
    process_job_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::BOOST_BIND(&CMasterHandler::process_job_func
        , this)));
    process_tasks_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::BOOST_BIND(&CMasterHandler::dist_tasks_func
        , this)));
}

CMasterHandler::~CMasterHandler() {
    process_job_thread->join();
}

int32_t CMasterHandler::submit_job(const int32_t job_id) {
    //push job id into blocking queue.
    int ret;
    ret = _job_ids.push_front(job_id);

    return ret;
}

int32_t CMasterHandler::stop_job(const int32_t id) {
    // Your implementation goes here
    return 0;
}

int32_t CMasterHandler::kill_job(const int32_t id) {
    // Your implementation goes here
    return 0;
}

int32_t CMasterHandler::report_task_state(const int32_t state) {
    // Your implementation goes here
    return 0;
}

//thread function.
void CMasterHandler::process_job_func(void) {
    //FIXME: how to destory?
   //   CMasterCore core;

    for(;;)
    {
        int32_t job_id;
        //get job id from blocking queue.
        _job_ids.pop_front(job_id);

        LOG(INFO) << "received a job.";
        LOG(INFO) << "job id: " << job_id;

        //fetch job from database by job ID
        if( false == core.fetch_job(job_id) )
        {
            LOG(ERROR) << "Fail to fectch job(" << job_id << ") from database.";
            continue;
        }

        LOG(INFO)  << "print job...";
        core.print_job(job_id);

        //parse job into tasks
        if( false == core.parse_job(job_id) )
        {
            LOG(ERROR) << "Fail to parse job(" << job_id << ") into tasks.";
            continue;
        }
	//FIXME zhangyifei
	//push jod id into execue queue
	 _execute_queue.push_front(job_id);
	LOG(INFO) << "push job_id into execute queue";
	//core.exec_job(job_id);
    }
}

void CMasterHandler::dist_tasks_func(void) {
    //invoke rpc provided by agent.
    //CMasterCore core;
    int32_t job_id;
    for(;;)
    {
       //get job id from execute queue
    	_execute_queue.pop_front(job_id);
	LOG(INFO) <<"start to execute tasks job_Id:"<<job_id;
    	LOG(INFO) <<core.exec_job(job_id);
    }
}

}} // end openflow::master

