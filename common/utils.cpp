// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-27
// Description:
//
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <signal.h>
#include <sys/time.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>
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

bool CUtils::block_signal(int signo)
{
    sigset_t sigset;
    if (-1 == sigemptyset(&sigset))
    {
        LOG(ERROR) << "empty sigset error: " << strerror(errno);
        return false;
    }
    if (-1 == sigaddset(&sigset, signo))
    {
        LOG(ERROR) << "add " << strsignal(signo) << " to sigset error: " << strerror(errno);
        return false;
    }

    LOG(INFO) << "block " << strsignal(signo) << " successfully";
    errno = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    if (errno != 0)
    {
        LOG(ERROR) << "block signals error " << strerror(errno);
        return false;
    }

    return true;
}

std::string CUtils::generate_uuid()
{
    boost::uuids::random_generator rgen;
    boost::uuids::uuid uuid = rgen();
    return boost::lexical_cast<std::string>(uuid);
}

std::string CUtils::get_local_ip(const char* dev)
{
    struct ifaddrs* addr_struct = NULL;
    struct ifaddrs* ifaddr= NULL;
    std::string local_ip = "";
    void* tmp_addr = NULL;

    if (0 != getifaddrs(&addr_struct))
    {
        VLOG(3) << "Get device ip error";
        return local_ip;
    }

    ifaddr = addr_struct;
    while (addr_struct != NULL)
    {
        if (addr_struct->ifa_addr->sa_family == AF_INET)
        {
            // is a valid IP4 Address
            tmp_addr = &((struct sockaddr_in*)addr_struct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmp_addr, addressBuffer, INET_ADDRSTRLEN);
            if (strcmp(addr_struct->ifa_name, dev) == 0)
            {
                local_ip = std::string(addressBuffer);
                break;
            }
        }
        else if (addr_struct->ifa_addr->sa_family == AF_INET6)
        {
            // is a valid IP6 Address
            tmp_addr = &((struct sockaddr_in *)addr_struct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmp_addr, addressBuffer, INET6_ADDRSTRLEN);
            if (strcmp(addr_struct->ifa_name, dev) == 0)
            {
                local_ip = std::string(addressBuffer);
                break;
            }
        }
        addr_struct = addr_struct->ifa_next;
    }
    if (ifaddr)
    {
        freeifaddrs(ifaddr);
    }

    return local_ip;
}

} // end namespace common
