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
#include <thrift/transport/TTransportException.h>
#include "../../config.h"
#include "rpc/agent/AgentService.h"
#include "task.h"

using namespace apache;

common::CThriftClientHelper<openflow::agent::AgentServiceClient>
    thrift_client_helper("127.0.0.1", openflow::OPENFLOW_AGENT_HANDLER_PORT);

bool init_agent_client()
{
    try
    {
        thrift_client_helper.connect();
    }
    catch(thrift::TException &e)
    {
        LOG(ERROR) << e.what();
        return false;
    }

    return true;
}

bool execute_task(openflow::task_info& task)
{
    LOG(INFO) << "execute task...";
    int32_t ret;
    ret = thrift_client_helper->execute_task(task);

    LOG(INFO) << "ret: " << ret;
    if (ret != 0)
    {
        LOG(ERROR) << "fail to execute task.";
        return false;
    }
    return true;
}

extern "C" int main(int argc, char *argv[])
{
    if (!init_agent_client())
    {
        return -1;
    }
    openflow::task_info task1;
    task1.task_id = 8;
    task1.task_name = "kobemiller";
    task1.cmd = "for((a=0; a < 1000000; ++a));do echo $a;done;";
    execute_task(task1);

    openflow::task_info task2;
    task2.task_id = 10;
    task2.task_name = "kobe";
    task2.cmd = "#!/bin/bash\n for((a=0; a < 10000; ++a));do echo $a;done;";
    execute_task(task2);

    openflow::task_info task3;
    task3.task_id = 5;
    task3.task_name = "miller";
    task3.cmd = "ps; ls; date; who; pwd;";
    execute_task(task3);

    openflow::task_info task4;
    task4.task_id = 1;
    task4.task_name = "kobemiller";
    task4.cmd = "for((a=0; a < 1000000; ++a));do echo $a;done;";
    while (!execute_task(task4))
    {
        sleep(1);
    }

    openflow::task_info task5;
    task5.task_id = 2;
    task5.task_name = "kobe";
    task5.cmd = "for((a=0; a < 10000; ++a));do echo $a;done; exit -1";
    while(!execute_task(task5))
    {
        sleep(1);
    }

    openflow::task_info task6;
    task6.task_id = 3;
    task6.task_name = "miller";
    task6.cmd = "ps; ls; date; who; pwd;";
    while (!execute_task(task6))
    {
        sleep(1);
    }

    openflow::task_info task7;
    task7.task_id = 4;
    task7.task_name = "kobemiller";
    task7.cmd = "for((a=0; a < 1000000; ++a));do echo $a;done;";

    openflow::task_info task8;
    task8.task_id = 6;
    task8.task_name = "kobe";
    task8.cmd = "#!/bin/bash\n for((a=0; a < 10000; ++a));do echo $a;done;";

    openflow::task_info task9;
    task9.task_id = 7;
    task9.task_name = "miller";
    task9.cmd = "ps; ls; date; who; pwd;";

    openflow::task_info task10;
    task10.task_id = 9;
    task10.task_name = "miller";
    task10.cmd = "ps; ls; date; who; pwd;";

    openflow::task_info task11;
    task11.task_id = 11;
    task11.task_name = "miller";
    task11.cmd = "ps; ls; date; who; pwd;";

    openflow::task_info task12;
    task12.task_id = 12;
    task12.task_name = "miller";
    task12.cmd = "ps; ls; date; who; pwd;";

    return 0;
}
