// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#ifndef OPENFLOW_MASTER_HANDLER_H
#define OPENFLOW_MASTER_HANDLER_H
#pragma once

#include "rpc/master/MasterService.h"

namespace openflow { namespace master {

class CMasterHandler: public MasterServiceIf
{
public:
    int32_t submit_job();
    int32_t stop_job();
    int32_t kill_job();
    int32_t report_task_state(const int32_t state);
};

}} // end openflow::master

#endif // OPENFLOW_MASTER_HANDLER_H
