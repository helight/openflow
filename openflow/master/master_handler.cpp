// Copyright (c) 2014, HelightXu
// Author: RenZhen<renzhengeek@163.com>
// Created: 2014-06-08
// Description:
//  Using rpc interface to process job.
#include <glog/logging.h>
#include "master_handler.h"

namespace openflow { namespace master {

CMasterHandler::CMasterHandler() {}

int32_t CMasterHandler::submit_job(const  openflow::job_info& job) {
  // Your implementation goes here
  LOG(INFO) << "received a job.";
  LOG(INFO) << "job id: " << job.job_id;
  LOG(INFO) << "job type: " << job.job_type;
  LOG(INFO) << "job name: " << job.job_name.c_str();
  LOG(INFO) << "job command: " << job.cmd.c_str();
  LOG(INFO) << "job owner: " << job.owner.c_str();

  return 0;
}

int32_t CMasterHandler::stop_job(const int32_t id) {
  // Your implementation goes here
  return 0;
}

int32_t CMasterHandler::kill_job(const int32_t id) {
  // Your implementation goes here
  return 0;
}

int32_t CMasterHandler::report_task_state(const int32_t state) {
  // Your implementation goes here
  return 0;
}

}} // end openflow::master

