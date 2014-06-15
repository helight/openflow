
/**
* 编辑器中元素属性编辑
*/
openflow.editor.prototype.property = (function () {
    /**
    * 属性编辑框
    */
    var __contructor = function () {
        this.container = $('<div id="property" class="module property left" style="width:350px;"/>');
        var content = $('<table style="width:100%;height:100%;" cellspacing="0" cellpadding="0"></table>').appendTo(this.container);
        var row = $('<tr></tr>').appendTo(content);
        var moveSide = $('<td style="width:4px;cursor:e-resize;background-color:#ccc;"></td>').appendTo(row);
        var contentSide = $('<td style="vertical-align:top;"></td>').appendTo(row);
        $('<div class="header" >属性</div>').appendTo(contentSide);
        this.body = $('<div class="module_content"></div>').appendTo(contentSide);

        this.parent = '#editor-parent';
        this.hide();

        //所有编辑过的元素属性
        this.cellProperties = {};
        this.properties = {};

        var _this = this;
        //重置大小
        this.resize = function(w) {
            if(_this.container && _this.parent) {
                if(!w) {
                    w= _this.container.width();
                }
                else {
                    _this.container.width(w);
                }
                var pw = _this.parent.width();
                var ew = pw - w - 5;
                $('#editor').width(ew);
                _this.container.height(_this.parent.height() - 5);
            }
        }

/*
        //窗口改变事件
        $(window).bind('resize', function () {
            if (_this.visible) {
                _this.resize(_this.container.width());
            }
        });
*/
        moveSide.bind('mousedown', function (evt) {
            var loc = openflow.getMouseLocation(evt);
            _this.resizeStart = true;
            _this.oldX = loc.x;
            return false;
            /*
            var l = loc.x - _this.container.offset().left;
            if (l > 0 && l < 5) {
                _this.resizeStart = true;
                _this.oldX = loc.x;
                _this.container.css({'cursor': 'e-resize','border-left-color  ': 'blue'});
            }*/
        });

        $(document).bind('mousemove', function (evt) {
            var loc = openflow.getMouseLocation(evt);

            /*var l = loc.x - _this.container.offset().left;
            if (l > 0 && l < 4) {
                _this.container.css({
                    'cursor': 'e-resize',
                    'border-left-color  ': 'blue !important'
                });
            }
            else if (_this.resizeStart == false) $('body').css('cursor', 'default');
*/
            if (_this.resizeStart) {
                $('body').css('cursor', 'e-resize');

                var dx = _this.oldX - loc.x;
                _this.oldX = loc.x;
                var w = _this.container.width() + dx;
                if (w < 120) w = 120;
                _this.resize(w);
                return false;
            }
        });

        $(document).bind('mouseup', function (evt) {
            $('body').css('cursor', 'default');
            //_this.container.css('cursor', 'default');
            if(_this.resizeStart) {
                _this.resizeStart = false;
                return false;
            }
        });

        this.cellPropertyMap = {
            'base': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' }
            },
            'connectLine':{
                'name':{type: 'string', name: '分支值',maxlength :64,width:'80%',description:'限制为64个字符'}
            },
            'shell_process': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' },
                'error_condition':{type:'number',name:'失败判定'},

                'task_timeout':{type:'number',name:'超时','default':0,description:'秒'},

                'nodes': { type: 'string', name: '运行脚本的IP',height: 30,description:'多IP间以逗号分隔',showRemote:true },
                'command': { type: 'string', name: 'Shell脚本', code:'shell', height: 150,description:'双击编辑框弹出脚本编辑器' }
            },
            'local_process': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' },
                'error_condition':{type:'number',name:'失败判定'},
                'task_timeout':{type:'number',name:'超时','default':0,description:'秒'},
                'command': { type: 'string', name: 'Shell脚本', code:'shell', height: 150,description:'双击编辑框弹出脚本编辑器' }
            },
            'install_process': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' },
                'error_condition':{type:'number',name:'失败判定'},
                'task_timeout':{type:'number',name:'超时','default':0,description:'秒'},
                'nodes': { type: 'string', name: '安装目标机器IP',height: 30,description:'多IP间以逗号分隔' },
                'path': { type: 'string', name: '安装路径',width:'80%',maxlength :128,description:'限制为128个字符' }
            },
            'download_process': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' },

                'task_timeout':{type:'number',name:'超时','default':0,description:'秒'},

                'remote' : {
                    'name':'远端（被下载文件所在节点）',
                    'type':'object',
                    'node': { type: 'string', name: 'IP',description:'仅限单个IP' },
                    'paths': { type: 'string', name: '下载文件路径', height: 50, description:'多文件以逗号分隔'}
                },
                'local':{
                    'name':'本地（执行文件下载节点）',
                    'type':'object',
                    'path': { type: 'string', name: '存放目录',width:'80%' },
                    'nodes':{type:'string',name:'IP列表', height: 30,description:'多IP间以逗号分隔'}
                }
            },
            'upload_process': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' },

                'task_timeout':{type:'number',name:'超时','default':0,description:'秒'},

                remote:{
                    'name':'远端（接受文件上传的节点）',
                    'type':'object',
                    'path': { type: 'string', name: '存放目录' },
                    'nodes': { type: 'string', name: 'IP列表', height: 30,description:'多IP间以逗号分隔' }
                },
                local:{
                    'name':'本地（执行上传文件的节点）',
                    'type':'object',
                    'node':{type:'string',name:'上传IP', description:'仅限单个IP'},
                    'paths': { type: 'string', name: '上传文件路径', height: 50,description:'多文件以逗号分隔' }
                }
            },
            'switch': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' }
            },
            'fork': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' }
            },
            'subflow': {
                'name': { type: 'string', name: '名称',maxlength :64,description:'限制为64个字符' },
                'description': { type: 'string', name: '描述', height: 30,description:'默认和名称相同' },
                'instance': { type: 'string', name: '关联实例',description:'双击会弹出“选择实例”对话框' }
            }
        };
        this.cellPropertyMap['process'] = this.cellPropertyMap['shell_process'];
    }

    /**
    * 设置和获取当前标题
    */
    __contructor.prototype.title = function (t) {
        if (t) this.container.find('div.header').html(t);
        return this.container.find('div.header').html();
    }

    /**
    * 清除
    */
    __contructor.prototype.clear = function () {
        //清除前面的属性
        if (this.cellProperties) this.cellProperties = {};
    }

    /**
    * 反解xml获取元素的属性
    */
    __contructor.prototype.decode = function (xml) {
        try{
            if (xml) {
                var json = jmUtils.xmlToJSON(xml);

                if (json && json.children && json.children.length > 0) {
                    var _this = this;
                    //解析节点属性
                    function decodeNodeProperty(p,key,node) {
                        //如果属性为一个结构，则处理子属性
                        if(node.children.length > 0) {
                            for(var j in node.children) {
                                var cn = node.children[j];
                                var k = key?(key + '.' + node.name.toLowerCase()): node.name.toLowerCase();
                                decodeNodeProperty(p,k,cn);
                            }
                        }
                        else {
                            var value = node.value;
                            var name = node.name.toLowerCase();
                            //按{x.x.x.x}的方式组合key
                            if(key) name = key + '.' + name;
                            if (name && value) {
                                p[name] = $.trim(value);
                            }
                            for(var k in node.attributes) {
                                var attr = node.attributes[k];
                                if(typeof attr === 'string') {
                                    p[name + '.' + k] = $.trim(attr);
                                }
                            }
                        }
                    }
                    //获取节点属性
                    function getNodeProperty(node) {
                        if (node) {
                            var id = node.attributes ? node.attributes['id'] : '';
                            if (typeof id != 'undefined') {
                                var p = _this.cellProperties[id] = {};
                                for (var i in node.children) {
                                    var n = node.children[i];
                                    var name = n.name.toLowerCase();

                                    //此属性不是自定义属性
                                    if (name == 'branches' || name == 'next') continue;
                                    decodeNodeProperty(p,'',n);
                                }

                            }
                                //收集流程属性
                            else if (node.name.toLowerCase() == 'properties') {
                                for (var i in node.children) {
                                    var n = node.children[i];
                                    if (n.name.toLowerCase() == 'property') {
                                        var name = n.attributes['name'];
                                        var value = n.attributes['value'];
                                        var des = n.attributes['description'];
                                        if (name) {
                                            _this.properties[name] = {
                                                'value': value,
                                                'description':des
                                            };
                                        }
                                    }
                                }
                            }
                        }
                    }

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
                                    if (_this.properties[name]) {
                                        _this.properties[name].value = value;
                                    }
                                    else {
                                        _this.properties[name] = {
                                            'value': value
                                        };
                                    }
                                }
                            }
                        }
                    }

                    //循环读取flow下的节点
                    for(var i in json.children) {
                        var n = json.children[i];
                        if (n.name.toLowerCase() == 'template') {
                            for(var j in n.children) {
                                getNodeProperty(n.children[j]);
                            }
                        }
                        else if (n.name.toLowerCase() == 'instance') {
                            for(var j in n.children) {
                                getInstanceProperty(n.children[j]);
                            }
                        }
                    }
                }
            }
        }
        catch (e)
        {
            openflow.message.warning('解析元素属性失败');
        }
    }

    /**
    * 获取指定元素的属性
    */
    __contructor.prototype.getCellProperties = function (id) {
        if (this.cellProperties && this.cellProperties[id]) return this.cellProperties[id];
        return {};
    }

    /*
    * 显示指定的单元属性
    */
    __contructor.prototype.show = function (editor, target) {
        if((target && this.target == target) || (!target && this.target == editor)) return;
        this.editor = editor;
        if (this.target && !target) target = this.target;
        target = target || editor;

        this.initValues();//收集值
        this.target = target;
        this.createPanel(target);

        if (!this.visible) {
            if (this.container.width() > 0) this.container.width(0);
            if (typeof this.parent === 'string') {
                this.parent = $(this.parent);
                this.container.appendTo(this.parent);
                $('<div class="clear"></div>').appendTo(this.parent);
            }
            this.container.show();
            this.resize(350);
            this.visible = true;
        }
    }
    /**
    * 隐藏
    */
    __contructor.prototype.hide = function () {
        if (typeof this.parent === 'string') {
            var tmp = $(this.parent);
            if (tmp.length > 0) {
                this.parent = tmp;
                this.container.appendTo(this.parent);
                $('<div class="clear"></div>').appendTo(this.parent);
            }
            else {
                return;
            }
        }

        this.initValues();//收集值

        var w = this.parent.width();
        var _container = this.container;
        this.container.animate({ 'width': 0 }, function () {
            _container.hide();
        });
        $('#editor').animate({ 'width': w });
        this.visible = false;
    }

    /**
     * 创建属性对应的输入框tr
     *
     */
    function createCellPropertyTR(p,key,cell,properties) {
        var tr = $('<div class="cell-property-item" />');
        var label = $('<div class="label"></div>');
        var name = '<b>' + p.name + '</b>';
        if(p.description) {
            name = name + '<span>('+p.description+')</span>';
        }

        label.html(name);
        label.appendTo(tr);

        var td2 = $('<div class="input"></div>')
        var input = null;
        if (!openflow.editable) {
            input = $('<div data-name="' + key + '" ></div>');
        }
        else if (p.code) {
            //input = $('<div name="'+key+'"></div>');
            input=$('<textarea id="input_'+key+'" type="text" data-name="' + key + '" rows="2" />');
        }
        if (p.height && openflow.editable) {
            if (!input) input = $('<textarea id="input_'+key+'" type="text" data-name="' + key + '" rows="2" />');
            input.height(p.height);
        }
        else {
            if (!input) {
                input = $('<input id="input_' + key + '" type="text" data-name="' + key + '" />');
            }
            if(openflow.editable && key === 'instance') {
                input.bind('dblclick',function() {
                    var _this = $(this);
                    var selecter = new openflow.instanceSelecter(function(id,name) {
                        _this.attr('instance',id);
                        _this.val(name);
                    });
                    selecter.show();
                    });
            }
        }

        if(p.width) {
            input.width(p.width);
        }
        if(p.code) {
            var codecontainer= $('<div class="code"></div>').appendTo(td2);
            if(openflow.editable)  {
                    codecontainer.attr('contenteditable',true);
                    codecontainer.toggleClass('code-editor',true);
                }
            input.appendTo(codecontainer);
        }
        else if(key == 'error_condition') { //出错判定
            var container= $('<div class="container clearfix"></div>').appendTo(td2);
            input.appendTo(container);
            input.css({float:'left'});
            var checkbox = $('<input class="element" type="checkbox" />').appendTo(container);
            $('<span class="element">百分比(%)</span>').appendTo(container);
            if(properties[key]) {
                checkbox.attr('checked',properties[key].indexOf('%') > 0);
            }
            else {
                input.val(0);
            }
            if(!openflow.editable)  {
                 checkbox.attr('disabled',true);
            }
            //$('<div class="clear" />').appendTo(container);
        }

        else if(key == 'nodes' && p.showRemote) { //如果为节点，则显示是否远程执行
            label.toggleClass('container',true);

            var checkbox = $('<input class="element" type="checkbox" title="格式示例:user@ip:port#password" data-name="nodes.remote" />').appendTo(label);
            $('<span class="element">远程执行</span>').appendTo(label);
            var isremote = properties['nodes.remote'] === true || properties['nodes.remote'] == 'true';
            checkbox.attr('checked',isremote);

            if(!openflow.editable)  {
                 checkbox.attr('disabled',true);
            }
            input.appendTo(td2);
        }


            /*var curinput = input;
            var curcontainer = td2;

            var nodescontainer = $('<div></div>');
            var addnodemark = $('<a href="#">增加服务器</a>').click(function() {
                addNode('','','','','',++nodeindex);
                return false;
            });

            if(isremote) {
                showRemoteNodes();
            }
            else {
                curinput.appendTo(curcontainer);
            }

            var nodeindex=0;
            function addNode(ip,port,user,pwd,timeout,index,expanded) {
                index = index || nodeindex || 0;
                var propertypanel = new openflow.expandBanner(nodescontainer,expanded);
                 var nodecontainer = $('<div class="cell-nodes"></div>');
                //添加到展开栏中
                propertypanel.add(ip || "node",nodecontainer,true);
                propertypanel.show();
                var ipct = $('<div></div>');
                $('<label></label>').appendTo(ipct).html('IP');
                $('<input type="text" data-name="ip" data-nodes="'+index+'" />').appendTo(ipct).val(ip).change(function() {
                    propertypanel.title(this.value || 'node');
                });;
                ipct.appendTo(nodecontainer);
                var portct = $('<div></div>');
                $('<label></label>').appendTo(portct).html('端口');
                $('<input type="number" data-name="port" data-nodes="'+index+'" />').appendTo(portct).val(port);
                portct.appendTo(nodecontainer);
                var userct = $('<div></div>');
                $('<label></label>').appendTo(userct).html('用户名');
                $('<input type="text" data-name="user" data-nodes="'+index+'" />').appendTo(userct).val(user);
                userct.appendTo(nodecontainer);
                var pwdct = $('<div></div>');
                $('<label></label>').appendTo(pwdct).html('密码');
                $('<input type="text" data-name="password" data-nodes="'+index+'" />').appendTo(pwdct).val(pwd);
                pwdct.appendTo(nodecontainer);
                var timeoutct = $('<div></div>');
                $('<label></label>').appendTo(timeoutct).html('超时时间');
                $('<input type="number" data-name="timeout" data-nodes="'+index+'" />').appendTo(timeoutct).val(timeout);
                timeoutct.appendTo(nodecontainer);
                addnodemark.appendTo(nodescontainer);

                nodeindex = index;
                return propertypanel;
            }

            //显示为远程执行框
            function showRemoteNodes() {
                nodescontainer.html('');
                var nodes = properties['nodes'];
                if(nodes && jmUtils.isArray(nodes) && nodes.length > 0) {
                    for(var i=0;i<nodes.length ;i++) {
                        var node = nodes[i];
                        addNode(node['ip'],node['port'],node['user'],node['password'],node['timeout'],i,i == nodes.length-1);
                    }
                }
                else {
                    addNode('','','','','',nodeindex,true);
                }
                nodescontainer.appendTo(curcontainer);
            }

            checkbox.bind('change',function() {
                var nodes = properties['nodes'];
                if(this.checked) {
                    curinput.remove();
                    if(typeof nodes === 'string') {
                        properties['nodes'] = [];
                        var ips = nodes.split(';');
                        for(var i in ips) {
                            var ip = ips[i];
                            if($.trim(ip)) {
                                properties['nodes'].push({ip:ip});
                            }
                        }
                    }
                    showRemoteNodes();
                }
                else {
                    nodescontainer.remove();
                    if(jmUtils.isArray(nodes)) {
                        var ips='';
                        for(var i=0;i<nodes.length;i++) {
                            ips += (ips?';':'') + nodes[i].ip;
                        }
                        curinput.val(ips);
                    }
                    curinput.appendTo(curcontainer);
                }
            });
        }*/
        else {
            input.appendTo(td2);
        }

        $('<div class="clear" />').appendTo(label);

        if(key == 'description') desinput = input;
        if (key == 'name') {
            properties[key] = properties[key] || cell.value();
            //如果说明为空。则默认显示为名称
            input.bind('blur',function() {
                if(desinput && !desinput.val()) {
                    desinput.val($(this).val());
                }
            });
        }

        if(p.maxlength) {
            input.attr('maxlength',p.maxlength);
        }

        //如果已级编辑过则显示
        if (properties[key]) {
            if (openflow.editable) {
                if(key == 'error_condition') {
                    input.val(properties[key].replace('%',''));
                }
                else {
                    input.val(properties[key]);
                }
            }
            else {
                input.html(properties[key]);
            }
        }
        //设置默认值
        else if(typeof p['default'] != 'undefined') {
            input.val(p['default']);
        }

        //不可编辑
        if (openflow.editable == false) {
            input.attr('disabled', true);
            input.attr('contenteditable', false);
        }
        else if(name == 'name') {
            input.bind('blur', function () {
                var name = $(this).attr('data-name');
                if (!name) return;
                if (name == 'name') {
                    cell.value($(this).val());
                    _this.editor.graph.refresh();
                }
            });
        }
        //由于div编辑时会无限申长。所以外套一个可滚动的div
        if (p.code) {
            input.attr('data-code',p.code);
            input.attr('title','双击打开编辑窗口');
            input.bind('dblclick',function() {
                var codecontainer=$('<div style="height:100%;width:100%;overflow:auto;" class="codeeditor"> </div>');
                var codeinput=$('<textarea />').appendTo(codecontainer);
                $(codeinput).height(380);
                codeinput.val(input.val());
                var codeeditor;

                codecontainer.dialog({
                    width:600,
                    height:380,
                    modal:true,
                    maximizable:true,
                    resizable:true,
                    title:'代码编辑',
                    buttons:[{'text':'确定',handler:function() {
                        input.val(codeeditor.getValue());
                       codecontainer.dialog('close');
                    }},{'text':'取消',handler:function() {
                       codecontainer.dialog('close');
                    }}]
                });
                //联关编辑器
                codeeditor = CodeMirror.fromTextArea(codeinput[0], {
                    matchBrackets: true,
                    mode: p.code
                });
            });
        }
        td2.appendTo(tr);
        return tr;
    }

    /**
    * 生成节点属性输入框
    */
    function createCellPropertyPanel(cell) {
        var _this = this;
        if (!this.cellPanel) this.cellPanel = $('<div></div>');
        else this.cellPanel.html('');

        var panel = $('<div style="margin-left:2px;text-align:left;color:#000;font-weight:bold;" />');
        var ps = this.cellPropertyMap['base'];//默认属性
        //如果是节点,否则为连线
        if(cell.is(jmCell)) {
            //获取当前节点类型的属性
            var style = cell.styleName || cell.style;
            if (!style) return;
            //先去除所有后缀，主要是以下划线分隔的状态style
            var extIndex = style.indexOf('-');
            if(extIndex > 0) {
                style=style.substring(0,extIndex);
            }
            ps = this.cellPropertyMap[style] || this.cellPropertyMap['base'];
        }
        else if(cell.is(jmConnectLine)) {
            ps = this.cellPropertyMap['connectLine'] || this.cellPropertyMap['base'];
        }

        if (ps) {
            var properties = this.cellProperties[cell.id] || (this.cellProperties[cell.id] = {});
            var codeinputs = [];
            var desinput = null;
            for (var i in ps) {
                var p = ps[i];
                if(p.type == 'object') {
                    var propertypanel = new openflow.expandBanner(panel);
                    var ppanel = $('<div></div>');
                    //添加到展开栏中
                    propertypanel.add(p.name,ppanel,true);
                    for(var j in p) {
                        var p2 = p[j];
                        if(typeof p2 == 'object') {
                            var tr = createCellPropertyTR(p2,i + '.' + j,cell,properties);
                            tr.appendTo(ppanel);
                        }
                    }
                    propertypanel.show();
                }
                else {
                    var tr = createCellPropertyTR(p,i,cell,properties)
                    tr.appendTo(panel);
                }
            }
        }
        if (!openflow.editable) {
            var panelhead = $('<div class="expand_banner cellPropertyHead">' + (cell.value() || '') + '属性</div>');
            panelhead.appendTo(this.cellPanel);
        }
        panel.appendTo(this.cellPanel);

        this.cellPanel.appendTo(this.body);
    }

    /**
    * 生成流程图属性框
    */
    function createFlowPropertyPanel() {
        if (!this.propertyPanel) {
            this.propertyPanel = $('<div></div>');
        }
        this.propertyPanel.html('');
        var propanel = $('<table class="jm-table" cellpadding="0" cellspacing="0" />').appendTo(this.propertyPanel);
        //缓存输入框
        this.propertyPanel.inputs = {};

        var header = $('<thead></thead>').appendTo(propanel);
        var headtr = $('<tr/>').appendTo(header);
        $('<th style="width:80px;">属性名</th>').appendTo(headtr);
        if (openflow.flag == 3) $('<th>当前值</th>').appendTo(headtr);
        else {
            $('<th>默认值</th>').appendTo(headtr);
        }
        if(openflow.editable) $('<th>说明</th>').appendTo(headtr);
        $('<th class="add"></th>').appendTo(headtr);
        var propertyIndex = 0;

        //生成属性输入框
        function createPropertyInput(n, p) {
            var tr = $('<tr> </tr>');
            var td1 = $('<td style="padding:0;"/>')
            var obj = {};

            if (openflow.editable) {
                obj.name = $('<input type="text" name="name" />');
                obj.name.attr('title', '属性名只能为下划线、字母或数字,首字母只能为下划线或字母并且不能以openflow关健司开头');
                obj.name.bind('keypress', function (e) {
                    var code = e.keyCode || e.charCode;
                    var txt = $.trim(obj.name.val());
                    $(this).toggleClass('error', false);
                    //首字母只能为下划线或字母
                    if (txt == '') {
                        var r = (code == 95 || (code > 64 && code < 91) || (code > 96 && code < 123));
                        if (r == false) {
                            //$(this).attr('title','属性名首字母只能为下划线或字母');
                            $(this).toggleClass('error', true);
                        }
                        return r;
                    }
                    else if (txt.toLowerCase() == 'iflo' && (code == 119 || code == 87)) {
                        //openflow.message.tip('属性名不能以openflow开头');
                        $(this).toggleClass('error', true);
                        return false;
                    }
                    //属性名只能为数字或字母
                    var r = (code == 95 || code == 8 || code == 46 || code == 32 || code == 37 || code == 39 || (code > 64 && code < 91) || (code > 96 && code < 123) || (code >= 48 && code <= 57));
                    if (r == false) {
                        //openflow.message.tip('属性名首字母只能为下划线,字母或数字');
                        $(this).toggleClass('error', true);
                    }
                    return r;
                });
                if (!openflow.editable) {
                    obj.name.attr('disabled', true);//只有模板状态下可以修改名称
                    obj.name.toggleClass('disabled', true);
                }

                obj.value = $('<input type="text" name="value" />');
                obj.des = $('<textarea type="text" name="description" />');

                if (n) obj.name.val(n);
                if (n && p) {
                    if (p['value']) obj.value.val(p['value']);
                    if (p['description']) obj.des.val(p['description']);
                }
            }
            else {
                obj.name = $('<div name="name" />');
                obj.value = $('<div name="value" />');
                obj.des = $('<div name="description" />');
                if (n) obj.name.html(n);
                if (n && p) {
                    if (p['value']) obj.value.html(p['value']);
                    obj.name.attr('title', p['description']);
                    //if (p['description']) obj.des.html(p['description']);
                    tr.attr('title',p['description']);
                }
            }
            obj.name.appendTo(td1);
            var td2 = $('<td style="padding:0;" />')
            obj.value.appendTo(td2);

            if (openflow.flag == 3) {
                obj.value.attr('disabled', true);//实例状态下不能修改
                obj.value.toggleClass('disabled', true);
                obj.des.attr('disabled', true);//实例状态下不能修改
                obj.des.toggleClass('disabled', true);
            }
            td1.appendTo(tr);
            td2.appendTo(tr);

            if(openflow.editable) {
                var td3 = $('<td style="padding:0;" />');
                var editdes = $('<a href="#">...</a>').appendTo(td3);
                editdes.click(function() {
                    var p = $(this).offset();
                    var panret = obj.des.parent();
                    var left = p.left - panret.width();
                    var top = p.top;
                    panret.css({
                        left:left,
                        top:top
                    });
                    panret.show();
                    obj.des[0].focus();
                    return false;
                });
                editdes.attr('title',obj.des.val());
                var desparent = $('<div></div>');
                desparent.css({
                    'position':'absolute',
                    'background-color':'white',
                    'width':'180px',
                    'height':'80px',
                    'border':'1px solid black',
                    'box-shadow': '0 0 20 0 #353535 inset'
                });
                obj.des.appendTo(desparent);
                obj.des.css({
                    'width':'100%', //'172px',
                    'height':'100%'//,'72px'
                });
                obj.des.bind('blur',function() {
                    $(this).parent().hide();
                    editdes.attr('title',$(this).val());
                })
                desparent.hide();
                desparent.appendTo('body');
                td3.appendTo(tr);
            }

            var td4 = $('<td style="padding:0;" class="add" />')
            if (openflow.editable) {
                if (this.flowProadd) this.flowProadd.remove();
                this.flowProadd = $('<a href="#" class="add" style="margin:2px;display:block;"></a>');
                this.flowProadd.html('+');
                this.flowProadd.appendTo(td4);
                var _this = this;
                var addclick = function () {
                    var t = createPropertyInput.call(_this);
                    t.appendTo(propanel);
                    return false;
                };
                this.flowProadd.unbind('click', addclick);
                this.flowProadd.bind('click', addclick);
            }
            td4.appendTo(tr);

            this.propertyPanel.inputs[n || propertyIndex ++] = obj;
            return tr;
        }
        var index = 0;
        //生成已存在的属性框
        if (this.properties) {
            for (var i in this.properties) {
                var t = createPropertyInput.call(this, i, this.properties[i]);
                t.appendTo(propanel);
                index++;
            }
        }

        //模板状态下补齐输入框
        if (openflow.editable) {
            for (; index < 1; index++) {
                var t = createPropertyInput.call(this);
                t.appendTo(propanel);
            }
        }

        if (this.propertyPanel.parent().length == 0) this.propertyPanel.appendTo(this.body);
    }

    /**
    * 生成实例运行时属性栏
    */
    function createInstancePropertyPanel(editor,target) {
        if (!this.propertyPanel) {
            this.propertyPanel = $('<div></div>');
        }
        //this.propertyPanel.html('');
        if(!this.instanceState) {
            this.instanceState = $('<table width="100%" cellpadding="0" cellspacing="0" style="border:1px solid #969696;border-left:0;border-right:0;"></table>').appendTo(this.propertyPanel );
        }
        var instanceHtml = '<tr><td style="padding:0 4px;background:#E9EBEE;width:80px;border-right:#ccc 1px solid;">Container</td>';
        instanceHtml += '<td style="padding:4px 10px;">'+(openflow.currentInstance.stateInfo?openflow.currentInstance.stateInfo.containerIp:'')+'</td></tr>';

        var instanceExtraInfo = openflow.currentInstance.stateInfo?openflow.formatXmlValue(openflow.currentInstance.stateInfo.ExtraInfo):'';
        instanceHtml += '<tr><td style="padding:0 4px;background:#E9EBEE;width:80px;border-right:#ccc 1px solid;" title="'+instanceExtraInfo+'">'+
                            (openflow.currentInstance.stateInfo?openflow.instanceStatus.getStateName(openflow.currentInstance.stateInfo.State):'') +'</td>';
        instanceHtml += '<td style="padding:4px 10px;"><a href="#" onclick="javascript:openflow.currentInstance.showRunningDetail();return false;">查看运行轨迹</a></td></tr>';
        var des =(openflow.currentInstance.data?openflow.currentInstance.data.Description:'');
        instanceHtml += '<tr><td colspan="2" style="border-top:1px solid #969696;padding:10px;background-color:#E9EBEE;">' +
                        des + '</td></tr>';
        this.instanceState.html(instanceHtml);

        //创建可展开的属性栏
        if(!this.propertyExpendPanel) this.propertyExpendPanel= new openflow.expandBanner(this.propertyPanel);
        if(!this.instancePropertyTable) {
            this.instancePropertyTable= $('<table width="100%" cellpadding="0" cellspacing="0"></table>');
            //添加到展开栏中
            this.propertyExpendPanel.add('全局属性',this.instancePropertyTable,true);
        }

        if(openflow.currentInstance.stateInfo) {
            if(openflow.currentInstance.stateInfo.Properties) {
                var pshtml= '';
                for (var i in openflow.currentInstance.stateInfo.Properties) {
                    var p = openflow.currentInstance.stateInfo.Properties[i];
                    if (p) {
                        pshtml += '<tr><td ' + (p.Description?('title="'+openflow.formatXmlValue(p.Description)+'"'):'')+
                                ' style="padding:0 4px;background:#E9EBEE;width:80px;border:#969696 1px solid;border-top:0;border-left:0;text-align:right;">' + openflow.formatXmlValue(p.PropertyName) + '</td>';
                        pshtml += '<td style="padding:0 4px;background:#F6F6F6;border:#969696 1px solid;border-top:0;border-right:0;">' +
                                    openflow.formatXmlValue(p.PropertyValue) +'</td></tr>';
                    }
                }
                this.instancePropertyTable.html(pshtml);
                if(!pshtml) this.propertyExpendPanel.hide();
                else {
                    this.propertyExpendPanel.show();
                }
            }

            if(!this.nodeExpendPannel) {
                this.nodeExpendPannel= new openflow.expandBanner(this.propertyPanel);
            }
            //添加一个节点信息展示栏
            if(!this.nodeInfoTable) {
                this.nodeInfoTable= $('<table width="100%" cellpadding="0" cellspacing="0" style="border:1px solid #969696;border-left:0;border-right:0;background:#E9EBEE;"></table>');
                //添加到展开栏中
                this.nodeExpendPannel.add('过程(ID)',this.nodeInfoTable,true);
            }
            if(!this.nodeInfo) {
                this.nodeInfo = {};
                var _this= this;
                function goToCell() {
                    var id=$(this).attr('data-id');
                    for (var i in openflow.currentInstance.stateInfo.Flowables) {
                        var n =openflow.currentInstance.stateInfo.Flowables[i];
                        if(n.Id == id) {
                            createInstancePropertyPanel.call(_this,editor,n);
                            break;
                        }
                    }
                    return false;
                }

                this.nodeInfo.nextButton = $('<a href="#" style="float:right;margin-right:2px;"><img src="/content/mxgraph/images/next.png" border="0" /></a>').appendTo(this.nodeExpendPannel.banner);
                this.nodeInfo.nextButton.click(goToCell);
                this.nodeInfo.preButton = $('<a href="#" style="float:right;margin-right:2px;"><img src="/content/mxgraph/images/pre.png" border="0" /></a>').appendTo(this.nodeExpendPannel.banner);
                this.nodeInfo.preButton.click(goToCell);

                var tr = $('<tr></tr>');
                this.nodeInfo.startTime = $('<td title="开始时间" style="border:#808080 1px solid;border-left:0;border-top:0;text-align:center;padding:2px;"></td>');
                this.nodeInfo.startTime.appendTo(tr);
                this.nodeInfo.state = $('<td title="状态" style="border:#808080 1px solid;border-left:0;border-top:0;text-align:center;padding:2px;"></td>');
                this.nodeInfo.state.appendTo(tr);
                //执行时长
                this.nodeInfo.runTimes = $('<td title="运行时长" style="border:#808080 1px solid;border-left:0;border-top:0;text-align:center;padding:2px;"></td>');
                this.nodeInfo.runTimes.appendTo(tr);
                //执行次数
                this.nodeInfo.runCount = $('<td title="轮次" style="border:#808080 1px solid;border-left:0;border-top:0;border-right:0;text-align:center;padding:2px;"></td>');
                this.nodeInfo.runCount.appendTo(tr);
                tr.appendTo(this.nodeInfoTable);

                //任务信息展示
                //任务头信息
                this.nodeInfo.taskHead= $('<td colspan="4"></td>').appendTo($('<tr></tr>').appendTo(this.nodeInfoTable));

                this.nodeInfo.taskTd= $('<td colspan="4"></td>').appendTo($('<tr></tr>').appendTo(this.nodeInfoTable));
                this.nodeInfo.taskTable= $('<table width="100%" cellpadding="0" cellspacing="0"></table>').appendTo(this.nodeInfo.taskTd);

            }

            /**
            * 生成节点对象
            */
            function showCellInfo(node) {
                //记录当前选择的节点ID
                this.currnetShowCellId = node.Id;
                var cell =  editor.editor.getCell(node.Id);
                var nodeTitle =  (cell && cell.value?cell.value() : '') + '('+ node.Id +')';
                //nodehtml = '<tr><td style="border:#808080 1px solid;border-left:0;border-top:0;text-align:center;padding:2px;">';
                this.nodeInfo.startTime.html($jm.formatDate(node.StartTime * 1000,'yyyy/MM/dd HH:mm:ss'));
                //nodehtml += '</td><td style="border:#808080 1px solid;border-left:0;border-top:0;text-align:center;padding:2px;">';
                this.nodeInfo.state.attr('title',node.ExtraInfo);
                this.nodeInfo.state.html(openflow.instanceStatus.getStateName(node.State));
                //nodehtml += '</td><td style="border:#808080 1px solid;border-left:0;border-top:0;text-align:center;padding:2px;">';
                if(node.EndTime && node.EndTime != '-') {
                    var sptime= $jm.timeSpan((node.EndTime - node.StartTime) * 1000);
                    this.nodeInfo.runTimes.html(sptime.toString());
                }
                else {
                    this.nodeInfo.runTimes.html('-');
                }
                //nodehtml += '</td><td style="border:#808080 1px solid;border-left:0;border-top:0;border-right:0;text-align:center;padding:2px;">';
                this.nodeInfo.runCount.html(node.Rounds);//轮次
                //nodehtml += '</td></tr>';
                //任务信息
                //nodehtml += '<tr><td colspan="4"><table width="100%" cellpadding="0" cellspacing="0">';
                this.nodeInfo.taskHead.html('任务信息(' + (node.Tasks?node.Tasks.length:0) + ')');
                var taskhtml= '';
                if(node.Tasks) {
                    for(var j in node.Tasks) {
                        var task = node.Tasks[j];
                        taskhtml += '<tr><td style="width:25%;border:#808080 1px solid;border-left:0;text-align:center;padding:2px;">';
                        var logurl = openflow.root + 'executor/tasklog?uuid=' + task.Uuid + '&host=' + task.IP;
                        taskhtml += '<a href="'+logurl+'" title="查看运行日志" target="_blank">' + task.TaskId + '</a></td><td style="width:25%;border:#808080 1px solid;border-left:0;text-align:center;padding:2px;">';
                        taskhtml += (task.RemoteIP || task.IP) + '</td><td style="width:25%;border:#808080 1px solid;border-left:0;text-align:center;padding:2px;">';
                        taskhtml += openflow.instanceStatus.getStateName(task.State) +
                                    '</td><td style="width:25%;border:#808080 1px solid;border-left:0;border-right:0;text-align:center;padding:2px;">';
                        if(task.EndTime && task.EndTime != '-') {
                            var sptime= $jm.timeSpan((task.EndTime - task.StartTime) * 1000);
                             taskhtml += sptime.toString();
                        }
                        else {
                            taskhtml += '-';
                        }

                        taskhtml += '</td></tr>'; //运行时长
                        taskhtml += '<tr><td colspan="4">' + task.Uuid + '</td></tr>';
                        taskhtml += '<tr><td colspan="4">' + openflow.formatXmlValue(task.ExtraInfo) + '</td></tr>';
                    }
                }
                this.nodeInfo.taskTable.html(taskhtml);
                return nodeTitle;
            }

            //表示没有手动选择节点,且当前已在显示
            var showed= false;
            var currentNode = null;
            this.nodeInfo.preButton.hide();
            this.nodeInfo.nextButton.hide();
            if(!this.target || jmUtils.isType(this.target,jmCell)) {
                //openflow.log('current cell id:' + this.currnetShowCellId);
                //如果指定了目标节点，则更改当前节点为目标节点，否则不改变
                this.currnetShowCellId = target && target.Id? target.Id : this.currnetShowCellId;
                //openflow.log('target cell id:' + this.currnetShowCellId);
                if(this.currnetShowCellId) {
                    //找到当前节点
                    for (var i in openflow.currentInstance.stateInfo.Flowables) {
                        var p = openflow.currentInstance.stateInfo.Flowables[i];
                        if(p.Id == this.currnetShowCellId) {
                            currentNode= p;
                            break;
                        }
                    }
                }
            }

            //生成节点属性
            for (var i in openflow.currentInstance.stateInfo.Flowables) {
                var p = openflow.currentInstance.stateInfo.Flowables[i];
                if(this.target && jmUtils.isType(this.target,jmCell)) {
                    if(p.Id == this.target.id) {
                        var title = showCellInfo.call(this,p);
                        //更新节点信息
                        this.nodeExpendPannel.title(title);
                        showed = true;
                        break;
                    }
                }
                else if(currentNode && currentNode.State == openflow.instanceStatus.Running) {
                    if(p.Id == currentNode.Id) {
                        var title = showCellInfo.call(this,p);
                        //更新节点信息
                        this.nodeExpendPannel.title(title);
                        showed= true;
                        continue;
                    }
                    else if(p.State == openflow.instanceStatus.Running) {
                        //如果没有显示，但当前状态为running 则显示上一条为当前节点
                        if(showed == false) {
                            this.nodeInfo.preButton.attr('data-id',p.Id);
                            this.nodeInfo.preButton.show();
                        }
                        //如果已显示。则当前为下一条
                        else {
                            this.nodeInfo.nextButton.attr('data-id',p.Id);
                            this.nodeInfo.nextButton.show();
                            break;
                        }
                    }
                }
                else if(p.State == openflow.instanceStatus.Running) {
                    if(showed == false) {
                        var title = showCellInfo.call(this,p);
                        //更新节点信息
                        this.nodeExpendPannel.title(title);
                        showed= true;
                        continue;
                    }
                    else {
                         this.nodeInfo.nextButton.attr('data-id',p.Id);
                            this.nodeInfo.nextButton.show();
                            break;
                    }
                }
            }
            if(showed == false) this.nodeExpendPannel.hide();
            else this.nodeExpendPannel.show();
        }
        this.propertyPanel.appendTo(this.body);
    }

    /**
    * 生成属性输入框
    */
    __contructor.prototype.createPanel = function (target) {
        //this.initValues();
        if (this.cellPanel) this.cellPanel.remove();
        if (this.propertyPanel && openflow.editable) this.propertyPanel.remove();

        //组合标题为：名称(ID,UUID)
        var title = '';
        if(openflow.currentInstance && openflow.currentInstance.data.Id) {
            title = openflow.currentInstance.data.Name || '';
            title += '(' + openflow.currentInstance.data.Id;
            if(openflow.currentInstance.data.Uuid) title += ',' + openflow.currentInstance.data.Uuid;
            title += ')';
        }
        else if(this.editor.data) {
            title = this.editor.data.Name || '';
            title += '(' + this.editor.data.Id;
            title += ')';
        }
        var _this = this;
        this.title(title);

        if (this.editor) {
            if(openflow.flag==3) { //为实例运行时状态
                createInstancePropertyPanel.call(this, this.editor,target);
               }
            else {
                //如果是节点的属性
                if (jmUtils.isType(target,jmCell) || jmUtils.isType(target,jmConnectLine)) {
                    createCellPropertyPanel.call(this, target);
                }
                else {
                    createFlowPropertyPanel.call(this, this.editor,target);
                }
            }
        }

        /*
            //表示对象为当前流程，则显示当前流程属性框
        else if (this.editor == target) {
            createFlowPropertyPanel.call(this, target);
        }  */
    }

    /**
    * 收集所有属性的值
    */
    __contructor.prototype.initValues = function () {
        if (openflow.flag == 3) return;//运行模式下不处理
        //收集节点属性
        if (this.target && this.target.style &&
            (jmUtils.isType(this.target,jmCell) || jmUtils.isType(this.target,jmConnectLine))) {
            var style= typeof this.target.styleName == 'string'?this.editor.subStyle(this.target.styleName):'base';
            var properties = this.cellProperties[this.target.id] || (this.cellProperties[this.target.id] = {});
            //获取当前节点类型的属性
            var ps = this.cellPropertyMap[style] || this.cellPropertyMap['base'];
            var self =  this;
            this.body.find('input,textarea,select,div[data-code="shell"]').each(function (i, p) {
                var _this = self;
                var name = $(this).attr('data-name');
                var value = $.trim($(this).val());
                if(this.type == 'checkbox') {
                    value = $(this).is(':checked');
                }
                if (name) {
                    //if (this.editor && this.editor.save) this.editor.save();
                    if ($(this).attr('data-code')) {
                        properties[name] = value || this.outerText;
                    }
                    else if ($(this).attr('instance')) {
                        properties[name] = $(this).attr('instance');
                    }
                    else if(name == 'error_condition') {
                            var checkbox = $(this).parent().find('input[type="checkbox"]');
                            if(checkbox.is(':checked')) {
                                properties[name] = value+ '%';
                            }
                            else {
                                properties[name] = value
                            }
                        }
                    /*else if ($(this).attr('data-nodes')) {
                        var index = $(this).attr('data-nodes');
                        var nodesps = properties['nodes'];
                        if(!nodesps || !jmUtils.isArray(nodesps)) nodesps=properties['nodes']=[];//初始化nodes数组
                        var nodep = nodesps[index] || (nodesps[index]={});
                        nodep[name] = value;
                    }*/
                    else {
                        properties[name] = value;
                        if(name == 'name') {
                            _this.target.value(value);
                            _this.editor.graph.refresh();
                        }
                    }
                }
            });
        }
        //收集流程属性
        if (this.propertyPanel) {
            this.properties = {};
            for (var i in this.propertyPanel.inputs) {
                var input = this.propertyPanel.inputs[i];
                var name = $.trim(input.name.val());
                if (input.name && name) {
                    this.properties[name] = {
                        'value': input.value.val(),
                        'description':input.des.val()
                    };
                }
            }
        }
    }
    return new __contructor();
})();
