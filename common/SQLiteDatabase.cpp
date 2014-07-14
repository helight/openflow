// Copyright (c) 2014, OpenFlow
// Copyright (c) 2014, OpenFlow
// Author: RenZhen<renzhen@163.com>
// Created: 2014-07-12
// Description:
//
#include <cstring>
#include <glog/logging.h>
#include <sqlite3.h>
#include "SQLiteTable.h"
#include "SQLiteDatabase.h"

namespace common {

CTable *CSQLiteDatabase::new_table(const std::string &tbname, const std::string &tbitem)
{
    //FIXME: Should judge whether this table exists
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
    if (sqlite3_open_v2(_conn_str.c_str(), &_db_id, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
    {
        int errcode = sqlite3_errcode(_db_id);
        LOG(ERROR) << "opened " << _dbname << " ERROR("
            << errcode << "): " << sqlite3_errmsg(_db_id);

        return false;
    }

    return true;
}

bool CSQLiteDatabase::close()
{
    if (_db_id)
    {
        sqlite3_close(_db_id);
        _db_id = NULL;
    }

    return 0;
}

} //namespace of common
