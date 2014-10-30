namespace cpp openflow.master
namespace php openflow.master

include "../common.thrift"

service MasterService
{
    /* serve for web client */
    i32 submit_job(1: i32 id),
    i32 stop_job(1: i32 id),
    i32 kill_job(1: i32 id),

    /* serve for agent client */
    i32 report_task_state(1: common.agent_state state)
}

