// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>
// Created: 2014-06-29
// Description:
//
#include <boost/format.hpp>
#include <glog/logging.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/algorithm/string.hpp>
#include <thrift/transport/TTransportException.h>
#include <boost/algorithm/string.hpp>
#include "../common/table.h"
#include "../common/database.h"
#include "../common/dataset.h"
#include "../config.h"
#include "job_scheduler.h"
#include "job_parse.h"
#include "agent_client.h"
#include "master_opdb.h"

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

        std::vector<openflow::task_info> job_tasks;
        CJobParse& job_parse = boost::serialization::singleton<CJobParse>::get_mutable_instance();
        //parse job into tasks
        if(false == job_parse.parse_job(job_id, job_tasks))
        {
            LOG(ERROR) << "Fail to parse job(" << job_id << ") into tasks.";
            continue;
        }
        LOG(INFO) << "push job_id into execute queue";
    }
}

void CJobScheduler::clean_thread()
{
    while (true)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        // @todo
        LOG(INFO) << "do clean";
    }
}

int32_t CJobScheduler::submit_job(const int32_t job_id)
{
    //push job id into blocking queue.
    int ret;
    ret = _wait_jobs_queue.push_front(job_id);
    return ret;
}

int32_t CJobScheduler::report_task_state(const openflow::task_state &state)
{
    // CMasterDB db(common::DB_SQLITE,openflow::OPENFLOW_DB_DBNAME);
    // if(false == db.connect("../web/database/openflow.db"))
    // {
    //     LOG(ERROR) << "connect to db error";
    //     return -1;
    // }

    // if(false == db.optable(openflow::OPENFLOW_DB_TASKSTATETABLENAME))
    // {
    //     LOG(ERROR) << "open table error";
    //     return -2;
    // }

    // std::string sql = boost::str(boost::format(
    //         "UPDATE  TaskState SET task_status='%d',task_result='%s' "
    //         "where job_id='%d' and task_id='%d';")
    //     % state.task_status % state.task_result % state.job_id % state.task_id);
    // if(false == db.execute(sql))
    // {
    //     LOG(ERROR) << "execut task state receive sql error";
    //     return -3;
    // }
    // db.close();
    //
    return 0;
}

}} //enf namespace openflow::master
