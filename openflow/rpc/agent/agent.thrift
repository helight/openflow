namespace cpp openflow.agent
namespace php openflow.agent

include "../common.thrift"

service AgentService
{
    /*执行task*/
    i32 execute_task(1:common.task_info task);

    /*agent测试用：显示当前执行的任务*/
    i32 show_running_task();

    /*中止一个task*/
    i32 kill_task(1:common.task_info task);

    /*上报自身状态信息*/
    i32 report_status();
}

