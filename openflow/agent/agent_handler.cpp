// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
// deal with tasks received from master

#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <glog/logging.h>
#include "agent_handler.h"

namespace openflow { namespace agent {
    CAgentHandler::CAgentHandler() {}

    CAgentHandler::~CAgentHandler() {}

    int32_t CAgentHandler::execute_task(const openflow::task_info &task)
    {
	int i = 0;
	
        /*用fork execlp执行脚本程序*/
        LOG(INFO) << "receive a task."
        << "task id: " << task.task_id
        << "task name: " << task.task_name
        << "start handling..."<<i++;

        /*获取任务执行时间的开始*/
        gettimeofday(&start_time, NULL);

        /*检测是否超出并发度*/
        if ( fork_cnt >= fork_max )
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

                /*将执行结果保存到以task_id命名的文件中*/
                char id_src[255];
                sprintf(id_src, "%d", task.task_id);
                /*文件路径*/
               // char id_des[255] = "/home/km/Desktop/openflow_result_data/";
                char id_des[255] = "/tmp/data";
		strcat(id_des, id_src);
                if( (fd = creat(id_des, 0644)) == -1 )
                    LOG(ERROR) << "creat error"; 

                /*关闭stdout和stderr，任务执行结果输出到task_id文件中*/ 
                dup2(fd, 1);
                dup2(fd, 2);
                if ( execlp("bash", "bash", "-x", p, NULL) < 0 )
                {
                    LOG(ERROR) << "execlp error";
                    return -1;
                }
                //LOG(INFO) << "execlp failed...";
                //exit(1);

            }
            else if ( fork_rv > 0 )
            {
             	if ( wait(&child_status) != fork_rv )
             		LOG(ERROR) << "wait error";
             	else
             	{
                	/*计算任务执行时间*/
                	gettimeofday(&end_time, NULL);
                	time_use = track_time(start_time, end_time);

                	/*父进程捕捉子进程返回值*/
                	LOG(INFO) << "The task (pid = " << fork_rv << ") returned:";
                	parent_catch_sig(child_status);
                
                	/*将返回值转化为SUCCESS,FAILED*/
                	sorf();
                }

                /*任务最终状态的输出*/
                LOG(INFO) << "handling done..."
                << "done waiting for " << fork_rv 
                << "task execution status is " << task_status
                << "task pid is " << task_pid
                << "the time of exeution is " << time_use;
            }
        }

        return 0;
    }

    int32_t CAgentHandler::parent_catch_sig(int32_t status)
    {
        if ( WIFEXITED(status) )
        {
            LOG(INFO) << "task exited, exit code= " << WEXITSTATUS(status);
        }
        else if ( WIFSIGNALED(status) )
        {
            LOG(ERROR) << "task killed (signal " << WTERMSIG(status) <<
#ifdef WCOREDUMP
        	       WCOREDUMP(status) ? " (core file generated)" : ")\n";
#else
                ")\n";
#endif
        }
        else if ( WIFSTOPPED(status) )
        {
            LOG(ERROR) << "child stopped (signal " << WSTOPSIG(status) << ")\n";
        }
        else if ( WIFCONTINUED(status) )
        {
        	LOG(ERROR) << "child continued";
        }
        else
            LOG(ERROR) << "unexpected status(" << status << ")\n";    

        return 0;
    }

    float CAgentHandler::track_time(struct timeval start_time, struct timeval end_time)
    {
        float t;
        t = 1000000 * (end_time.tv_sec - start_time.tv_sec) +
            (end_time.tv_usec - start_time.tv_usec);
        t /= 1000000;
        
        return t;
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
            char *pid = NULL;
            sprintf(pid, "%d", task_pid);
            if ( execlp("kill", pid, NULL) < 0 )
            {
                LOG(ERROR) << "execlp error";
                return -1;
            }
            //LOG(INFO) << "execlp failed...";
            //exit(1);
       }
        else
        {
            /*将task从任务队列中删除*/
            if ( task_queue.erase(task.task_id) )
                LOG(INFO) << "ok: task " << task.task_id << " removed!";
            else
                LOG(INFO) << "oops: task " << task.task_id << " not found!";
        }

        return 0;
    }

    int32_t CAgentHandler::show_running_task()
    {
        /*测试用
         * 测试任务队列
         */
        LOG(INFO) << "*********************************"
        << "the task queue is: ";
        std::map<int32_t, std::string>::iterator task_queue_iter;
        for ( task_queue_iter = task_queue.begin();
            task_queue_iter != task_queue.end(); task_queue_iter++ )
            LOG(INFO) << task_queue_iter->first <<  " " << task_queue_iter->second;
        LOG(INFO) << "*********************************";

        return 0;
    }

    int32_t CAgentHandler::report_status()
    {
        return 0;
    }

} }


