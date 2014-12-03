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
#include <boost/shared_ptr.hpp>
#include "blocking_queue.h"
#include "rpc/agent/AgentService.h"

namespace openflow { namespace agent {

    class CAgentHandler : public AgentServiceIf
    {
        public:
            CAgentHandler();
            ~CAgentHandler();

            bool receive_task(const openflow::task_info &task);

            int32_t execute_task();

	    int32_t report_status();

        private:
            common::CBlockingQueue<openflow::task_info> _tasks;
            boost::shared_ptr<boost::thread> execute_task_thread;
	    boost::shared_ptr<boost::thread> agent_state_thread;

       //     int32_t execute_task();

       //     int32_t kill_task(const openflow::task_info &task);

       //     /*计算task执行时间*/
       //     float track_time(struct timeval, struct timeval);

       //     /*跟踪子进程*/
       //     int32_t parent_catch_sig(int32_t);

       //     int32_t show_running_task();

       //     int32_t report_status();

       // private:


       //     /*统计任务执行之间*/
       //     struct timeval start_time, end_time;
       //     float time_use;

       //     /*task的pid*/
       //     pid_t task_pid;

       //     /*任务返回码*/
       //     int32_t child_status;

       //     /*正在执行任务的数量*/
       //     //int32_t task_count = 0;

       //     /*控制fork的数量*/
       //     static const int32_t fork_max = 10;
       //     static int32_t fork_cnt = 0;

       //     /*任务队列*/
       //     std::map<int32_t, std::string> task_queue;

       //     /*将task的执行返回值转化为成功失败*/
       //     std::string task_status;
       //     void sorf()
       //     {
       //         if ( child_status == 0 )
       //             task_status = "SUCCESS";
       //         else
       //             task_status = "FAILED";
       //     }
    };

} }

#endif
