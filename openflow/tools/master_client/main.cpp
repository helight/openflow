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
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <main_template.h>
#include <thrift_helper.h>
#include <utils.h>
#include "rpc/master/MasterService.h"
#include "job.h"

namespace openflow { namespace tools { namespace master_client{

using namespace apache::thrift;

void submit_job(const uint32_t job_id)
{
    //submit job to master server
    LOG(INFO) << "submit job... " << job_id;
    common::CThriftClientHelper<master::MasterServiceClient> *thrift_client_helper =
        new  common::CThriftClientHelper<master::MasterServiceClient>("127.0.0.1", 9080);
    try
    {
        thrift_client_helper->connect();
        int32_t ret = thrift_client_helper->get()->submit_job(job_id);
        if (ret < 0) {
            LOG(ERROR) << "fail to submit job.";
        }
        else
        {
            LOG(INFO) << "submit job(" << job_id << ") successfully.";
        }
    }
    catch (TException& exn)
    {
        LOG(ERROR) <<  exn.what();
    }

    thrift_client_helper->close();
    delete thrift_client_helper;
}

void report_agent_state()
{
    //AddMe ZhangYiFei 2014/10/30 test master side reporte_agent_sate interface
    openflow::agent_state state;
    state.remain_mem = "600MB";
    state.mem_use_percent = "80%";
    state.cpu_idle_percent = "70%";
    state.cpu_load = "1.2 4.3 5.6";
    state.ipaddr = "192.168.0.1";
    state.swap_use_percent = "0%";
    common::CThriftClientHelper<master::MasterServiceClient> *thrift_client_helper =
        new  common::CThriftClientHelper<master::MasterServiceClient>("127.0.0.1", 9080);
    try
    {
        thrift_client_helper->connect();
        int32_t ret = thrift_client_helper->get()->report_agent_state(state);
        if (ret < 0)
        {
            LOG(INFO) << ret;
        }
    }
    catch (TException& exn)
    {
        LOG(ERROR) << exn.what();
    }

    thrift_client_helper->close();
    delete thrift_client_helper;
}

void get_agent_state()
{
    common::CThriftClientHelper<master::MasterServiceClient> *thrift_client_helper =
        new  common::CThriftClientHelper<master::MasterServiceClient>("127.0.0.1", 9080);
    try
    {
        std::vector<openflow::agent_state> agent_state;
        thrift_client_helper->connect();
        thrift_client_helper->get()->get_agent_info(agent_state);
        LOG(INFO) << "agent num: " << agent_state.size();
    }
    catch (TException& exn)
    {
        LOG(ERROR) << exn.what();
    }

    thrift_client_helper->close();
    delete thrift_client_helper;
}

extern "C" int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "params not enought: ./client cmd" << std::endl;
        std::cout << "./client submit_job/report_agent/get_agent" << std::endl;
        return -1;
    }
    // init glog,gflags
    FLAGS_logbufsecs = 0;           // write log no cache
    FLAGS_max_log_size = 300;       // max log size for each log file
    FLAGS_log_dir = boost::str(boost::format("%s/") % common::CUtils::get_program_path());
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("master_client");

    std::string cmd = argv[1];
    std::cout << "CMD: " << cmd << std::endl;

    if (0 == strcmp(cmd.c_str(), "submit_job"))
    {
        if (argc < 3)
        {
            std::cout << "params not enought: ./client submit_job job_id" << std::endl;
            return -1;
        }
        uint32_t job_id = boost::lexical_cast<uint32_t>(*argv[2]);
        submit_job(job_id);
    }
    else if (0 == strcmp(cmd.c_str(), "report_agent"))
    {
        report_agent_state();
    }
    else if (0 == strcmp(cmd.c_str(), "get_agent"))
    {
        get_agent_state();
    }
    else
    {
        std::cout << "params not enought: ./master cmd" << std::endl;
        std::cout << "./master submit_job/report_agent/get_agent" << std::endl;
        return -1;
    }

    return 0;
}

}}} //namespace openflow::tools::master_client
