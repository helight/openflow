#ifndef DATASET_H
#define DATASET_H
#pragma once

#include <string>
#include "common_config.h"

namespace common {
/**
 * DataSet: Sigleton class, create different types of database.
 */
class CDataSet
{
public:
    CDatabase *new_database(DB_TYPE dbtype, const std::string &dbname);
    //get sigleton instance
    static CDataSet *GetInstance();

private:
    //prevent from creating instance in other place
    CDataSet();
    CDataSet(const CDataSet &);
    const CDataSet &operator =(const CDataSet &);
};

} //namespace common
#endif // DATASET_H
