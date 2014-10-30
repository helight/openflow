//Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: db操作的二次封装
//
#ifndef MASTER_OPDB_H
#define MASTER_OPDB_H
#pragma once

#include "../common/table.h"
#include "../common/database.h"
#include "../common/dataset.h"
#include <boost/algorithm/string.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <glog/logging.h>
#include "../common/db_config.h"

namespace openflow { namespace master {

class CMasterDB {

public:
	CMasterDB(const common::DB_TYPE dbtype,const std::string &dbname);
	~CMasterDB();
	bool  connect(const std::string &constr);
	bool optable(const std::string &tabname);
	int execute(const std::string &sql, std::vector<std::string> &result, int32_t& columns);
	bool execute(const std::string &sql);

private:
	const std::string _constr;
	common::CTable *table;
	common::CDatabase *db;
	common::CDataSet &ds = boost::serialization::singleton<common::CDataSet>::get_mutable_instance();
};
}} //end namespace openflow::master
#endif // MASTER_OPDB_H
