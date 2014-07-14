// Copyright (c) 2014, OpenFlow
// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-05-05
// Description:
//

#include <iostream>
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <main_template.h>
#include <thrift_helper.h>
#include <utils.h>
#include "master_handler.h"
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
    common::CThriftServerHelper<CMasterHandler, MasterServiceProcessor> _openflow_master;
};

bool CMainHelper::init(int argc, char* argv[])
{
    // init glog,gflags
    FLAGS_logbufsecs = 0;           // write log no cache
    FLAGS_max_log_size = 300;       // max log size for each log file
    FLAGS_log_dir = boost::str(boost::format("%s/") % common::CUtils::get_program_path());
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("openflow_master");
    return true;
}

bool CMainHelper::run()
{
    LOG(INFO) << "openflow master start ...";
    LOG(INFO) << "thread num: " << FLAGS_thread_num ;

    _openflow_master.serve(OPENFLOW_MASTER_HANDLER_PORT, FLAGS_thread_num);

    return true;
}

void CMainHelper::fini()
{
}

extern "C" int main(int argc, char* argv[])
{
    CMainHelper main_helper;

    return main_template(&main_helper, argc, argv);
}

}} // end openflow::master
