// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com> helight<helight@zhwen.org>
// Created: 2014-07-03
// Description:
// deal with tasks received from master
#include <glog/logging.h>
#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "agent_handler.h"
#include "task_execute.h"

namespace openflow { namespace agent {
<<<<<<< HEAD
    CAgentHandler::CAgentHandler() 
    {
        init();
    }

    CAgentHandler::~CAgentHandler() {}

    void CAgentHandler::init()
    {
        set_fork_max(kForkMax);
        set_fork_cnt(kForkCnt);
    }

    void CAgentHandler::set_fork_max(const uint32_t max)
    {
        fork_max = max;	
    }

    uint32_t CAgentHandler::get_fork_max()const
    {
        return fork_max;
    }

    void CAgentHandler::set_fork_cnt(const uint32_t count)
    {
        fork_cnt = count;	
    }

    uint32_t CAgentHandler::get_fork_cnt()const
    {
        return fork_cnt;
    }

    int32_t CAgentHandler::execute_task(const openflow::task_info &task)
    {
        /*用fork execlp执行脚本程序*/
        LOG(INFO) << "receive a task."
        << "task id: " << task.task_id
        << "task name: " << task.task_name
        << "start handling...";

        /*获取任务执行时间的开始*/
        gettimeofday(&start_time, NULL);

        /*检测是否超出并发度*/
        if ( get_fork_cnt() >= get_fork_max() )
        {
            LOG(INFO) << "fork to the max";
            return -2;  //-2代表fork超出数量
        }
        else
        {
            /*开始执行任务*/

            /*将任务插入到任务队列里*/
            task_queue.insert(std::pair<int32_t, std::string>(task.task_id, task.task_name));

           int32_t fork_rv;
            if ( (fork_rv = fork()) == -1 )
                LOG(ERROR) << "fork error.";
            else if ( fork_rv == 0 )
            {
                /*将task.cmd存入task_name命名的文件中*/
                const char *p = task.task_name.data();
                std::ofstream fout;
                fout.open(p);
                fout << task.cmd << std::endl;
                fout.close();

                int32_t fd;

                /*获取当前进程的pid,为后边可能要杀死进程使用*/
                task_pid = getpid();
=======
CAgentHandler::CAgentHandler() {}
>>>>>>> 60f691c0f8aa0dc826f563ea6368deb25a13c902

CAgentHandler::~CAgentHandler() {}

int32_t CAgentHandler::execute_task(const openflow::task_info &task)
{
    CTaskExecute& task_excutor =
        boost::serialization::singleton<CTaskExecute>::get_mutable_instance();

    return task_excutor.start_task(task);
}

/*agent测试用：显示当前执行的任务*/
int32_t CAgentHandler::show_running_task()
{
    return 0;
}

/*中止一个task*/
int32_t CAgentHandler::kill_task(const openflow::task_info &task)
{
    return 0;
}

/*上报自身状态信息*/
int32_t CAgentHandler::report_status()
{
    return 0;
}

} }

