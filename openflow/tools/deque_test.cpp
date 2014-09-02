// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-01-05
// Description:
//

#include <iostream>
#include <deque>

void test()
{
    std::deque <char> test(5,'2');
    test.push_back('1');
    test.push_back('5');

    std::cout << test.size() << std::endl;
    std::cout << sizeof(test) << std::endl;
    std::cout << test.max_size() << std::endl;

    int count = test.size();
    for (int i = 0; i < count; i++)
    {
        std::cout << "test.pop: " << test.front() << std::endl;
        test.pop_front();
    }
}

int main(int argc, char* argv[])
{
    test();

    return 0;
}
