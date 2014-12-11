// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
// deal with tasks received from master
#include <string.h>
#include <glog/logging.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "agent_handler.h"
#include "agent_execute.h"

void signal_handler(int32_t sig, siginfo_t *info, void* cld)
{
    ((openflow::agent::CAgentHandler*)cld)->signal_handle(sig);
}

namespace openflow { namespace agent {
    /*
    CAgentHandler::CAgentHandler() 
    {
        execute_task_thread = boost::shared_ptr<boost::thread>
    	(new boost::thread(boost::BOOST_BIND(&CAgentHandler::execute_task, this)));
    	LOG(INFO) << "kobemiller";
    }

    CAgentHandler::~CAgentHandler() 
    {
    	execute_task_thread->join();
    }
*/

    CAgentHandler::CAgentHandler()
    {
        msg_init();
        shm_init();

        if ( (pid = fork()) == -1 )
        {
            LOG(ERROR) << "fork error";
            exit(-1);
        }
        else if ( pid == 0 )
        {
            execute_task();
        }
    }
    CAgentHandler::~CAgentHandler()
    {
    }

    int32_t CAgentHandler::msg_init()
    {
        key_t MSGKEY = 2048;
        msgid = msgget(MSGKEY, IPC_CREAT|IPC_EXCL|0666);
        if ( msgid == -1 )
        {
            msgid = msgget(MSGKEY, 0);
            if (  msgid < 0 )
            {
                LOG(ERROR) << "msgget error";
                exit(-1);
            }
        }
        return 0;
    }
    void CAgentHandler::regist_signal()
    {
    	struct sigaction sa;
		sa.sa_sigaction = sig_handler;
		sigemptyset(&sa.sa_mask);
		sigaddset(&sa.sa_mask, SIGINT);
		sa.sa_flags = SA_SIGINFO;

		if ( sigaction(SIGCHLD, &sa, NULL) == -1 )
			LOG(ERROR) << "parent sigaction(SIGCHLD) error\n";
    }
    int32_t CAgentHandler::shm_init()
    {
        key_t SHMKEY = 4096;
        shmid = shmget(SHMKEY, sizeof(int32_t), IPC_CREAT|IPC_EXCL|0666);
        if ( shmid == -1 )
        {
            shmid = shmget(SHMKEY, sizeof(int32_t), IPC_EXCL|0666);
            if ( shmid == -1 )
            {
                LOG(ERROR) << "shmget error";
                exit(-1);
            }
        }
        task_cnt = (int32_t*)shmat(shmid, NULL, 0);
        if ( task_cnt == (void *)-1 )
        {
            LOG(ERROR) << "shmat error";
            exit(-2);
        }
        *task_cnt = 0;

        return 0;
    }

    void CAgentHandler::signal_handle(int32_t sig)
    {
    	int32_t p, status;
    	p = wait(&status);
		LOG(INFO) << "parent catch signal: " << sig << std::endl;
		if ( sig == SIGCHLD )
		{
			if ( WIFEXITED(status) )
		    {
		        LOG(INFO) << ">> child exited, exit code= " << WEXITSTATUS(status) << std::endl;
		    }
		    else if ( WIFSIGNALED(status) )
		    {
		        LOG(ERROR) << ">> child killed (signal " << WTERMSIG(status) << ")\n";
		    }
		    else if ( WIFSTOPPED(status) )
		    {
		        LOG(ERROR) << ">> child stopped (signal " << WSTOPSIG(status) << ")\n";
		    }
		#ifdef WIFCONTINUED
		    else if ( WIFCONTINUED(status) )
		    {
		        LOG(ERROR) << ">> child continued\n";
		    }
		#endif
		    else
		        LOG(ERROR) << "unexpected CChild.child_status(" << status << ")\n";
		}
		else
		{
			LOG(ERROR) << ">> child exec error\n";
		    exit(1);
		}
    }
    bool CAgentHandler::receive_task(const openflow::task_info &task)
    {
        bool receive_rv = false;
        if (*task_cnt >= TASK_MAX )
        {
            LOG(ERROR) << "receive to the max, cannot handle any more";
        }
        else
        {
            msg_task.msg_type = 1;
            msg_task.Task.task_id = task.task_id;
            strcpy(msg_task.Task.task_name, task.task_name.c_str());
            strcpy(msg_task.Task.cmd, task.cmd.c_str());
            if ( msgsnd(msgid, &(msg_task), sizeof(msg_task), 0) == -1 )
                LOG(INFO) << "receive failed\n", std::cout << "receive failed\n";
            else
            {
                LOG(INFO) << "receive success\n", std::cout << std::endl << ">>>receive success\n";
                (*task_cnt)++;
                std::cout << std::endl << ">>>receive task: " << *task_cnt << std::endl;
                receive_rv = true;
            }
        }
        return receive_rv;
    }

    int32_t CAgentHandler::execute_task()
    {
        CTaskExecute main_execute;
        while ( 1 )
        {
        	if(msgrcv(msgid, &(msg_task), sizeof(msg_task), 0, 0) == -1 )
            {
                LOG(ERROR) << "msgrcv error";
                exit(-1);
            }
/*
            int32_t pid;
            if ( (pid = fork()) == -1 )
                LOG(ERROR) << "fork error";
            else if ( pid == 0 )
            {
                if ( (pid = fork()) == -1 )
                    LOG(ERROR) << "fork error";
                else if ( pid > 0 )
                    exit(0);
                sleep(3);
                CChild child;
        	    child.handler_task(msg_task.Task);
                (*task_cnt)--;
                std::cout << std::endl << "<<<real_execute child: " << *task_cnt << std::endl;
                exit(0);
            }
            if ( waitpid(pid, NULL, 0) != pid )
            {
                LOG(ERROR) << "waitpid error";
                exit(1);
            }
*/ 
        	int32_t re_rv = real_execute(msg_task.Task);
        	if ( re_rv != 0 )
        		LOG(ERROR) << "real_execute error";
                
        }
        return 0;
    }

    int32_t CAgentHandler::real_execute(const openflow::agent::conv_task_info &task)
    {
    	int32_t pid;
        if ( (pid = fork()) == -1 )
            LOG(ERROR) << "fork error";
        else if ( pid == 0 )
        {
            if ( (pid = fork()) == -1 )
                LOG(ERROR) << "fork error";
            else if ( pid > 0 )
                exit(0);
            sleep(3);
            CChild* ptr = new CChild;
        	ptr->handler_task(task);
            (*task_cnt)--;
            std::cout << std::endl << "<<<real_execute child: " << *task_cnt << std::endl;
            delete ptr;
            exit(0);
        }
        if ( waitpid(pid, NULL, 0) != pid )
        {
            LOG(ERROR) << "waitpid error";
            exit(1);
        }
/* 
        regist_signal();
   	    int32_t pid;
        if ( (pid = fork()) == -1 )
            LOG(ERROR) << "fork error";
        else if ( pid == 0 )
        {
            CChild child;
        	child.handler_task(task);
            (*task_cnt)--;
            std::cout << std::endl << "<<<real_execute child: " << *task_cnt << std::endl;
            sleep(3);
            exit(0);
        }
*/ 
        return 0;
    }

} }

