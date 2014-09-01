// Copyright (c) 2014, HelightXu
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
// deal with tasks received from master

#include <stdlib.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <glog/logging.h>
#include "agent_handler.h"

using namespace std;

namespace openflow { namespace agent {
    CAgentHandler::CAgentHandler() {}

    CAgentHandler::~CAgentHandler() {}

    int32_t CAgentHandler::execute_task(const openflow::task_info &task)
    {
        /*用fork execlp执行脚本程序*/
        LOG(INFO) << "receive a task.";
        LOG(INFO) << "task id: " << task.task_id;
        LOG(INFO) << "task name: " << task.task_name;
        LOG(INFO) << "start handling...";

        /*获取任务执行时间的开始*/
        gettimeofday(&start_time, NULL);

        /*将任务插入到任务队列里*/
        task_queue.insert(pair<int32_t, string>(task.task_id, task.task_name));

        /*检测是否超出并发度*/
        if ( fork_cnt >= fork_max )
        {
            LOG(INFO) << "fork to the max";
            return -2;  //-2代表fork超出数量
        }
        else
        {
            /*开始执行任务*/
            int32_t fork_rv;
            if ( (fork_rv = fork()) == -1 )
                LOG(INFO) << "fork error.";
            else if ( fork_rv == 0 )
            {
                /*将task.cmd存入task_name命名的文件中*/
                const char *p = task.task_name.data();
                ofstream fout;
                fout.open(p);
                fout << task.cmd << endl;
                fout.close();

                int32_t fd;

                /*获取当前进程的pid,为后边可能要杀死进程使用*/
                task_pid = getpid();

                /*将执行结果保存到以task_id命名的文件中*/
                char id_src[255];
                sprintf(id_src, "%d", task.task_id);
                /*文件路径*/
                char id_des[255] = "/home/km/Desktop/openflow_result_data/";
                strcat(id_des, id_src);
                if( (fd = creat(id_des, 0644)) == -1 )
                    LOG(INFO) << "creat error";

                /*关闭stdout和stderr，任务执行结果输出到task_id文件中*/
                dup2(fd, 1);
                dup2(fd, 2);
                execlp("bash", "bash", "-x", p, NULL);

                /*execlp执行出错的输出*/
                LOG(INFO) << "execlp failed...";
                exit(1);
            }
            else
            {
                int32_t wait_rv;
                /*wait返回值，确定任务执行成功与否*/
                wait_rv = wait(&child_status);
                /*将返回值转化为SUCCESS,FAILED*/
                sorf();

                /*计算任务执行时间*/
                gettimeofday(&end_time, NULL);
                time_use = 1000000 * (end_time.tv_sec - start_time.tv_sec) +
                    (end_time.tv_usec - start_time.tv_usec);
                time_use /= 1000000;

                /*任务最终状态的输出*/
                LOG(INFO) << "handling done...";
                LOG(INFO) << "done waiting for " << fork_rv << ",wait returned "
                    << wait_rv;
                LOG(INFO) << "task execution status is " << task_status;
                LOG(INFO) << "task pid is " << task_pid;
                LOG(INFO) << "the time of exeution is " << time_use;
            }
        }

        return 0;
    }

    int32_t CAgentHandler::kill_task(const openflow::task_info &task)
    {
        /*杀死任务：用kill pid 杀死任务所在的进程，并将任务从运行队列中删除*/
        int32_t fork_rv;
        if ( (fork_rv = fork()) == -1 )
            LOG(INFO) << "fork error!";
        else if ( fork_rv == 0 )
        {
            /*用kill结束task所在的进程*/
            char *pid;
            sprintf(pid, "%d", task_pid);
            execlp("kill", pid, NULL);
            LOG(INFO) << "execlp failed...";
            exit(1);
        }
        else
        {
            /*将task从任务队列中删除*/
            if ( task_queue.erase(task.task_id) )
                LOG(INFO) << "ok: task " << task.task_id << " removed!\n";
            else
                LOG(INFO) << "oops: task " << task.task_id << " not found!\n";
        }

        return 0;
    }

    int32_t CAgentHandler::show_task()
    {
        /*测试用
         * 测试任务队列
         */
        LOG(INFO) << "*********************************" << endl;
        LOG(INFO) << "the task queue is: " << endl;
        map<int32_t, string>::iterator task_queue_iter;
        for ( task_queue_iter = task_queue.begin();
            task_queue_iter != task_queue.end(); task_queue_iter++ )
            LOG(INFO) << task_queue_iter->first <<  " " << task_queue_iter->second;
        LOG(INFO) << "*********************************" << endl;

        return 0;
    }

    int32_t CAgentHandler::report_status()
    {
        return 0;
    }

} }


