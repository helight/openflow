// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-25
// Description:
//
#ifndef COMMON_DB_HELP_H
#define COMMON_DB_HELP_H
#pragma once

#include <boost/thread/mutex.hpp>

struct sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

class CSqliteHelp
{
public:
    CSqliteHelp(const std::string& db_name);
    CSqliteHelp();
    ~CSqliteHelp();
    bool init();

public:
    bool check_create_table(const std::string& tbname, const std::string& tbitem);
    bool query(const std::string& sql, sqlite3_stmt** stmt);
    bool update(const std::string& sql);
    bool check_table_by_name(const std::string& tbname);
    uint32_t get_count(const std::string& tbname, const std::string& con);

private:
    boost::mutex _mutex;
    sqlite3* _sqlite;
    std::string _db_name;
};

#endif // COMMON_DB_HELP_H
