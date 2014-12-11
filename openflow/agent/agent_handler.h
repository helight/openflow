// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
//

#ifndef OPENFLOW_AGENT_HANDLER_H
#define OPENFLOW_AGENT_HANDLER_H
#pragma once

#include <sys/msg.h>
#include <sys/shm.h>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "blocking_queue.h"
#include "rpc/agent/AgentService.h"

void signal_handler(int32_t, siginfo_t*, void*);

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

        void regist_signal();
        void signal_handle(int32_t sig);

        int32_t msg_init();
        int32_t shm_init();
    private:
        struct MSG_TASK
        {
            long int msg_type;
            openflow::agent::conv_task_info Task;
        };
        struct MSG_TASK msg_task; 
        int32_t msgid;

        int32_t shmid;
        int32_t* task_cnt = NULL;

        int32_t pid;
        const static int32_t TASK_MAX = 10;
    };
} }
#endif
