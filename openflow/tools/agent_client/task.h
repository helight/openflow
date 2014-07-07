#ifndef TASK_H
#define TASK_H
#pragma once

#include <iostream>
#include "rpc/agent/AgentService.h"

namespace tools { namespace agent_client {

    class CTask
    {
        public:
            int receive_task();

        private:
            std::string _name;
            int _id;
    };
} }

#endif
