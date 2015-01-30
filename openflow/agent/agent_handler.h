// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
//

#ifndef OPENFLOW_AGENT_HANDLER_H
#define OPENFLOW_AGENT_HANDLER_H
#pragma once

#include <vector>
#include <map>
#include <string>
#include "rpc/agent/AgentService.h"

// Set default value
const uint32_t kForkMax = 10;
const uint32_t kForkCnt = 0;

namespace openflow { namespace agent {

    class CAgentHandler : public AgentServiceIf
    {
        public:
            CAgentHandler();
            ~CAgentHandler();

            /* initialize function */
            void init();

            /* set and get fork max count*/
            void set_fork_max(uint32_t max);
            uint32_t get_fork_max() const;

            /* set and get fork max count*/
            void set_fork_cnt(uint32_t count);
            uint32_t get_fork_cnt() const;

            int32_t execute_task(const openflow::task_info &task);

            int32_t kill_task(const openflow::task_info &task);

            /*计算task执行时间*/
            float track_time(struct timeval, struct timeval);

            /*跟踪子进程*/
            int32_t parent_catch_sig(int32_t);

            int32_t show_running_task();

            int32_t report_status();

        private:

            /*统计任务执行之间*/
            struct timeval start_time, end_time;
            float time_use;

            /*task的pid*/
            pid_t task_pid;

            /*任务返回码*/
            int32_t child_status;

            /*正在执行任务的数量*/
            //int32_t task_count = 0;

            /*控制fork的数量*/
            uint32_t fork_max;
            uint32_t fork_cnt;

            /*任务队列*/
            std::map<int32_t, std::string> task_queue;

            /*将task的执行返回值转化为成功失败*/
            std::string task_status;
            void sorf()
            {
                if ( child_status == 0 )
                    task_status = "SUCCESS";
                else
                    task_status = "FAILED";
            }
    };

} }

#endif
