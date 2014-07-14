// Copyright (c) 2014, OpenFlow
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#ifndef COMMON_DB_HELP_H
#define COMMON_DB_HELP_H
#pragma once

#include <boost/thread/mutex.hpp>
#include "database.h"
#include "table.h"

struct sqlite3_stmt;
typedef struct sqlite3_stmt sqlite3_stmt;

namespace common {

class CSQLiteTable : public CTable
{
public:
    CSQLiteTable(const CDatabase *db, const std::string& tbname, const std::string& tbitem = NULL);
    ~CSQLiteTable();
    bool init();

public:
    bool check_table_by_name();
    bool check_create_table();

    bool query(const std::string& sql);
    bool non_query(const std::string& sql);
    bool read(std::vector<std::string> &row);
    uint32_t get_count(const std::string& filter);
    uint32_t get_lastinsert_rowid(void);

protected:
    //not allowd to create instance by default constructor.
    CSQLiteTable()
    {}

private:
    boost::mutex _mutex;
    //FIXME: not used properly this moment.
    sqlite3_stmt* _stmt;
};

} // end namespace common
#endif // COMMON_DB_HELP_H
