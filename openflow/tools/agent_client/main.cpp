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
    openflow::task_info task1;
    task1.task_id = 8;
    task1.task_name = "kobemiller";
    task1.cmd = "#!/bin/bash ps;ls -e;date;who;pwd;";

    openflow::task_info task2;
    task2.task_id = 10;
    task2.task_name = "kobe";
    task2.cmd = "#!/bin/bash; ps;ls;date;who;pwd;";

    openflow::task_info task3;
    task3.task_id = 5;
    task3.task_name = "miller";
    task3.cmd = "#!/bin/bash; ps; ls; date; who; pwd;";

    tools::agent_client::CTask Ctask;
    
    int tmp = Ctask.receive_task();
    if ( tmp != 0 )
    {
        LOG(ERROR) << "fail to receive task.";
        return -1;
    }

    LOG(INFO) << "execute task...";
    tmp = thrift_client_helper->execute_task(task1);
    if ( tmp != 0 )
    {
        LOG(ERROR) << "fail to execute task.";
        return -1;
    }
   
//	sleep(3);
    thrift_client_helper.close();
    common::CThriftClientHelper<openflow::agent::AgentServiceClient>
        thrift_client_helper2("127.0.0.1", openflow::OPENFLOW_AGENT_HANDLER_PORT);
    thrift_client_helper2.connect();
    tmp = thrift_client_helper2->execute_task(task2);
    if ( tmp != 0 )
    {
        LOG(ERROR) << "fail to execute task.";
        return -1;
    }

    tmp = thrift_client_helper2->execute_task(task3);
    if ( tmp != 0 )
    {
        LOG(ERROR) << "fail to execute task.";
        return -1;
    }

    thrift_client_helper2->show_running_task();
    
    return 0;
}
