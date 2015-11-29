
openflow = openflow || {};
/**
* openflow工作流编辑器
*/
openflow.editor = (function () {
    function __contructor(option) {
        option = option || {};
        this.option = option;
        this.container = option.container;
        if (!this.container) {
            this.container = document.createElement('div');
            window.document.body.appendChild(this.container);
        }
        //初始化编辑器
        this.init();
    }

    /**
    * 初始化编辑器
    */
    __contructor.prototype.init= function()  {
        if (this.option.style) {
            openflow.css(this.container, this.option.style);
        }

        this.editor = new jmEditor(this.option);
        this.graph = this.editor.graph;

        var self = this;
        //检查元素是否可以添加
        this.editor.validCell = function(cell) {
            var style = self.subStyle(cell.styleName);
            var cells = this.getCells();
            for(var i=0;i< cells.length;i++) {
                var c = cells[i];
                var cstyle = self.subStyle(c.styleName);
                if(style == 'start' && cstyle == 'start') {
                    openflow.message.warning('开始元素有且只能有一个');
                    return false;
                }
                else if(style == 'end' && cstyle == 'end') {
                    openflow.message.warning('结束元素有且只能有一个');
                    return false;
                }
            }
            return true;
        };
        //检查元素是否可以相连
        this.editor.validConnect = function(from,to) {
            var fromstyle = self.subStyle(from.styleName);
            if(fromstyle == 'start') {
                if(from.connects.count() > 0) {
                    openflow.message.warning('开始元素有且只能有一个出线');
                    return false;
                }
            }
            else if(fromstyle == 'join' || fromstyle.indexOf('process') >= 0) {
                //如果fork已存在出口
                if(from.connects.count(function(con) {
                        return con.from == from;
                    }) > 0) {
                    openflow.message.warning(fromstyle + '元素有且只能有一个出线');
                    return false;
                }
            }
            return true;
        };
    }

    /**
    * 重新计算画布大小
    */
    __contructor.prototype.resize = function () {
        return false;//暂时作废
        var cells = this.graph.getModel().cells;
        var h = $(this.container).height();
        for (var i in cells) {
            var geo = this.graph.getModel().getGeometry(cells[i]);
            if (geo) {
                var th = geo.y + geo.height;
                if (th > h) h = th + 10;
            }
        }
        $(this.container).height(h);
    }

    /**
    * 小图标状态映射
    */
     __contructor.prototype.imgStateMapping={
        none:'',
        loading:'/content/primitive/state/loading.gif',
        init:'/content/primitive/state/init.gif',
        running:'/content/primitive/state/running.gif',
        suspending:'/content/primitive/state/suspending.gif',
        stopped:'/content/primitive/state/stopped.gif',
        stopping:'/content/primitive/state/stopping.gif',
        error:'/content/primitive/state/error.gif',
        success:'/content/primitive/state/success.gif'
     },

    /**
    * 设置元素的状态标识
    *
    */
    __contructor.prototype.setOverlay=function(cell,img,tooltip,click,a,b,c,d) {
        if (cell != null) {
            var size = 16;

            if(img && this.imgStateMapping[img]){
                //更新节点对应的样式状态
                var style = this.subStyle(cell.styleName);
                this.setCellStyle(cell,style + '-' + img);
                img=this.imgStateMapping[img];
            }
            cell.setOverlay(img,size,size,tooltip);
        }
    }

    /**
    * 通过ID设置节点状态
    */
    __contructor.prototype.setOverlayById = function (id, img, tooltip, click) {
        var cell = this.editor.getCell(id);
        this.setOverlay(cell, img, tooltip, click);
    }

    /**
    * 设置所有节点的状态
    */
    __contructor.prototype.setAllCellOverlay = function (img, tooltip) {
        var cells = this.editor.cells;
        var self = this;
        cells.each(function(i,cell) {
            if (cell && (cell.styleName.indexOf('process') >= 0 || cell.styleName.indexOf('subflow') >= 0)) {
                self.setOverlay(cell, img, tooltip);
            }
        });
    }

    /**
    * 初始化所有节点状态
    */
    __contructor.prototype.initAllCellOverlay = function () {
        this.setAllCellOverlay('/content/primitive/state/loading.gif', '加载状态中...');
    }

    /**
    * 移动元素
    */
    __contructor.prototype.moveCell = function (cell, dx, dy) {
        this.editor.moveCells(cell,dx,dy);
    }

    /**
    * 移动所有选择的元素
    */
    __contructor.prototype.moveSelectionCells=function(dx, dy) {
        var cells = this.editor.getSelectedCells();
        this.editor.moveCells(cells, dx, dy);
    }

    /**
    * 改变节点的样式并刷新
    */
    __contructor.prototype.setCellStyle=function(cell,style){
        if(typeof cell === 'string' || typeof cell === 'number') cell = this.editor.getCell(cell);
        cell.setStyle(style);
        this.graph.refresh();
    }

    /**
    * 绑定元素选择事件,只有当选择单个元素时，才响应事件
    */
    __contructor.prototype.setSelectionChangeEvent = function (change) {

        var self = this;
        /**
        * 监听元素选择事件
        */
        this.editor.on('select',function(cell,sel) {
            var style = cell.styleName;
            var _this = self;
            //如果被选择，则改变样式
            if(sel) {
                var cells = _this.editor.getSelectedCells();//获取所有选择的元素
                if (cells && cells.length == 1) {
                    switch (cell.styleName) {
                        case 'process': {
                            _this.setCellStyle(cell,'process-selected');
                            break;
                        }
                        case 'shell_process': {
                            _this.setCellStyle(cell,'shell_process-selected');
                            break;
                        }
                        case 'switch': {
                            _this.setCellStyle(cell, 'switch-selected');
                            break;
                        }
                        case 'subflow': {
                            _this.setCellStyle(cell,'subflow-selected');
                            break;
                        }
                    }
                    change(cell);
                    return;
                }
                else {
                    //获取所有已选择的连线
                    var lines = _this.editor.getSelectedConnects();
                    //如果当前只选中一条线
                    if(lines.length == 1) {
                        change(cell);
                        return;
                    }
                }
            }
            else {
                if(style && (style.indexOf('-selected') > 0 || style.indexOf('_selected') > 0)) {
                    style = style.replace('-selected','').replace('_selected','');
                    cell.setStyle(style);
                }
            }
            change(null);
        });
    }

    /**
    * 初始化工具栏
    */
    __contructor.prototype.initToolbar = function(param,callback) {
        //图元
        var cellImages= {
            start:openflow.root + 'content/primitive/start.png',
            shell_process:openflow.root + 'content/primitive/shell.png',
            local_process:openflow.root + 'content/primitive/local.png',
            install_process:openflow.root + 'content/primitive/install.png',
            subflow:openflow.root + 'content/primitive/subflow.png',
            upload:openflow.root + 'content/primitive/upload.png',
            download:openflow.root + 'content/primitive/download.png',
            'switch':openflow.root + 'content/primitive/switch.png',
            'fork':openflow.root + 'content/primitive/fork.png',
            'join':openflow.root + 'content/primitive/join.png',
            'end':openflow.root + 'content/primitive/end.png'
        };
        var style = {
            //stroke:'rgb(138,212,222)',
            //fill : '#3BBCAA',
            lineWidth:2,
            close:true,
            radius:6
        };
        //style.shadow = new jmShadow(0,0,10,'#0FA248');
        //开始
        //字体样式
        style.fontStyle = {
            fill:'#000',
            textAlign:'center',
            'textBaseline' : 'middle',
            font:'12px 微软雅黑'
        };
        style.connectLine = {
            stroke:'RGB(0, 60, 120)',
            normalStroke:'RGB(40, 80, 160)',
            overStroke:'red',
            selectedStroke:'red',
            lineWidth:1,
            fontStyle:{
                fill:'#000',
                textAlign:'center',
                'textBaseline' : 'middle',
                font:'12px 微软雅黑'
            }
        };
        //状态小图标样式
        style.overlay = {
            margin:{
                left:-13,
                top:-13
            }
        };
        style.resizable = false;
        style.shape = 'img';
        style.image = cellImages['start'];
        style.width = style.height = 60;
        this.editor.regStyle('start', style);
        this.editor.regStyle('start-selected', style);
        this.editor.regStyle('start-loading', style);
        this.editor.regStyle('start-init', style);
        this.editor.regStyle('start-running', style);
        this.editor.regStyle('start-suspending', style);
        this.editor.regStyle('start-stopped', style);
        this.editor.regStyle('start-stopping', style);
        this.editor.regStyle('start-error', style);
        this.editor.regStyle('start-success', style);
        //this.editor.regStyle('start-selected', style);

        //流程进程fillColor=#adc5ff;gradientColor=#7d85df;glass=1;rounded=1;
        style = jmUtils.clone(style);
        style.resizable = false;
        style.width = 105;
        style.height = 64;
        style.image = cellImages['shell_process'];
        style.fill = this.graph.createLinearGradient(0,0,0,'100%');
        style.fill.addColorStop(0,'#43F589');
        style.fill.addColorStop(0.5,'#0FA248');
        style.fill.addColorStop(1,'#43F589');

        this.editor.regStyle('shell_process', style);
        this.editor.regStyle('shell_process-loading', style);
        this.editor.regStyle('shell_process-init', style);
        this.editor.regStyle('shell_process-suspending', style);
        this.editor.regStyle('shell_process-stopped', style);
        this.editor.regStyle('shell_process-stopping', style);
        this.editor.regStyle('shell_process-error', style);

        this.editor.regStyle('process', style);
        this.editor.regStyle('process-loading', style);
        this.editor.regStyle('process-init', style);
        this.editor.regStyle('process-suspending', style);
        this.editor.regStyle('process-stopped', style);
        this.editor.regStyle('process-stopping', style);
        this.editor.regStyle('process-error', style);
        // 流程进程成功样式
        this.editor.regStyle('shell_process-success', style);
        this.editor.regStyle('process-success', style);

        // 流程进程正在执行中样式
        style = jmUtils.clone(style);
        this.editor.regStyle('shell_process-running', style);
        this.editor.regStyle('process-running', style);

        // 流程进程选择样式
        var selstyle = jmUtils.clone(style);
        selstyle.stroke = '#EA9012';
        //selstyle.lineWidth = 2;
        selstyle.fill = '#463EE3';
        //selstyle.fontStyle.fill = '#000';
        this.editor.regStyle('shell_process-selected', selstyle);
        this.editor.regStyle('process-selected', selstyle);
        this.editor.regStyle('process_selected', selstyle);

        style = jmUtils.clone(style);
        style.image = cellImages['install_process'];
        this.editor.regStyle('install_process', style);
        this.editor.regStyle('install_process-loading', style);
        this.editor.regStyle('install_process-init', style);
        this.editor.regStyle('install_process-suspending', style);
        this.editor.regStyle('install_process-stopped', style);
        this.editor.regStyle('install_process-stopping', style);
        this.editor.regStyle('install_process-error', style);
        this.editor.regStyle('install_process-running', style);

        style = jmUtils.clone(style);
        style.image = cellImages['local_process'];
        this.editor.regStyle('local_process', style);
        this.editor.regStyle('local_process-loading', style);
        this.editor.regStyle('local_process-init', style);
        this.editor.regStyle('local_process-suspending', style);
        this.editor.regStyle('local_process-stopped', style);
        this.editor.regStyle('local_process-stopping', style);
        this.editor.regStyle('local_process-error', style);
        this.editor.regStyle('local_process-running', style);

        // 子流程
        var subflow_style = jmUtils.clone(style);
        subflow_style.fill = 'rgb(247,190,32)';
        subflow_style.stroke = 'rgb(248,195,60)';
        subflow_style.image = cellImages['subflow'];
        //subflow_style.resizable = true;
        subflow_style.width = 105;
        subflow_style.height = 64;
        // 状态小图标样式
        subflow_style.overlay = {
            margin:{
                left:-4,
                top:-4
            }
        };
        this.editor.regStyle('subflow', subflow_style);
        this.editor.regStyle('subflow-selected', subflow_style);
        this.editor.regStyle('subflow-loading', subflow_style);
        this.editor.regStyle('subflow-init', subflow_style);
        this.editor.regStyle('subflow-running', subflow_style);
        this.editor.regStyle('subflow-suspending', subflow_style);
        this.editor.regStyle('subflow-stopped', subflow_style);
        this.editor.regStyle('subflow-stopping', subflow_style);
        this.editor.regStyle('subflow-error', subflow_style);
        this.editor.regStyle('subflow-success', subflow_style);

        // 上传文件
        var upload_style = jmUtils.clone(style);
        //upload_style.resizable = true;
        upload_style.width = 104;
        upload_style.height = 64;
        upload_style.image = cellImages['upload'];
        this.editor.regStyle('upload_process', upload_style);
        this.editor.regStyle('upload_process-selected', upload_style);
        this.editor.regStyle('upload_process-loading', upload_style);
        this.editor.regStyle('upload_process-init', upload_style);
        this.editor.regStyle('upload_process-running', upload_style);
        this.editor.regStyle('upload_process-suspending', upload_style);
        this.editor.regStyle('upload_process-stopped', upload_style);
        this.editor.regStyle('upload_process-stopping', upload_style);
        this.editor.regStyle('upload_process-error', upload_style);
        this.editor.regStyle('upload_process-success', upload_style);

         // 下载文件
        var download_style = jmUtils.clone(style);
        download_style.image = cellImages['download'];
        this.editor.regStyle('download_process', download_style);
        this.editor.regStyle('download_process-selected', download_style);
        this.editor.regStyle('download_process-loading', download_style);
        this.editor.regStyle('download_process-init', download_style);
        this.editor.regStyle('download_process-running', download_style);
        this.editor.regStyle('download_process-suspending', download_style);
        this.editor.regStyle('download_process-stopped', download_style);
        this.editor.regStyle('download_process-stopping', download_style);
        this.editor.regStyle('download_process-error', download_style);
        this.editor.regStyle('download_process-success', download_style);

        // 条件
        var switch_style = jmUtils.clone(style);
        switch_style.width = 105;
        switch_style.height = 64;
        switch_style.image = cellImages['switch'];
        // 状态小图标样式
        switch_style.overlay = {
            margin:{
                left:-20,
                top:-20
            }
        };
        this.editor.regStyle('switch', switch_style);
        this.editor.regStyle('switch-loading', switch_style);
        this.editor.regStyle('switch-init', switch_style);
        this.editor.regStyle('switch-running', switch_style);
        this.editor.regStyle('switch-suspending', switch_style);
        this.editor.regStyle('switch-stopped', switch_style);
        this.editor.regStyle('switch-stopping', switch_style);
        this.editor.regStyle('switch-error', switch_style);
        this.editor.regStyle('switch-success', switch_style);
        this.editor.regStyle('switch-selected', switch_style);

        // 并行组件
        var fork_style = jmUtils.clone(style);
        fork_style.resizable = false;
        fork_style.width = 105;
        fork_style.height = 24;
        fork_style.image = cellImages['fork'];
        this.editor.regStyle('fork', fork_style);
        this.editor.regStyle('fork-selected', fork_style);
        this.editor.regStyle('fork-loading', fork_style);
        this.editor.regStyle('fork-init', fork_style);
        this.editor.regStyle('fork-running', fork_style);
        this.editor.regStyle('fork-suspending', fork_style);
        this.editor.regStyle('fork-stopped', fork_style);
        this.editor.regStyle('fork-stopping', fork_style);
        this.editor.regStyle('fork-error', fork_style);
        this.editor.regStyle('fork-success', fork_style);

        // 流程汇集
        var join_style = jmUtils.clone(style);
        join_style.width = 105;
        join_style.height = 24;
        join_style.image = cellImages['join'];
        this.editor.regStyle('join', join_style);
        this.editor.regStyle('join-selected', join_style);
        this.editor.regStyle('join-loading', join_style);
        this.editor.regStyle('join-init', join_style);
        this.editor.regStyle('join-running', join_style);
        this.editor.regStyle('join-suspending', join_style);
        this.editor.regStyle('join-stopped', join_style);
        this.editor.regStyle('join-stopping', join_style);
        this.editor.regStyle('join-error', join_style);
        this.editor.regStyle('join-success', join_style);

        // 结束
        var end_style = jmUtils.clone(style);
        end_style.close = true;
        end_style.lineWidth = 1;
        end_style.image = cellImages['end'];
        end_style.width = 65;
        end_style.height = 65;
        end_style.connectable = false;
        this.editor.regStyle('end', end_style);
        this.editor.regStyle('end-selected', end_style);
        this.editor.regStyle('end-loading', end_style);
        this.editor.regStyle('end-init', end_style);
        this.editor.regStyle('end-running', end_style);
        this.editor.regStyle('end-suspending', end_style);
        this.editor.regStyle('end-stopped', end_style);
        this.editor.regStyle('end-stopping', end_style);
        this.editor.regStyle('end-error', end_style);
        this.editor.regStyle('end-success', end_style);


        if (openflow.editable && this.editor.isEnabled()) {

            //初始化工具栏
            this.toolbar.init(param);

            //流程开始
            this.toolbar.add(openflow.root + 'content/primitive/icon/start.png', 'start', '开始', '开始');

            //流程状态
            this.toolbar.add(openflow.root + 'content/primitive/icon/shell.png', 'shell_process','shell','shell');
            this.toolbar.add(openflow.root + 'content/primitive/icon/local.png', 'local_process','本地执行','local_process');
            //过程
            this.toolbar.add(openflow.root + 'content/primitive/icon/subflow.png', 'subflow','子流程','子流程');
            //条件判断
            this.toolbar.add(openflow.root + 'content/primitive/icon/switch.png', 'switch','条件','');
            //上传文件
            this.toolbar.add(openflow.root + 'content/primitive/icon/upload.png', 'upload_process','上传','上传');
            //下载文件
            this.toolbar.add(openflow.root + 'content/primitive/icon/download.png', 'download_process','下载','下载');

            //流程并发
            this.toolbar.add(openflow.root + 'content/primitive/icon/fork.png', 'fork','并行','');
            //流程同步
            this.toolbar.add(openflow.root + 'content/primitive/icon/join.png', 'join', '汇集','');

             //安装流程
            this.toolbar.add(openflow.root + 'content/primitive/icon/install.png', 'install_process','安装','install');

            //流程结束
            this.toolbar.add(openflow.root + 'content/primitive/icon/end.png', 'end', '结束', '结束');

            //this.toolbar.loadMyComponents(style);// 加载自定义组件

        }
        if (param.showTools !== false) {
            //显示工具箱
            this.showTools();
        }
        if (param.showOutLine !== false) {
            //显示视图
            //this.outline = this.showOutLine();
        }

        //预加载图片
        var win = openflow.message.showWaiting('加载图元中...');
        jmUtils.loadImg(cellImages,function() {
            if(callback) callback();
            win.close();
        });
    }

    /**
    * 添加自定义形状
    */
    __contructor.prototype.addShape = function (name,shape) {
        //注册到当前编辑器中
        this.graph.registerShape(name, shap);
    }

    /**
    * 载入json
    */
    __contructor.prototype.read = function (json) {
        if (this.property && this.property.clear) this.property.clear();
        //try {
            //json = decodeURIComponent(json);
        //}
        //catch(e) {}
        //如果是原xml格式的，则转为当前json格式
        if(json && json.indexOf('<mxGraphModel>') >= 0) {
            json = mxXmlTojmJSONAdaptor(json);//使用mx格式适配成当前结构
        }
        this.editor.fromJSON(json);
    }

    /**
    * 把当前图转为XML格式文档
    */
    __contructor.prototype.toXml = function () {
        this.property.initValues();//收集属性值
        return this.editor.toJSON().toString();
    }

    /**
    * 通过样式获取元素
    */
    __contructor.prototype.getCellByStyle = function (style) {
        var self = this;
        var cell = this.editor.cells.get(function(c) {
            return self.subStyle(c.styleName) == style;
        });
        return cell;
    }

    /**
    * 处理样式名称。去除后缀
    */
    __contructor.prototype.subStyle = function(style) {
        var extIndex = style.indexOf('-');
        if(extIndex > 0) {
            style = style.substring(0,extIndex);
        }
        return style;
    }

    /**
    * 节点XML标识下级节点的名称
    */
    __contructor.prototype.nextXmlName = {
        'start': 'to',
        'shell_process': 'next',
        'install_process':'next',
        'download_process': 'next',
        'upload_process': 'next',
        'process': 'next',
        'subflow':'next',
        'switch': 'to',
        'fork': 'to',
        'join':'next'
    }

    /**
    * 转为模板XML格式
    */
    __contructor.prototype.toTemplateXml = function (xml,showtip,isdraft) {
        var start = this.getCellByStyle('start');//获取开始元素
        if (start == undefined && !isdraft) {
            if(showtip) openflow.message.error("找不到开始节点");
            return '';
        }
        //xml = $(xml || this.toXml());
        //var cells = xml.find('mxCell[parent=1]');

        this.property.initValues();//收集属性值
        var xmlcontent = '<?xml version="1.0" encoding="utf-8" ?>\n' +
        '<template>\n' +
           '\t<name>\n' +
                '<![CDATA[' + this.data.Name + ']]>\n' +
            '\t</name>\n\n' +
            '\t<description>\n' +
                '<![CDATA[' + this.data.Description + ']]>\n' +
            '\t</description>\n\n' +

            '\t<properties>\n';

        if (this.property.properties) {
            for (var k in this.property.properties) {
                xmlcontent += '\t\t<property name="' + openflow.formatXmlValue(k) + '" value="' +
                    openflow.formatXmlValue(this.property.properties[k].value) +
                    '" description="'+ openflow.formatXmlValue(this.property.properties[k].description) +
                    '" default=""/>\n';
            }
        }
        xmlcontent += '\t</properties>\n\n';

        var self = this;
        var lst = {};//记录已处理过的节点

        //把节点转为xml文档
        function convertXml(cell) {
            if (!cell || !cell.id || lst[cell.id]) return;//如果已处理过。则不生成
            lst[cell.id] = true;
            var _this = self;
            var style = cell.styleName;
            if(!style) return;
            //先去除所有后缀，主要是以下划线分隔的状态style
            style = _this.subStyle(style);
            if(style == 'process') style= 'shell_process';

            xmlcontent += '\t<' + style + ' id="' + cell.id + '" ';

            var ps = _this.property.getCellProperties(cell.id);
            if (!ps['name'] || ps['name'] == '') ps['name'] = cell.value();

            var targets=[];
            if (style != 'switch' && style != 'fork') {

                if (cell.connects && cell.connects.count() > 0) {
                    cell.connects.each(function(i,c) {
                        var target = c.to;
                        if (target && target.id != cell.id) {
                            xmlcontent += (_this.nextXmlName[style] || 'to') + '="' + target.id + '"';
                            targets.push(target);
                        }
                    });
                    if(targets.length > 1) {
                        throw style + '只能有一个目标';
                    }
                }

                    var branches = 0;
                    cell.connects.each(function(i,c) {
                        var target = c.to;
                        //计算进入分支个数
                        if (target && target.id == cell.id) {
                            branches++;
                        }
                    });
                if (style == 'join') {
                    xmlcontent += ' branches="' + branches + '" ';
                }
                //else if(branches > 1) {
                    //throw style + '只能有一个进入连线';
                //}
                //子流程
                else if(style == 'subflow') {
                    var instanceid = ps['instance'];
                    if(!instanceid) {
                        throw '子流程必须指定实例';
                    }
                    xmlcontent += ' instance="'+instanceid+'"';
                }
            }
            xmlcontent += '>\n';

            var addedkeys = {};
            for (var i in ps) {
                if (!i || addedkeys[i] || i == 'nodes.remote') continue;
                var p= ps[i];
                if(typeof p === 'function') continue;

                addedkeys[i] = 1;//标识为已处理过的key
                if(i.indexOf('.') > 0) {
                    //暂时写死支持二层健值
                    var keys = i.split('.');
                    xmlcontent += '\t\t<' + keys[0] + '>\n';

                    xmlcontent += '\t\t\t\t<' + keys[1] + '>\n';
                    xmlcontent += '\t\t\t\t\t<![CDATA[' + p + ']]>\n';
                    xmlcontent += '\t\t\t\t</' + keys[1] + '>\n';

                    //处理同级下面的属性加到一起
                    for(var j in ps) {
                        if (!j || addedkeys[j] || i.indexOf('.') < 0) continue;
                        var keys2 = j.split('.');
                        //父节点相同的一块处理
                        if(keys2[0] == keys[0]) {
                            xmlcontent += '\t\t\t\t<' + keys2[1] + '>\n';
                            xmlcontent += '\t\t\t\t\t<![CDATA[' + ps[j] + ']]>\n';
                            xmlcontent += '\t\t\t\t</' + keys2[1] + '>\n';
                            addedkeys[j] = 1;//记录为已处理过的
                        }
                    }

                    xmlcontent += '\t\t</' + keys[0] + '>\n';
                }
                else {
                    if(i=='nodes') {
                        if(ps['nodes.remote']) {
                            xmlcontent += '\t\t<' + i + ' remote="true">\n';
                        }
                       else {
                            xmlcontent += '\t\t<' + i + ' remote="false">\n';
                       }
                    }
                    else {
                        xmlcontent += '\t\t<' + i + '>\n';
                    }
                    xmlcontent += '\t\t\t<![CDATA[' + p + ']]>\n';
                    xmlcontent += '\t\t</' + i + '>\n';
                }
            }

            //并行和条件
            if (style === 'switch' || style === 'fork') {
                if (cell.connects && cell.connects.count() > 0) {
                    if (style == 'switch') {
                        xmlcontent += '\n\t\t<next>\n';
                        var defaultcount = 0;
                        cell.connects.each(function(i,c) {
                            var target = c.to;
                            if (target) {
                                if(target.id != cell.id) {
                                    var value = c.value();
                                    if (value) {
                                        xmlcontent += '\t\t\t<case to="' + target.id + '" where="' + openflow.formatXmlValue(value) + '"/>\n';
                                    }
                                    else {
                                        xmlcontent += '\t\t\t<default to="' + target.id + '" />\n';
                                        defaultcount++;
                                    }
                                    targets.push(target);
                                }
                            }
                        });
                        if (defaultcount == 0) {
                            throw "switch必须包含一个默认的分支";
                        }
                        else if (defaultcount > 1) {
                            throw "switch只能包含一个默认的分支";
                        }
                        xmlcontent += '\t\t</next>\n';
                    }
                    else if (style == 'fork') {
                        xmlcontent += '\n\t\t<branches>\n';
                        cell.connects.each(function(i,c) {
                            var target = c.to;
                            if (target) {
                                if(target.id != cell.id) {
                                    xmlcontent += '\t\t\t<next to="' + target.id + '" />\n';
                                    targets.push(target);
                                }
                            }
                        });
                        xmlcontent += '\t\t</branches>\n';
                    }
                }
            }

            xmlcontent += '\t</' + style + '>\n\n';

            for (var i in targets) {
                convertXml(targets[i]);//递归
            }
        }

        convertXml(start);//从start开始
        //如果为草稿则处理所有节点
        if(isdraft) {
            var cells = this.editor.cells;
            for(var i in cells) {
                if(cells[i].vertex == 1) {
                    convertXml(cells[i]);
                }
            }
        }
        xmlcontent += '</template>';
        return xmlcontent;
    }

    /**
    * 编辑当前模板信息
    */
    __contructor.prototype.edit = function (callback) {
        var _this = this;
        if (!this.editWin) {
            var editcontent = this.editWin = $('<div />');
            var winbody = $('<ul class="edit-container" />').appendTo(editcontent);

            var lblname = $('<span for="txtname"></span>');
            lblname.html('名称');
            var txtname = $('<input type="text" maxlength="32" name="txtname" />');
            var namediv = $('<li></li>');
            var lbldes = $('<span for="txtdes"></span>');
            lbldes.html('描述');
            var txtdes = $('<textarea name="txtdes" type="text" maxlength="256"/>');
            var desdiv = $('<li></li>');
            var lblcategory = $('<span for="txtcategory">类别</span>');
            var txtcategory = $('<input type="text" readonly maxlength="16" name="txtcategory" />');
            var lnkSelect = $('<a href="#">...</a>');
            var selectCategory = function() {
                var left = txtcategory.offset().left;
                var top = txtcategory.offset().top;
                openflow.category.selecter.show(left,top,function(category) {
                    if(!category) return;
                    txtcategory.val(category.Name);
                    txtcategory.attr('data-id',category.Id);
                });
            };

            lnkSelect.click(selectCategory);
            txtcategory.bind('dblclick',selectCategory);
            var categorydiv = $('<li></li>');
            lblname.appendTo(namediv);
            txtname.appendTo(namediv);
            namediv.appendTo(winbody);
            lbldes.appendTo(desdiv);
            txtdes.appendTo(desdiv);
            desdiv.appendTo(winbody);
            lblcategory.appendTo(categorydiv);
            txtcategory.appendTo(categorydiv);
            lnkSelect.appendTo(categorydiv);
            categorydiv.appendTo(winbody);

            txtname.bind('blur',function() {
                if(!txtdes.val()) {
                    txtdes.val($(this).val());
                }
            });

            this.editWin.txtname = txtname;
            this.editWin.txtdes = txtdes;

        }
        this.editWin.txtname.val(this.data.Name || '');
        this.editWin.txtdes.val(this.data.Description || '');
        if(this.data.CategoryId) {
            txtcategory.val(this.data.CategoryName);
            txtcategory.attr('data-id',this.data.CategoryId);
        }
        else {
            openflow.category.selecter.get(this.data.CategoryId,function(category) {
                if(category) {
                    txtcategory.val(category.Name);
                    txtcategory.attr('data-id',category.Id);
                }
            });
        }
        this.editWin.dialog({
                title:openflow.flag == 0?'模板信息':'组件信息',
                closable:true,
                minimizable:false,
                modal:true,
                iconCls:'icon-edit',
                width:400,
                height:250,
                buttons: [{
                    text: '确定', handler: function () {
                        var txtname = _this.editWin.find('input[type=text][name=txtname]');
                        var nv = txtname.val();
                        if (nv == '') {
                            openflow.message.warning('名称不可为空');
                            txtname.focus();
                            return;
                        }
                        var txtdes = _this.editWin.find('textarea[name=txtdes]');
                        nv = txtdes.val();
                        if (nv == '') {
                            openflow.message.warning('描述不可为空');
                            txtdes.focus();
                            return;
                        }
                        var txtcategory = _this.editWin.find('input[type=text][name=txtcategory]');
                        _this.name(txtname.val());
                        _this.data.Description = txtdes.val();
                        _this.data.CategoryId = txtcategory.attr('data-id');
                        _this.editWin.dialog('close');
                        //执行回调
                        if (callback) callback.call(_this);
                    }
                }, {
                    text: '取消', handler: function () {
                        _this.editWin.dialog('close');
                    }
                }]
            });
    }

    /**
    * 保存当前模板
    */
    __contructor.prototype.save = function (isdraft, callback) {
        try
        {
            if(typeof isdraft == 'function') {
                callback=isdraft;
                isdraft= false;
            }
            var xml = this.toXml();
            var template = this.toTemplateXml(xml,!isdraft,isdraft);
            if (template == undefined) {
                if(!isdraft) openflow.message.warning('请正确编辑流程');
                else {
                    callback();
                }
                return;
            }
            var name = this.data ? this.data.Name : null;
            var des = this.data ? this.data.Name : null;
            if (!isdraft && (!name || !des)) {
                openflow.message.warning('名称和描述不可为空');
                this.edit(this.save);
                return;
            }

            var data = { EditorXml: xml, TemplateXml: template };
            data.Name = name;
            data.CategoryId = this.data.CategoryId;
            data.Description = des;
            //data.Image =  this.editor.toImage();//获取图像

            if(openflow.flag == 4) {
                data.url= '/flows/template?cmd=' + (this.data && this.data.Id?'update':'insert');
                if(this.data && this.data.Id) data.Id = this.data.Id;
            }
            else {
                var cmd= 'updatetemplate';
                if (this.data && this.data.Id) {
                    data.Id = this.data.Id;
                }
                else {
                    cmd = "inserttemplate";
                }
                data.url= '/flows/templateServer.js?cmd=' + cmd;
            }

            //保存草稿
            if(isdraft) {
                cmd='savedraft';
                data.LastUpdateUser = openflow.user;
            }
            else {
                var tip = openflow.message.tip('保存中,请稍候...');
            }


            var _this= this;
            //请求保存
            openflow.request('post', data, function (r) {
                if(tip) tip.hide();
                if (r && Number(r) > 0) {
                    // if (!data.Id && !isdraft) {
                    if (!data.Id) {
                        data.Id = Number(r);
                        window.location = openflow.checkUrl('/flows?mode='+openflow.flag+'&id=' + data.Id);
                    }
                    if(!isdraft) {
                        _this.data = data;
                        _this.isChanged = false;//标识为未修改
                        openflow.message.success('保存成功');
                    }
                }
                if (callback) callback(r);
            });
        }
        catch (e) {
            if(!isdraft) openflow.message.error(e.toString());
        }
    }

    /**
    * 当前流程名称
    */
    __contructor.prototype.name = function (n) {
        if (n) {
            if (this.data) {
                this.data.Name = n;
                /*if (!this.title) {
                    this.title = $('<div class="flow-title"></div>').appendTo('body');
                }
                this.title.html(n);*/
                document.title= n;
            }
            //如果有属性框则改变其标题
            if(this.property) {
                this.property.title(n);
            }
        }
        else {
            if (this.data) return this.data.Name;
        }
    }

    /**
    * 修改权限对话框
    */
    __contructor.prototype.updatePermission = function() {
        var data = openflow.currentInstance?openflow.currentInstance.data:this.data;
        if(data.CreateUser != openflow.user) {
            openflow.message.warning('无权修改');
            return;
        }
        var container = $('<div class="updatePermission"></div>');
        //var id = openflow.currentInstance?openflow.currentInstance.data.Id:this.data.Id;
        var url = openflow.root + (openflow.flag == 0?'servers/templateServer.js?cmd=updatepermission':'servers/instanceServer.js?cmd=updatepermission');

        //var manager = openflow.currentInstance?openflow.currentInstance.data.Manager:this.data.Manager;
        //var reader = openflow.currentInstance?openflow.currentInstance.data.Reader:this.data.Reader;
        var label1 = $('<label>管理权限<span>(RTX英文名，以分号分隔)</span></label>').appendTo(container);
        var managerInput = $('<textarea ></textarea>').appendTo(container).val(data.Manager);
        var label2 = $('<label>只读权限<span>(RTX英文名，以分号分隔)</span></label>').appendTo(container);
        var readerInput = $('<textarea ></textarea>').appendTo(container).val(data.Reader);

        container.dialog({
            width: 500,
            height: 320,
            modal:true,
            buttons:[{
                text:'确定',
                handler:function() {
                    var manager = managerInput.val();
                    var reader = readerInput.val();
                    openflow.request('post',{
                        url:url,
                        manager:manager,
                        reader:reader,
                        id:data.Id
                    },function(r) {
                        if(r) {
                            openflow.message.success('修改成功');
                            data.Manager = manager;
                            data.Reader = reader;
                            container.dialog('close');
                        }
                        else {
                            openflow.message.success('修改失败');
                        }
                    });
                }
            },{
                text:'取消',
                handler:function() {
                    container.dialog('close');
                }
            }],
            title:'权限修改（以分号分隔的RTX名）'
        });
    }
    return __contructor;
})();

/**
 * 将原有的mxgraph文档格式适配成当前使用的格式
 */
function mxXmlTojmJSONAdaptor(xml) {
    var obj = jmUtils.xmlToJSON(xml);
    var root = obj.children[0].children[0];
    var json = {cells:[]};
    var allcells = {};
    for(var i in root.children) {
        var n = root.children[i];
        if(n.attributes['vertex'] == '1') {
            var id = n.attributes['id'];
            var cell = allcells[id] || (allcells[id] = {
                id : id,
                outs : []
            });
            cell.style = n.attributes['style'];
            cell.value = n.attributes['value'];
            for(var j in n.children) {
                var cn = n.children[j];
                if(cn.name == 'mxGeometry') {
                    cell.position = {x:Number(cn.attributes['x']),y:Number(cn.attributes['y'])};
                    cell.width = Number(cn.attributes['width']);
                    //cell.height = Number(cn.attributes['height']);
                    break;
                }
            }
            json.cells.push(cell);
        }
        else if(n.attributes['edge'] == '1') {
            var c = {};
            c.id = n.attributes['id'];
            c.from = n.attributes['source'];
            c.to = n.attributes['target'];
            c.value = n.attributes['value'];
            if(c.from) {
                var fromCell = null;
                for(var j in json.cells) {
                    var cell = json.cells[j];
                    if(cell.id == c.from) {
                        fromCell = cell;
                        break;
                    }
                }
                if(!fromCell) {
                    fromCell = {id:c.from,outs:[]};
                    allcells[fromCell.id] = fromCell;
                }
                fromCell.outs.push(c);
            }
        }
    }
    return json;
}

