#!/bin/bash

#创建数据库,表
#database: openflowDB
#tables:
#job: 存储作业xml

SQLITE="../../../thirdparty/sqlite/bin/sqlite3"
DBNAME="openflow.db";  #数据库名称
TABLENAME_JOB="tbJobs";     #数据库中表的名称

#测试命令是否执行成功
function if_ok()
{
  if test $? -ne 0; then
   exit 1;
   echo $1;
  fi
}

#创建数据库
echo ".exit" | ${SQLITE} ${DBNAME};
if_ok "create database [fail].";

#创建数据表
create_table_job_sql="create table IF NOT EXISTS ${TABLENAME_JOB}
                      (job_id INTEGER PRIMARY KEY AUTOINCREMENT,
                       job_name TEXT NOT NULL,
                       xml_desc TEXT,
                       time TEXT);";
echo "${create_table_job_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job table [fail].";

echo "Database and tables created sucessfully. [Done]."
