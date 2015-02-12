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
#include <gflags/gflags.h>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <main_template.h>
#include <thrift_helper.h>
#include <utils.h>
#include "rpc/master/MasterService.h"
#include "job.h"

namespace openflow { namespace tools { namespace master_client{

using namespace apache::thrift;

class CMainHelper : public common::IMainHelper
{
private:
    bool init(int argc, char* argv[]);
    bool run();
    void fini();

private:
    common::CThriftClientHelper<master::MasterServiceClient> *thrift_client_helper;
};

bool CMainHelper::init(int argc, char* argv[])
{

    thrift_client_helper = new  common::CThriftClientHelper<master::MasterServiceClient>("127.0.0.1", 9080);
    if (NULL == thrift_client_helper)
    {
        LOG(ERROR) << "Fail to create master client.";
        return false;
    }
    return true;
}

bool CMainHelper::run()
{
    try
    {
        thrift_client_helper->connect();
    }
    catch (TException& exn)
    {
        LOG(ERROR) << exn.what();
        return false;
    }

    openflow::job_info info;
    info.job_name = "testor";

    //get local time
    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    info.time = boost::posix_time::to_iso_extended_string(t);

    tools::master_client::CJob job;

    //store job into databse
    job.set_xml("demo.xml");
    int ret = job.store("openflow.db", info);
    if (ret < 0)
    {
        LOG(ERROR) << "Fail to store job into database.";
        return false;
    }

    //submit job to master server
    LOG(INFO) << "submit job...";
    try
    {
        ret = thrift_client_helper->get()->submit_job(info.job_id);
        if (ret < 0) {
            LOG(ERROR) << "fail to submit job.";
            return false;
        }

    }
    catch (TException& exn)
    {
        LOG(ERROR) <<  exn.what();
        return false;
    }

    LOG(INFO) << "submit job(" << info.job_id << ") successfully.";

    //AddMe ZhangYiFei 2014/10/30 test master side reporte_agent_sate interface
    openflow::agent_state state;
    state.remain_mem = "600MB";
    state.mem_use_percent = "80%";
    state.cpu_idle_percent = "70%";
    state.cpu_load = "1.2 4.3 5.6";
    state.ipaddr = "192.168.0.1";
    state.swap_use_percent = "0%";
    try
    {
        ret = thrift_client_helper->get()->report_agent_state(state);
        if (ret < 0)
        {
            LOG(INFO) << ret;
            return false;
        }
    }
    catch (TException& exn)
    {
        LOG(ERROR) << exn.what();
        return false;
    }

    thrift_client_helper->close();

    return true;
}

void CMainHelper::fini()
{
    if(!thrift_client_helper)
    {
        delete thrift_client_helper;
    }
}

extern "C" int main(int argc, char* argv[])
{
    // init glog,gflags
    FLAGS_logbufsecs = 0;           // write log no cache
    FLAGS_max_log_size = 300;       // max log size for each log file
    FLAGS_log_dir = boost::str(boost::format("%s/") % common::CUtils::get_program_path());
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("master_client");

    CMainHelper main_helper;

    return main_template(&main_helper, argc, argv);
}

}}} //namespace openflow::tools::master_client
