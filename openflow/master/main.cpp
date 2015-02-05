// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-05-05
// Description:
//

#include <iostream>
#include <exception>
#include <boost/format.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/thread.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <main_template.h>
#include <thrift_helper.h>
#include <utils.h>
#include "master_handler.h"
#include "job_parse.h"
#include "job_scheduler.h"
#include "../config.h"

DEFINE_int32(thread_num, 4, "thead num for rpc server");

namespace openflow { namespace master {

using namespace apache;

class CMainHelper : public common::IMainHelper
{
private:
    bool init(int argc, char* argv[]);
    bool run();
    void fini();

private:
    void init_singleton();
    bool start_scheduler_thread();

private:
    boost::scoped_ptr<boost::thread> _job_scheduler_thread;
    boost::scoped_ptr<boost::thread> _job_clean_thread;
    common::CThriftServerHelper<CMasterHandler, MasterServiceProcessor> _openflow_master;
};

bool CMainHelper::init(int argc, char* argv[])
{
    init_singleton();

    return true;
}

bool CMainHelper::run()
{

    if (!start_scheduler_thread())
    {
        LOG(ERROR) << "start_scheduler_thread error";
        return false;
    }
    LOG(INFO) << "openflow master start ...thread num: " << FLAGS_thread_num ;

    try
    {
        _openflow_master.serve(OPENFLOW_MASTER_HANDLER_PORT, FLAGS_thread_num);
    }
    catch (std::exception& exn)
    {
        LOG(ERROR) << "CMainHelper::run: " << exn.what();
    }

    return true;
}

void CMainHelper::fini()
{
}

void CMainHelper::init_singleton()
{
    boost::serialization::singleton<CJobScheduler>::get_const_instance();
    boost::serialization::singleton<CJobParse>::get_const_instance();
}

bool CMainHelper::start_scheduler_thread()
{
    CJobScheduler& job_scheduler =
        boost::serialization::singleton<CJobScheduler>::get_mutable_instance();
    _job_scheduler_thread.reset(new boost::thread(boost::bind(
                &CJobScheduler::scheduler_thread, &job_scheduler)));
    _job_clean_thread.reset(new boost::thread(boost::bind(
                &CJobScheduler::clean_thread, &job_scheduler)));
    return true;
}

extern "C" int main(int argc, char* argv[])
{
    // init glog,gflags
    FLAGS_logbufsecs = 0;           // write log no cache
    FLAGS_max_log_size = 300;       // max log size for each log file
    FLAGS_log_dir = boost::str(boost::format("%s/") % common::CUtils::get_program_path());
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("openflow_master");

    CMainHelper main_helper;
    return main_template(&main_helper, argc, argv);
}

}} // end openflow::master
