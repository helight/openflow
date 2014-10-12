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
    task1.cmd = "for((a=0; a < 1000000; ++a));do echo $a;done;";

    openflow::task_info task2;
    task2.task_id = 10;
    task2.task_name = "kobe";
    task2.cmd = "for((a=0; a < 10000; ++a));do echo $a;done;";

    openflow::task_info task3;
    task3.task_id = 5;
    task3.task_name = "miller";
    task3.cmd = "ps; ls; date; who; pwd;";

    tools::agent_client::CTask Ctask;
    
    // int tmp = Ctask.receive_task();
    // if ( tmp )
    // {
    //     LOG(ERROR) << "fail to receive task.";
    //     return -1;
    // }

    LOG(INFO) << "execute task...";
    bool tmp;
    tmp = thrift_client_helper->receive_task(task3);
    tmp = thrift_client_helper->receive_task(task1);
    tmp = thrift_client_helper->receive_task(task2);
    // if ( tmp != 0 )
    // {
    //     LOG(ERROR) << "fail to execute task.";
    //     return -1;
    // }

    // tmp = thrift_client_helper->execute_task(task2);
    // if ( tmp != 0 )
    // {
    //     LOG(ERROR) << "fail to execute task.";
    //     return -1;
    // }

    // tmp = thrift_client_helper->execute_task(task3);
    // if ( tmp != 0 )
    // {
    //     LOG(ERROR) << "fail to execute task.";
    //     return -1;
    // }

    // thrift_client_helper->show_running_task();

    return 0;
}
