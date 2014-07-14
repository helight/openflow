#ifndef TABLE_H
#define TABLE_H
#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "database.h"
/**
 * Table abstract class.
 */
namespace common {

class CDatabase;

class CTable
{
public:
    virtual ~CTable(){};
    //init environment
    virtual bool init() = 0;

protected:
    CTable(){};
    CTable(const CDatabase *db, const std::string& tbname, const std::string& tbitem = NULL) :
          _db(db), _tbname(tbname), _tbitem(tbitem)
    {}

public:
    //FIXME: should be moved to database class
    bool check_table_by_name(const std::string& tbname);
    bool check_create_table(const std::string& tbitem);

    //execute query.
    virtual bool query(const std::string &sql) = 0;
    //get a row, return ture; or get nothing, return false.
    virtual bool read(std::vector<std::string> &row) = 0;
    //execute non-query.
    virtual bool non_query(const std::string &sql) = 0;
    /**
     * get the number of rows.
     * @filter: where clause.
     */
    virtual uint32_t get_count(const std::string &filter) = 0;
    //get this table last insert rowid.
    virtual  uint32_t get_lastinsert_rowid(void) = 0;

protected:
    const CDatabase *_db;         //point to  database that own this table.

public:
    const std::string _tbname;
    const std::string _tbitem;    //table schema.
};

} //namespace common
#endif // TABLE_H
