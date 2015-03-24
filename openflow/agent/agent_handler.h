// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com> helight<helight@zhwen.org>
// Created: 2014-07-03
// Description:
//

#ifndef OPENFLOW_AGENT_HANDLER_H
#define OPENFLOW_AGENT_HANDLER_H
#pragma once

#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "blocking_queue.h"
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

<<<<<<< HEAD
            /* initialize function */
            void init();

            /* set and get fork max count*/
            void set_fork_max(uint32_t max);
            uint32_t get_fork_max() const;

            /* set and get fork max count*/
            void set_fork_cnt(uint32_t count);
            uint32_t get_fork_cnt() const;

            int32_t execute_task(const openflow::task_info &task);
=======
    int32_t execute_task(const openflow::task_info &task);
>>>>>>> 60f691c0f8aa0dc826f563ea6368deb25a13c902

    /*agent测试用：显示当前执行的任务*/
    int32_t show_running_task();

    /*中止一个task*/
    int32_t kill_task(const openflow::task_info &task);

    /*上报自身状态信息*/
    int32_t report_status();

<<<<<<< HEAD
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
=======
};
>>>>>>> 60f691c0f8aa0dc826f563ea6368deb25a13c902

} }

#endif
