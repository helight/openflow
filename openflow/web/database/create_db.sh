#!/bin/bash

#创建数据库,表
#database: openflowDB
#tables:
#job: 存储作业xml

#SQLITE="../../../thirdparty/sqlite/bin/sqlite3"
SQLITE="sqlite3"
DBNAME="openflow.db"  #数据库名称

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
create_table_job_sql="create table IF NOT EXISTS tbTemplate
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       Name TEXT NOT NULL,
                       EditorXml TEXT,
                       TemplateXml TEXT,
                       Description TEXT,
                       Flag INTEGER NOT NULL,
                       Category INTEGER NOT NULL,
                       Creater TEXT NOT NULL,
                       Reader TEXT NOT NULL,
                       Uptime TEXT);";
echo "${create_table_job_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_job table [fail].";

create_table_instance_sql="create table IF NOT EXISTS tbInstance
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       TemplateId INTEGER NOT NULL,
                       Uuid TEXT NOT NULL,
                       Name TEXT NOT NULL,
                       InstanceXml TEXT,
                       Description TEXT,
                       State INTEGER NOT NULL,
                       Flag INTEGER NOT NULL,
                       Creater TEXT NOT NULL,
                       Reader TEXT NOT NULL,
                       Uptime TEXT);";
echo "${create_table_instance_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_instance table [fail].";

create_table_tempdraft_sql="create table IF NOT EXISTS tbTemplateDraft
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       Name TEXT NOT NULL,
                       EditorXml TEXT,
                       TemplateXml TEXT,
                       Description TEXT,
                       Creater TEXT NOT NULL,
                       Uptime TEXT);";
echo "${create_table_tempdraft_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_tempdraft table [fail].";

create_table_component_sql="create table IF NOT EXISTS tbComponent
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       Name TEXT NOT NULL,
                       EditorXml TEXT,
                       TemplateXml TEXT,
                       Description TEXT,
                       Creater TEXT NOT NULL,
                       Uptime TEXT);";
echo "${create_table_component_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_component table [fail].";

create_table_running_sql="create table IF NOT EXISTS tbRunning
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       InstanceId INTEGER NOT NULL,
                       Uuid TEXT,
                       StartTime TEXT,
                       EndTime TEXT,
                       State TEXT NOT NULL,
                       Uptime TEXT);";
echo "${create_table_job_running_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_running table [fail].";

create_table_runninghis_sql="create table IF NOT EXISTS tbRunningHistory
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       InstanceId INTEGER NOT NULL,
                       Uuid TEXT,
                       StartTime TEXT,
                       EndTime TEXT,
                       State TEXT NOT NULL,
                       Uptime TEXT);";
echo "${create_table_job_runninghis_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_runninghis table [fail].";

# job_category
create_table_job_category_sql="create table IF NOT EXISTS tbCategory
                            (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                             Parent INTEGER NOT NULL,
                             Name TEXT NOT NULL,
                             Creater TEXT NOT NULL,
                             Desc TEXT,
                             Uptime TEXT);";
echo "${create_table_job_category_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_category table [fail].";

create_table_clock_sql="create table IF NOT EXISTS tbClock
                      (Id INTEGER PRIMARY KEY AUTOINCREMENT,
                       InstanceId INTEGER NOT NULL, #实例id'
                       RuleString TEXT,             #定时规则
                       StartTime TEXT,              #定时任务开始时间',
                       EndTime TEXT,                #定时任务结束时间',
                       LastTime TEXT NOT NULL,      #定时任务上次运行时间',
                       CurrentRound INTEGER NOT NULL,  #当前运行次数',
                       MaxRounds INTEGER  NOT NULL, #'最大运行次数
                       Uptime TEXT);";

echo "${create_table_job_clock_sql}" | ${SQLITE} ${DBNAME};
if_ok "create job_clock table [fail].";

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
