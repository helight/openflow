// Copyright (c) 2014, HelightXu
// Author: RenZhen<renzhen@163.com>
// Created: 2014-07-12
// Description:
//
#include <string>
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

bool CSQLiteDatabase::open()
{
    if (sqlite3_open(_dbname.c_str(), &_db_id) != SQLITE_OK)
    {
        int errcode = sqlite3_errcode(_db_id);
        LOG(ERROR) << "opened " << _db_name << " ERROR("
            << errcode << "): " << sqlite3_errmsg(_db_id);

        return false;
    }
}

} //namespace of common
