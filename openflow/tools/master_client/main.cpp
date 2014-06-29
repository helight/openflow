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
#include <glog/logging.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "rpc/master/MasterService.h"
#include "CJob.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace openflow::master;

int main(int argc, char** argv) {
  boost::shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  MasterServiceClient client(protocol);

  try {
    transport->open();

    openflow::job_info info;
    info.job_name = "testor";
    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    info.time = boost::posix_time::to_iso_extended_string(t);

    tools::master_client::CJob job;

    job.set_xml("demo.xml");
    int ret = job.store("../../web/database/openflow.db", info);
    if (ret != 0) {
        LOG(ERROR) << "Fail to store job into database.";
        return -1;
    }

    LOG(INFO) << "submit job...";
    ret = client.submit_job(info.job_id);
    if (ret != 0) {
        LOG(WARNING) << "fail to submit job.";
    }

    LOG(INFO) << "submit job(" << info.job_id << ") successfully.";

    transport->close();
  } catch (TException &tx) {
    LOG(ERROR) << tx.what();
    return 1;
  }

  return 0;
}
