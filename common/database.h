#ifndef DATABASE_H
#define DATABASE_H
#pragma once

#include <vector>
#include <sqlite3.h>
#include "db_config.h"
#include "table.h"

namespace common {

class CTable;

/**
 * database abstract class
 */
class CDatabase
{
public:
    CDatabase(DB_TYPE dbtype, const std::string &dbname) :
        _dbtype(dbtype), _dbname(dbname)
    {}
    virtual ~CDatabase() {};

    virtual CTable *new_table(const std::string &tbname, const std::string &tbitem="") = 0;
    virtual void set_connect_str(const std::string &conn_str) = 0;
    virtual bool open(void) = 0;
    virtual bool close(void) = 0;

protected:
    CDatabase() {};

private:
     bool check_table_by_name(const std::string& tbname);

public:
    DB_TYPE _dbtype;
    db_handler_type *_db_handler;     //database handler, such as sqlite3 for sqlite.
    std::string _dbname;
    std::string _conn_str;
    //FIXME: better to use table name get table pointer.
    std::vector<CTable *> _tables;
};
} //namespace of common
#endif // DATABASE_H
