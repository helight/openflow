///req
//require("openflow.js");
if (!openflow) openflow = {};

/**
* 实例状态映射关系
* 增加状态的话请在getStateName中增加名称获取
*/
openflow.instanceStatus = {
    /// <summary>
    /// 初始化
    /// </summary>
    Init:0,
    /// <summary>
    /// 执行中
    /// </summary>
    Running: 1,
    Stopping:2,
    /// <summary>
    /// 已停止
    /// </summary>
    Stopped:3,
    /// <summary>
    /// 暂停中
    /// </summary>
    Suspending:4,

    /// <summary>
    /// 成功执行
    /// </summary>
    Success:6,
    /// <summary>
    /// 异常
    /// </summary>
    Faild:5,
    /**
    * 获取状态名称
    */
    getStateName: function(state) {
        switch(state) {
            case this.Init: {
                return '初始化';
            }
            case this.Running: {
                return '执行中';
            }
            case this.Stopping: {
                return '停止中';
            }
            case this.Stopped: {
                return '已停止';
            }
            case this.Suspending: {
                return '已暂停';
            }
            case this.Success: {
                return '成功';
            }
            case this.Faild: {
                return '异常';
            }
        }
        return '未知状态';
    }
};

openflow.instance = function (template) {
    if (template) {
        this.init(template);
    }
    this.server='/servers/instanceServer.js';
}

/**
* 初始化模板信息
*/
openflow.instance.prototype.init = function (template) {
    //根据模板生成当前实例信息
    this.data = {
        Name: template.Name,
        Description: template.Description,
        InstanceXml: '',
        properties: template.properties,
        TemplateId: template.Id,
        Template: template
    };
}

/**
* 生成当前实例的xml描述
*/
openflow.instance.prototype.toXml = function () {
    this.data.InstanceXml = '<?xml version="1.0" encoding="utf-8" ?>\n';
    this.data.InstanceXml += '<instance template="' + this.data.TemplateId + '"><parameters>\n';

    if(!this.data.params || !this.data.params['ip']) {
        this.data.InstanceXml += '\t\t<parameter name="ip">\n';
        this.data.InstanceXml += '\t\t\t<![CDATA[127.0.0.1]]>';
        this.data.InstanceXml += '\t\t</parameter>\n';
    }

    if (this.data.params) {
        for (var k in this.data.params) {
            this.data.InstanceXml += '\t\t<parameter name="' + k + '">\n';
            this.data.InstanceXml += '\t\t\t<![CDATA[' + this.data.params[k] + ']]>\n';
            this.data.InstanceXml += '\t\t</parameter>\n';
        }
    }
    this.data.InstanceXml += '</parameters>\n</instance>';
    return this.data.InstanceXml;
}

/**
* 弹出确认窗
* 并让用户填写参数
*/
openflow.instance.prototype.confirm = function (name,callback) {
    /**
    * 生成参数输入框
    */
    function createPropertyPanel(ps) {
        var panel = $('<div></div>');
        //创建输入框
        function createInput(p,n,k,v,h) {
            var tr = $('<div />');
            var label = $('<span class="label"></span>');
            label.html(n);
            label.appendTo(tr);
            //var td2 = $('<td></td>')
            h.appendTo(tr);
            h.val(v);
            //td2.appendTo(tr);
            tr.appendTo(p);
            return h;
        }

        var ps1panel = $('<div class="instance-editor-container"></div>').appendTo(panel);
        var nameinput = createInput(ps1panel,'名称','name', this.data.Name,$('<input type="text" name="name" />'));
        var desinput = createInput(ps1panel,'说明', 'description',this.data.Description,$('<textarea type="text" name="description" />'));
        nameinput.bind('blur',function() {
            if(!desinput.val()) {
                desinput.val(nameinput.val());
            }
        });

        var messagerecipientinput = createInput(ps1panel,'告警或报表接收人 (RTX名，分号隔开，留空表示删除设置)',
            'recipient',this.data.MessageRecipient,$('<input type="text" name="recipient" />'));
        var messagetypecontainer = $('<div></div>');
        var rtx = $('<input type="checkbox" name="rtx" data-value="1" />').appendTo(messagetypecontainer);
        $('<span>RTX</span>').appendTo(messagetypecontainer);
        var mail = $('<input type="checkbox" name="mail" data-value="2" />').appendTo(messagetypecontainer);
        $('<span>邮件</span>').appendTo(messagetypecontainer);
        var tel = $('<input type="checkbox" name="tel" data-value="4" />').appendTo(messagetypecontainer);
        $('<span>手机短信</span>').appendTo(messagetypecontainer);
        $('<div class="clear"></div>').appendTo(messagetypecontainer);

        if(this.data.MessageType) {
            if(this.data.MessageType % 2 == 1) {
                rtx.attr('checked',true);
            }
            var tmp = Math.floor(this.data.MessageType / 2);
            if(tmp % 2 == 1) {
                mail.attr('checked',true);
            }
            tmp = Math.floor(tmp / 2);
            if(tmp % 2 == 1) {
                tel.attr('checked',true);
            }
        }
        var messagetype = createInput(ps1panel,'告警发送方式', 'reciver',this.data.MessageType,messagetypecontainer);
        if(ps) {
            var ps2panel = $('<div class="instance-editor-params"></div>').appendTo(panel);
            $('<h3>实例参数</h3>').appendTo(ps2panel);
            for (var i in ps) {
                var p = ps[i];
                createInput(ps2panel,i,i, p,$('<input type="text" name="'+i+'" />'));
            }
        }
        return panel;
    }

    var winbody = $('<div class="property" style="width:90%;"/>')

    var ps;
    //实例或模板属性
    var properties = this.data.properties || this.editor.property.properties || this.editor.data.properties;
    if (properties) {
        //从属性中获取变量，生成输入框
        for (var k in properties) {
            var p = properties[k];
            if (p.value) {
                var m;// = /\{[^\}]*\}/g.exec(p.value);
                var v = p.value;
                while (m = /%\{[^\}]*\}/g.exec(v)) {
                    if(!ps) ps={};
                    var mstr = m[0].toString();
                    mstr=mstr.substring(2,mstr.length - 1);
                    ps[mstr] = this.data.params?(this.data.params[mstr] || ''):'';
                    v = v.substring(m.index + m[0].length);
                }
            }
        }
    }
    ps = createPropertyPanel.call(this,ps);
    ps.appendTo(winbody);

    var _this = this;
    winbody.dialog({
        title: name + '实例', //标题
        modal:true,
        width: 600,
        top:100,
        buttons: [
            {   text: name + '实例',
                handler:function () {
                    var inputs = winbody.find('input,textarea');
                    _this.data.params = {};
                    var msgtype = 0;
                    inputs.each(function (i, n) {
                        var name = $(n).attr('name');
                        if($(n).attr('type') == 'checkbox') {
                            var ck = $(n).is(':checked');
                            if(ck) {
                                msgtype += Number($(n).attr('data-value'));
                            }
                        }
                        else if (name == 'name') {
                            _this.data.Name = $.trim($(n).val());
                        }
                        else if (name == 'description') {
                            _this.data.Description = $(n).val();
                        }
                        else if(name == 'recipient') {
                            _this.data.MessageRecipient = $(n).val();
                        }
                        else {
                            _this.data.params[name] = $(n).val();
                        }
                    });

                    if(!_this.data.Name) {
                        openflow.message.warning('实例名称不可为空！');
                        return;
                    }
                    _this.data.MessageType = msgtype;
                    if (callback) {
                        callback.call(_this);//回调
                        winbody.dialog('close');
                    }
                }
            },
            {
                text: '取消',
                handler: function () {
                    winbody.dialog('close');
                }
            }
        ]
    });

}

/**
* 注册当前实例
*/
openflow.instance.prototype.register = function (callback) {
    var _this = this;
    //确认
    this.confirm('创建',function () {
        var wait = openflow.message.showWaiting('注册中...');
        openflow.request('post', {
            url: _this.server + '?cmd=insert',
            name: this.data.Name,
            des: this.data.Description,
            msgrecipient:this.data.MessageRecipient,
            msgtype:this.data.MessageType,
            xml: this.toXml(),
            ip:openflow.params['ip'] ||'',
            tid: this.data.TemplateId
        }, function (data) {
            wait.close();
            if (data) {
                openflow.message.success('注册实例成功')
                window.location = openflow.checkUrl('/editor?mode=3&id=' + _this.data.TemplateId + '&instance=' + data);
            }

            if (callback) callback(data);
        });
    });
}

/**
* 修改当前实例
*/
openflow.instance.prototype.update = function (callback) {

    var wait = openflow.message.showWaiting('保存中...');
    openflow.request('post', {
        url: this.server + '?cmd=update',
        id: this.data.Id,
        name: this.data.Name,
        des: this.data.Description,
        msgrecipient:this.data.MessageRecipient,
        msgtype:this.data.MessageType,
        xml: this.toXml(),
        ip:openflow.params['ip'] ||''
    }, function (data) {
        wait.close();
        if (data) {
            openflow.message.success('保存实例成功')
            //window.location = openflow.checkUrl('/editor?mode=3&id=' + _this.data.TemplateId + '&instance=' + data);
        }

        if (callback) callback(data);
    });
}

/**
* 设置节点状态样式
*/
openflow.instance.prototype.setCellStateStyle= function(cell,state,extrainfo) {
    if(cell && cell.type != 'jmCell') {
        cell = this.editor.editor.getCell(cell);
    }
    if(!cell) return;
    //去除节点状态 设为正常。不带_的状态
    var style= cell.styleName;
    var index=style.indexOf('-');
    if(index > 0) {
        style=style.substring(0,index);
        this.editor.setCellStyle(cell, style);
    }

    var statename= '[' + openflow.instanceStatus.getStateName(state) + '] ' + (extrainfo || '');
    switch (state) {
        case -1: {//无状态
            this.editor.setOverlayById(cell.id, 'none', statename);
            break;
        }
        case openflow.instanceStatus.Init: {
            this.editor.setOverlayById(cell.id, 'init', statename);
            break;
        }
            //执行中
        case openflow.instanceStatus.Running: {
            this.editor.setCellStyle(cell, style + '-running');
            this.editor.setOverlayById(cell.id, 'running', statename);
            break;
        }
            //暂停中
        case openflow.instanceStatus.Suspending: {
            this.editor.setOverlayById(cell.id, 'suspending', statename);
            break;
        }
        //失败
        case openflow.instanceStatus.Stopping: {
            this.editor.setOverlayById(cell.id, 'stopping', statename);
            break;
        }
        //失败
        case openflow.instanceStatus.Stopped: {
            this.editor.setOverlayById(cell.id, 'stopped', statename);
            break;
        }
            //失败
        case openflow.instanceStatus.Faild: {
            this.editor.setOverlayById(cell.id, 'error', statename);
            break;
        }
            //成功
        case openflow.instanceStatus.Success: {
            this.editor.setCellStyle(cell, style + '-success');
            this.editor.setOverlayById(cell.id, 'success', '成功');
            break;
        }
    }
}

/**
* 更新状态
*/
openflow.instance.prototype.setCellState= function(data) {
    this.initOperationButton(data.State);//初始化状态控件
    if (data.Flowables) {
        //循环设置节点状态
        for (var i in data.Flowables) {
            var node = data.Flowables[i];
            this.setCellStateStyle(node.Id,node.State,node.ExtraInfo);
        }
    }

    //如果已经成功执行，则把所有正在运行的节点设为成功
    if(data.State == openflow.instanceStatus.Success
        || data.State == openflow.instanceStatus.Stopped
        || data.State == openflow.instanceStatus.Stopping
        || data.State == openflow.instanceStatus.Suspended
        || data.State == openflow.instanceStatus.Faild) {
        var self = this;
        this.editor.editor.cells.each(function(i,cell) {
            if(cell.styleName && cell.styleName.indexOf('-running') > 0) {
                self.setCellStateStyle(cell,data.State);
            }
        });
    }
    //表示没有状态
    else if(data.State === -1) {
        var self = this;
        this.editor.editor.cells.each(function(i,cell) {
                self.setCellStateStyle(cell,'-1');
        });
    }
}

/**
* 获取节点的状态信息
*/
openflow.instance.prototype.getCellStateInfo = function(id) {
    if(this.stateInfo) {
        for (var i in this.stateInfo.Flowables) {
            var node = this.stateInfo.Flowables[i];
            if(node.Id == id) return node;
        }
    }
}

/**
* 加载节点状态信息
*/
openflow.instance.prototype.loadStatus = function (editor, id,index,_this,cmd) {
    var thistemp = openflow.currentInstance || (openflow.currentInstance = _this) || (openflow.currentInstance = this);
    thistemp.loadindex = index === undefined ? (thistemp.loadindex || 0) : index;
    openflow.request('post', {
        id: id || thistemp.data.Id,
        uuid: thistemp.data.Uuid || '',
        url: thistemp.server + '?cmd=' + (cmd||'getflowstate')
    }, function (data, msg,result) {
        if (data) {
            data = decodeURIComponent(data);
            openflow.log(data);
            //网络错误则重新获取
            if (data != 3) {
                data = eval('(' + data + ')');
                if(!data) return;
                if(result && result.containerIp) data.containerIp = result.containerIp;
                else if(thistemp.stateInfo && thistemp.stateInfo.containerIp) {
                    data.containerIp = thistemp.stateInfo.containerIp;
                }
                thistemp.stateInfo = data;
                thistemp.editor.property.show(thistemp.editor);//生成属性框
                thistemp.loadindex = 0;
                thistemp.setCellState(data);//更新界面元素状态

                //记录二种状态
                if(cmd == 'getflowstatefromdb') {
                    thistemp.dbState = data.State;
                    //如果rpc状态和db中的状态不一至则出错递加
                    if(thistemp.rpcState && thistemp.rpcState != thistemp.dbState) {
                        this.loadStatusErrCount ++;
                    }
                    if(this.loadStatusErrCount > 3) {
                        openflow.message.error('流程状态记录与实时状态不一致，请连系管理人员解决!');
                        return;
                    }
                }
                else {
                    thistemp.rpcState = data.State;
                }

                //如果正在执行
                if (data.State == openflow.instanceStatus.Running) {
                    this.stateTimeout = setTimeout(thistemp.loadStatus, 2000, thistemp.editor, thistemp.data.Id, 0, thistemp);
                }
                    //如果为未启动或成功，则从库中查询一次获取最近一次状态
                else if (cmd != 'getflowstatefromdb') {
                    thistemp.loadStatus(thistemp.editor, thistemp.data.Id, 0, thistemp, 'getflowstatefromdb');
                }

                return;
            }
            else {
                thistemp.loadindex++;
                if (thistemp.loadindex > 3) {
                    thistemp.editor.setAllCellOverlay('editors/images/overlays/forbidden.png', msg);
                    return;//如果异常加载三次以上则不再请求
                }
            }

            this.stateTimeout = setTimeout(thistemp.loadStatus, 2000, thistemp.editor, thistemp.data.Id, thistemp.loadindex, thistemp);
        }
    });
}

/**
 * 解析XML获取实例参数
 *
 */
openflow.instance.prototype.decodeParams= function(xml) {
    var _this = this;
    /**
    * 获取实例的参数
    */
    function getInstanceProperty(node) {
        if (node.children) {
            for (var i in node.children) {
                var n = node.children[i];
                if (n.name.toLowerCase() == 'parameter') {
                    var name = n.attributes['name'];
                    var value = $.trim(n.value);
                    _this.data.params[name] = value;
                }
            }
        }
    }
    if(!_this.data.params) _this.data.params= {};
    var json = jmUtils.xmlToJSON(xml);
    //循环读取flow下的节点
    for(var i in json.children) {
        var n = json.children[i];
        if (n.name.toLowerCase() == 'instance') {
            for(var j in n.children) {
                getInstanceProperty(n.children[j]);
            }
        }
    }
}

/**
* 获取实例
*/
openflow.instance.prototype.loadInstance = function (editor, id, uuid,callback) {
    var _this = this;
    this.editor = editor;
    var tip = openflow.message.showWaiting('loading...');
    openflow.request('post', {
        id: id,
        url: _this.server + '?cmd=getwithtemplate'
    }, function (data) {
        if (tip) tip.close();
        if (data) {
            _this.data = data;
            data.Uuid = uuid;
            _this.loadindex = 0;
            var enabled = openflow.admin || openflow.user == data.CreateUser || (data.Manager && data.Manager.indexOf(';' + openflow.user + ';') >= 0);
            if(!enabled) {
                //没有只读权限
                if(!data.Reader || data.Reader.indexOf(';' + openflow.user + ';') < 0) {
                    openflow.message.warning('您没有查看权限',true);
                    return;
                }
            }
            var editor = _this.editor = initeditor(false,function() {
                if(!data.Template) {
                    openflow.message.error('获取模板信息失败');
                    return;
                }
                editor.data = data.Template;
                if (editor.data.EditorXml) editor.read(editor.data.EditorXml);
                editor.name(data.Name);
                if (!editor.data.properties) editor.data.properties = {};
                editor.property.properties = {};

                if (editor.data.TemplateXml) editor.property.decode(editor.data.TemplateXml);
                if (_this.data.InstanceXml) _this.decodeParams(_this.data.InstanceXml);

                //显示属性框
                editor.property.show(editor);
                _this.initOperationButton(_this.data.State);//初始化状态控件

                // 对demo和usage特殊处理，分别对应文件demo.xml和usgae.xml
                if (id == 'demo' || id=="usage") {
                    var cells = editor.editor.cells;

                    var demostate={Flowables:[
                        ]};
                    cells.each(function(i,cell) {
                        if (cell.styleName &&
                            cell.styleName.indexOf('start') == -1 && cell.styleName.indexOf('end') == -1) {
                                demostate.Flowables.push({
                                    Id:cell.id,
                                    State:i % 6
                                });

                        }
                    });
                    _this.setCellState(demostate);
                }
                else {
                    //如果流程不是初始化状态则获取状态信息
                    if (_this.data.State != openflow.instanceStatus.Init) {
                        editor.initAllCellOverlay();//初始化所有节点状态
                        //开始加载执行状态
                        _this.loadStatus(editor, id,0);
                    }
                }
                if (callback) callback(editor, _this);
            });

        }
        else {
            openflow.message.tip('获取当前流程信息失败！', true);
        }
    });
}

/**
* 根据状态初始化操作按钮
*/
openflow.instance.prototype.initOperationButton = function (state) {
    var _this = this;
    if(!this.operationToolbar) {
        this.operationToolbar= $('<ul class="operation_Toolbar"></ul>').appendTo('#editor-toolbar');
    }
    switch (state) {
        //正在执行中
        case openflow.instanceStatus.Running: {
            if (this.startButton) {
                this.editor.removeTool(this.startButton);
                this.startButton = null;
            }
            if (this.resumeButton) {
                this.editor.removeTool(this.resumeButton);
                this.resumeButton = null;
            }
            if (this.reExecButton) {
                this.editor.removeTool(this.reExecButton);
                this.reExecButton = null;
            }
            if (!this.suspendButton) {
                this.suspendButton = this.editor.addTool('暂停流程', 'content/images/suspend.png', function (evt) {
                    _this.operation('suspendflow');
                },this.operationToolbar);
            }
            if (!this.stopButton) {
                this.stopButton = this.editor.addTool('停止流程', 'content/images/stop.png', function (evt) {
                    _this.operation('stopflow');
                },this.operationToolbar);
            }
            break;
        }
        //正在暂停中
        case openflow.instanceStatus.Suspending: {
            if (this.startButton) {
                this.editor.removeTool(this.startButton);
                this.startButton = null;
            }
            if (this.reExecButton) {
                this.editor.removeTool(this.reExecButton);
                this.reExecButton = null;
            }
            if (this.suspendButton) {
                this.editor.removeTool(this.suspendButton);
                this.suspendButton = null;
            }
            if (this.stopButton) {
                this.editor.removeTool(this.stopButton);
                this.stopButton = null;
            }
            if (!this.resumeButton) {
                this.resumeButton = this.editor.addTool('继续流程', 'content/images/resume.png', function (evt) {
                    _this.operation('resumeflow');
                },this.operationToolbar);
            }
            break;
        }
        //执行失败
        case openflow.instanceStatus.Stopped:
        case openflow.instanceStatus.Faild: {
            if (this.startButton) {
                this.editor.removeTool(this.startButton);
                this.startButton = null;
            }
            if (this.suspendButton) {
                this.editor.removeTool(this.suspendButton);
                this.suspendButton = null;
            }
            if (this.resumeButton) {
                this.editor.removeTool(this.resumeButton);
                this.resumeButton = null;
            }
            if (this.reExecButton) {
                this.editor.removeTool(this.reExecButton);
                this.reExecButton = null;
            }
             if (this.stopButton) {
                this.editor.removeTool(this.stopButton);
                this.stopButton = null;
            }
            this.startButton = this.editor.addTool('重新启动流程', 'content/images/start.png', function (evt) {
                _this.operation('startflow',false);
            },this.operationToolbar);
            this.reExecButton = this.editor.addTool('重试流程', 'content/images/start.png', function (evt) {
                _this.operation('startflow',true);
            },this.operationToolbar);
            break;
        }
            //已成功执行

        case openflow.instanceStatus.Success: {
            if (this.startButton) {
                this.editor.removeTool(this.startButton);
                this.startButton = null;
            }
            if (this.suspendButton) {
                this.editor.removeTool(this.suspendButton);
                this.suspendButton = null;
            }
            if (this.resumeButton) {
                this.editor.removeTool(this.resumeButton);
                this.resumeButton = null;
            }
            if (this.reExecButton) {
                this.editor.removeTool(this.reExecButton);
                this.reExecButton = null;
            }
             if (this.stopButton) {
                this.editor.removeTool(this.stopButton);
                this.stopButton = null;
            }
            this.startButton = this.editor.addTool('重新启动流程', 'content/images/start.png', function (evt) {
                _this.operation('startflow');
            },this.operationToolbar);
            break;
        }
        //未执行
        case openflow.instanceStatus.Init:
        default: {
            if (this.suspendButton) {
                this.editor.removeTool(this.suspendButton);
                this.suspendButton = null;
            }
            if (this.resumeButton) {
                this.editor.removeTool(this.resumeButton);
                this.resumeButton = null;
            }
            if (this.reExecButton) {
                this.editor.removeTool(this.reExecButton);
                this.reExecButton = null;
            }
             if (this.stopButton) {
                this.editor.removeTool(this.stopButton);
                this.stopButton = null;
            }
            if (!this.startButton) {
                this.startButton = this.editor.addTool('启动流程', 'content/images/play.png', function (evt) {
                    _this.operation('startflow');
                    /*openflow.currentInstance.confirm('启动',function() {
                        _this.operation('startflow');//保存
                    });
                    */
                },this.operationToolbar);
                //openflow.message.warning('流程未执行');
            }
            break;
        }
    }
}

/**
 * 对流程进行操作
 * startflow,suspendflow,resumeflow
 */
openflow.instance.prototype.operation = function (method,re) {
    var wait = openflow.message.showWaiting(method + '...');
    this.loadStatusErrCount = 0;//加载状态错误次数重置
    this.opState = method;//当前操作状态
    var _this = this;
    openflow.request('post', {
        id: this.data.Id,
        reexec:re,
        url: this.server + '?cmd=' + method
    }, function (data, msg) {
        wait.close();
        if (data) {
            _this.loadStatus(_this.editor, _this.data.Id,0);//刷新状态
            openflow.message.success('操作成功');
        }
    });
}

/**
 * 用户权限
 */
openflow.instance.prototype.permission = function() {
    var ps = {};
    ps.manager = openflow.admin || openflow.user == this.data.CreateUser ||
                (this.data.Manager && this.data.Manager.indexOf(';' + openflow.user + ';') >= 0);
    return ps;
}

/**
 * 显示运行轨迹
 *
 */
openflow.instance.prototype.showRunningDetail = function(id,uuid) {
    id = id || this.data.Id;
    var uuid = uuid || this.data.Uuid;
    var win = $('<div style="padding:8px;overflow:auto;"><div class="jm_wait">加载中...</div></div>');
    win.window({
        modal:true,
        top:80,
        width:600,
        height:300,
        minimizable:false,
        title:'运行轨迹'
    });
    openflow.request('post', {
        id:id,
        uuid:uuid,
        url:this.server + '?cmd=getRunningDetail'
    },function(data,msg) {
        if(data && data.length > 0) {
            var html = '';
            for(var i=0;i<data.length;i++) {
                var d=data[i];
                //nodetype 0表示process,1表示task
                html += '【'+openflow.formatDate(d.TimePoint)+'】: 【'+(d.NodeType?'task':'process')+'】:' + d.NodeName +
                ' 【运行时长】:' + d.NodeDuration + '秒 ' + (d.NodeType?'':('【运行轮次】:' + d.NodeRounds)) + '<br />';
            }
            win.html(html);
        }
        else {
            win.html(msg || '无');
        }
    });
}

/**
 * 实例选择器
 */
openflow.instanceSelecter = function(params,callback) {
    if(typeof params === 'function') {
        callback = params;
        params = null;
    }
    this.callback = callback;
    var container = $('<div></div>');
    var tb = $('<div></div>').appendTo(container);
    var pager = $('<div id="instance_pager2"></div>').appendTo(container);
    this.params = params;

    var _this = this;
    window.selecter_linkInstance = function(id,name,template) {
        if(_this.callback) _this.callback(id,name,template);
        container.window('close');
        return false;
    };

    /**
     * 查询实例
     */
    function search(p) {
        p = Number(p) || 1;
        openflow.request('post', {
            page: p,
            flag: 0,
            count: 10,
            templateId:_this.params?_this.params.template:'',
            user:openflow.user,
            url: openflow.root + 'servers/instanceServer.js?cmd=search'
        }, function (data,err) {
            if (data) {
                createTable(data.data);
                pager.paging({ index: data.page, count: data.pageCount });
            }
            else if(err) {
                container.html(err.toString());
            }
            //重置父页高度
            resetParentHeight();
        });
    }

    var instanceTB = new $jm.table({
            parent: tb,
            id: 'tbinstances',
            className: 'jm-table fullarea',
            sortChange:function(sort) {
                search(0);
            },
            header: [{value:"名称",name:'a.Name'}, {value:"模板",'name':'a.TemplateId'},{value:"IP地址",name:'b.Ip'}]
        });
    pager.paging({
        showCount: 6, //当前页附近显示多少页码，比如，4或5表示前后显二个页码，6表示前后显示三个,都会除以2
        change: function (p) {
            search(p);
        }
    });
    //生成模板表格
    function createTable(data) {
        instanceTB.rows=[];
        if (data) {
            for (var i = 0; i < data.length; i++) {

                var namecell = '<a href="#" onclick="javascript:return selecter_linkInstance(' + data[i].Id +',\''+data[i].Name+'\','+data[i].TemplateId+');">' +
                (data[i].Name || '') + '</a>';
                instanceTB.insertRow([
                {
                    value: namecell,
                    style: 'width:200px;'
                }, {
                    value: data[i].TemplateName || '',
                    style: 'width:120px;'
                }, {
                    value: data[i].Ip || '',
                    style: 'width:120px;'
                }]);
            }
        }
        instanceTB.render();
    }
    this.show = function(left,top,callback) {
        if(typeof left === 'function') {
            callback = left;
            left = 100;
        }
        if(callback) this.callback = callback;

        left = left || 100;
        top = top || 100;
        search(0);
        //instancewin.show();
        container.window({title:'选择实例',left:left,top:top,width:600,height:350,modal:true});
    }
}
