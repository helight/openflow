// Copyright: Tencent Tech. Co., Ltd.
// Author: helightxu
// Date: 2014/7/21
#ifndef COMMON_THREAD_POOL_H
#define COMMON_THREAD_POOL_H

#include <boost/bind.hpp>
#include "blocking_queue.h"

namespace common {

struct StThreadTask
{
    boost::function<void ()> task_fun;
    void *args;
};

class CThreadPool :boost::noncopyable
{
public:
    CThreadPool(const uint32_t num = 4): _is_stop(true), _thread_num(num)
    {
        start();
    }

    ~CThreadPool()
    {
        if (_is_stop)
        {
            stop_all();
        }
    }

    void add_task(const StThreadTask& task)
    {
        if (!_is_stop)
        {
            _task_queue.push_back(task);
        }
    }

    uint32_t wait_nums()
    {
        return _task_queue.size();
    }

private:
    void start()
    {
        assert(_thread_num > 0);

        for(uint32_t i = 0; i < _thread_num; i++)
        {
            boost::shared_ptr<boost::thread> thread(
                new boost::thread(boost::BOOST_BIND(&CThreadPool::run, this)));
            _thread_worker.push_back(thread);
        }
        _is_stop = false;
    }

    void run()
    {
        while(!_is_stop)
        {
            StThreadTask task;
            _task_queue.pop_front(task);
            task.task_fun();
        }
    }

    void stop_all()
    {
        for(uint32_t i = 0; i < _thread_worker.size(); ++i)
        {
            _thread_worker[i]->join();
        }
    }

private:
    bool _is_stop;
    uint32_t _thread_num;
    CBlockingQueue<StThreadTask> _task_queue;
    std::vector<boost::shared_ptr<boost::thread> > _thread_worker;
};

} // end namespace common

#endif //COMMON_THREAD_POOL_H
