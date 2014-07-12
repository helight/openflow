// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#ifndef COMMON_DB_HELP_H
#define COMMON_DB_HELP_H
#pragma once

#include <boost/thread/mutex.hpp>
#include "table.h"

struct sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

namespace common {

class CSqliteTable : public CTable
{
public:
    CSqliteTable(const CDatabase *db, const std::string& tbname, const std::string& tbitem = NULL);
    ~CSqliteTable();
    bool init();

public:
    bool check_table_by_name();
    bool check_create_table();
    bool query(const std::string& sql);
    bool update(const std::string& sql);
    uint32_t get_count(const std::string& filter);

protected:
    //not allowd to create instance by default constructor.
    CSqliteTable();

private:
   bool impl_query(const std::string& sql, sqlite3_stmt** stmt);

private:
    boost::mutex _mutex;
    //FIXME: not used properly this moment.
    sqlite3_stmt* _stmt;
};

} // end namespace common
#endif // COMMON_DB_HELP_H
