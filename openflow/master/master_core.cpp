// Copyright (c) 2014, HelightXu
// Author: RenZhen<renzhen@163.com>
// Created: 2014-06-29
// Description:
//
#include <sqlite3.h>
#include <glog/logging.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include "master_core.h"

namespace openflow { namespace master {
    bool CMasterCore::fetch_job(const int32_t job_id)
    {
        sqlite3_stmt *stmt;
        //FIXME(renzhen): how to get table name.
        std::string sql = "SELECT * FROM tbJobs WHERE job_id=:job_id;";
        if ( !_db->query(sql, &stmt) )
        {
            LOG(ERROR) << "Query error.";
            return false;
        }

        int index;
        index = sqlite3_bind_parameter_index(stmt, ":job_id");
        if( SQLITE_OK != sqlite3_bind_int(stmt, index, job_id) )
        {
            LOG(ERROR) << "Fail to binding parameter xml.";
            return false;
        }

        if ( SQLITE_ROW == sqlite3_step(stmt) )
        {
            openflow::job_info info;
            info.job_id = (int32_t)sqlite3_column_int(stmt, 0);
            int bytes = sqlite3_column_bytes(stmt, 1);
            info.job_name.assign((char *)sqlite3_column_text(stmt, 1), bytes);

            bytes = sqlite3_column_bytes(stmt, 2);
            info.xml_desc.assign((char *)sqlite3_column_text(stmt, 2), bytes);

            bytes = sqlite3_column_bytes(stmt, 3);
            info.time.assign((char *)sqlite3_column_text(stmt, 3), bytes);

            _jobs.insert(std::pair<int32_t, openflow::job_info>(job_id, info));
        }

        return true;
    }

    bool CMasterCore::parse_job(const int32_t job_id)
    {
        boost::property_tree::ptree pt;
        boost::property_tree::ptree root;

        std::stringstream ss;
        ss << _jobs[job_id].xml_desc;
        boost::property_tree::xml_parser::read_xml(ss, pt);
        root = pt.get_child("job");

        const std::string task_member[] = {"name","description","nodes", "command"};
        std::vector<StTask> tasks;
        StTask task;
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
                tasks.push_back(task);
            }
        }
        _tasks.insert(std::pair<int32_t, std::vector<StTask> >(job_id, tasks));

        return true;
    }

    void CMasterCore::print_job(const int32_t job_id)
    {
        openflow::job_info info = _jobs[job_id];
        LOG(INFO) << "Job ID: " << info.job_id;
        LOG(INFO) << "Job name: " << info.job_name;
        LOG(INFO) << "Job xml description: " << info.xml_desc;
        LOG(INFO) << "time: " << info.time;
    }

    void CMasterCore::print_tasks(const int32_t job_id)
    {
        std::vector<StTask> tasks;
        tasks = _tasks[job_id];

        for(std::vector<StTask>::iterator it = tasks.begin(); it != tasks.end(); it++)
        {
            LOG(INFO) << "Task name: " << it->name;
            LOG(INFO) << "Task description: " << it->description;
            LOG(INFO) << "Task nodes: " << it->nodes;
            LOG(INFO) << "Task command: " << it->cmd;
        }
    }
}} //enf namespace openflow::master
