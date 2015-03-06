#ifndef OPENFLOW_CONFIG_H
#define OPENFLOW_CONFIG_H
#pragma once
#include <iostream>

namespace openflow {

enum
{
    OPENFLOW_MASTER_HANDLER_PORT    = 9080,
    OPENFLOW_MASTER_AGENT_PORT      = 9081,
    OPENFLOW_AGENT_HANDLER_PORT     = 9090,
};

enum
{
    AGENT_OK            = 0,
    AGENT_TASK_OVER_MAX = 1000,
    AGENT_FORK_ERROR    = 1001,
};

enum
{
    MASTER_OK            = 0,
    MASTER_JOB_OVER_MAX  = 2000,
    MASTER_DB_CONN_ERROR = 2001,
};

enum
{
    JOB_STATE_READY     = 0,  // 任务初始化状态
    JOB_STATE_RUNNING   = 1,  // 任务执行
    JOB_STATE_ERROR     = 2,  // 任务执行失败
    JOB_STATE_STOPED    = 3,  // 任务终止
    JOB_STATE_FINISHED  = 4   // 所有任务执行成功并完成
};

enum
{
    TASK_STATE_READY    = 0, // 任务初始化状态
    TASK_STATE_RUNNING  = 1, // 任务正在执行
    TASK_STATE_ERROR    = 2, // 调度问题导致任务无法调起的错误
    TASK_STATE_FINISHED = 3  // 无论任务运行结果如何标示任务跑完了
};

struct StTask
{
    std::string name;
    std::string description;
    std::string nodes;
    std::string cmd;
};

//FIXME ZhangYiFei add dbconf
const std::string OPENFLOW_DB_DBNAME = "openflow.db";
const std::string OPENFLOW_DB_JOBTABLENAME = "toJob";
const std::string OPENFLOW_DB_SQLITE_CONNSTR = "../web/database/openflow.db";
const std::string OPENFLOW_DB_AGENTSTATETABLENAME = "AgentState";
const std::string OPENFLOW_DB_TASKSTATETABLENAME = "TaskState";

} // end namespace openflow

#endif // OPENFLOW_CONFIG_H
