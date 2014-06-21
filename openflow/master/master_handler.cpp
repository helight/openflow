// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//

#include "master_handler.h"

namespace openflow { namespace master {

CMasterHandler::CMasterHandler() {}

int32_t CMasterHandler::submit_job(const  ::openflow::job_info& job) {
  // Your implementation goes here
  printf("submit_job()...\n");
  printf("job id: %d\n", job.job_id);
  printf("job type: %d\n", job.job_type);
  printf("job name: %s\n", job.job_name.c_str());
  printf("job command: %s\n", job.cmd.c_str());
  printf("job owner: %s\n", job.owner.c_str());

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

