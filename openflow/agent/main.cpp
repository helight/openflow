// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-04
// Description:
//

#include <iostream>
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <main_template.h>
#include <thrift_helper.h>
#include <utils.h>
#include "agent_handler.h"
#include "../config.h"

DEFINE_int32(thread_num, 4, "thread num for rpc server");

namespace openflow { namespace agent{

    using namespace apache;

    class CMainHelper : public common::IMainHelper
    {
        private:
            bool init(int argc, char *argv[]);
            bool run();
            void fini();

        private:
            common::CThriftServerHelper<CAgentHandler, AgentServiceProcessor>_openflow_agent;
    };

    bool CMainHelper::init(int argc, char *argv[])
    {
        FLAGS_logbufsecs = 0;
        FLAGS_max_log_size = 300;
        FLAGS_log_dir = boost::str(boost::format("%s/") % common::CUtils::get_program_path());
        google::ParseCommandLineFlags(&argc, &argv, true);
        google::InitGoogleLogging("openflow_agent");

        return true;
    }

    bool CMainHelper::run()
    {
        LOG(INFO) << "openflow agent start...";
        LOG(INFO) << "thread num: " << FLAGS_thread_num;

        _openflow_agent.serve(openflow::OPENFLOW_AGENT_HANDLER_PORT, FLAGS_thread_num);

        return true;
    }

    void CMainHelper::fini()
    {
        
    }

    extern "C" int main(int argc, char *argv[])
    {
        CMainHelper main_helper;

        return main_template(&main_helper, argc, argv);
    }

} }
