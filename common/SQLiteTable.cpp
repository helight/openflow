// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sqlite3.h>
#include "SQLiteTable.h"

// for sqlite conf
DEFINE_string(sqlitedb, "openflow.db", "sqlite db name for meta node");
DEFINE_string(tb_jobs, "tbJobs", "tbname for jobs table");
DEFINE_string(jobs_items, "job_id, job_name, xml_desc, time", "item for table tb_jobs");


namespace common {

CSqliteTable::CSqliteTable(const CDatabase *db, const std::string& tbname,
    const std::string& tbitem = NULL) :
    CTable(db, tbname, tbitem), _stmt(NULL)
{}

CSqliteTable::~CSqliteTable()
{
    if (_stmt)
    {
        sqlite3_finalize(stmt);
        _stmt = NULL;
    }
}

bool CSqliteTable::init()
{
    //FIXME: should take the job of creating table?
    check_create_table();

    return true;
}

bool CSqliteTable::check_table_by_name()
{
    int num_rows = 0;
    int num_cols = 0;
    char* errmsg = NULL;
    char** result = NULL;

    std::string check_sql = boost::str(boost::format("select * from %s limit 1;") % _tbname);
    int ret = sqlite3_get_table(_db._db_id, check_sql.c_str(), &result,
                                &num_rows, &num_cols, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << check_sql << " sqlite db:" << _db._db_id << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
    }
    sqlite3_free_table(result);

    return (SQLITE_OK == ret);
}

// std::string tbname = "tbErrorDataNode";
// ip, disk, timestamp
bool CSqliteTable::check_create_table()
{
    bool is_ok = true;

    if (!check_table_by_name(_tbname))
    {
        std::string sql = boost::str(boost::format(
                "drop table if exists %s; create table %s (%s);")
            % _tbname % _tbname % _tbitem);
        char* errmsg = NULL;
        int ret = sqlite3_exec(_db._db_id, sql.c_str(), 0, 0, &errmsg);
        if (ret != SQLITE_OK)
        {
            LOG(ERROR) << sql << " sqlite db:" << _db._db_id << " ERROR: " << errmsg;
            sqlite3_free(errmsg);
            is_ok = false;
        }
    }

    return is_ok;
}

bool CSqliteTable::update(const std::string& sql)
{
    char* errmsg = NULL;
    int ret = sqlite3_exec(_db._db_id, sql.c_str(), 0, 0, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

bool CSqliteTable::query(const std::string& sql)
{
    impl_query(sql, &_stmt);
}

// call sqlite3_finalize(stmt) to free memery after stmt used
bool CSqliteTable::impl_query(const std::string& sql, sqlite3_stmt** stmt)
{
    sqlite3_stmt *stmt_tmp = NULL;
    const char *errmsg = NULL;
    int nret = 0;

    nret = sqlite3_prepare(_db._db_id, sql.c_str(), sql.size(), &stmt_tmp, &errmsg);
    if(SQLITE_OK == nret)
    {
        *stmt = stmt_tmp;
    }
    else
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_finalize(stmt_tmp);
    }
    // sqlite3_finalize(stmt); free memery after stmt used
    //
    return (SQLITE_OK == nret);
}

uint32_t CSqliteTable::get_count(const std::string& filter)
{
    uint32_t count = 0;
    std::string sql = boost::str(boost::format("select count(*) from %s %s") % _tbname % filter);
    sqlite3_stmt *stmt = NULL;
    if(impl_query(sql, &stmt))
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            count = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);

    return count;
}

} // end namespace common
