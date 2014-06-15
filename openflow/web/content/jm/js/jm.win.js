
/**
* 把元素居中显示
* @method center
* @param {Object} obj 需要居中的对象
* @param {Object} parent 居中对象的容器,不填为window.document
* @for $jm
**/
$jm.center = function (obj, parent) {
    obj = $(obj);
    if (!parent) parent = $(window);
    var bgWidth = parent.width();
    var bgHeight = parent.height();
    var popupHeight = obj.height();
    var popupWidth = obj.width();
    var t = (bgHeight - popupHeight) / 2 + parent.scrollTop();
    var l = (bgWidth - popupWidth) / 2;
    obj.css({
        "top": t > 0 ? t : 0,
        "left": l > 0 ? l : 0
    });
};

/**
* 获取当前浏览览器窗口大小
* 取body跟window大小的最大值
* @for $jm
* @method winSize
* @return Object {w:,h:}
**/
$jm.winSize = function () {
    var win = $(window);
    var body = $(window.document);
    var size = { w: Math.max(win.width(), body.width()), h: Math.max(win.height(), body.height()) };
    return size;
};

/**
* 最大化对象
* @method fullWin
* @param {Object} p 为对象所在容器
* @param {Object} w 为需要最大化的对象
* @param {Number} offsetx 横坐标偏移量
* @param {Number} offsety 纵坐标偏移量
* @for $jm
**/
$jm.fullWin = function (w, p, offsetx, offsety) {
    if (!p) p = window;
    p = $(p);
    w = $(w);
    if (!offsetx) offsetx = 0;
    if (!offsety) offsety = 0;
    w.width(p.width() + offsetx);
    w.height(p.height() + offsety);
};

/**
* 获取所有的缓存窗口
* @method getWins
* @return {Array} 所有窗口数组
**/
$jm.getWins = function () {
    var wins = $jm.cache.get('jm_win_container'); //从缓存中获取窗体
    if (wins == undefined) {
        wins = [];
        $jm.cache.set('jm_win_container', wins);
    }
    return wins;
}

/**
* 缓存窗口
**/
$jm.pushWin = function (win) {
    var tmp = $jm.getWinById(win.id());
    if (!tmp) {
        var wins = $jm.getWins();
        wins.push(win);
    }
}

/**
* 通过ID获取窗口
* @method getWinById
* @param {Number} winid 窗体ID
* @for $jm
**/
$jm.getWinById = function (winid) {
    if (!winid) return;
    var wins = $jm.getWins(); //从缓存中获取窗体
    if (wins) {
        var len = wins.length;
        for (var i = 0; i < len; i++) {
            if (wins[i].id() == winid) return wins[i];
        }
    }
};

/**
* 通过标题查找窗口
* @method getWinByTitle
* @param {String} title 窗口标题
* @for $jm
**/
$jm.getWinByTitle = function (title) {
    if (!title) return;
    var wins = $jm.getWins(); //从缓存中获取窗体
    if (wins) {
        var len = wins.length;
        for (var i = 0; i < len; i++) {
            if (wins[i].title == title) return wins[i];
        }
    }
};

/**
* 移除指定的窗口
* @method _removewin
* @param {Number} winid 窗口ID
* @for $jm
* @private
**/
$jm._removewin = function (winid) {
    var wins = $jm.getWins(); //从缓存中获取窗体
    if (wins) {
        wins = $.grep(wins, function (val, k) {
            return val.id() != winid;
        });
        $jm.cache.set('jm_win_container', wins);
    }
};

/**
* 关闭窗口通过ID
* @method closeWinById
* @param {Number} id 窗口ID
* @for $jm
**/
$jm.closeWinById = function (id) {
    var win = $jm.getWinById(id);
    if (!win) return;
    win.close();
};

/**
* 获取当前最顶层窗口的zindex
* @method get_top_win
* @private
* @for $jm
**/
$jm.get_top_win = function () {
    var topwin;
    var wins = $jm.getWins(); //从缓存中获取窗体
    if (wins) {
        var len = wins.length;
        for (var i = 0; i < len; i++) {
            if (wins[i].winState == $jm.winState.min) continue;
            if (!topwin || wins[i].zindex() > topwin.zindex()) topwin = wins[i];
        }
    }
    return topwin;
};

/**
* 窗口大小枚举,normal: 0, min: 1, max: 2
* @property winState
* @for $jm
**/
$jm.winState = { normal: 0, min: 1, max: 2 };


/**
* 窗体实类
* @class win
* @namespace $jm
* @requires $jm
* @param {Object} params 应用参数,窗体配置信息,
* 比如button 为{ text: '', style: '', className: 'jm_win_button', click: function () { return false; } };
**/
$jm.win = function (params) {
    if (!params) return;

    /**
    * 系统中窗口最大索引,为自增整型
    * @method windowIndex
    * @param {Number} index 可选,设置当前窗口索引
    * @return Number 返回系统中窗口最大索引
    **/
    var windowIndex = function (index) {
        if (index) {
            $jm.cache.set('jm_win_index', index); //获取索引
        }
        else return $jm.cache.get('jm_win_index') || 0; //设置索引
    }
    /**
    * 此窗体的索引,自最大索引自增1
    * @property Index
    * @type Number
    * @private
    **/
    this.Index = windowIndex() + 1;
    //索引递加一
    windowIndex(this.Index);

    //参数初始化

    /**
    * 当前窗口实体
    * @property body
    * @type Object
    * @private
    **/
    this._body = (params.body) ? (typeof (params.body) == "object" ? params.body : $("#" + params.body)) : ""; //body参数可以为对象或标签ID   

    /**
    * 标识为系统窗口对象
    * @property jmwin
    * @type Boolean   
    **/
    this.jmwin = true;

    var headheight = params.headheight || 22;
    var height = params.height;// || 300;
    var width = params.width || 420;
    var position = params.position || 'auto';
    var left = position == 'left' ? 0 : (params.left || '25%');
    var top = position == 'top' ? 0 : (params.top || 80);
    var style = params.style || "";
    var __title = params.title || params.name || '';
    var __id = params.id || 'jm_win_' + this.Index;

    //当前参数
    this.params = params;

    //如果没有锁定边界，则默认锁定不能出父容器
    if (!this.params.bounds) this.params.bounds = { top: true, right: true, bottom: true, left: true };
    else {
        //如果不设置是否锁定，则默认锁定边界
        if (this.params.bounds.top != false) this.params.bounds.top = true;
        if (this.params.bounds.right != false) this.params.bounds.right = true;
        if (this.params.bounds.bottom != false) this.params.bounds.bottom = true;
        if (this.params.bounds.left != false) this.params.bounds.left = true;
    }

    /**
    * 获取窗体ID
    * @method id
    * @return Number 返回当前窗体的ID
    **/
    this.id = function () { return __id; }

    /**
    * 是否为模式窗口
    * @property modal
    * @type Boolean
    **/
    this.modal = params.modal;

    //默认为正常状态
    this.__oldState = this.winState = $jm.isNull(params.state) ? $jm.winState.normal : params.state;

    /**
    * 是否可以改变大小
    * @property resizeable
    * @type Boolean
    **/
    this.resizeable = params.resizeable != false;

    /**
    * 是否可以最大化
    * @property canMax
    * @type Boolean
    **/
    this.canMax = params.canMax != false;

    /**
    * 是否可以最小化
    * @property canMin
    * @type Boolean
    **/
    this.canMin = params.canMin != false;

    /**
    * 窗口关闭回调
    * @event closeCallback    
    **/
    this.closeCallback = params.closeCallback;
    /**
    * 获得焦点回调
    * @event focusCallback    
    **/
    this.focusCallback = params.focusCallback;
    /**
    * 失去焦点回调
    * @event subCallback    
    **/
    this.subCallback = params.subCallback;

    //显示的内容
    this.content = params.content || '';

    /**
    * 窗口主体
    * @property winBody   
    * @type Object 
    **/
    this.winBody = $('<div class="jm_win_body"></div>');

    /**
    * 窗口标题栏
    * @property winHead   
    * @type Object 
    **/
    this.winHead = $('<div class="jm_win_head" jm_winid="' + this.id() + '"></div>');
    //窗口图标，如果没有指定则显示默认的
    this.ico = this.params.ico ? $('<img src="' + this.params.ico + '" class="jm_win_ico"/>') : $('<div class="jm_win_ico"/>');
    this.ico.appendTo(this.winHead);
    /**
    * 窗口标题
    * @property winTitle   
    * @type Object 
    **/
    this.winTitle = $('<span class="jm_win_title" jm_winid="' + this.id() + '"></span>');
    /**
    * 窗口底部
    * @property winBottom   
    * @type Object 
    **/
    this.winBottom = $('<div class="jm_win_bottom" jm_winid="' + this.id() + '"></div>');
    //窗口遮板
    if (params.iframe) {
        this.bodyModel = $('<div class="jm_win_body_model" jm_winid="' + this.id() + '"></div>');
    }
    /**
    * 整个窗口区域
    * @property win   
    * @type Object 
    **/
    this.win = $('<div class="jm_win" id="' + this.id() + '"></div>');

    this.bodyParent = null;

    /**
    * 窗口父容器
    * @property parent   
    * @type Object 
    **/
    this.parent = params.parent ? (typeof (params.parent) == 'string' ? $((params.parent[0] == '#' ? '' : '#') + params.parent) : $(params.parent)) : $(document.body);

    if (this.modal) {
        /**
        * 模式窗口遮板
        * @property fullDiv   
        * @type Object 
        **/
        this.fullDiv = $('<div class="jm_win_full_div"></div>');
    }

    /**
    * 当前层叠
    * @property _zindex   
    * @type Number 
    * @private
    **/
    this._zindex = params.zIndex || 100;

    /**
    * 获取或设置当前层叠
    * @method zindex
    * @param {Number} index 可选,层叠值
    **/
    this.zindex = function (index) {
        if (index) {
            this.win.css('z-index', index + '');
            this._zindex = index;
        }
        else {
            return this._zindex;
        }
    };

    /**
    * 获取或设置当前窗口内容的html
    * @method html
    * @param {String} h 可选,HTML内容
    **/
    this.html = function (h) {
        if (this.winBody) return this.winBody.html(h);
    }

    /**
    * 是否被激活
    * @property __actived   
    * @type Noolean 
    * @private
    **/
    this.__actived = false;

    /**
    * 获取或激活当前窗口
    * @method actived
    * @param {Boolean} v 可选,激活与否
    * @return Boolean 当前是否被激活
    **/
    this.actived = function (v) {
        if (v == true || v == false) {
            this.__actived = v;
            this.__showModelDiv(!v);
        }
        return this.__actived;
    }

    /**
    * 显示和隐藏遮板
    * @method actived
    * @param {Boolean} v 可选,显示与否
    **/
    this.__showModelDiv = function (v) {
        if (this.bodyModel) {
            if (v == true) {
                this.bodyModel.show();
            }
            else {
                this.bodyModel.hide()
            }
        }
    }

    /**
    * 获取或设置当前窗口标题
    * @method title
    * @param {String} t 可选,标题
    * @return String 当前标题
    **/
    this.title = function (t) {
        if (!$jm.isNull(t)) {
            __title = t;
            this.winTitle.html(t);
        }
        return __title;
    }

    /**
    * 获取或设置当前窗口的宽
    * @method width
    * @param {Number} w 可选,宽度
    * @return Number 当前宽度
    **/
    this.width = function (w) {
        if (!$jm.isNull(w)) {
            this.win.width(w);
            //w = this.win.width();
            //this.winBody.width(w - 4);
            this.resize();
        }
        else {
            return this.win.width();
        }
    }

    /**
    * 获取或设置当前窗口的高
    * @method height
    * @param {Number} h 可选,高度
    * @return Number 当前高度
    **/
    this.height = function (h) {
        if (!$jm.isNull(h)) {
            this.win.height(h);
            h = this.win.height();
            //如果有底部操作按钮，则还得除去底部高度
            if (params.button && params.button.length > 0) {
                h -= this.winBottom.height();
            }
            this.winBody.height(h - headheight - 6); //设置里面元素新高度
            this.resize();
        }
        else {
            return this.win.height();
        }
    }

    /**
    * 改变大小事件
    * @event resize   
    **/
    this.resize = function () {
        if (this.resizeCallback) this.resizeCallback();
    }

    /**
    * 获取或设置当前窗口的横坐标
    * @method left
    * @param {Number} l 可选,横坐标
    * @return Number 当前横坐标
    **/
    this.left = function (l) {
        if (!$jm.isNull(l)) {
            //l = l < 0 ? 0 : l;
            this.win.css('left', l);
        }
        else
            return this.win.position().left;
    }
    /**
    * 获取或设置当前窗口的纵坐标
    * @method top
    * @param {Number} l 可选,纵坐标
    * @return Number 当前纵坐标
    **/
    this.top = function (t) {
        if (!$jm.isNull(t)) {
            // t = t < 0 ? 0 : t;
            this.win.css('top', t);
        }
        else
            return this.win.position().top;
    }

    /**
    * 标识当前是否可见
    * @property __visible
    * @type Boolean
    * @private
    **/
    this.__visible = false;

    /**
    * 获取或设置当前窗口是否可见
    * @method visible
    * @param {Boolean} v 可选,是否可见
    * @return Boolean 当前是否可见
    **/
    this.visible = function (v) {
        if (this.__visible && v == false) {
            this.__visible = false;
            this.sub();
            this.hide();
        }
        else if (!this.__visible && v == true) {
            this.win.show();
            this.changeState(this.__oldState); //恢复大小
            this.__visible = true;
        }
        else if (v == true && this.winState == $jm.winState.min) {
            this.changeState(this.__oldState); //恢复大小
            this.__visible = true;
        }
        return this.__visible;
    }

    /**
    * 初始化按钮,加载到窗口底部的
    * @method __initButtons
    * @param {Object} par 按钮参数,比如:{text:'测试',click:function{alert('click');}}
    * @private
    **/
    this.__initButtons = function (par) {
        if (!par) return;
        //窗口按钮初始化        
        var btn = $('<input type="button" class="' +
        (par.className ? par.className : 'jm_win_button') + '" value="' + par.text + '" />');

        var win = this;
        //点击是按钮
        if (par.click) btn.click(function () {
            par.click.call(win);
        });

        btn.appendTo(this.winBottom);
    }

    /**
    * 初始化主体窗口
    * @method __initBody    
    * @private
    **/
    this.__initBody = function () {
        if (style && style != '') this.winBody.attr('class', style);
        if (this.title) {            
            this.winHead.css('height', headheight + 'px');
            this.title(this.title()); //设置标题
            this.winTitle.appendTo(this.winHead); //加入标题
            //功能区域    
            if (this.params.canClose != false) {
                //添加关闭按钮
                var closebtn = $("<a class='cmdbtn close' onmousedown='javascript:if(event.stopPropagation)event.stopPropagation();' onclick='javascript:$jm.closeWinById(\"" + this.id() + "\");return false' href='javascript:;'>X</a>");
                closebtn.appendTo(this.winHead);
            }
            if (this.resizeable && this.canMax) {
                //最大化
                var maxbtn = $("<a class='cmdbtn max' onmousedown='javascript:if(event.stopPropagation)event.stopPropagation();' onclick='javascript:$jm.getWinById(\"" + this.id() + "\").changeState();return false' href='javascript:;'>口</a>");
                maxbtn.appendTo(this.winHead);
            }
            if (this.canMin) {
                //最小化
                var minbtn = $('<a class="cmdbtn min" onmousedown="javascript:if(event.stopPropagation)event.stopPropagation();" onclick="javascript:$jm.getWinById(\'' + this.id() + '\').min();return false;">一</a>');
                minbtn.appendTo(this.winHead);
            }
            this.winHead.appendTo(this.win);
        }
        this.winBody.appendTo(this.win); //加入主体到窗口中
        this.winBody.html('<div class="jm_wait"></div>'); //显示等待
        this.win.css('left', left).css('top', top);
        if (this.params && this.params.overflow) {
            this.winBody.css({ 'overflow': this.params.overflow });
        }
    }

    this.__initBody(); //初始化主体窗口

    /**
    * 加载窗体内容
    * @method loadContent    
    * @private
    **/
    this.loadContent = function () {
        //如果已定义内容加载函数,则直接调用
        if (this.params.load) {
            this.params.load();
            return;
        }
        //如果应用为目录
        if (this.content || this.params.folder) {
            if (this.content != '' && !this.params.folder) {
                this.winBody.html(this.content);
            }

            if (params.resizeCallback) {
                //大小改变回调
                this.resizeCallback = params.resizeCallback;
            }
        }
        //为外部对象
        else if (this._body) {
            //清空
            this.html('');
            this.bodyParent = this._body[0].parentElement;
            this._body.appendTo(this.winBody);
            this._body.show();
        }
        //如果为外链接
        else if (!$jm.isNull(this.params.url)) {
            //如果是用帧显示
            if (this.params.iframe) {
                var fm = $('<iframe frameborder="0" width="100%" height="100%"></iframe>');
                fm.attr('src', this.params.url);
                //清空
                this.html('');
                fm.appendTo(this.winBody);
            }
            else {//否则加载内容
                var winid = this.id(); //当前窗体的ID
                $jm.load(this.params.url, function (html) {
                    var win = $jm.getWinById(winid); //得到窗体
                    if (win) {
                        win.content = $jm.checkUrl(html);
                        win.winBody.html(win.content);
                    }
                });
            }
        }

        //加入遮板
        if (this.bodyModel) this.bodyModel.appendTo(this.winBody);
    }

    /**
    * 窗体手标按下事件
    * @event __winmousedown  
    * @param {Object} evt 事件对象  
    * @private
    **/
    this.__winmousedown = function (evt) {
        var winobj = $jm.getWinById(this.id || evt.id);
        if (winobj) {
            winobj.focus(); //获得焦点
        }
    };

    /**
    * 标题栏双击事件
    * @event __headdblclick  
    * @param {Object} evt 事件对象  
    * @private
    **/
    this.__headdblclick = function () {
        var winid = $(this).attr('jm_winid');
        var win = $jm.getWinById(winid);
        if (win) {
            win.changeState();
        }
    }

    /**
    * 显示当前窗口
    * @method show  
    * @param {Function} callback 显示后回调
    **/
    this.show = function (callback) {
        //如果加载回调返回false.则直接返回
        if (this.params.loading && !this.params.loading(this.id())) {
            return false;
        }

        if (this.visible()) {
            this.focus();
            return;
        }

        //模式窗体
        if (this.modal && this.fullDiv) {
            //加入模式遮板到背景窗口
            this.fullDiv.appendTo(this.parent);
            //设置遮板大小
            this.fullDiv.resize.call(this);
        }

        this.win.appendTo(this.parent);

        //添加按钮
        if (params.button && params.button.length > 0) {
            this.winBottom.html('');
            for (var i in params.button)
                this.__initButtons(params.button[i]);

            this.winBottom.appendTo(this.win);
        }

        this.left(left);        
        this.width(width);
        this.height(height);

        //如果归中
        if (params.position == 'center') $jm.center(this.win, this.parent);
        this.top(top);

        //保存原大小
        this.saveState();

        this.win.unbind('mousedown', this.__winmousedown);
        this.win.bind('mousedown', this.__winmousedown);

        //注册移动事件
        var jmmvdrag = new $jm.objMove();
        jmmvdrag.Register(this);

        //如果可以改变大小
        if (this.resizeable) {
            var jmmddrag = new $jm.objResize();
            jmmddrag.Register(this); //注册大小改变对象

            if (this.canMax) {
                //双击标题.最大化
                this.winHead.unbind('dblclick', this.__headdblclick);
                this.winHead.bind('dblclick', this.__headdblclick);
            }
        }
        else {//改变指针
            this.win.css('cursor', 'default');
        }

        if (this.title && $jm.menu) {
            //菜单项
            var items =
            [
                {
                    text: '最大化',
                    tag: this.id(),
                    disabled: !this.resizeable || !this.canMax, //是否可最大化
                    click: function () {
                        var winid = $(this).attr('tag');
                        var win = $jm.getWinById(winid);
                        if (win) win.max();
                    }
                },
                {
                    text: '最小化',
                    disabled: !this.canMin, //是否可最小化
                    tag: this.id(),
                    click: function () {
                        var winid = $(this).attr('tag');
                        var win = $jm.getWinById(winid);
                        if (win) win.min();
                    }
                },
                {
                    text: '关闭', tag: this.id(), click: function () {
                        var winid = $(this).attr('tag');
                        $jm.closeWinById(winid); //关闭
                    }
                }
            ];
            //绑定标题栏菜单
            //var headmenu = new $jm.menu({ target: this.winHead, items:items});
                this.winHead.menu({ 'items': items });
                this.winTitle.menu({ 'items': items });
                this.ico.menu({ 'items': items });
                
        }

        this.zindex(this.zindex()); //先设置其层级

        //置顶
        this.focus();

        //状态为可见
        this.__visible = true;

        //if ($jm.isNull(this.content) || this.content == '') {
        //通过地址加载内容
        this.loadContent();
        //}

        //调用回调
        if (callback) callback();

        //this.saveState(); //保存初始状态

        //如果已最大化启动
        if (this.winState == $jm.winState.max) {
            setTimeout('var win = $jm.getWinById("' + this.id() + '");if (win)win.max();', 500); //最大化
        }

        //加入到缓存集合中    
        $jm.pushWin(this);
    }

    /**
    * 改变最大化或最小化状态
    * @method changeState  
    * @param {Number} state 为$jm.winState
    **/
    this.changeState = function (state) {
        //if (!this.resizeable) return;
        //如果指定要改变到某状态..否则对其进行反转
        if (state == $jm.winState.normal ||
        ($jm.isNull(state) && this.winState == $jm.winState.max))
            this.normal(); //恢复正常大小
        else if (state == $jm.winState.max ||
        ($jm.isNull(state) && this.winState == $jm.winState.normal))
            this.max(); //最大化
        else if (state == $jm.winState.min)
            this.min(); //最小化
    }

    /**
    * 隐藏
    * @method hide    
    **/
    this.hide = function (d) {
        this.win.hide(d);
    }

    /**
    * 保存当前状态大小
    * @method saveState 
    * @private   
    **/
    this.saveState = function () {
        if (this.winState == $jm.winState.min) return;

        if (this.winState != $jm.winState.max) {
            //原大小
            this.__oldSize = { w: this.width(), h: this.height() };
            //原位置
            this.__oldPosition = { x: this.left(), y: this.top() };
        }
        this.__oldState = this.winState;
    }

    /**
    * 最小化当前窗口
    * @method min  
    *   
    **/
    this.min = function () {
        this.saveState(); //保存当前状态
        var winid = this.id();
        this.win.animate({ left: 0, top: 0, width: 0, height: 0 }, 500, function () {
            var win = $jm.getWinById(winid);
            //win.left(-1000);
            win.top(-1000);
            win.sub();
            var topwin = $jm.get_top_win(); //获取顶层窗口
            if (topwin) { topwin.focus(); }
        });
        this.winState = $jm.winState.min;
    }

    /**
    * 最大化当前窗口
    * @method max  
    *   
    **/
    this.max = function () {
        //如果已经为最大化,则不处理
        if (!this.resizeable) return;

        this.saveState(); //保存当前状态
        
        this.winState = $jm.winState.max;

        var w = this.parent.width();
        var h = (this.parent.get(0) == window.document.body ? $(window).height() : this.parent.height()) - 10;

        //设置位置为顶上
        this.left(0);
        this.top(0);

        this.width(w);
        this.height(h);
    }

    /**
    * 当前窗口恢复正常
    * @method normal  
    *   
    **/
    this.normal = function () {
        this.winState = $jm.winState.normal;
        this.width(this.__oldSize.w);
        this.height(this.__oldSize.h);
        this.left(this.__oldPosition.x);
        this.top(this.__oldPosition.y);        
    }

    /**
    * 窗口获得焦点
    * @method focus  
    *   
    **/
    this.focus = function () {
        //如果已激活则直接返回
        if (this.actived() == true) return;
        this.actived(true);

        var topwin = $jm.get_top_win(); //获取顶层窗口
        if (topwin) {

            //如果它比当前顶层小.则设置它大于顶层
            if (topwin.id() != this.id()) {
                var maxindex = topwin.zindex();
                if (this.zindex() <= maxindex) {
                    if (this.modal) {
                        this.fullDiv.css('z-index', maxindex + 1);
                        this.zindex(maxindex + 2);
                    }
                    else
                        this.zindex(maxindex + 1);
                }
                topwin.sub();
            }
        }
        //遮层底于当前窗口一层
        if (this.modal) this.fullDiv.css('z-index', this.zindex() - 1);
        this.win.removeClass('jm_win_sub'); //高度当前窗口

        if (this.focusCallback) this.focusCallback(this.params.handle); //回调

        this.visible(true); //显示出来
    }

    /**
    * 窗口失去焦点
    * @method sub  
    *   
    **/
    this.sub = function () {
        //如果本来就未激活则直接返回
        if (this.actived() == false) return;

        //去除样式灰度       
        this.win.addClass('jm_win_sub');
        this.actived(false);
        if (this.subCallback) this.subCallback(this.params.handle); //回调       
    }

    /**
    * 关闭窗口
    * @method close  
    * @param {Function} callback 关闭窗口回调
    *   
    **/
    this.close = function (callback) {
        if (this.params.closing && this.params.closing() == false) return false;
        if (this.bodyParent) {
            this._body.hide();
            this._body.appendTo(this.bodyParent);
            this.bodyParent = null;
        }

        if (this.fullDiv) this.fullDiv.remove();

        this.win.remove();
        $jm._removewin(this.id());
        var topwin = $jm.get_top_win(); //获取顶层窗口
        if (topwin) topwin.focus(); //激活最上层的窗口
        //状态为不可见
        this.__visible = false;

        this.actived(false);

        //调用回调
        if (this.closeCallback) {
            this.closeCallback(this.params.handle);
        }

        if (callback) callback(this.params.handle);
    }

    //原大小
    this.__oldSize = { w: this.width(), h: this.height() };
    //原位置
    this.__oldPosition = { x: this.left(), y: this.top() };

    if (this.fullDiv) {
        /**
        * 重置模式窗品遮板大小
        **/
        this.fullDiv.resize = function () {
            var winsize = $jm.winSize(); //获取浏览器窗口大小
            this.fullDiv.width(winsize.w);
            this.fullDiv.height(winsize.h);
        };

        var winid = this.id();
        //窗口大小改变时，重新定大小
        $(window).resize(function () {
            var win = $jm.getWinById(winid);
            if (win && win.fullDiv) {
                win.fullDiv.resize.call(win);
            }
        });
    }

    //加入到缓存集合中    
    $jm.pushWin(this);

    return this;
}

/**
* 对象移动插件
* @class $jm.objMove
* @for $jm.win
*   
**/
$jm.objMove = function () {
    //拖拽对象
    var _win;
    //拖动状态
    var _flag = false;
    //拖拽对象的当前位置
    var _objCurrentLocation;
    //鼠标最后位置
    var _mouseLastLocation;

    /**
    * 鼠标按下,开始拖放事件
    * @event _mdHandler
    * @param evt {Object} 事件对象
    * @for $jm.objMove
    * @private
    **/
    var _mdHandler = function (evt) {
        //不在最大化状态下可以移动
        if ((evt.button == 0 || evt.button == 1) && _win && _win.winState != $jm.winState.max) {
            //显示遮板
            _win.__showModelDiv(true);

            evt = evt || window.event;
            //标记
            _flag = true;

            //拖拽对象位置初始化
            _objCurrentLocation = {
                x: _win.win.position().left, //.offset()
                y: _win.win.position().top
            };

            //鼠标最后位置初始化
            _mouseLastLocation = $jm.getMouseLocation(evt);

            //解决鼠标离开时事件丢失问题
            //注册事件(鼠标移动)
            $(document).unbind("mousemove", _mmHandler);
            $(document).bind("mousemove", _mmHandler);
            //$(document).bind("touchmove", _mmHandler);
            //注册事件(鼠标松开)
            $(document).unbind("mouseup", _muHandler);
            $(document).bind("mouseup", _muHandler);

            //取消事件的默认动作
            if (evt.preventDefault)
                evt.preventDefault();
            else
                evt.returnValue = false;
        }
    };

    /**
    * 鼠标移动,开始拖放事件
    * @event _mmHandler
    * @param evt {Object} 事件对象
    * @for $jm.objMove
    * @private
    **/
    var _mmHandler = function (evt) {
        if (_flag) {
            evt = evt || window.event;
            //当前鼠标的x,y座标
            var _mouseCurrentLocation = $jm.getMouseLocation(evt);

            //拖拽对象座标更新(变量)
            _objCurrentLocation.x = _objCurrentLocation.x + (_mouseCurrentLocation.x - _mouseLastLocation.x);
            _objCurrentLocation.y = _objCurrentLocation.y + (_mouseCurrentLocation.y - _mouseLastLocation.y);

            //将鼠标最后位置赋值为当前位置
            _mouseLastLocation = _mouseCurrentLocation;

            //如果锁定左边界，则X坐标不可小于0
            if (_win.params.bounds.left && _objCurrentLocation.x < 1) _objCurrentLocation.x = 1;
            //锁定左边界
            if (_win.params.bounds.right) {
                //最大X坐标为容器宽度减去窗口宽度
                var maxright;
                if ($jm.element.type(_win.parent) == 'body' ||
                $jm.element.type(_win.parent) == 'window' ||
                $jm.element.type(_win.parent) == 'document') {
                    var winsize = $jm.winSize(); //获取浏览器窗口大小，取最大值
                    maxright = winsize.w - _win.width() - 10;
                }
                else {
                    maxright = _win.parent.width() - _win.width() - 10;
                }
                if (_objCurrentLocation.x > maxright) _objCurrentLocation.x = maxright;
            }
            //如果锁定顶部边界，则不可越边容器顶部
            if (_win.params.bounds.top && _objCurrentLocation.y < 1) _objCurrentLocation.y = 1;
            //锁定底部边界
            //不让窗口越过底部边界
            if (_win.params.bounds.bottom) {
                //最大y坐标为容器高度减去窗口高度
                var maxbottom;
                if ($jm.element.type(_win.parent) == 'body' ||
                $jm.element.type(_win.parent) == 'window' || 
                $jm.element.type(_win.parent) == 'document') {
                    var winsize = $jm.winSize(); //获取浏览器窗口大小，取最大值
                    maxbottom = winsize.h - _win.height() - 10;
                }
                else {
                    maxbottom = _win.parent.height() - _win.height() - 10;
                }
                if (_objCurrentLocation.y > maxbottom) _objCurrentLocation.y = maxbottom;
            }

            //拖拽对象座标更新(位置)//并保证不出界
            //if (_objCurrentLocation.x > 1)
            _win.win.css("left", _objCurrentLocation.x + "px");
            //if (_objCurrentLocation.y > 1)
            _win.win.css("top", _objCurrentLocation.y + "px");
        }
    };

    /**
    * 鼠标松开
    * @event _muHandler
    * @param evt {Object} 事件对象
    * @for $jm.objMove
    * @private
    **/
    var _muHandler = function (evt) {
        if (_flag) {
            evt = evt || window.event;
            //注销鼠标事件
            _cleanHandlers();
            //标记
            _flag = false;
        }
    };

    /**
    * 注销鼠标事件(mousemove mouseup)
    * @method _cleanHandlers   
    * @for $jm.objMove
    * @private
    **/
    var _cleanHandlers = function () {
        if (_win) {
            $(document).unbind("mousemove", _mmHandler);
            //$(document).unbind("touchmove");
            $(document).unbind("mouseup", _muHandler);

            //隐藏遮板
            _win.__showModelDiv(false);
        }
    };

    /**
    * 注册拖拽(参数为$jm.win对象)
    * @method Register   
    * @for $jm.objMove
    * @param {Object} win $jm.win对象
    **/
    this.Register = function (win) {
        //赋值
        _win = win;
        //注册事件(鼠标按下)
        _win.winHead.unbind("mousedown", _mdHandler);
        _win.winHead.bind("mousedown", _mdHandler);
    }
};

/**
* 窗体大小修改类
* @class $jm.objResize
* @for $jm.win
*   
**/
$jm.objResize = function () {
    var _win; //对象   
    var _flag = 0; //拖拉状态0=否，1=右;2=下;3表示右下
    var _curSize; //当前大小
    var _lastLocation; //最后位置
    var _resizeareawidth = 6; //可响应拉的宽度
    //鼠标按下
    var mdHandler = function (evt) {
        if (_win) {
            evt = evt || window.event;
            //拖拽对象位置初始化
            _curSize = {
                w: _win.width(),
                h: _win.height()
            };

            //鼠标最后位置初始化
            _lastLocation = $jm.getMouseLocation(evt);
            //标记
            var tmpflag = 0;
            if (_win.win.offset().top + _curSize.h < _lastLocation.y + _resizeareawidth) _flag = 2; //表示向下拉
            //var scrolw = _win.winBody.get(0).offsetWidth - _win.winBody.get(0).scrollWidth;
            //if (_win.win.offset().left + _win.win.width() > _lastLocation.x && _win.win.offset().left + _win.win.width() - _resizeareawidth < _lastLocation.x) tmpflag = 1;
            if (_win.win.offset().left + _curSize.w < _lastLocation.x + _resizeareawidth) tmpflag = 1; //向右拉

            if (_flag == 2 && tmpflag == 1) {
                _flag = 3; //表示为右下拉
                $(this).css('cursor', 'se-resize');
            }
            else if (tmpflag == 1) {
                _flag = tmpflag;
                $(this).css('cursor', 'e-resize');
            }
            else if (_flag == 2) {
                $(this).css('cursor', 's-resize');
            }
            else {
                $(this).css('cursor', 'default');
                return;
            }

            //显示遮板
            _win.__showModelDiv(true);

            //注册事件(鼠标移动)
            $(document).unbind("mousemove", mmHandler);
            $(document).bind("mousemove", mmHandler);
            //$(document).bind("touchmove", mmHandler);
            //_win.parent.bind("mousemove", mmHandler);
            //注册事件(鼠标松开)
            $(document).unbind("mouseup", muHandler);
            $(document).bind("mouseup", muHandler);
            //_win.parent.bind("mouseup", muHandler);

            //取消事件的默认动作
            if (evt.preventDefault)
                evt.preventDefault();
            else
                evt.returnValue = false;
        }
    };

    //鼠标移动
    var mmHandler = function (evt) {
        if (_flag != 0) {
            evt = evt || window.event;
            //当前鼠标的x,y座标
            var _mouseCurrentLocation = $jm.getMouseLocation(evt);
            //拖拽对象座标更新(变量)
            _curSize.w = _curSize.w + (_mouseCurrentLocation.x - _lastLocation.x);
            _curSize.h = _curSize.h + (_mouseCurrentLocation.y - _lastLocation.y);

            //将鼠标最后位置赋值为当前位置
            _lastLocation = _mouseCurrentLocation;

            //更新对象大小
            if (_flag == 1 || _flag == 3) _win.width(_curSize.w);
            if (_flag == 2 || _flag == 3) _win.height(_curSize.h);

            //取消事件的默认动作
            if (evt.preventDefault)
                evt.preventDefault();
            else
                evt.returnValue = false;
        }
    };

    //鼠标松开
    var muHandler = function (evt) {
        if (_flag != 0) {
            evt = evt || window.event;
            //注销鼠标事件(mousemove mouseup)
            cleanMouseHandlers();
            //标记
            _flag = 0;
            //隐藏遮板
            _win.__showModelDiv(false);
        }
    };

    //注销鼠标事件(mousemove mouseup)
    var cleanMouseHandlers = function () {
        if (_win) {
            _win.win.css('cursor', 'default');
            $(document).unbind("mousemove", mmHandler);
            //$(document).unbind("touchmove");
            $(document).unbind("mouseup", muHandler);
        }
    };

    /**
    * 注册大小改变对象(参数为$jm.win对象)
    * @method Register   
    * @for $jm.objResize
    * @param {Object} win $jm.win对象
    **/
    this.Register = function (win) {
        //赋值
        _win = win;
        //注册事件(鼠标按下)
        _win.win.unbind("mousedown", mdHandler);
        _win.win.bind("mousedown", mdHandler);
    }
};

/**
* 窗体按钮枚举 OK: 1, YesOrNo: 2
* @property buttons
* @type Json
* @for $jm
**/
$jm.buttons = { OK: 1, YesOrNo: 2 };

/**
* 消息框
* 参数例如:{content:'消息',title'消息弹窗',button:$jm.buttons.YesOrNo,oktext:'是',ok:function(){alert('ok');},notext:'否',no:function(){alert('否');}}
* @class $jm.msgbox
* @param {Object} pars 生成弹窗的参数
**/
$jm.msgbox = function (pars) {
    var button = [];
    var _box;
    if (pars.button == $jm.buttons.YesOrNo) {
        button.push({
            text: pars.yestext || '是',
            click: function () {
                if (pars.yes) {
                    if (false === pars.yes()) {
                        return false;
                    }
                }
                this.close();
            }
        });
        button.push({
            text: pars.notext || '否',
            click: function () {
                if (pars.no) {
                    if (false === pars.no()) {
                        return false;
                    }
                }
                this.close();
            }
        });
    }
    else {
        button.push({
            text: pars.oktext || '确定',
            click: function () {
                if (pars.ok) {
                    if (false === pars.ok()) {
                        return false;
                    }
                }
                this.close();
            }
        });
    }
    pars.model = typeof pars.model === 'undefined' ? true : pars.model;
    pars.button = button,
    pars.canMax = typeof pars.canMax === 'undefined' ? false : pars.canMax; ;
    pars.canMin = typeof pars.canMin === 'undefined' ? false : pars.canMin; ;
    pars.position = typeof pars.position === 'undefined' ? 'top' : pars.position; ; ;
    _box = new $jm.win(pars);

    /**
    * 弹出当前窗口
    * @method show
    * @for $jm.msgbox
    **/
    this.show = function () {
        _box.show();
        return _box;
    };
};

/**
* 加载为jquery插件
**/
$.fn.window = function (options) {
    var id = $(this).attr('id');
    var win = $jm.getWinById(id); //通过ID获取当前窗口
    if (win) return win;

    var options = $.extend({
        id: id,
        body: this
    }, options);

    //生成新窗口
    return new $jm.win(options);
};
