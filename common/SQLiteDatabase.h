#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H
#pragma once

#include <iostream>
#include "db_config.h"
#include "database.h"

namespace common {
/**
 * SQLite Databse class.
 * like factory for SQLiteTable, hosts all table instances.
 */
class CSQLiteDatabase : public CDatabase
{
public:
    ~CSQLiteDatabase()
    {}
    CSQLiteDatabase(DB_TYPE dbtype, const std::string &dbname) :
        CDatabase(dbtype, dbname)
    {}

    CTable *new_table(const std::string &tbname, const std::string &tbitem="");
    void set_connect_str(const std::string &conn_str);
    bool open();
    bool close();

private:
    //not allowed create instance by default constructor.
    CSQLiteDatabase()
    {}
};

} //namespace of common
#endif // SQLITEDATABASE_H
