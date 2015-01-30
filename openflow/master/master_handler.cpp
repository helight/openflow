// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhengeek@163.com>
// Created: 2014-06-08
// Description:
//  Using rpc interface to process job.
#include <boost/format.hpp>
#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>
#include "master_handler.h"
#include "master_core.h"
#include "master_conn.h"
#include "master_opdb.h"
#include "../config.h"
#include <time.h>

namespace openflow { namespace master {

CMasterHandler::CMasterHandler() {
    process_job_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::BOOST_BIND(&CMasterHandler::process_job_func
        , this)));
    process_tasks_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::BOOST_BIND(&CMasterHandler::dist_tasks_func
        , this)));
    process_state_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::BOOST_BIND(&CMasterHandler::store_state
        , this)));
}

CMasterHandler::~CMasterHandler() {
    process_job_thread->join();
    process_tasks_thread->join();
    process_state_thread->join();
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

void CMasterHandler::get_current_jobinfo(openflow::execute_jobinfo& _return){
    
    _return.current_jobnum = 1;
    _return.failure_jobnum = 1;
    _return.done_jobnum = 1;
    _return.success_jobnum = 1;
    return;
}

int32_t CMasterHandler::report_agent_state(const openflow::agent_state &state) {
    // Your implementation goes here
    // FIXME ZhangYiFei
    // this function need to do
    // 1. Called by agent
    // 2. agent transfer state struct to master
    // 3. decomposition the state struct
    // 4. store to sqilte3
    state_map.insert(std::pair<std::string,openflow::agent_state>(state.ipaddr,state));
    time_t now_time;
    now_time = time(NULL);
    timexpire.insert(std::pair<std::string,time_t>(state.ipaddr,now_time));
    std::cout << "receive ok" <<std::endl;
    return 0;
}

int32_t CMasterHandler::report_task_state(const openflow::task_state &state) {
    
	CMasterDB db(common::DB_SQLITE,openflow::OPENFLOW_DB_DBNAME);
        if(false == db.connect("../web/database/openflow.db")) {
		LOG(ERROR) << "connect to db error";
		return -1;
   	}

       if(false == db.optable(openflow::OPENFLOW_DB_TASKSTATETABLENAME)) {
    		LOG(ERROR) << "open table error";
		return -2;
        }

      std::string sql = boost::str(boost::format("UPDATE  TaskState SET task_status='%d',task_result='%s' where job_id='%d' and task_id='%d';")
				% state.task_status %state.task_result %state.job_id %state.task_id);
      if(false == db.execute(sql)) {
	LOG(ERROR) << "execut task state receive sql error";
	return -3;
     }
    db.close();
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

//thread function
int32_t CMasterHandler::store_state(){

 	CMasterDB db(common::DB_SQLITE,openflow::OPENFLOW_DB_DBNAME);
        if(false == db.connect("../web/database/openflow.db")) {
		LOG(ERROR) << "connect to db error";
		return -1;
   	}

    	if(false == db.optable(openflow::OPENFLOW_DB_AGENTSTATETABLENAME)) {
    		LOG(ERROR) << "open table error";
		return -2;
   	}
	std::cout << "connect to db success" << std::endl;
	while(1){
		
		std::map<std::string,openflow::agent_state>::iterator it;
		std::cout <<"start to deal with data" <<std::endl;
		it = state_map.begin();
		while(it != state_map.end()){
	//输出state 信息到日志 测试用
		  LOG(INFO)<<"INERT INTO "<<(it->second).ipaddr;
   	       	  std::string sql = boost::str(boost::format("INSERT INTO AgentState values('%s','%s','%s','%s','%s','%s');")
				%(it->second).ipaddr %(it->second).remain_mem %(it->second).mem_use_percent %(it->second).cpu_idle_percent %(it->second).cpu_load %(it->second).swap_use_percent);
   	          if(false == db.execute(sql)) {
		     LOG(ERROR) << "execut agent state receive sql error";
		     return -3;
          	  }
             }
	   sleep(60);
        }
 }
}} // end openflow::master

