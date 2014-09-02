// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#ifndef COMMON_DB_HELP_H
#define COMMON_DB_HELP_H
#pragma once

#include <boost/thread/mutex.hpp>
#include "sqlite_db.h"
#include "table.h"

struct sqlite3_stmt;
typedef struct sqlite3_stmt sqlite3_stmt;

namespace common {

class CSQLiteTable : public CTable
{
public:
    CSQLiteTable(const CSQLiteDatabase* db, const std::string& tbname, const std::string& tbitem = "");
    ~CSQLiteTable();
    bool init();

public:
    int32_t query(const std::string& sql, std::vector<std::string> &result, int32_t& columns);
    bool non_query(const std::string& sql);
    uint32_t get_row_count(void);
    uint32_t get_lastinsert_rowid(void);

protected:
    //not allowd to create instance by default constructor.
    CSQLiteTable()
    {}

private:
    boost::mutex _mutex;
    const CSQLiteDatabase* _db;   //point to  database that hold this table.
};

} // end namespace common
#endif // COMMON_DB_HELP_H
