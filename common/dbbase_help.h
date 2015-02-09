#ifndef COMMON_DBBASE_HELP_H
#define COMMON_DBBASE_HELP_H
#pragma once

namespace common {

enum DbType
{
    DBTYPE_SQLITE = 0,
    DBTYPE_MYSQL  = 1
};


class CDbBaseHelp
{
public:
    CDbBaseHelp() {};
    virtual ~CDbBaseHelp() {};

public:
    virtual bool init(const std::string& db_name, const std::string& db_user, const std::string& db_passwd, const std::string& db_host);
    virtual bool close();

    virtual bool query(const std::string& sql, std::vector<std::string> &result, int32_t& columns);
    virtual bool update(const std::string& sql);

    virtual uint32_t get_row_count(const std::string& table, const std::string& con);
    virtual uint32_t get_lastinsert_rowid();
};

} //namespace of common
#endif // COMMON_DBBASE_HELP_H
