// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhengeek@163.com>
// Created: 2014-07-21
// Description:
// 1.using blocking_queue;
// 2.using boost::thread;
// Get string from user, put into blocking_queue, then thread print string.

#include <iostream>
#include <boost/thread.hpp>
#include "blocking_queue.h"

common::CBlockingQueue<std::string> queue;

void worker_func()
{
    while (1)
    {
        std::string str;
        queue.pop_back(str);
        std::cout << "You said: " << str << "." << std::endl;
    }

}

int main(void)
{
    std::cout << "main: startup..." << std::endl;
    boost::thread speakerThread(worker_func);

    while(1)
    {
        std::string str;
        std::cout << "please speak some words:" << std::endl;
        getline(std::cin, str);

        if (str != "")
        {
            queue.push_back(str);
        }else{
            break;
        }
    }

    //FIXME: speakerThread may be bloked on queue so that never return.
    std::cout << "main: waiting for thread..." << std::endl;
    speakerThread.join();

    std::cout <<"main: done." << std::endl;
    return 0;
}
