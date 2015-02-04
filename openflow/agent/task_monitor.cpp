// Copyright (c) 2015, OpenFlow
// Author: helight<helight@zhwen.org>
// Created: 2015-02-03
// Description:
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <boost/lexical_cast.hpp>
#include <boost/serialization/singleton.hpp>
#include <glog/logging.h>
#include "task_execute.h"
#include "task_monitor.h"

namespace openflow { namespace agent {

CTaskMonitor::CTaskMonitor() : _stop(false) {}

void CTaskMonitor::signal_thread()
{
    while (!_stop)
    {
        int signo = wait_signal();
        if (-1 == signo)
        {
            LOG(ERROR) << "wait error";
            break;
        }
        if (SIGTERM == signo)
        {
            on_executor_terminated();
        }
        else
        {
            while (true)
            {
                int exited_status;
                pid_t task_pid = waitpid(-1, &exited_status, WNOHANG);

                if (0 == task_pid)
                {
                    break;
                }
                else if(task_pid > 0)
                {
                    on_task_end(task_pid, exited_status);
                }
                else
                {
                    if (errno != ECHILD)
                    {
                        LOG(ERROR) << "wait error: " << strerror(errno);
                    }
                    break;
                }
                VLOG(4) << "task pid: " << task_pid;
            }
        }
    }

    if (!_stop)
    {
        LOG(ERROR) << "task process abnormally exited";
    }
}

int CTaskMonitor::wait_signal() const
{
    int signo = -1;
    do
    {
        sigset_t sigset;
        if (-1 == sigemptyset(&sigset))
        {
            LOG(ERROR) << "empty sigset ERROR: " << strerror(errno);
            break;
        }

        // add SIGCHLD signal
        if (-1 == sigaddset(&sigset, SIGCHLD))
        {
            LOG(ERROR) << "add SIGCHLD to sigset error: " << strerror(errno);
            break;
        }

        // add SIGTERM signal
        if (-1 == sigaddset(&sigset, SIGTERM))
        {
            LOG(ERROR) << "add SIGTERM to sigset error: " << strerror(errno);
            break;
        }

        // wait for SIGCHILD
        int errcode = sigwait(&sigset, &signo);
        if (errcode != 0)
        {
            LOG(ERROR) << "sigwait error: " << strerror(errno);
            break;
        }
    } while (0);

    return signo;
}

void CTaskMonitor::on_executor_terminated()
{
}

void CTaskMonitor::on_task_end(pid_t task_pid, int32_t exited_status)
{
    LOG(INFO) << "task end : " << task_pid;
    int signo = -1;
    std::string extra_info;
    int state = -1;
    if (WIFEXITED(exited_status))
    {
        int32_t exit_code = WEXITSTATUS(exited_status);
        if (0 == exit_code)
        {
            state = 0;
            LOG(INFO) << "task : " << task_pid << " exited successfully";
        }
        else
        {
            extra_info = boost::lexical_cast<std::string>(exit_code);
        }
        LOG(INFO) << ">> child exited, exit code= " << WEXITSTATUS(exited_status) << std::endl;
    }
    else if (WIFSIGNALED(exited_status) )
    {
        signo = WTERMSIG(exited_status);
        extra_info = strsignal(signo);
        LOG(ERROR) << ">> child killed (signal " << WTERMSIG(exited_status) << ")\n";
    }
    else
    {
        extra_info =  "unknown";
        LOG(ERROR) << "unexpected CTaskMonitor.exited_status(" << exited_status<< ")\n";
    }

    // @todo
    // update task info to db
    {
        CTaskExecute& task_excute =
            boost::serialization::singleton<CTaskExecute>::get_mutable_instance();
        task_excute.update_task_state(task_pid, state);

        task_excute.delete_task_by_pid(task_pid);
    }
}

}} // end openflow agent
