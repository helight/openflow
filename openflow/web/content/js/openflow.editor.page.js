
/**
* 初始化编辑器
*/
function initeditor(enabled,callback) {
    var editor = openflow.currentEditor = new openflow.editor({
        container: document.getElementById('editor'),
        connectable: openflow.editable,
        enabled: enabled,
        movable:enabled,
        resizable:true
    });

    //元素单击事件
    editor.setSelectionChangeEvent(function(cell) {
        if (cell && cell.selected) {
            editor.property.show(editor, cell);
        }
        else {
            editor.property.show(editor, editor);
        }
    });

    /*editor.container.width = $('#editor-parent').width();
    editor.container.height = $('#editor-parent').height();*/
    //初始化工具栏
    editor.initToolbar({
        editor: editor,
        showOutLine: true,
        showTools: true,
        showComponent: openflow.editable
    },callback);

    return editor;
}

/**
* 创建新模板
*/
function createEmptyEditor(callback) {
    var editor = initeditor(true,function() {
            //初始化
        editor.data = {};
        editor.data.properties = {};
        editor.edit();//必须输入名称
        window.document.title = '创建模板';
        setTimeout(function () {
            //10秒后开始保存草稿
            setTimeout(saveDraft,10000);
            //重置高度
            //resetParentHeight();
        }, 200);
        if (callback) callback(editor);
    });
    return editor;
}


//加载xml
function loadopenflow(editor, id,callback) {
    var tip = openflow.message.showWaiting('loading...');
    var isdraft = true;

    if(openflow.flag == 4) {
        openflow.request('get', {
            url:openflow.root + 'servers/componentServer.js?cmd=get&id='+ (id || 0) + '&rnd=' + Math.random()
        }, loadTemplateCallback);
    }
    else {
        openflow.request('get', {
            url:openflow.root + 'servers/templateServer.js?cmd=getdraft&id='+ (id || 0) + '&user=' + openflow.user + '&rnd=' + Math.random()
        }, loadTemplateCallback);
    }

    /**
    * 加载模板完成后的回调
    */
    function loadTemplateCallback (data) {
        if (data) {
            //如果是草稿，则询问是否加载草稿，不是草稿则直接加载
            if(openflow.flag == 4 || (isdraft && confirm('有您上次未保存的草稿，确定加载草稿，取消不加载.')) || isdraft==false) {
                if (tip) tip.close();
                var enabled = openflow.flag == 4 || isdraft || openflow.admin || openflow.user == data.CreateUser || (data.Manager && data.Manager.indexOf(';' + openflow.user + ';') >= 0);
                var editor = initeditor(enabled,function() {
                    editor.data = data;
                    editor.name(data.Name);
                    if (!editor.data.properties) editor.data.properties = {};
                    editor.property.properties = {};

                    if (data.EditorXml) editor.read(data.EditorXml);
                    if (data.TemplateXml) editor.property.decode(data.TemplateXml);

                    resetParentHeight();
                    if (callback) callback(editor);
                });
            }
            //如果没有选择加载草稿，则去加载原数据
            else if(isdraft && id) {
                 isdraft=false;
                 openflow.request('get', {
                        url:openflow.root + 'servers/templateServer.js?cmd=readtemplate&id=' + id + '&rnd=' + Math.random()
                    }, loadTemplateCallback);
            }
            else {
                if (tip) tip.close();
                createEmptyEditor(callback);//创建空模板
            }
        }
        else {
            if(id) {
                if(isdraft && openflow.flag != 4) {
                    isdraft=false;
                     openflow.request('get', {
                            url:openflow.root + 'servers/templateServer.js?cmd=readtemplate&id='+ id + '&rnd=' + Math.random()
                        }, loadTemplateCallback);
                }
                else {
                    if (tip) tip.close();
                    openflow.message.tip('获取当前流程信息失败！', true);
                }
            }
            else {
                if (tip) tip.close();
                createEmptyEditor(callback);//创建空模板
            }
        }
    }
}

/**
* 调试任务
*/
function debugTask(editor,cell) {
    var ps = editor.property.cellProperties[cell.id];
    window.taskDebug = {};
    var container=$('<div style="height:90%;width:98%;font-szie:14px;overflow:auto;"> </div>');
    /**
    * 生成参数输入框
    */
    function createPropertyPanel(ps) {
        var panel = $('<table cellpadding="0" cellspacing="0" />');
        //创建输入框
        function createInput(n,k,v,h) {
            var tr = $('<tr />');
            var label = $('<td class="label"/>');
            label.html(n);
            label.appendTo(tr);
            var td2 = $('<td></td>')
            var input;
            if (h) {
                input = $('<textarea type="text" name="' + k + '" />');
            }
            else {
                input = $('<input type="text" name="' + k + '" />');
            }
            input.appendTo(td2);
            input.val(v);
            td2.appendTo(tr);
            tr.appendTo(panel);
            return input;
        }

        for (var i in ps) {
            var p = ps[i];
            createInput(i,i, p.value);
        }
        return panel;
    }

    var psarr = {};
    editor.property.initValues();
    if (editor.property.properties) {
        //从属性中获取变量，生成输入框
        for (var k in editor.property.properties) {
            var p = editor.property.properties[k];
            if (p.value) {
                var m;// = /\{[^\}]*\}/g.exec(p.value);
                var v = p.value;
                while (m = /%\{[^\}]*\}/g.exec(v)) {
                    var mstr = m[0].toString();
                    mstr=mstr.substring(2,mstr.length - 1);
                    psarr[mstr] = '';
                    v = v.substring(m.index + m[0].length);
                }
            }
        }
    }
    var pspanel = createPropertyPanel.call(this,psarr);
    pspanel.appendTo(container);

    $('<div>脚本或命令:</div>').appendTo(container);
    var codeinput= $('<textarea />').appendTo(container);
    codeinput.css({width:'100%',height:80});
    $('<div>节点:</div>').appendTo(container);
    var nodesinput=$('<input type="text" />').appendTo(container);
    nodesinput.css({width:'200px'});

    codeinput.val(ps['command']);
    nodesinput.val(ps['nodes']);

    var buttons = $('<div style="width:200px;margin:8px auto;" />').appendTo(container);
    var wait = window.taskDebug.wait = $('<div class="jm_wait"></div>').appendTo(container).hide();
    window.taskDebug.stateinfo = $('<div style="border:1px solid white;background-color:white;overflow:auto;"></div>').appendTo(container)

    var btndebug = window.taskDebug.btndebug = $('<button >调试</button>').appendTo(buttons).click(function() {
        btndebug.hide();
        var code = codeeditor.getValue();
           var host = nodesinput.val();
           if(!host) {
                openflow.message.error('请指定节点');
                return;
           }
           var properties = '';
           var inputpars = [];
           //收集用户输入的参数
           var inputs = pspanel.find('input,textarea');
           inputs.each(function(i,pt) {
                var name = $(pt).attr('name');
                var value = $(pt).val();
                //properties += name + '=' + encodeURIComponent(value) + ',';
                inputpars.push({name:name,value:value});
           });
           for(var k in editor.property.properties) {
                var value = editor.property.properties[k].value;
                //替换掉参数变量
                for(var j in inputpars) {
                    value = value.replace(new RegExp('%\\{'+inputpars[j].name+'\\}'),inputpars[j].value);
                }
                properties += k + '=' + encodeURIComponent(value) + ',';
           }
           //同时把参数也传入
          for(var j in inputpars) {
            properties += inputpars[j].name + '=' + encodeURIComponent(inputpars[j].value) + ',';
           }

           //调用服务调试此任务
           executor.startTask('',code,host,properties,function(r,err) {
                if(r) {
                    wait.show();
                    window.taskDebug.host = host;
                    window.taskDebug.id = r;
                    var logurl = openflow.root + 'executor/tasklog?uuid=' + window.taskDebug.id +
                                    '&host=' + encodeURIComponent(window.taskDebug.host);
                    window.taskDebug.loglink = '<br /><a href="'+logurl+'" target="_blank">更多日志&nbsp;&gt;&gt;</a>';
                    window.taskDebug.queryState = function() {
                        //获取任务执行的状态信息
                        executor.queryTask(window.taskDebug.id,window.taskDebug.host,function(st,error) {
                            if(error) {
                                openflow.message.error(error);
                                window.taskDebug.wait.hide();
                                window.taskDebug.btndebug.show();
                                return;
                            }
                            else {
                                switch(st.state) {
                                    case executor.taskState.success:
                                    case executor.taskState.unknown:
                                    case executor.taskState.not_found:
                                    case executor.taskState.error: {
                                        window.taskDebug.wait.hide();
                                        window.taskDebug.stopBtn.hide();
                                        window.taskDebug.suspendBtn.hide();
                                        window.taskDebug.resumeBtn.hide();
                                        window.taskDebug.btndebug.show();
                                        break;
                                    }
                                    case executor.taskState.running: {
                                        window.taskDebug.stopBtn.show();
                                        window.taskDebug.suspendBtn.show();
                                        window.taskDebug.resumeBtn.hide();
                                        setTimeout(window.taskDebug.queryState,2000);
                                        break;
                                    }
                                    case executor.taskState.suspended: {
                                        window.taskDebug.stopBtn.hide();
                                        window.taskDebug.suspendBtn.hide();
                                        window.taskDebug.resumeBtn.show();
                                        break;
                                    }
                                }
                                window.taskDebug.stateinfo.html(executor.taskState.getName(st.state));
                                executor.viewLog(window.taskDebug.id,window.taskDebug.host,16,function(d) {
                                    if(d) {
                                        d = d.replace(/\n/g,'<br />');
                                    }
                                    window.taskDebug.stateinfo.html(executor.taskState.getName(st.state) + ' <br /><br />' + d + window.taskDebug.loglink);
                                    container[0].scrollTop = container[0].scrollHeight;
                                });

                            }
                        });
                    }
                    window.taskDebug.queryState();
                }
                else {
                    btndebug.show();
                    openflow.message.success(err || '任务运行失败！');
                }
           });
    });
    window.taskDebug.stopBtn = $('<button >停止</button>').appendTo(buttons).click(function(){
        executor.stopTask(window.taskDebug.id,window.taskDebug.host,function(r,err){
            if(err) openflow.message.error(err);
        });
    }).hide();
    window.taskDebug.suspendBtn = $('<button >暂停</button>').appendTo(buttons).click(function(){
        executor.suspendTask(window.taskDebug.id,window.taskDebug.host,function(r,err){
            if(err) openflow.message.error(err);
        });
    }).hide();
    window.taskDebug.resumeBtn = $('<button >继续</button>').appendTo(buttons).click(function(){
        executor.resumeTask(window.taskDebug.id,window.taskDebug.host,function(r,err){
            if(err) openflow.message.error(err);
            else {
                setTimeout(window.taskDebug.queryState,2000);
            }
        });
    }).hide();

    var debugwin = new $jm.win({
        title: '调试', //标题
        content:container,
        modal: true,
        canMax: true, //禁止最大化
        canMin: false,
        canClose:true,
        resizeable: true,
        //top: 40,
        width: 600,
        height: 380,
        position: 'center',//位置，center=居中,top=顶部,left=左边
        bounds: { left: true, top: true, right: true, bottom: true }//边界锁定,//默认全锁定，此参数为一个json对象：如{left:false,top:true,right:true,bottom:false}//表示锁定顶部与右边，左边与底部可越界
    });
    debugwin.show();

    //联关编辑器
    var codeeditor = CodeMirror.fromTextArea(codeinput[0], {
        matchBrackets: true,
        mode: 'shell'
    });
    container.find('.CodeMirror').height(80);
}

/**
* 加载模板回调，如果是模板状态则添加创建实例按钮
*/
function loadcallback(editor,obj) {
    //如果为模板查看状态
    if (openflow.flag == 0) {
        editor.addTool('创建实例', 'content/images/toolbar/create_instance.png', function (evt) {
            if(!editor.editor.isEnabled()) {
                openflow.message.warning('没有权限');
                return;
            }
            if (openflow.flag == 0) {
                if (editor.data && editor.data.Id) {
                    openflow.message.tip('如果已修改模板，请先保存再启动实例');
                    editor.property.initValues();
                    editor.data.properties = editor.property.properties;
                    //创建实例并注册
                    var instance = new openflow.instance(editor.data);
                    instance.register();
                }
                else {
                    openflow.message.tip('请先保存模板');
                }
            }
            else {
                openflow.message.tip('当前状态下无法启动');
            }
        });
        //添 加查看对应的实例
        editor.addTool('查看实例', 'content/images/toolbar/instance.png', function (evt) {
            var instanceSelecter = new openflow.instanceSelecter({template:openflow.params['id']},function(id,name,template) {
                window.location = openflow.root + 'editor.js?mode=3&id=' + template + '&instance=' + id;
            });
            instanceSelecter.show(200,100);
        });
        //5秒后开始保存草稿
        //setTimeout(saveDraft,5000);
    }
    else if(openflow.flag == 3) {
        editor.addTool('修改参数', 'content/images/toolbar/save.png', function (evt) {
            if(openflow.currentInstance.permission().manager) {
                openflow.currentInstance.confirm('保存',function() {
                    openflow.currentInstance.update();//保存
                });
            }
            else {
                openflow.message.warning('无权限');
            }
        });
        //添 加查看对应的实例
        editor.addTool('转到模板', 'content/images/icon13.png', function (evt) {
           window.location = openflow.root + 'editor.js?mode=0&id=' + openflow.params['id'];
        });
    }
    editor.addTool('修改权限', 'content/images/toolbar/privilege.png', function (evt) {
            if (openflow.editable) {
                if (editor.data && editor.data.Id) {
                   editor.updatePermission();
                }
                else {
                    openflow.message.tip('请先保存模板');
                }
            }
            else if(openflow.flag == 3) {
                editor.updatePermission();
            }
        });

    //显示属性框
    editor.property.show(editor);
    resetParentHeight();
    setEditorSize();
    editor.isChanged= false;//默认为没有修改
    window.document.title= (openflow.flag == 4?'组件:':((openflow.currentInstance?'实例:':'模板:'))) + editor.data.Name;

    //添加右健菜单
    editor.graph.bind('mouseup',function(evt) {
        if(evt.button == 2 && evt.target && evt.target.findParent) {
            var cell = evt.target.type == 'jmCell' ?evt.target: evt.target.findParent('jmCell');////向上查询其父容器jmcell
            if(cell) {
                var menus = editor.editor.menus();
                var style = editor.subStyle(cell.styleName);
                if(openflow.editable && (style == 'process' || style == 'shell_process' || style == 'local_process')) {
                    menus.add('调试',function() {
                        debugTask(editor,cell);
                    }).show();
                }
                else if(openflow.flag == 3 && openflow.currentInstance) {
                    var st = openflow.currentInstance.getCellStateInfo(cell.id);//获取节点的状态信息
                    if(st && style == 'subflow') {
                        var ps = editor.property.getCellProperties(cell.id);
                        menus.add("查看子流程",function() {
                            var subflowwin = new $jm.win({
                                title: cell.value() + ' 【子流程明细】', //标题
                                url:openflow.root + 'editor.js?mode=3&id=1&uuid='+st.Uuid+'&instance=' + ps['instance'],
                                iframe:true,
                                modal: true,
                                canMax: true,
                                canMin: false,
                                resizeable: true,
                                state:$jm.winState.max,
                                //top: 40,
                                width: 800,
                                height: 600,
                                position: 'center',//位置，center=居中,top=顶部,left=左边
                                bounds: { left: true, top: true, right: true, bottom: true }//边界锁定,//默认全锁定，此参数为一个json对象：如{left:false,top:true,right:true,bottom:false}//表示锁定顶部与右边，左边与底部可越界
                            });
                            subflowwin.show();
                        }).show();
                    }
                    if(style == 'process' || style == 'shell_process') {
                        menus.add("查看脚本",function() {
                             var ps = editor.property.getCellProperties(cell.id);
                             var command = ps['command'];
                             //if(command) {
                               //command = openflow.formatXmlValue(command);
                             //}
                             var codeview = $('<div style="font-size:14px;" class="codeview"></div>');
                                //codeview.html(command);
                            codeview.window({title:'脚本',width:600,height:400});
                            CodeMirror(codeview[0],{mode:'shell',value:command,readOnly:true});

                        }).show();
                        menus.add("查看服务器",function() {
                            //获取当前节点的状态信息
                            var info = openflow.currentInstance.getCellStateInfo(cell.id);
                            if(!info) info = {};
                            if(info && info.Tasks && info.Tasks.length > 0) {
                                var html = '';
                                var len = info.Tasks.length;
                                //var rows = Math.floor(len / 6);
                                //if(len % 6 != 0) rows ++;

                                for(var i=0;i<len;i++) {
                                    html += '<li ';
                                    var task = info.Tasks[i];
                                    var statename =  '[' + openflow.instanceStatus.getStateName(task.State) + ']'+
                                                    openflow.formatXmlValue(task.ExtraInfo);
                                    html += 'title="'+statename+'" ';
                                    switch(task.State) {
                                        case openflow.instanceStatus.Stopped: {
                                            html += 'class = "stoped"';
                                            break;
                                        }
                                        case openflow.instanceStatus.Success: {
                                            html += 'class = "success"';
                                            break;
                                        }
                                        case openflow.instanceStatus.Faild: {
                                            html += 'class = "failed"';
                                            break;
                                        }
                                        case openflow.instanceStatus.Suspending: {
                                            html += 'class = "suspend"';
                                            break;
                                        }
                                    }
                                    html += '>' + (task.RemoteIP || task.IP) + '</li>';
                                }
                                html += '<div class="clear"></div>';
                                $('#taskServerContainer #task-servers').html(html);
                            }
                            $('#taskServerContainer').show();
                            $('#taskServerContainer').window({
                                    title:'服务器状况',
                                    modal:true,
                                    width:600,
                                    height:400
                            });
                        }).show();
                    }
                }

            }
        }
    });
}

$(function () {

    var id = openflow.params['id'];
    var uuid = openflow.params['uuid'];
    var instance = openflow.params['instance'];

    //加载jmgraph
    loadJMGraph(function() {
        //如果不支持的浏览器，则直接返回
        if(!jmUtils.isSupportedBrowser()) {
            $('#editor-parent').html('<div class="notsupported">不支持当前浏览器，请用支持html5的浏览器体验.<br /><br /><span>推荐使用</span><a target="_blank" href="http://google.cn/Chrome">chrome<a/></div>');
            return;
        }
        if (id && instance) {
            openflow.currentInstance = new openflow.instance();
            openflow.currentInstance.loadInstance(editor, instance,uuid, loadcallback);
        }
        else {
            loadopenflow(editor, id, loadcallback);
        }
    });


    $(window).resize(setEditorSize);

    //重置高度
    resetParentHeight();
});

/**
* 保存草稿
*/
function saveDraft() {
    //如果有修改，则定时保存草稿
    if(openflow.currentEditor) {
        if(openflow.currentEditor.isChanged) {
            openflow.currentEditor.save(true,function(r) {
                openflow.currentEditor.isChanged= false;//每保存都标识为未修改
                setTimeout(saveDraft,5000);
            });
        }
        else {
            setTimeout(saveDraft,5000);
        }
    }
}

//设置画布大小
function setEditorSize (callback) {
    //setTimeout(function() {
        var winw= $(window).width();
        //openflow.log('window.width:' + winw);
        var winh= $(window).height();
        var editorh= winh- $('#editor-toolbar').height();
        $('#editor-body').width(winw).height(editorh);
        //openflow.log('editor-body.width:' + $('#editor-body').width());
        $('#editor-body-right').height(editorh);
        if(!openflow.editable) {
            $('#editor-body-left').hide();
            $('#editor-body-right').width(winw);
        }
        else {
            $('#editor-body-left').height(editorh);
            $('#editor-body-right').width(winw - $('#editor-body-left').width() - 4);
        }
        $('#editor-parent').height(editorh);
        //$('#editor').height(editorh);

        //openflow.log('editor-body-right.width:' + $('#editor-body-right').width());
        if(openflow.currentEditor) {
            var w = $('#editor-parent').width();
            var h = editorh;
            if(openflow.currentEditor.editor.graph.canvas.width < w || openflow.currentEditor.editor.graph.canvas.height < h)
            {
                openflow.currentEditor.editor.graph.canvas.width = Math.max(w,openflow.currentEditor.editor.graph.canvas.width);
                openflow.currentEditor.editor.graph.canvas.height = Math.max(h,openflow.currentEditor.editor.graph.canvas.height);
                openflow.currentEditor.editor.graph.refresh();
            }
            if(openflow.currentEditor.property) openflow.currentEditor.property.resize(350);
        }
    //},200);
}
