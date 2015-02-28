// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>, helight<helight@zhwen.org>
// Created: 2014-07-04
// Description:
//

#include <iostream>
#include <cstdlib>

#include <boost/format.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/thread.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <main_template.h>
#include <thrift_helper.h>
#include <utils.h>

#include "../config.h"
#include "agent_handler.h"
#include "master_client.h"
#include "task_execute.h"
#include "task_monitor.h"

DEFINE_int32(thread_num, 4, "thread num for rpc server");
DEFINE_int32(max_tasks_num, 4, "max tasks for app");
DEFINE_string(master_host, "127.0.0.1", "");

namespace openflow { namespace agent {

using namespace apache;

class CMainHelper : public common::IMainHelper
{
private:
    bool init(int argc, char *argv[]);
    bool run();
    void fini();

private:
    bool init_singleton();
    bool run_task_monitor();

private:
    common::CThriftServerHelper<CAgentHandler, AgentServiceProcessor> _openflow_agent;

    boost::scoped_ptr<boost::thread> _signal_monitor_thread;
    boost::scoped_ptr<boost::thread> _heart_beat_thread;
};

bool CMainHelper::init(int argc, char *argv[])
{
    init_singleton();

    return true;
}

bool CMainHelper::init_singleton()
{
    LOG(INFO) << "master host: " << FLAGS_master_host
        << " master port: " << OPENFLOW_MASTER_HANDLER_PORT;
    CTaskExecute& task_excute =
        boost::serialization::singleton<CTaskExecute>::get_mutable_instance();
    if (!task_excute.init(FLAGS_master_host, OPENFLOW_MASTER_HANDLER_PORT))
    {
        LOG(FATAL) << "task execute init failed";
    }

    CMasterClient& master_client =
        boost::serialization::singleton<CMasterClient>::get_mutable_instance();
    master_client.init(FLAGS_master_host, OPENFLOW_MASTER_HANDLER_PORT);

    boost::serialization::singleton<CTaskMonitor>::get_const_instance();

    return true;
}

bool CMainHelper::run()
{
    if (!common::CUtils::block_signal(SIGCHLD) || !common::CUtils::block_signal(SIGTERM))
    {
        LOG(ERROR) << "block signal SIGCHLD and SIGTERM error";
        return false;
    }

    // run heartbeat thread
    CTaskExecute& task_excute =
        boost::serialization::singleton<CTaskExecute>::get_mutable_instance();
    _heart_beat_thread.reset(new boost::thread(boost::bind(
                &CTaskExecute::report_heart_beat_thread, &task_excute)));

    // run task monitor thread
    CTaskMonitor& task_monitor =
        boost::serialization::singleton<CTaskMonitor>::get_mutable_instance();
    _signal_monitor_thread.reset(new boost::thread(boost::bind(
                &CTaskMonitor::signal_thread, &task_monitor)));

    LOG(INFO) << "openflow agent start, thread num: " << FLAGS_thread_num;
    _openflow_agent.serve(openflow::OPENFLOW_AGENT_HANDLER_PORT, FLAGS_thread_num);

    return true;
}

void CMainHelper::fini()
{
}

extern "C" int main(int argc, char *argv[])
{
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 300;
    FLAGS_log_dir = boost::str(boost::format("%s/") % common::CUtils::get_program_path());
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("openflow_agent");

    CMainHelper main_helper;
    return main_template(&main_helper, argc, argv);
}

}} // end
