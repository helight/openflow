/**
 * Autor: RenZhen<renzhengeek@163.com>
 * Created: 2014.6.20
 * Description:
 * fake web client, in order to test Master server.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <glog/logging.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <thrift_helper.h>
#include "rpc/master/MasterService.h"
#include "job.h"

using namespace openflow::master;
using namespace apache;

int main(int argc, char** argv) {
    common::CThriftClientHelper<MasterServiceClient>
        thrift_client_helper("127.0.0.1", 9080);

    thrift_client_helper.connect();

    openflow::job_info info;
    info.job_name = "testor";

    //get local time
    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    info.time = boost::posix_time::to_iso_extended_string(t);

    tools::master_client::CJob job;

    //store job into databse
    job.set_xml("demo.xml");
    int ret = job.store("../../web/database/openflow.db", info);
    if (ret != 0) {
        LOG(ERROR) << "Fail to store job into database.";
        return -1;
    }

    //submit job to master server
    LOG(INFO) << "submit job...";
    ret = thrift_client_helper->submit_job(info.job_id);
    if (ret != 0) {
        LOG(ERROR) << "fail to submit job.";
    }

    LOG(INFO) << "submit job(" << info.job_id << ") successfully.";

    return 0;
}
