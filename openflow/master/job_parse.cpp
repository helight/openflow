// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>, helight<helight@zhwen.org>
// Created: 2014-06-29
// Description:
//
#include <boost/format.hpp>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/serialization/singleton.hpp>
#include <thrift/transport/TTransportException.h>
#include <boost/algorithm/string.hpp>
#include <dbhelp_factory.h>
#include <utils.h>
#include "../config.h"
#include "job_parse.h"

namespace openflow { namespace master {

bool CJobParse::fetch_job(const int32_t job_id, openflow::job_info& info)
{
    common::CSqliteHelp& db_help =
                boost::serialization::singleton<common::CSqliteHelp>::get_mutable_instance();

    std::string sql = boost::str(boost::format("SELECT * FROM tbJobs WHERE job_id=%d;")
                                  % job_id);
    std::vector<std::string> rows;
    int num_rows, num_cols;

    num_rows  = db_help.query(sql, rows, num_cols);
    if ( num_rows < 0)
    {
        LOG(ERROR) << "Query error.";
        return false;
    }

    info.job_id = boost::lexical_cast<int32_t>(rows[num_cols + 0]);
    info.job_name = rows[num_cols + 1];
    info.xml_desc = rows[num_cols + 2];
    info.time = rows[num_cols + 3];

    return true;
}

bool CJobParse::parse_job(const int32_t job_id
                          , std::list<std::list<openflow::task_info*>*>& job_tasks)
{
    openflow::job_info job_info;
    if (!fetch_job(job_id, job_info))
    {
        LOG(ERROR) << "cant't get this job: " << job_id;
        return false;
    }
    boost::property_tree::ptree pt;
    boost::property_tree::ptree root;

    std::stringstream ss;
    ss << job_info.xml_desc;
    boost::property_tree::xml_parser::read_xml(ss, pt);
    root = pt.get_child("job");

    common::CSqliteHelp& db_help =
                boost::serialization::singleton<common::CSqliteHelp>::get_mutable_instance();
    /*
       const std::string task_member[] = {"name","description","nodes", "command"};
       std::vector<StTask> tasks;
       StTask task;
    */
    const std::string task_member[] = {"name","description","nodes", "command"};


    int id = 1;
    for(boost::property_tree::ptree::iterator it = root.begin(); it != root.end(); it++)
    {
        if(it->first == "shell_process")
        {
            std::list<openflow::task_info*> *task_list = new std::list<openflow::task_info*>;
            openflow::task_info *task =  new openflow::task_info;
            boost::property_tree::ptree pt;
            std::string val;
            boost::property_tree::ptree shell_process = it->second;
            int i=0;
            for(; i < 4; i++)
            {
                pt = shell_process.get_child(task_member[i]);
                val = pt.data();
                LOG(INFO) << "XML elemnets: " << val;

                switch(i)
                {
                case 0:
                    task->name = val;
                    break;
                case 1:
                    task->description = val;
                    break;
                case 2:
                    task->nodes = val;
                    break;
                case 3:
                    task->cmd = val;
                    break;
                }
            }

            //FIXME 临时测试,兼容之前task_info结构体
            task->uuid = common::CUtils::generate_uuid();
            task->job_id = job_id;
            task->task_id = id;
            task->task_name = task->name;
            id++;

            task_list->push_back(task);

            //任务入库
            std::string sql = boost::str(boost::format(
                    "INSERT INTO TaskState (job_id,task_id,task_name,cmd,desc) values('%d','%d','%s','%s','%s');")
                % job_id % task->task_id % task->task_name % task->cmd % task->description);
            if(!db_help.update(sql)) {
                LOG(ERROR) << "execut inert task sql error";
                return false;
            }

            job_tasks.push_back(task_list);
        }
    }

    print_tasks(job_tasks);

    return true;
}

void CJobParse::print_tasks(std::list<std::list<openflow::task_info*>*>& job_tasks)
{
    std::list<std::list<openflow::task_info*>*>::iterator iter = job_tasks.begin();
    for (; iter != job_tasks.end(); ++iter)
    {
        std::list<openflow::task_info*>* task_list = *iter;
        std::list<openflow::task_info*>::iterator it = task_list->begin();
        for(; it != task_list->end(); ++it)
        {
            openflow::task_info* task = (openflow::task_info*)*it;
            LOG(INFO) << "Task name: " << task->name << "Task description: " << task->description
                << "Task nodes: " << task->nodes << "Task command: " << task->cmd;
        }
    }
}

}} //enf namespace openflow::master
