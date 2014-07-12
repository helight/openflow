#ifndef TABLE_H
#define TABLE_H
#pragma once

/**
 * Table abstract class.
 */
namespace common {

class CTable
{
public:
    virtual ~CTable() {};
    //init environment
    virtual bool init() = 0;

protected:
    CTable(const CDatabase *db, const std::string& tbname, const std::string& tbitem = NULL) :
        _db(db), _tbname(tbname), _tbitem(tbitem)
    {}

public:
    //FIXME: should be moved to database class
    bool check_table_by_name(const std::string& tbname);
    bool check_create_table(const std::string& tbitem);

    //execute query.
    virtual bool query(const std::string &sql) = 0;
    //execute non-query.
    virtual bool update(const std::string &sql) = 0;
    /**
     * get the number of rows.
     * @filter: where clause.
     */
    virtual uint32_t get_count(const std::string &filter);

public:
    std::string _tbname;
    std::string _tbitem;    //table schema.
protected:
    CDatabase *_db;         //point to  database that own this table.
};

} //namespace common
#endif // TABLE_H
