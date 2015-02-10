// Copyright (c) 2014, OpenFlow
// Author: Helight<helight@zhwen.org>
// Created: 2015-02-09
// Description:
//
#include <boost/serialization/singleton.hpp>
#include <glog/logging.h>
#include "dbhelp_factory.h"

namespace common {

CDbhelpFactory::CDbhelpFactory() : _is_init(false)  {};

bool CDbhelpFactory::init(const std::string& db_name, const std::string& db_user
                          , const std::string& db_passwd, const std::string& db_host)
{
    _db_name = db_name;
    _db_user = db_user;
    _db_passwd = db_passwd;
    _db_host = db_host;

    return true;
}

bool CDbhelpFactory::get_dbhelp(const int32_t type, CDbBaseHelp& db_help)
{
    bool ret = false;

    if (_is_init)
    {
        if (DBTYPE_SQLITE == type)
        {
            db_help = boost::serialization::singleton<CSqliteHelp>::get_mutable_instance();
            if (db_help.init(_db_name, _db_user, _db_passwd, _db_host))
            {
                LOG(ERROR) << "sqlite dbhelp init error";
            }
            else
                ret = true;
        }
        else if (DBTYPE_MYSQL == type)
        {
        }
    }

    return ret;
}

} //namespace of common
