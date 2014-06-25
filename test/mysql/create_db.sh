#!/bin/bash

#创建数据库,表
#数据库名称: openflowDB
#tables:
#job: 存储作业xml

HOSTNAME=localhost;                                
PORT="3306";
USERNAME="root";
PASSWORD="renzhen";
DBNAME="openflowDB";  #数据库名称
TABLENAME_JOB="job";     #数据库中表的名称

#测试命令是否执行成功
function if_ok()
{
  if test $? -ne 0; then
   exit 1;
   echo $1; 
  fi
}
 
#创建数据库
create_db_sql="create database IF NOT EXISTS ${DBNAME}";
mysql -h${HOSTNAME}  -P${PORT}   -u${USERNAME} -p${PASSWORD} -e "${create_db_sql}";
if_ok "create database [fail].";

#创建数据表
create_table_job_sql="create table IF NOT EXISTS ${TABLENAME_JOB} 
                      (id INT, 
                      xml TEXT, 
                      primary key(id)) engine=innodb default charset=utf8 auto_increment=1;";
mysql -h${HOSTNAME} -P${PORT} -u${USERNAME} -p${PASSWORD} ${DBNAME} -e"${create_table_job_sql}";
if_ok "create job table [fail].";

echo "Database and tables created sucessfully. [Done]."
