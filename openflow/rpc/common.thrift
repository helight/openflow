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
//FIXME:zhangyifei
    1: i32 task_id,
    2: string task_name,
    3: string cmd,
    4: string nodes,
    5: string description,
    6: string name
<<<<<<< HEAD
=======
}

struct agent_state
{
//AddMe zhangyifei
   1: string remain_mem, //内存剩余量
   2: string mem_use_percent, //内存使用百分比
   3: string cpu_idle_percent, //cpu空闲率
   4: string cpu_load, //cpu 1分钟  五分钟  十五分钟负载 放在一个字符串里面 空格分割
   5: string ipaddr, //ip地址
   6: string swap_use_percent //交换分区使用百分比
>>>>>>> jeff
}
