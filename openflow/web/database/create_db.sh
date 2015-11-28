#!/bin/bash

#创建数据库,表
#database: openflowDB
#tables:
#job: 存储作业xml

#SQLITE="../../../thirdparty/sqlite/bin/sqlite3"
SQLITE="sqlite3"
DBNAME="openflow.db"  #数据库名称
TABLENAME_JOB="tbJobs"     #数据库中表的名称
TABLENAME_JOB_CATEGORY="tbJobCategory"
TABLENAME_JOB_INSTANCE="tbJobInstance"
TABLENAME_TASKSTATE="tbTaskState"
TABLENAME_AGENTSTATE="tbAgentState"


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
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       Category INTEGER NOT NULL,
                       Name TEXT NOT NULL,
                       Creater TEXT NOT NULL,
                       EditorXml TEXT,
                       TemplateXml TEXT,
                       Description TEXT,
                       Uptime TEXT);";
echo "${create_table_job_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job table [fail].";

# job_category
create_table_job_category_sql="create table IF NOT EXISTS ${TABLENAME_JOB_CATEGORY}
                            (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                             Name TEXT NOT NULL,
                             Creater TEXT NOT NULL,
                             Desc TEXT,
                             Uptime TEXT);";
echo "${create_table_job_category_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_category table [fail].";

#创建agent_state表
#FIXME ZhangYiFei
create_table_agent_state_sql="create table IF NOT EXISTS ${TABLENAME_AGENTSTATE}
                      (ipaddr TEXT,
                       remain_mem TEXT NOT NULL,
                       mem_use_percent TEXT,
                       cpu_idle_percent TEXT,
		               cpu_load TEXT,
		               swap_use_percent TEXT);";

echo "${create_table_agent_state_sql}" | ${SQLITE} ${DBNAME};
if_ok "create agent_state table [fail].";

#创建TASK_state表
#FIXME ZhangYiFei
create_table_task_state_sql="create table IF NOT EXISTS ${TABLENAME_TASKSTATE}
                            (job_id INTEGER,
                             task_id INTEGER  NOT NULL,
                             task_name TEXT NOT NULL,
                             cmd TEXT	NOT NULL,
                             desc TEXT,
                             task_status INTEGER,
                             task_result TEXT,
                             ipaddr TEXT
                            );";

echo "${create_table_task_state_sql}" | ${SQLITE} ${DBNAME};
if_ok "create task_state table [fail].";


echo "Database and tables created sucessfully. [Done]."
