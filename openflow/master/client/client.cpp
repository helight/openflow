/**
 * Autor: RenZhen<renzhengeek@163.com>
 * Created: 2014.6.20
 * Description:
 * fake web client, in order to test Master server.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "rpc/master/MasterService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace openflow::master;

int main(int argc, char** argv) {
  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  MasterServiceClient client(protocol);

  try {
    transport->open();

    openflow::job_info job;
    job.job_id = 0;
    job.job_type = ::openflow::job_types::CMD;
    job.job_name = "testor";
    job.cmd = "ls /home/";
    job.owner = "renzhen";

    int ret = client.submit_job(job);
    printf("submit_job()...\n");

    transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
    return 1;
  }

  return 0;
}
