// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#include <boost/format.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sqlite3.h>
#include "db_help.h"

// for sqlite conf
DEFINE_string(sqlitedb, "openflow.db", "sqlite db name for meta node");
DEFINE_string(tb_jobs, "tbJobs", "tbname for jobs table");
DEFINE_string(jobs_items, "job_id, job_name, xml_desc, time", "item for table tb_jobs");

namespace common {

CSqliteHelp::CSqliteHelp(const std::string& db_name) :
    _sqlite(NULL), _db_name(db_name)
{}

CSqliteHelp::CSqliteHelp() : _sqlite(NULL)
{}

CSqliteHelp::~CSqliteHelp()
{
    if (_sqlite)
    {
        sqlite3_close(_sqlite);
        _sqlite = NULL;
    }
}

bool CSqliteHelp::init()
{
    _db_name = FLAGS_sqlitedb;
    if (sqlite3_open(_db_name.c_str(), &_sqlite) != SQLITE_OK)
    {
        int errcode = sqlite3_errcode(_sqlite);
        LOG(ERROR) << "opened " << _db_name << " ERROR("
            << errcode << "): " << sqlite3_errmsg(_sqlite);

        return false;
    }
    check_create_table(FLAGS_tb_jobs, FLAGS_jobs_items);

    return true;
}

bool CSqliteHelp::check_table_by_name(const std::string& tbname)
{
    int num_rows = 0;
    int num_cols = 0;
    char* errmsg = NULL;
    char** result = NULL;

    std::string check_sql = boost::str(boost::format("select * from %s limit 1;") % tbname);
    int ret = sqlite3_get_table(_sqlite, check_sql.c_str(), &result,
                                &num_rows, &num_cols, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << check_sql << " sqlite db:" << _sqlite << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
    }
    sqlite3_free_table(result);

    return (SQLITE_OK == ret);
}

// std::string tbname = "tbErrorDataNode";
// ip, disk, timestamp
bool CSqliteHelp::check_create_table(const std::string& tbname,
                                     const std::string& tbitem)
{
    bool is_ok = true;

    if (!check_table_by_name(tbname))
    {
        std::string sql = boost::str(boost::format(
                "drop table if exists %s; create table %s (%s);")
            % tbname % tbname % tbitem);
        char* errmsg = NULL;
        int ret = sqlite3_exec(_sqlite, sql.c_str(), 0, 0, &errmsg);
        if (ret != SQLITE_OK)
        {
            LOG(ERROR) << sql << " sqlite db:" << _sqlite << " ERROR: " << errmsg;
            sqlite3_free(errmsg);
            is_ok = false;
        }
    }

    return is_ok;
}

bool CSqliteHelp::update(const std::string& sql)
{
    char* errmsg = NULL;
    int ret = sqlite3_exec(_sqlite, sql.c_str(), 0, 0, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

// call sqlite3_finalize(stmt) to free memery after stmt used
bool CSqliteHelp::query(const std::string& sql, sqlite3_stmt** stmt)
{
    sqlite3_stmt *stmt_tmp = NULL;
    const char *errmsg = NULL;
    int nret = 0;

    nret = sqlite3_prepare(_sqlite, sql.c_str(), sql.size(), &stmt_tmp, &errmsg);
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

uint32_t CSqliteHelp::get_count(const std::string& tbname, const std::string& con)
{
    uint32_t count = 0;
    std::string sql = boost::str(boost::format("select count(*) from %s %s") % tbname % con);
    sqlite3_stmt *stmt = NULL;
    if(query(sql, &stmt))
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
