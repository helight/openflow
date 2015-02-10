//Copyright (c) 2014, OpenFlow
// Author: ZhangYiFei<zyfforlinux@163.com>, helight<helight@zhwen.org>
// Created: 2014-10-13
// Description: db操作的二次封装
//
#include <boost/algorithm/string.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <glog/logging.h>
#include "master_dbhelp.h"

namespace openflow { namespace master {

//数据库的初始化
CMasterDB::CMasterDB(const common::DbType dbtype,const std::string &dbname)
{
}

//数据库的资源释放
CMasterDB::~CMasterDB()
{
	//数据库的关闭
}

//数据库的连接
bool CMasterDB::connect(const std::string &connstr)
{
    return true;
}

//操作数据库表
bool CMasterDB::optable(const std::string &tabname)
{
	return true;
}

int CMasterDB::execute(const std::string &sql, std::vector<std::string> &result, int32_t& columns)
{
	//数据库查询,返回查询到的结果数
	//输入sql语句,以及一个string的vector来保存查询结果
	//columns 返回查询到的结果数
	LOG(INFO)<<"test";

    return 0;
}

bool CMasterDB::execute(const std::string &sql)
{
    return true;
}

void CMasterDB::close()
{
}

}} //end namespace
