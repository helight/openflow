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
//FIXME ZhangYifei
//   1: i32 task_id,

    1: string name,
    2: string description,
    3: string nodes,
    4: string cmd,
//临时使用,后期agent+master规范task_info结构体
    5: i32 task_id,
    6: string task_name
}
