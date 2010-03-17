#!/bin/bash
# Copyright (c) 2015, HelightXu
# Author: Zhwen Xu<HelightXu@gmail.com>
# Created: 2015-02-28
# Description:
#
main_server_bin=openflow_agent

count=`ps aux|grep $main_server_bin | grep -v grep| grep -v ".log."|wc -l`

if [ "$1" = "start" ]; then
    if [ $count -eq 0 ]; then
        nohup ./$main_server_bin --max_tasks_num=8 --master_host=127.0.0.1 --v=4&
    fi
elif [ "$1" = "stop"]; then
    if [ $count -gt 0 ]; then
        echo "$main_server_bin is running, going to stop"
        killall -9 $main_server_bin
    fi
fi
