namespace cpp openflow
namespace php openflow

struct job_info
{
    1: i32 job_id,
    2: string job_name,
    3: string xml_desc,
    4: string time
}

struct task_info
{
    1: i32 task_id,
    2: string task_name,
    3: string cmd
}