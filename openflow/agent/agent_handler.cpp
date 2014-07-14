// Copyright (c) 2014, OpenFlow
// Copyright (c) 2014, OpenFlow
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
// deal with tasks received from master

#include <glog/logging.h>
#include "agent_handler.h"

namespace openflow { namespace agent {
    CAgentHandler::CAgentHandler() {}

    int32_t CAgentHandler::submit_task(const int32_t t_id)
    {
        LOG(INFO) << "receive a task.";
        LOG(INFO) << "handling...";
        for ( int i = 0; i < t_id; i++ )
            LOG(INFO) << "openflow!!!";

        return 0;
    }

    int32_t CAgentHandler::kill_task()
    {
        return 0;
    }
} }


