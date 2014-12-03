// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
// deal with tasks received from master

#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "../config.h"
#include "agent_handler.h"
#include "agent_execute.h"
#include "agent_conn.h"
#include "sys_info.h"

namespace openflow { namespace agent {
    CAgentHandler::CAgentHandler() 
    {
    	execute_task_thread = boost::shared_ptr<boost::thread>
    	(new boost::thread(boost::BOOST_BIND(&CAgentHandler::execute_task, this)));

	// FIXME:(zomoe)
	// new thread for report agent_status
	agent_state_thread = boost::shared_ptr<boost::thread>
	(new boost::thread(boost::BOOST_BIND(&CAgentHandler::report_status,this)));

    }

    CAgentHandler::~CAgentHandler() 
    {
    	execute_task_thread->join();
	agent_state_thread->join();
    }

    int32_t CAgentHandler::report_status()
    { 
	    // FIXME:(zomoe)
	    // enabled timefd as timeout !
	    struct tmp_info tmp_p;
	    openflow::agent_state state;
	    struct itimerspec new_value;
	    int fd;
	    struct timespec now;
	    uint64_t exp_t;
	    ssize_t s;
	    fd = timerfd_create(CLOCK_REALTIME, 0);
	    if (fd == -1)
		  LOG(ERROR) << "timerfd_create error";
	    while(true){
		    Sys_Info *sys =new Sys_Info(tmp_p);
		    state.remain_mem = tmp_p.remain_mem;
		    state.mem_use_percent = tmp_p.mem_use_percent;
		    state.cpu_idle_percent = tmp_p.cpu_idle_percent;
		    state.cpu_load = tmp_p.cpu_load;
		    state.ipaddr = tmp_p.ipaddr;
		    state.swap_use_percent = tmp_p.swap_use_percent;
		    try 
		    {
			    /* OPENFLOW_MASTER_IPADDR as a marco, defined the master ip_addr, which located in ../config.h */
			    CagentConn master(openflow::OPENFLOW_MASTER_IPADDR,openflow::OPENFLOW_MASTER_HANDLER_PORT);
			    LOG(INFO) << "connect to master success";
			    master.report_status(state);
			    LOG(INFO) << "for report_status end";
		    }
		    catch(apache::thrift::TException &e)
		    {
			    LOG(ERROR) << e.what();
			    return -1;
		    }

		    /* Create a CLOCK_REALTIME absolute timer with initial
		       expiration and interval as specified in command line */

		    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
			  LOG(ERROR) << "clock_gettime";

		    /* Timeout time as 60 sec ! 
		     * it can be replaced with TIMEOUT as a macro
		     * which should be define in ../config.h
		     */

		    new_value.it_value.tv_sec = now.tv_sec + openflow::AGENT_REPORT_TIMEOUT;
		    // new_value.it_value.tv_sec = now.tv_sec + 60;
		    new_value.it_value.tv_nsec = now.tv_nsec;

		    /* timer accuracy set in secs level ,so tv_nsec will be set with zero*/
		    new_value.it_interval.tv_nsec = 0;

		    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
			  LOG(ERROR) << "timerfd_settime";
		    s = read(fd, &exp_t, sizeof(uint64_t));
		    if (s != sizeof(uint64_t))
			  LOG(ERROR) << "read error";
		    delete sys;
	    }

	    return 0;

    }
    bool CAgentHandler::receive_task(const openflow::task_info &task)
    {
        bool receive_rv;
        receive_rv = _tasks.push_front(task);
        LOG(INFO) << "receive success\n";
        std::cout << "task receive success\n";

        return receive_rv;
    }

    int32_t CAgentHandler::execute_task()
    {
        CTaskExecute main_execute;
        
        for ( ; ; )
        {
            if ( main_execute.fork_cnt >= main_execute.FORK_MAX )
            {
                LOG(ERROR) << "fork to the max";
                return -2;  //-2代表fork超出数量
            }
            else 
            {
                main_execute.fork_cnt++;

                CChild child;
                openflow::task_info task;
                _tasks.pop_back(task);
                child.handler_task(task);
                child.running_task_queue.insert(std::pair<int32_t, std::string>(task.task_id, task.task_name));
                child.parent_trace_child();
                child.trace_time();
                child.get_task_status();
            }
        }
        return 0;
    }

//     int32_t CAgentHandler::execute_task()
//     {
//         openflow::task_info task;
//         _tasks.pop_back(task);
//         /*用fork execlp执行脚本程序*/
//         LOG(INFO) << "receive a task."
//         << "task id: " << task.task_id
//         << "task name: " << task.task_name
//         << "start handling...";

//         /*获取任务执行时间的开始*/
//         gettimeofday(&start_time, NULL);

//         /*检测是否超出并发度*/
//         if ( fork_cnt >= fork_max )
//         {
//             LOG(INFO) << "fork to the max";
//             return -2;  //-2代表fork超出数量
//         }
//         else
//         {
//             /*开始执行任务*/

//             /*将任务插入到任务队列里*/
//             task_queue.insert(std::pair<int32_t, std::string>(task.task_id, task.task_name));

//            int32_t fork_rv;
//             if ( (fork_rv = fork()) == -1 )
//                 LOG(ERROR) << "fork error.";
//             else if ( fork_rv == 0 )
//             {
                /*将task.cmd存入task_name命名的文件中*/
                // const char *p = task.task_name.data();
                // std::ofstream fout;
                // fout.open(p);
                // fout << task.cmd << std::endl;
                // fout.close();

                // int32_t fd;

                // /*获取当前进程的pid,为后边可能要杀死进程使用*/
                // task_pid = getpid();

                // /*将执行结果保存到以task_id命名的文件中*/
                // char id_src[255];
                // sprintf(id_src, "%d", task.task_id);
                // /*文件路径*/
                // char id_des[255] = "/home/km/Desktop/openflow_result_data/";
                // strcat(id_des, id_src);
                // if( (fd = creat(id_des, 0644)) == -1 )
                //     LOG(ERROR) << "creat error"; 

                // /*关闭stdout和stderr，任务执行结果输出到task_id文件中*/ 
                // dup2(fd, 1);
                // dup2(fd, 2);
                // if ( execlp("bash", "bash", "-x", p, NULL) < 0 )
                // {
                //     LOG(ERROR) << "execlp error";
                //     return -1;
                // }
                //LOG(INFO) << "execlp failed...";
                //exit(1);

//             }
//             else if ( fork_rv > 0 )
//             {
//              	if ( wait(&child_status) != fork_rv )
//              		LOG(ERROR) << "wait error";
//              	else
//              	{
//                 	/*计算任务执行时间*/
//                 	gettimeofday(&end_time, NULL);
//                 	time_use = track_time(start_time, end_time);

//                 	/*父进程捕捉子进程返回值*/
//                 	LOG(INFO) << "The task (pid = " << fork_rv << ") returned:";
//                 	parent_catch_sig(child_status);
                
//                 	/*将返回值转化为SUCCESS,FAILED*/
//                 	sorf();
//                 }

                // /*任务最终状态的输出*/
                // LOG(INFO) << "handling done..."
                // << "done waiting for " << fork_rv 
                // << "task execution status is " << task_status
                // << "parent pid is " << getppid() << "- "
                // << i++ << "- "
                // << "task pid is " << task_pid
                // << "the time of exeution is " << time_use;
//             }
//         }

//         return 0;
//     }

//     int32_t CAgentHandler::parent_catch_sig(int32_t status)
//     {
//         if ( WIFEXITED(status) )
//         {
//             LOG(INFO) << "task exited, exit code= " << WEXITSTATUS(status);
//         }
//         else if ( WIFSIGNALED(status) )
//         {
//             LOG(ERROR) << "task killed (signal " << WTERMSIG(status) <<
// #ifdef WCOREDUMP
//         	       WCOREDUMP(status) ? " (core file generated)" : ")\n";
// #else
//                 ")\n";
// #endif
//         }
//         else if ( WIFSTOPPED(status) )
//         {
//             LOG(ERROR) << "child stopped (signal " << WSTOPSIG(status) << ")\n";
//         }
//         else if ( WIFCONTINUED(status) )
//         {
//         	LOG(ERROR) << "child continued";
//         }
//         else
//             LOG(ERROR) << "unexpected status(" << status << ")\n";    

//         return 0;
//     }

//     float CAgentHandler::track_time(struct timeval start_time, struct timeval end_time)
//     {
//         float t;
        // t = 1000000 * (end_time.tv_sec - start_time.tv_sec) +
        //     (end_time.tv_usec - start_time.tv_usec);
        // t /= 1000000;
        
        // return t;
//     }

//     int32_t CAgentHandler::kill_task(const openflow::task_info &task)
//     {
//         /*杀死任务：用kill pid 杀死任务所在的进程，并将任务从运行队列中删除*/
//         int32_t fork_rv;
//         if ( (fork_rv = fork()) == -1 )
//             LOG(INFO) << "fork error!";
//         else if ( fork_rv == 0 )
//         {
//             /*用kill结束task所在的进程*/
//             char *pid = NULL;
//             sprintf(pid, "%d", task_pid);
//             if ( execlp("kill", pid, NULL) < 0 )
//             {
//                 LOG(ERROR) << "execlp error";
//                 return -1;
//             }
//             //LOG(INFO) << "execlp failed...";
//             //exit(1);
//        }
//         else
//         {
//             /*将task从任务队列中删除*/
//             if ( task_queue.erase(task.task_id) )
//                 LOG(INFO) << "ok: task " << task.task_id << " removed!";
//             else
//                 LOG(INFO) << "oops: task " << task.task_id << " not found!";
//         }

//         return 0;
//     }

//     int32_t CAgentHandler::show_running_task()
//     {
//         /*测试用
//          * 测试任务队列
//          */
//         LOG(INFO) << "*********************************"
//         << "the task queue is: ";
//         std::map<int32_t, std::string>::iterator task_queue_iter;
//         for ( task_queue_iter = task_queue.begin();
//             task_queue_iter != task_queue.end(); task_queue_iter++ )
//             LOG(INFO) << task_queue_iter->first <<  " " << task_queue_iter->second;
//         LOG(INFO) << "*********************************";

//         return 0;
//     }

//     int32_t CAgentHandler::report_status()
//     {
//         return 0;
//     }

} }


