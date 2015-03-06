#ifndef COMMON_DBHELP_FACTORY_H
#define COMMON_DBHELP_FACTORY_H
#pragma once

#include "dbbase_help.h"
#include "sqlite_help.h"

namespace common {

class CDbhelpFactory
{
public:
    CDbhelpFactory();

public:
    bool init(const std::string& db_name, const std::string& db_user
              , const std::string& db_passwd, const std::string& db_host);
    // type: 0 -> sqlite ; 1 -> mysql
    bool get_dbhelp(const int32_t type, CDbBaseHelp& db_help);

private:
    bool _is_init;
    std::string _db_name;
    std::string _db_user;
    std::string _db_passwd;
    std::string _db_host;
};

} //namespace of common
#endif // CDbhelpFactory
