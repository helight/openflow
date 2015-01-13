#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <glog/logging.h>
#include <blocking_queue.h>
#include "agent_execute.h"

void sig_handler(int32_t sig, siginfo_t *info, void* cld)
{
    ((openflow::agent::CChild*)cld)->signal_handler(sig);
	//((openflow::agent::CChild*)cld)->signal_handler(sig, ((openflow::agent::CChild*)cld)->child_status);
}

namespace openflow { namespace agent {
	CTaskExecute::CTaskExecute() {}
	CTaskExecute::~CTaskExecute() {}

	int32_t CTaskExecute::show_running_task()
	{
		/*测试用
         * 测试任务队列
         */
        LOG(INFO) << "*********************************"
        << "the task queue is: ";
        std::map<int32_t, std::string>::iterator task_queue_iter;
        for ( task_queue_iter = running_task_queue.begin();
            task_queue_iter != running_task_queue.end(); task_queue_iter++ )
            LOG(INFO) << task_queue_iter->first <<  " " << task_queue_iter->second;
        LOG(INFO) << "*********************************";

        return 0;
	}

	CChild::CChild() 
    {
        std::cout << "**************constructor" << std::endl;
        parent_trace_child();
        gettimeofday(&start_time, NULL);
    }
	CChild::~CChild()
    {
        std::cout << "**************destructor" << std::endl;
        gettimeofday(&end_time, NULL);
        trace_time();
        get_task_status();
    }

	int32_t CChild::handler_task(const openflow::agent::conv_task_info &task)
	{
		LOG(INFO) << "start handling...\n"
		<< "task id: " << task.task_id << std::endl
		<< "task name: " << (task.task_name) << std::endl
		<< ".............................\n";

		
		int32_t fork_rv;
		if ( (fork_rv = fork()) == -1 )
			LOG(ERROR) << "fork error.\n";
		else if ( fork_rv == 0 )
		{
                int32_t fd;

                /*获取当前进程的pid,为后边可能要杀死进程使用*/
                task_pid = getpid();

                /*将执行结果保存到以task_id命名的文件中*/
                char id_src[255];
                sprintf(id_src, "%d", task.task_id);
                /*文件路径*/
                char id_des[255] = "~/execute_data/";
                strcat(id_des, id_src);
                if( (fd = creat(id_des, 0644)) == -1 )
                    LOG(ERROR) << "creat error";

                /*关闭stdout和stderr，任务执行结果输出到task_id文件中*/
                dup2(fd, 1);
                dup2(fd, 2);
                if ( execlp("bash", "bash", "-cx", task.cmd, NULL) < 0 )
                {
                    LOG(ERROR) << "execlp error";
                    return -1;
                }
        }
        task_pid = wait(&child_status);
			if ( WIFEXITED(child_status) )
		    {
		        LOG(INFO) << ">> child exited, exit code= " << WEXITSTATUS(child_status) << std::endl;
		    }
		    else if ( WIFSIGNALED(child_status) )
		    {
		        LOG(ERROR) << ">> child killed (signal " << WTERMSIG(child_status) << ")\n";
		    }
		    else if ( WIFSTOPPED(child_status) )
		    {
		        LOG(ERROR) << ">> child stopped (signal " << WSTOPSIG(child_status) << ")\n";
		    }
		#ifdef WIFCONTINUED
		    else if ( WIFCONTINUED(child_status) )
		    {
		        LOG(ERROR) << ">> child continued\n";
		    }
		#endif
		    else
		        LOG(ERROR) << "unexpected CChild.child_status(" << child_status << ")\n";

        change_status();
        return 0;
	}

	void CChild::parent_trace_child()
	{
		struct sigaction sa;
		sa.sa_sigaction = sig_handler;
		sigemptyset(&sa.sa_mask);
		sigaddset(&sa.sa_mask, SIGINT);
		sa.sa_flags = SA_SIGINFO;

		if ( sigaction(SIGCHLD, &sa, NULL) == -1 )
			LOG(ERROR) << "parent sigaction(SIGCHLD) error\n";
	}

	int32_t CChild::kill_task()
	{
		char* pid = NULL;
		sprintf(pid, "%d", task_pid);
		if ( execlp("kill", pid, NULL) < 0 )
		{
			LOG(ERROR) << "execlp error\n";
			return -1;
		}
		return 0;
	}

	void CChild::trace_time()
	{
        time_use = 1000000 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec);
        time_use /= 1000000;
	}

	void CChild::signal_handler(int32_t sig)
	{
/*         task_pid = wait(&child_status);
		LOG(INFO) << "parent catch signal: " << sig << std::endl;
		if ( sig == SIGCHLD )
		{
			if ( WIFEXITED(child_status) )
		    {
		        LOG(INFO) << ">> child exited, exit code= " << WEXITSTATUS(child_status) << std::endl;
		    }
		    else if ( WIFSIGNALED(child_status) )
		    {
		        LOG(ERROR) << ">> child killed (signal " << WTERMSIG(child_status) << ")\n";
		    }
		    else if ( WIFSTOPPED(child_status) )
		    {
		        LOG(ERROR) << ">> child stopped (signal " << WSTOPSIG(child_status) << ")\n";
		    }
		#ifdef WIFCONTINUED
		    else if ( WIFCONTINUED(child_status) )
		    {
		        LOG(ERROR) << ">> child continued\n";
		    }
		#endif
		    else
		        LOG(ERROR) << "unexpected CChild.child_status(" << child_status << ")\n";
		}
		else
		{
			LOG(ERROR) << ">> child exec error\n";
		    exit(1);
		}
*/
		LOG(INFO) << "parent catch signal: " << sig << std::endl;
	}

	void CChild::get_task_status()
	{
		LOG(INFO) << "task execution status is: " << task_status << std::endl
		<< "task pid is:    " << task_pid << std::endl
        << "parent pid is:  " << getpid() << std::endl
		<< "time of execution is: " << time_use << std::endl;
	}

}}
