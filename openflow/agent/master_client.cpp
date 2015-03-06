// Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>, helight<helight@zhwen.org>
// Created: 2014-10-13
// Description: 客户端操作类的二次封装
//
#include <glog/logging.h>
#include "master_client.h"

namespace openflow { namespace agent {

CMasterClient::CMasterClient() : _is_init(false) {}

//连接客户端要进行的初始化
bool CMasterClient::init(const std::string& host, uint16_t port)
{
    if (!_is_init)
    {
        _host = host;
        _port = port;
        //客户端操作类的初始化
        _master = new common::CThriftClientHelper<openflow::master::MasterServiceClient>(_host, _port);
        _is_init = true;
    }

    return _is_init;
}

//关闭到客户端的连接
CMasterClient::~CMasterClient()
{
	delete _master;
}

// 发送任务
int32_t CMasterClient::report_task_state(const openflow::task_state& state)
{
    int32_t ret = -1;

    boost::mutex::scoped_lock lock(_mutex);
    try
    {
        _master->connect();
        ret = _master->get()->report_task_state(state);
        _master->close(); //显示调用close方法;
    }
    catch (apache::thrift::TException& ex)
    {
        LOG(ERROR) << "rpc error: " << ex.what();
    }

    return ret;
}

int32_t CMasterClient::report_agent_state(const openflow::agent_state &state)
{
    int32_t ret = -1;

    boost::mutex::scoped_lock lock(_mutex);
    try
    {
        _master->connect();
        ret = _master->get()->report_agent_state(state);
        _master->close(); //显示调用close方法;
    }
    catch (apache::thrift::TException& ex)
    {
        LOG(ERROR) << "rpc error: " << ex.what();
    }

    return ret;
}

}}
