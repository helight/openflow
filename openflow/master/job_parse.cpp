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
#include "job_parse.h"
#include "master_opdb.h"

namespace openflow { namespace master {

bool CJobParse::fetch_job(const int32_t job_id, openflow::job_info& info)
{
    common::CDataSet &ds = boost::serialization::singleton<common::CDataSet>::get_mutable_instance();
    common::CDatabase *db = ds.new_database(common::DB_SQLITE, "openflow.db");

    //SQLite database file path just like connect string.
    db->set_connect_str("../web/database/openflow.db");
    //open connect handler
    db->open();

    //FIXME: table name should not be fixed.
    common::CTable *table = db->new_table("tbJobs");
    if (!table)
    {
        LOG(ERROR) << "Fail to create table.";
        //FIXME: ugly code.
        db->close();        //close database connection.
        delete table;
        delete db;
        return false;
    }

    std::string sql = boost::str(boost::format("SELECT * FROM tbJobs WHERE job_id=%d;")
                                  % job_id);
    std::vector<std::string> rows;
    int num_rows, num_cols;

    num_rows  = table->query(sql, rows, num_cols);
    if ( num_rows < 0)
    {
        LOG(ERROR) << "Query error.";
         //FIXME: ugly code.
        db->close();        //close database connection.
        delete table;
        delete db;
        return false;
    }

    info.job_id = boost::lexical_cast<int32_t>(rows[num_cols + 0]);
    info.job_name = rows[num_cols + 1];
    info.xml_desc = rows[num_cols + 2];
    info.time = rows[num_cols + 3];

    db->close();        //close database connection.
    delete table;
    delete db;

    return true;
}

bool CJobParse::parse_job(const int32_t job_id, std::vector<openflow::task_info>& job_tasks)
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

    CMasterDB db(common::DB_SQLITE,openflow::OPENFLOW_DB_DBNAME);
    if(false == db.connect("../web/database/openflow.db")) {
        LOG(ERROR) << "connect to db error";
        return -1;
    }

    if(false == db.optable(openflow::OPENFLOW_DB_TASKSTATETABLENAME)) {
        LOG(ERROR) << "open table error";
        return -2;
    }

    //FIXME ZhangYiFei
    /*
       const std::string task_member[] = {"name","description","nodes", "command"};
       std::vector<StTask> tasks;
       StTask task;
       */
    const std::string task_member[] = {"name","description","nodes", "command"};
    openflow::task_info task;

    int id = 1;
    for(boost::property_tree::ptree::iterator it = root.begin(); it != root.end(); it++)
    {
        if(it->first == "shell_process")
        {
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
                    task.name = val;
                    break;
                case 1:
                    task.description = val;
                    break;
                case 2:
                    task.nodes = val;
                    break;
                case 3:
                    task.cmd = val;
                    break;
                }
            }

            //FIXME 临时测试,兼容之前task_info结构体
            task.task_name = task.name;
            task.task_id = id;
            id++;
            job_tasks.push_back(task);

            //任务入库
            std::string sql = boost::str(boost::format(
                    "INSERT INTO TaskState (job_id,task_id,task_name,cmd,desc) values('%d','%d','%s','%s','%s');")
                % job_id % task.task_id % task.task_name % task.cmd % task.description);
            if(false == db.execute(sql)) {
                LOG(ERROR) << "execut inert task sql error";
                return -3;
            }
        }
    }

    return true;
}

}} //enf namespace openflow::master
