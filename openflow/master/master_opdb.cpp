#include "../common/table.h"
#include "../common/database.h"
#include "../common/dataset.h"
#include <boost/algorithm/string.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <glog/logging.h>
#include "master_opdb.h"

namespace openflow { namespace master {

//数据库的初始化
CMasterDB::CMasterDB(const common::DB_TYPE dbtype,const std::string &dbname)
{
    db = ds.new_database(dbtype,dbname);
}

//数据库的资源释放
CMasterDB::~CMasterDB()
{
	//数据库的关闭
	db->close();
	delete table;
	delete db;
}

//数据库的连接
bool CMasterDB::connect(const std::string &connstr)
{
	//1.设置连接字符串
	//2.连接数据库
    db->set_connect_str(connstr);
    return db->open();
	
}

//操作数据库表
bool CMasterDB::optable(const std::string &tabname)
{
	//1.设置表名字
    table = db->new_table(tabname);
    if (!table)
    {
        delete table;
        return false;
    }//根据返回结果在调用点记录日志

	return true;
}

int CMasterDB::execute(const std::string &sql, std::vector<std::string> &result, int32_t& columns)
{
	//数据库查询,返回查询到的结果数
	//输入sql语句,以及一个string的vector来保存查询结果
	//columns 返回查询到的结果数
	LOG(INFO)<<"test";
}

bool CMasterDB::execute(const std::string &sql)
{
	//插入操作
	
	if ( !table->non_query(sql))
    	{
		return false;//根据返回结果在调用点需要记录日志
        }
	return true;
	
}

void CMasterDB::close()
{
	
	db->close();
	delete table;
	delete db;
	
}
}} //end namespace
