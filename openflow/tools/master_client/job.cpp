// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>
// Created: 2014-06-27
// Description:
//
#include <iostream>
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <tinyxml.h>
#include "../../common/table.h"
#include "../../common/database.h"
#include "../../common/dataset.h"
#include "../../common/db_config.h"
#include "job.h"

namespace tools{ namespace master_client{

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
    if (loadOkay)
    {
        LOG(INFO) << _xml  << " opened.";
    }else{
        LOG(ERROR) << "Failed to load file: " << _xml;
        return -1;
    }

    TiXmlPrinter printer;
    printer.SetIndent( "\t" );
    doc.Accept( &printer );
    std::string xml =  printer.Str();

    //store job into database
    common::CDatabase *db = common::CDataSet::get_instance()->new_database(common::DB_SQLITE,
        db_name);

    //SQLite database file path just like connect string.
    db->set_connect_str("../../web/database/openflow.db");
    //open connect handler
    db->open();

    //FIXME: table name should not be fixed.
    common::CTable *table = db->new_table("tbJobs");

    std::string sql = boost::str(boost::format("INSERT INTO tbJobs(job_name, xml_desc, time) \
         VALUES( '%s', '%s', '%s');") % info.job_name % xml % info.time);
    if ( !table->non_query(sql))
    {
        LOG(ERROR) << "Non-Query error.";
        //FIXME: ugly design.
        db->close();        //close database connection.
        delete table;
        delete db;
        return -1;
    }
    LOG(INFO) << "xml saved to sqlite successfully.";

    //retrive this job ID, return it with info
    info.job_id = table->get_lastinsert_rowid();
    LOG(INFO) << "The last insert job ID is " << info.job_id << ".";

    db->close();        //close database connection.
    delete table;
    delete db;

    return info.job_id;
}

} //END OF NAMESPACE TOOLS
} //END FO NAMESPACE MASTER_CLIENT
