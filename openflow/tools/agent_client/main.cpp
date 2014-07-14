// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
// test agent server

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glog/logging.h>
#include <thrift_helper.h>
#include "../../config.h"
#include "rpc/agent/AgentService.h"
#include "task.h"

using namespace apache;

extern "C" int main(int argc, char *argv[])
{
    common::CThriftClientHelper<openflow::agent::AgentServiceClient>
        thrift_client_helper("127.0.0.1", openflow::OPENFLOW_AGENT_HANDLER_PORT);

    thrift_client_helper.connect();

    openflow::job_info info;
    info.job_id = 7;

    tools::agent_client::CTask task;
    
    int tmp = task.receive_task();
    if ( tmp != 0 )
    {
        LOG(ERROR) << "fail to receive task.";
        return -1;
    }

    LOG(INFO) << "submit task...";
    tmp = thrift_client_helper->submit_task(info.job_id);
    if ( tmp != 0 )
    {
        LOG(ERROR) << "fail to submit task.";
        return -1;
    }
    
    return 0;
}
