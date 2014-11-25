// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
//

#ifndef OPENFLOW_AGENT_HANDLER_H
#define OPENFLOW_AGENT_HANDLER_H
#pragma once

#include <sys/msg.h>
#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "blocking_queue.h"
#include "rpc/agent/AgentService.h"

namespace openflow { namespace agent {
/*
    class CAgentHandler : public AgentServiceIf
    {
        public:
            CAgentHandler();
            ~CAgentHandler();

            bool receive_task(const openflow::task_info &task);

            int32_t execute_task();

            int32_t real_execute(const openflow::task_info &task);

            
        private:
            common::CBlockingQueue<openflow::task_info> _tasks;
            boost::shared_ptr<boost::thread> execute_task_thread;
    };
*/
struct conv_task_info
{
    int32_t task_id;
    char task_name[256];
    char cmd[512];
};

    class CAgentHandler : public AgentServiceIf
    {
    public:
        CAgentHandler();
        ~CAgentHandler();

        bool receive_task(const openflow::task_info &task);

        int32_t execute_task();

        int32_t real_execute(const openflow::agent::conv_task_info &task);

        int32_t msg_init();
    private:
        struct MSG_TASK
        {
            long int msg_type;
            openflow::agent::conv_task_info Task;
        };
        struct MSG_TASK msg_task; 
        int32_t msgid;
        int32_t pid;
    };
} }
#endif
