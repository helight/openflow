// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-05-05
// Description:
//

#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <glog/logging.h>
#include "master_handler.h"


using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace  openflow::master;

int main(int argc, char **argv) {
  int port = 9090;
  boost::shared_ptr<CMasterHandler> handler(new CMasterHandler());
  boost::shared_ptr<TProcessor> processor(new MasterServiceProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);

  LOG(INFO) << "Starting the server...";
  server.serve();
  LOG(INFO) << "done.";

  return 0;
}
