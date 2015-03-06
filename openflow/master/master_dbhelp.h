//Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>
// Created: 2014-10-13
// Description: db操作的二次封装
//
#ifndef OPENFLOW_MASTER_MASTER_OPDB_H
#define OPENFLOW_MASTER_MASTER_OPDB_H
#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <glog/logging.h>
#include <dbbase_help.h>

namespace openflow { namespace master {

class CMasterDB {

public:
	CMasterDB(const common::DbType dbtype,const std::string &dbname);
	~CMasterDB();
	bool connect(const std::string &constr);
	bool optable(const std::string &tabname);
	int execute(const std::string &sql, std::vector<std::string> &result, int32_t& columns);
	bool execute(const std::string &sql);
	void close();

private:
	const std::string _constr;
};
}} //end namespace openflow::master
#endif // OPENFLOW_MASTER_MASTER_OPDB_H
