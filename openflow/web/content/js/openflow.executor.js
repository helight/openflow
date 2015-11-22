
/**
* 任务调度
*/
var executor = {
    taskState: {
        running:1,
        success:2,
        error:3,
        suspended:4,
        unknown:5,
        not_found:6,
        getName:function(n) {
            switch(n) {
                case this.running:{
                    return '运行中';
                }
                case this.success:{
                    return '运行成功';
                }
                case this.error:{
                    return '运行出错';
                }
                case this.suspended:{
                    return '暂停中';
                }
                case this.unknown:{
                    return '未知状态';
                }
                case this.not_found:{
                    return '任务不存在';
                }
            }
        }
    },
	url:openflow.root + 'servers/executorServer.js',
	startTask:function(id,command,host,properties,callback) {
        command = encodeURIComponent(command);
		//调用服务调试此任务
       openflow.request('post', {
            url:this.url + '?cmd=starttask',
            id:id,
            command:command,
            host:host,
            properties:properties
        }, callback);
	},
	/**
	* 获取任务执行状态信息
	*/
	queryTask:function(id,host,callback) {
		//调用服务调试此任务
       openflow.request('post', {
            url:this.url + '?cmd=querytask',
            id:id,
            host:host
        }, callback);
	},
    /**
    * 暂停任务
    */
    suspendTask:function(id,host,callback) {
        openflow.request('post', {
            url:this.url + '?cmd=suspendtask',
            id:id,
            host:host
        }, callback);
    },
    /**
    * 继续任务
    */
    resumeTask:function(id,host,callback) {
        openflow.request('post', {
            url:this.url + '?cmd=resumetask',
            id:id,
            host:host
        }, callback);
    },
    /**
    * 停止任务
    */
    stopTask:function(id,host,callback) {
        openflow.request('post', {
            url:this.url + '?cmd=stoptask',
            id:id,
            host:host
        }, callback);
    },
    viewLog:function(uuid,host,size,callback) {
        size = Number(size) * 1024;
        openflow.request('get', {
            url: openflow.root + 'servers/executorServer.js?cmd=viewlog&host=' + host + '&uuid=' + uuid + '&size=' + size
        }, function (data,err) {
            callback(data || err.toString());
        });
    },
    seekViewLog:function(uuid,host,page,pagesize,callback) {
        size = Number(size) * 1024;
        openflow.request('get', {
            url: openflow.root + 'servers/executorServer.js?cmd=seekviewlog&host=' + host + '&uuid=' + uuid + '&page='+page+'&pagesize=' + pagesize
        }, function (data,err) {
            callback(data || err.toString());
        });
    }
};
