// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>
// Created: 2014-07-12
// Description:
//
#include <boost/format.hpp>
#include <cstring>
#include <glog/logging.h>
#include <sqlite3.h>
#include "sqlite_table.h"
#include "sqlite_db.h"

namespace common {

CTable *CSQLiteDatabase::new_table(const std::string &tbname, const std::string &tbitem)
{
    //judge whether this table exists
    bool ret = check_table_by_name(tbname);
    if(!ret)
    {
        return NULL;
    }

    CTable *table = new CSQLiteTable(this, tbname, tbitem);
    if (!table ) {
        //FIXME：should throw exception?
        return NULL;
    }

    //add table
    _tables.push_back(table);

    return table;
}

void CSQLiteDatabase::set_connect_str(const std::string &conn_str)
{
    _conn_str = conn_str;
}

bool CSQLiteDatabase::open()
{
    if (sqlite3_open_v2(_conn_str.c_str(), &_db_handler, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
    {
        int errcode = sqlite3_errcode(_db_handler);
        LOG(ERROR) << "Fail to open " << _dbname << " ERROR("
            << errcode << "): " << sqlite3_errmsg(_db_handler);

        return false;
    }

    return true;
}

bool CSQLiteDatabase::close()
{
    if (_db_handler)
    {
        sqlite3_close(_db_handler);
        _db_handler = NULL;
    }

    return 0;
}

bool CSQLiteDatabase::check_table_by_name(const std::string &tbname)
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
