#include <iostream>
#include "thread_pool.h"

struct StInfo
{
    uint32_t task_id;
    uint32_t arg1;
    uint32_t arg2;
};

void print_task(StInfo& args)
{
    int count = 0;
    while (++count < 5)
    {
        std::cout << "I'm task with id: " << args.task_id
            << " args1: " << args.arg1 << " args2: " << args.arg2 << std::endl;
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    common::CThreadPool tp(4);

    for (int i = 0; i < 40; ++i)
    {
        common::StThreadTask task;
        StInfo t_info;
        t_info.task_id = i;
        t_info.arg1 = i * i;
        t_info.arg2 = i * i * i;

        task.task_fun = boost::bind(print_task, t_info);
        tp.add_task(task);
    }

    while (1)
    {
        sleep(5);
        std::cout << "sleep 1s" << std::endl;
    }

    return 0;
}
