#ifndef DATABASE_H
#define DATABASE_H
#pragma once

#include <sqlite3.h>
#include "common_config.h"

namespace common {
/**
 * database abstract class
 */
class CDatabase
{
public:
    virtual ~CDatabase() {
#if USING_SQLITE_DB
        if (_db_id)
        {
            sqlite3_close(_db_id);
            _db_id = NULL;
        }
#elif USING_MYSQL_DB
        //for mysql
#endif
    }

    virtual CTable *new_table(const std::string &tbname, const std::string &tbitem=NULL) = 0;
    virtual bool open();

protected:
    CDatabase(DB_TYPE dbtype, const std::string &dbname) :
        _dbtype(dbtype), _dbname(dbname)
    {}

public:
    DB_TYPE _dbtype;
    db_id_t *_db_id;     //database handler, such as sqlite3 for sqlite.
    std::string _dbname;
    //FIXME: better to use table name get table pointer.
    vector<CTable *> _tables;
};
} //namespace of common
#endif // DATABASE_H
