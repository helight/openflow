// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-07-05
// Description:
//

#include <gtest/gtest.h>
#include "blocking_queue.h"

TEST(CBlockingQueue, Init)
{
    common::CBlockingQueue<int> queue;
    queue.push_front(1);
    int n;
    queue.pop_front(n);
    ASSERT_EQ(1, n);
    queue.push_back(3);
    queue.pop_back(n);
    ASSERT_EQ(3, n);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
