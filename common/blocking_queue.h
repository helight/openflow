// Copyright (c) 2014, OpenFlow
// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//

#ifndef COMMON_BLOCKING_QUEUE_H
#define COMMON_BLOCKING_QUEUE_H
#pragma once

#include <assert.h>
#include <deque>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

namespace common {

template<typename T>
class CBlockingQueue : boost::noncopyable
{
public:
    CBlockingQueue(uint32_t max_elements = static_cast<uint32_t>(-1))
        : _max_elements(max_elements)
    {
        assert(max_elements > 0);
    }

    bool push_front(const T& item)
    {
        bool ret = false;
        {
            boost::mutex::scoped_lock lock(_mutex);
            if (!unlocked_is_full())
            {
                _queue.push_front(item);
                ret = true;
            }
        }
        _not_empty_condition.notify_one();

        return ret;
    }

    bool push_back(const T& item)
    {
        bool ret = false;
        {
            boost::mutex::scoped_lock lock(_mutex);
            if (!unlocked_is_full())
            {
                _queue.push_back(item);
                ret = true;
            }
        }
        _not_empty_condition.notify_one();

        return ret;
    }

    void pop_front(T& item)
    {
        {
            boost::mutex::scoped_lock lock(_mutex);
            while (_queue.empty())
            {
                _not_empty_condition.wait(lock);
            }
            item = _queue.front();
            _queue.pop_front();
        }
        _not_full_condition.notify_one();
    }

    void pop_back(T& item)
    {
        {
            boost::mutex::scoped_lock lock(_mutex);
            while (_queue.empty())
            {
                _not_empty_condition.wait(lock);
            }
            item = _queue.back();
            _queue.pop_back();
        }
        _not_full_condition.notify_one();
    }

    bool timed_pop_front(T& item, const uint32_t seconds = 10)
    {
        bool success = false;
        {
            boost::mutex::scoped_lock lock(_mutex);
            while (_queue.empty())
            {
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC_);
                xt.sec += seconds;
                _not_empty_condition.wait(lock);
            }
            if (!_queue.empty())
            {
                item = _queue.back();
                _queue.pop_back();
                success = true;
            }
        }
        if (success)
            _not_full_condition.notify_one();

        return success;
    }

    bool is_full() const
    {
        boost::mutex::scoped_lock lock(_mutex);
        return unlocked_is_full();
    }

private:
    bool unlocked_is_full() const
    {
        return _queue.size() >= _max_elements;
    }

private:
    boost::mutex _mutex;
    boost::condition_variable _not_empty_condition;
    boost::condition_variable _not_full_condition;
    uint32_t _max_elements;

    std::deque<T> _queue;
};

} // end namespace common

#endif // COMMON_BLOCKING_QUEUE_H
