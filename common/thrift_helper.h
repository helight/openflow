// Copyright (c) 2014, OpenFlow
// Copyright (c) 2014, OpenFlow
// Author: JianYi<eyjian@gmail.com> Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//

#ifndef COMMON_THRIFT_HELPER_H
#define COMMON_THRIFT_HELPER_H
#pragma once

#include <arpa/inet.h>
#include <glog/logging.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TSocketPool.h>
#include <thrift/transport/TTransportException.h>
#include <thrift/async/TAsyncChannel.h>

namespace common {

enum
{
    RPC_TIMEOUT     = 2000      // rpc timeout time 2s
};

using namespace apache;

// 用来判断thrift是否已经连接，包括两种情况：
// 1.从未连接过，也就是还未打开过连接
// 2.连接被对端关闭了
inline bool thrift_not_connected(
    thrift::transport::TTransportException::TTransportExceptionType type)
{
    return (thrift::transport::TTransportException::NOT_OPEN == type)
        || (thrift::transport::TTransportException::END_OF_FILE == type);
}
// 封装对thrift服务端的公共操作
template <class ThriftHandler, class ServiceProcessor>
class CThriftServerHelper
{
public:
    // 启动rpc服务，请注意该调用是同步阻塞的，所以需放最后调用
    bool serve(uint16_t port);
    bool serve(uint16_t port, uint8_t num_threads);
    void stop();

private:
    boost::shared_ptr<ThriftHandler> _handler;
    boost::shared_ptr<thrift::TProcessor> _processor;
    boost::shared_ptr<thrift::protocol::TProtocolFactory> _protocol_factory;
    boost::shared_ptr<thrift::concurrency::ThreadManager> _thread_manager;

    boost::shared_ptr<thrift::concurrency::PosixThreadFactory> _thread_factory;
    boost::shared_ptr<thrift::server::TServer> _server;
};

// 封装对thrift客户端的公共操作
template <class ThriftClient>
class CThriftClientHelper
{
public:
    CThriftClientHelper(const std::string& host, uint16_t port
                        , int timeout = RPC_TIMEOUT);
    ~CThriftClientHelper();
    void connect();
    void close();

    ThriftClient* operator ->() const
    {
        return _container_client.get();
    }

    ThriftClient* get()
    {
        return _container_client.get();
    }

private:
    std::string _host;
    uint16_t _port;
    int _timeout;
    boost::shared_ptr<thrift::transport::TSocketPool> _sock_pool;
    boost::shared_ptr<thrift::transport::TTransport> _socket;
    boost::shared_ptr<thrift::transport::TFramedTransport> _transport;
    boost::shared_ptr<thrift::protocol::TProtocol> _protocol;
    boost::shared_ptr<ThriftClient> _container_client;
};

///////////////////////////////////////////////////////////////////////////////
template <class ThriftHandler, class ServiceProcessor>
bool CThriftServerHelper<ThriftHandler, ServiceProcessor>::serve(uint16_t port)
{
    return serve(port, 1);
}

template <class ThriftHandler, class ServiceProcessor>
bool CThriftServerHelper<ThriftHandler, ServiceProcessor>::serve(uint16_t port, uint8_t num_threads)
{
    try
    {
        _handler.reset(new ThriftHandler);
        _processor.reset(new ServiceProcessor(_handler));
        _protocol_factory.reset(new thrift::protocol::TBinaryProtocolFactory());
        _thread_manager = thrift::server::ThreadManager::newSimpleThreadManager(num_threads);
        _thread_factory.reset(new thrift::concurrency::PosixThreadFactory());

        _thread_manager->threadFactory(_thread_factory);
        _thread_manager->start();

        _server.reset(new thrift::server::TNonblockingServer(
                _processor, _protocol_factory, port, _thread_manager));

        _server->serve();
    }
    catch (thrift::TException& tx)
    {
        LOG(ERROR) << "start thrift error: " << tx.what();
        return false;
    }

    LOG(INFO) << "container-thrift start";
    return true;
}

template <class ThriftHandler, class ServiceProcessor>
void CThriftServerHelper<ThriftHandler, ServiceProcessor>::stop()
{
    _server->stop();
}

///////////////////////////////////////////////////////////////////////////////

template <class ThriftClient>
CThriftClientHelper<ThriftClient>::CThriftClientHelper(
    const std::string& host, uint16_t port, int timeout)
    : _host(host)
      , _port(port)
      , _timeout(timeout)
{
    _sock_pool.reset(new thrift::transport::TSocketPool());
    _sock_pool->addServer(host, port);
    _sock_pool->setConnTimeout(timeout);
    _sock_pool->setRecvTimeout(timeout);
    _sock_pool->setSendTimeout(timeout);

    _socket = _sock_pool;
    _transport.reset(new thrift::transport::TFramedTransport(_socket));
    _protocol.reset(new thrift::protocol::TBinaryProtocol(_transport));

    _container_client.reset(new ThriftClient(_protocol));
}

template <class ThriftClient>
CThriftClientHelper<ThriftClient>::~CThriftClientHelper()
{
    close();
}

template <class ThriftClient>
void CThriftClientHelper<ThriftClient>::connect()
{
    if (!_transport->isOpen())
    {
        _transport->open();
    }
}

template <class ThriftClient>
void CThriftClientHelper<ThriftClient>::close()
{
    if (_transport->isOpen())
    {
        _transport->close();
    }
}

} // end namespace common

#endif // COMMON_THRIFT_HELPER_H
