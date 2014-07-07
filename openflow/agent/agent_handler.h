// Copyright (c) 2014, HelightXu
// Author: kobemiller<kobemiller@126.com>
// Created: 2014-07-03
// Description:
//

#ifndef OPENFLOW_AGENT_HANDLER_H
#define OPENFLOW_AGENT_HANDLER_H
#pragma once

#include <vector>
#include "rpc/agent/AgentService.h"

namespace openflow { namespace agent {

    class CAgentHandler : public AgentServiceIf
    {
        public:
            CAgentHandler();

            int32_t submit_task(const int32_t t_id);

            int32_t kill_task();
    };

} }

#endif
