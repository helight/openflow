#ifndef OPENFLOW_CONFIG_H
#define OPENFLOW_CONFIG_H
#pragma once

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

} // end namespace openflow

#endif // OPENFLOW_CONFIG_H
