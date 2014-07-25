// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sqlite3.h>
#include "sqlite_db.h"
#include "sqlite_table.h"

// for sqlite conf
DEFINE_string(sqlitedb, "openflow.db", "sqlite db name for meta node");
DEFINE_string(tb_jobs, "tbJobs", "tbname for jobs table");
DEFINE_string(jobs_items, "job_id, job_name, xml_desc, time", "item for table tb_jobs");


namespace common {

CSQLiteTable::CSQLiteTable(const CSQLiteDatabase *db, const std::string& tbname
                           ,const std::string& tbitem) : CTable(tbname, tbitem)
{
    _db = db;
}

CSQLiteTable::~CSQLiteTable()
{
}

bool CSQLiteTable::init()
{
    //FIXME: should take the job of creating table?
    //I think it's CSQLiteDatabase responsibility to check if a table exits.
    //check_create_table();

    return true;
}

bool CSQLiteTable::non_query(const std::string& sql)
{
    char* errmsg = NULL;

    boost::mutex::scoped_lock lock(_mutex);
    int ret = sqlite3_exec(_db->_db_handler, sql.c_str(), 0, 0, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

// Use of sqlite3_get_table  is not recommended.
int32_t CSQLiteTable::query(const std::string& sql, std::vector<std::string> &result
                         ,int32_t &columns)
{
    int num_rows = 0;
    int num_cols = 0;
    char* errmsg = NULL;
    char** result_tmp = NULL;

    int ret = sqlite3_get_table(_db->_db_handler, sql.c_str(), &result_tmp,
                                &num_rows, &num_cols, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " sqlite db:" << _db->_db_handler << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return -1;
    }

    char **result_start = result_tmp;
    for(int i=0; i < (num_rows+1)*num_cols; i++)
    {
        std::string tmp = *result_tmp++;
        result.push_back(tmp);
    }

    columns = num_cols;
    sqlite3_free_table(result_start);

    return num_rows;
}

uint32_t CSQLiteTable::get_row_count()
{
    uint32_t count = 0;
    sqlite3_stmt *stmt = NULL;
    const char *errmsg = NULL;
    int nret = 0;

    std::string sql = boost::str(boost::format("select count(*) from %s;") % _tbname);
    nret = sqlite3_prepare_v2(_db->_db_handler, sql.c_str(), sql.size(), &stmt, &errmsg);
    if(SQLITE_OK == nret)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            count = sqlite3_column_int(stmt, 0);
        }
    }
    else
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        count = -1;
    }

    sqlite3_finalize(stmt);
    return count;
}

uint32_t CSQLiteTable::get_lastinsert_rowid(void)
{
    return sqlite3_last_insert_rowid(_db->_db_handler);
}

} // end namespace common
