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
    AGENT_REPORT_TIMEOUT = 60 //zomoe add 
};

enum
{
    MASTER_OK            = 0,
    MASTER_JOB_OVER_MAX  = 2000,
    MASTER_DB_CONN_ERROR = 2001,
};

//FIXME ZhangYiFei add dbconf
const std::string OPENFLOW_DB_DBNAME = "openflow.db";
const std::string OPENFLOW_DB_JOBTABLENAME = "toJob";
const std::string OPENFLOW_DB_SQLITE_CONNSTR = "../web/database/openflow.db";
const std::string OPENFLOW_DB_AGENTSTATETABLENAME = "AgentState";
const std::string OPENFLOW_DB_TASKSTATETABLENAME = "TaskState";

//FIXME (zomoe) add details
const std::string OPENFLOW_MASTER_IPADDR = "127.0.0.1";

} // end namespace openflow

#endif // OPENFLOW_CONFIG_H
