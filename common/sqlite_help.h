#ifndef COMMON_SQLITE_HELP_H
#define COMMON_SQLITE_HELP_H
#pragma once

#include <boost/thread/mutex.hpp>
#include <sqlite3.h>

#include "dbbase_help.h"

namespace common {

class CSqliteHelp : public CDbBaseHelp
{
public:
    CSqliteHelp();
    virtual ~CSqliteHelp();

public:
    virtual bool init(const std::string& db_name, const std::string& db_user
              , const std::string& db_passwd, const std::string& db_host);

    virtual bool query(const std::string& sql, std::vector<std::string> &result, int32_t& columns);
    virtual bool update(const std::string& sql);
    virtual uint32_t get_row_count(const std::string& table, const std::string& con);
    virtual uint32_t get_lastinsert_rowid();

    virtual bool close();

private:
    bool check_table_by_name(const std::string& tbname);

private:
    bool _is_init;
    boost::mutex _mutex;
    std::string _db_name;     // db file path
    sqlite3* _db_handler;     //database handler.
};

} //namespace of common
#endif // COMMON_SQLITE_HELP_H
