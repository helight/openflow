// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sqlite3.h>
#include "sqlite_table.h"

// for sqlite conf
DEFINE_string(sqlitedb, "openflow.db", "sqlite db name for meta node");
DEFINE_string(tb_jobs, "tbJobs", "tbname for jobs table");
DEFINE_string(jobs_items, "job_id, job_name, xml_desc, time", "item for table tb_jobs");


namespace common {

CSQLiteTable::CSQLiteTable(const CDatabase *db, const std::string& tbname
    ,const std::string& tbitem) :
    CTable(db, tbname, tbitem), _stmt(NULL)
{}

CSQLiteTable::~CSQLiteTable()
{
    if (_stmt)
    {
        sqlite3_finalize(_stmt);
        _stmt = NULL;
    }
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
    int ret = sqlite3_exec(_db->_db_id, sql.c_str(), 0, 0, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

// call sqlite3_finalize(stmt) to free memery after stmt used
bool CSQLiteTable::set_query(const std::string& sql)
{
    sqlite3_stmt *stmt_tmp = NULL;
    const char *errmsg = NULL;
    int nret = 0;

    nret = sqlite3_prepare(_db->_db_id, sql.c_str(), sql.size(), &stmt_tmp, &errmsg);
    if(SQLITE_OK == nret)
    {
        _stmt = stmt_tmp;
    }
    else
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_finalize(stmt_tmp);
    }

    return (SQLITE_OK == nret);
}

bool CSQLiteTable::read(std::vector<std::string> &row)
{
    int col_count = sqlite3_column_count(_stmt);

    if (SQLITE_ROW == sqlite3_step(_stmt))
    {
        for (int i = 0; i < col_count; i++)
        {
           std::string tmp = (const char*)sqlite3_column_text(_stmt, i);
           row.push_back(tmp);
        }
        return true;
    }

    return false;
}

uint32_t CSQLiteTable::get_row_count()
{
    uint32_t count = 0;
    std::string sql = boost::str(boost::format("select count(*) from %s;") % _tbname);

    if(set_query(sql))
    {
        if (sqlite3_step(_stmt) == SQLITE_ROW)
        {
            count = sqlite3_column_int(_stmt, 0);
        }
    }

    return count;
}

uint32_t CSQLiteTable::get_lastinsert_rowid(void)
{
    return sqlite3_last_insert_rowid(_db->_db_id);
}

} // end namespace common
