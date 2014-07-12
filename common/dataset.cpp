// Copyright (c) 2014, HelightXu
// Author: RenZhen<renzhen@163.com>
// Created: 2014-07-12
// Description:
//

#include <iostream>
#include <string>
#include "common_config.h"
#include "dataset.h"

namespace common {
CDatabase *CDataSet::new_database(DB_TYPE dbtype, const std::string &dbname)
{
    CDatabase *db;

    switch (dbtype)
    {
    case DB_SQLITE:
        db = new CSQLiteDatabase(dbtype, dbname);
        break;
    case DB_MYSQL:
        //not implement for this moment.
        //db = new CMySQLDatabase(dbtype, dbname);
        break;
    default:
        db = NULL;
    }

    //FIXME: when NULL, shoud throw exception?
    return db;
}

CDataSet *CDataSet::GetInstance()
{
    static CDataSet instance;
    return &instance;
}

} //end of common
