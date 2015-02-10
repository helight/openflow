// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>
// Created: 2014-06-27
// Description:
//
#include <iostream>
#include <boost/format.hpp>
#include <boost/serialization/singleton.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <tinyxml.h>
#include <dbhelp_factory.h>
#include "job.h"

DEFINE_string(db_name, "../../web/database/openflow.db", "");

namespace openflow { namespace tools{ namespace master_client{

void CJob::set_xml(const std::string &filename)
{
    _xml = filename;
}

//FIXME(renzhen):
//interface is bad desgined.
int CJob::store(const std::string &db_name, openflow::job_info &info)
{
    //convert xml file to string
    TiXmlDocument doc( _xml.c_str() );

    bool loadOkay = doc.LoadFile();
    if (!loadOkay)
    {
        LOG(ERROR) << "Failed to load file: " << _xml;
        return -1;
    }
    LOG(INFO) << _xml  << " opened.";

    TiXmlPrinter printer;
    printer.SetIndent( "\t" );
    doc.Accept( &printer );
    std::string xml =  printer.Str();

    common::CSqliteHelp& db_help =
        boost::serialization::singleton<common::CSqliteHelp>::get_mutable_instance();
    if (db_help.init(FLAGS_db_name, "", "", ""))
    {
        LOG(ERROR) << "sqlite dbhelp init error";
        return -2;
    }

    std::string sql = boost::str(boost::format("INSERT INTO tbJobs(job_name, xml_desc, time) \
         VALUES( '%s', '%s', '%s');") % info.job_name % xml % info.time);
    if (!db_help.update(sql))
    {
        LOG(ERROR) << "Non-Query error.";
        return -3;
    }
    LOG(INFO) << "xml saved to sqlite successfully.";

    //retrive this job ID, return it with info
    info.job_id = db_help.get_lastinsert_rowid();
    LOG(INFO) << "The last insert job ID is " << info.job_id << ".";

    return info.job_id;
}


}}} //namespace openflow::tools::master_client
