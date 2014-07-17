#ifndef DATASET_H
#define DATASET_H
#pragma once

#include <string>
#include "db_config.h"
#include "database.h"

namespace common {
/**
 * DataSet: Sigleton class, create different types of database.
 */
class CDataSet
{
public:
    virtual ~CDataSet()
    {}
    CDatabase *new_database(DB_TYPE dbtype, const std::string &dbname);

protected:
    CDataSet() {};

private:
    //prevent from creating instance in other place
    CDataSet(const CDataSet &);
    const CDataSet &operator =(const CDataSet &);
};

} //namespace common
#endif // DATASET_H
