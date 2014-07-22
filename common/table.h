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
    virtual ~CTable() {};
    //init environment
    virtual bool init() = 0;

protected:
    CTable() {};
    CTable(const std::string& tbname, const std::string& tbitem = "") :
          _tbname(tbname), _tbitem(tbitem)
    {}

public:
    //execute query.
    virtual bool set_query(const std::string &sql) = 0;
    //get a row, return ture; or get nothing, return false.
    virtual bool read(std::vector<std::string> &row) = 0;
    //execute non-query.
    virtual bool non_query(const std::string &sql) = 0;
    /**
     * get the number of rows.
     */
    virtual uint32_t get_row_count(void) = 0;
    //get this table last insert rowid.
    virtual  uint32_t get_lastinsert_rowid(void) = 0;

public:
    const std::string _tbname;
    const std::string _tbitem;    //table schema.
};

} //namespace common
#endif // TABLE_H
