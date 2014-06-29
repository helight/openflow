// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-27
// Description:
//
#include <fcntl.h>
#include <sys/time.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "utils.h"

namespace common {

std::string CUtils::get_program_path()
{
    char buf[1024];
    buf[0] = '\0';

    int retval = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (retval > 0)
    {
        buf[retval] = '\0';

        // 保留这段废代码，以牢记deleted的存在，但由于这里只取路径部分，所以不关心它的存在
        // if (!strcmp(buf+retval-10," (deleted)"))
        //    buf[retval-10] = '\0';
        // 去掉文件名部分
        char* end = strrchr(buf, '/');
        if (NULL == end)
            buf[0] = 0;
        else
            *end = '\0';
    }

    return buf;
}

} // end namespace common
