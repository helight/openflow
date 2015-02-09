// Copyright (c) 2014, OpenFlow
// Author: Helight<helight@zhwen.org>
// Created: 2015-02-09
// Description:
//

#include <boost/format.hpp>
#include <glog/logging.h>
#include "sqlite_help.h"

namespace common {

CSqliteHelp::CSqliteHelp() : _is_init(false) {};
CSqliteHelp::~CSqliteHelp()
{
    close();
}

bool CSqliteHelp::init(const std::string& db_name, const std::string& db_user, const std::string& db_passwd, const std::string& db_host)
{
    if (!_is_init)
    {
        _db_name = db_name;
        if (sqlite3_open_v2(db_name.c_str(), &_db_handler, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        {
            int errcode = sqlite3_errcode(_db_handler);
            LOG(ERROR) << "Fail to open " << db_name << " ERROR("
                << errcode << "): " << sqlite3_errmsg(_db_handler);

            return false;
        }
        _is_init = true;
    }

    return true;
}

bool CSqliteHelp::close()
{
    if (_db_handler)
    {
        sqlite3_close(_db_handler);
        _db_handler = NULL;
    }

    return true;
}

bool CSqliteHelp::query(const std::string& sql, std::vector<std::string> &result, int32_t& columns)
{
    int num_rows = 0;
    int num_cols = 0;
    char* errmsg = NULL;
    char** result_tmp = NULL;

    int ret = sqlite3_get_table(_db_handler, sql.c_str(), &result_tmp,
                                &num_rows, &num_cols, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " sqlite db:" << _db_handler << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return false;
    }

    char **result_start = result_tmp;
    for(int i=0; i < ((num_rows + 1) * num_cols); i++)
    {
        std::string tmp = *result_tmp++;
        result.push_back(tmp);
    }

    columns = num_cols;
    sqlite3_free_table(result_start);

    return true;
}

bool CSqliteHelp::update(const std::string& sql)
{
    char* errmsg = NULL;

    boost::mutex::scoped_lock lock(_mutex);
    int ret = sqlite3_exec(_db_handler, sql.c_str(), 0, 0, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << sql << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
        return false;
    }

    return true;
}

uint32_t CSqliteHelp::get_row_count(const std::string& table, const std::string& con)
{
    uint32_t count = 0;
    sqlite3_stmt *stmt = NULL;
    const char *errmsg = NULL;
    int nret = 0;

    std::string sql;
    if (con.empty())
        sql = boost::str(boost::format("select count(*) from %s;") % table);
    else
        sql = boost::str(boost::format("select count(*) from %s where %s;") % table % con);
    nret = sqlite3_prepare_v2(_db_handler, sql.c_str(), sql.size(), &stmt, &errmsg);
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

uint32_t CSqliteHelp::get_lastinsert_rowid(void)
{
    return sqlite3_last_insert_rowid(_db_handler);
}

bool CSqliteHelp::check_table_by_name(const std::string& tbname)
{
    int num_rows = 0;
    int num_cols = 0;
    char* errmsg = NULL;
    char** result = NULL;

    std::string check_sql = boost::str(boost::format("select * from %s limit 1;") % tbname);
    int ret = sqlite3_get_table(_db_handler, check_sql.c_str(), &result,
                                &num_rows, &num_cols, &errmsg);
    if (ret != SQLITE_OK)
    {
        LOG(ERROR) << check_sql << " sqlite db:" << _db_handler << " ERROR: " << errmsg;
        sqlite3_free(errmsg);
    }
    sqlite3_free_table(result);

    return (SQLITE_OK == ret);
}

} //namespace of common
