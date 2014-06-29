// Copyright (c) 2014, HelightXu
// Author: RenZhen<renzhen@163.com>
// Created: 2014-06-27
// Description:
//
#include <iostream>
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <tinyxml.h>
#include <sqlite3.h>
#include "CJob.h"

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

        sqlite3 *db;
        int rc = sqlite3_open_v2(db_name.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);
        if( rc != SQLITE_OK){
            LOG(ERROR) << "Can't open database: " << sqlite3_errmsg(db);
            return -1;
        }

        std::string sql;
        sql = "INSERT INTO tbJobs(job_name, xml_desc, time) VALUES(\"" + info.job_name + "\",:xml,\""
            + info.time + "\");";

        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (!stmt){
            LOG(ERROR) << "SQL compiler error:" << sqlite3_errmsg(db);
            sqlite3_close(db);
            return -1;
        }

        //bind xml
        int index = sqlite3_bind_parameter_index(stmt, ":xml");
        rc = sqlite3_bind_text(stmt, index, xml.c_str(), xml.length(), NULL);
        if( rc != SQLITE_OK ){
            LOG(ERROR) << "Fail to binding parameter xml:" << sqlite3_errmsg(db);
            sqlite3_close(db);
            return -1;
        }

        rc = sqlite3_step(stmt);
        //FIXME:excute successfully, but always say Unkown error.
        /*
           if( rc != SQLITE_OK){
           LOG(INFO) << "SQL Excute: " << sqlite3_errmsg(db);
           sqlite3_finalize(stmt);
           sqlite3_close(db);
           return(1);
           }
           */
        LOG(INFO) << "xml saved to sqlite successfully.";

        info.job_id = sqlite3_last_insert_rowid(db);
        LOG(INFO) << "The last insert job ID is " << info.job_id << ".";

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

} //END OF NAMESPACE TOOLS
} //END FO NAMESPACE MASTER_CLIENT
