#ifndef AGENT_EXECUTE_H
#define AGENT_EXECUTE_H
#pragma once
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <glog/logging.h>
#include <blocking_queue.h>
#include "agent_handler.h"

void sig_handler(int32_t, siginfo_t*, void*);

namespace openflow { namespace agent {
class CTaskExecute
{
public:
	CTaskExecute();
	~CTaskExecute();

	int32_t show_running_task();

	std::map<int32_t, std::string> running_task_queue;
};

class CChild : public CTaskExecute
{
public:
	CChild();
	~CChild();

	int32_t handler_task(const openflow::agent::conv_task_info &task);

	int32_t kill_task();

	void trace_time();

	void parent_trace_child();

	void signal_handler(int32_t sig, int32_t child_status);\
	int32_t child_status;

	void get_task_status();

private:
	struct timeval start_time, end_time;
	float time_use;

	pid_t task_pid;

	std::string task_status;
	void change_status()
	{
		if ( child_status == 0 )
			task_status = "SUCCESS";
		else 
			task_status = "FAILED";
	}
};

}

}
#endif
