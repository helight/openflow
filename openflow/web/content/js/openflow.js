
/**
* openflow工作流编辑
*/
var openflow = (function ifow() {
    /**
    * 构造函数
    */
    function __constructor(url) {
        this.root = url;
    }

    /**
    * 设置样式
    * @param {Element} el 被设置样式的元素
    * @param {Object} style 样式值
    */
    __constructor.prototype.css = function (el, style) {
        if (style) {
            for (var k in style) {
                el.style[k] = style[k];
            }
        }
    }

    /**
    * 与根路径组合url
    */
    __constructor.prototype.checkUrl = function (url1, url2) {
        var url = (url1 || '') + (url2 || '');
        if (url && url[0] == '/') url = url.substr(1);
        return this.root + url;
    }

    /**
    * 获取url参数
    */
    __constructor.prototype.getParams = function () {
        var param = {};
        var search = window.location.search;
        if (search) {
            search = search.replace(/^\?/i, '');
            var ss = search.split('&');
            if (ss.length > 0) {
                for (var i in ss) {
                    var sp = ss[i].split('=');
                    if (sp.length == 2) {
                        param[sp[0]] = sp[1];
                    }
                }
            }
        }
        return param;
    }

    /**
    * 获取鼠标坐标
    * @method getMouseLocation
    * @param {Object} ev 事件对象
    * @param {Object} target 可选,事件发生对象
    * @for $jm
    **/
    __constructor.prototype.getMouseLocation=function (ev, target) {
        if (!target || target.length == 0) target = $('body');
        if (target.length == 0) return { x: 0, y: 0 };
        return {
            x: ev.clientX - target.offset().left,
            y: ev.clientY - target.offset().top
        };
    }

    return new __constructor('/');
})();

/**
* 请求服务端
*/
openflow.request = function (method, data, callback) {
    openflow.log(data);
    openflow.log(data.url);
    $.ajax({
        type: method || 'get',
        cache:false,
        url:data.url || (openflow.root + 'servers/openflow.ashx?rnd=' + Math.random()),
        data: data,
        success: function (data) {
            try {
                openflow.log(data);
                if (data) {
                    data = $.parseJSON(data);

                    if (data.State == 0) {
                        if (callback && data.Data != 'null') callback(data.Data,null,data);
                    }
                        //未登录
                    else if (data.State == 2) {
                        openflow.message.warning('登录失效,请重新登录');
                        //reLogin();
                    }
                    else {
                        openflow.message.error(data.Message);
                        if (callback) {
                            //当为网络错误时返回3的异常
                            if (data.State == 3) {
                                callback(data.State, data.Message);
                            }
                            else {
                                callback(null, data.Message);
                            }
                        }
                    }
                }
                else {
                    if (callback) callback(data);
                }
            }
            catch (e) {
                openflow.message.error(e.toString());
            }
        },
        error: function (e) {
            openflow.message.error(e.statusText);
            if (callback) callback(null,e.statusText);
        }
    });
}

/**
* 输出调试日志
*/
openflow.log = function (log) {
    if (window.console && window.console.log) {
        window.console.log(log);
    }
}

/**
* 格式化xml节点属性值，避免特殊字符
*/
openflow.formatXmlValue = function(value) {
    if(!value) return '';
    value = value.toString()
                 .replace(/&/g,'&amp;')
                 .replace(/\</g,'&lt;')
                 .replace(/\>/g,'&gt;')
                 .replace(/\'/g,'&apos;')
                 .replace(/\"/g,'&quot;') ;
    return value;
}

/**
* 格式化html特殊字符
*/
openflow.formatHtmlValue = function(value) {
    if(!value) return '';
    value = openflow.formatXmlValue(value);

    value = value.split(' ').join('&nbsp;')
                 .replace(/\n/g,'<br />')
                 ;
    return value;
}

/**
* 边框扩展栏。可收缩
*/
openflow.expandBanner= function(parent) {
    //当前工具栏父框
    if(!this.parent) this.parent= parent || '#editor-body-left';
    this.banners= [];
    this.contents= [];

    /**
    * 添加工具栏
    */
    this.addBanner= function(title,expended) {
        var banner = $('<div class="expand_banner"></div>').appendTo(this.parent);
        $('<div></div>').addClass('expand_item')
                        .addClass(expended?'expand_expanded':'expand_closed')
                        .appendTo(banner);
        this.titleElement = $('<div class="expand_banner_title">'+title+'</div>').appendTo(banner);

        //点击展开和收缩
        banner.click(function() {
            var expend = $(this).find('div.expand_expanded');
            if(expend.length > 0) {
                $(this).find('+div.expand_content').hide();
                expend.toggleClass('expand_expanded',false).toggleClass('expand_closed',true);
                $(this).attr('data-hidden',true);
            }
            else {
                $(this).find('+div.expand_content').show();
                $(this).find('div.expand_closed').toggleClass('expand_expanded',true).toggleClass('expand_closed',false);
                $(this).attr('data-hidden',false);
            }
        });
        this.banners.push(banner);
        return banner;
    }

    this.title= function(title) {
        if(title !=  this.titleElement.html()) this.titleElement.html(title);
    }

    /**
    * 添加工具栏和内容
    */
    this.add= function(title,content,expended) {
        if(typeof this.parent == 'string') this.parent= $(this.parent);
        this.banner = this.addBanner(title,expended);
        var contentContainer= $('<div class="expand_content"></div>').appendTo(this.parent);
        $(content).appendTo(contentContainer);
        this.contents.push(contentContainer);
        //if(!title) this.banner.hide();//如果没有设置标题则隐藏
    }

    /**
    * 移除当前栏
    */
    this.remove= function() {
        for(var i in this.banners) {
            this.banners[i].remove();
        }
        this.banners= [];
        for(var j in this.contents) {
            this.contents[j].remove();
        }
        this.contents= [];
    }

    /**
    * 隐藏当前栏
    */
    this.hide= function() {
        for(var i in this.banners) {
            this.banners[i].hide();
        }
        for(var j in this.contents) {
            this.contents[j].hide();
        }
    }

    /**
    * 显示当前栏
    */
    this.show= function() {
        for(var i in this.banners) {
            this.banners[i].show();
            if(this.contents[i] && (!this.banners[i].attr('data-hidden') || this.banners[i].attr('data-hidden') == false)) {
                this.contents[i].show();
            }
        }
    }
}

/**
* 格式化时间
*/
openflow.formatDate = function (date, format) {
    if(typeof date !== 'date') {
        date = new Date(date);
    }
    date = date || new Date();
    format = format || 'yyyy-MM-dd HH:mm:ss';
    var result = format.replace('yyyy', date.getFullYear().toString())
    .replace('MM', (date.getMonth()< 9?'0':'') + (date.getMonth() + 1).toString())
    .replace('dd', (date.getDate()<9?'0':'')+date.getDate().toString())
    .replace('HH', (date.getHours() < 9 ? '0' : '') + date.getHours().toString())
    .replace('mm', (date.getMinutes() < 9 ? '0' : '') + date.getMinutes().toString())
    .replace('ss', (date.getSeconds() < 9 ? '0' : '') + date.getSeconds().toString());

    return result;
}

/**
* 用于日历控制格式化
*/
function dateFormatter(date) {
    return openflow.formatDate(date,'yyyy-MM-dd');
}
/**
* 用于日历控制格式化
*/
function dateTimeFormatter(date) {
    return openflow.formatDate(date,'yyyy-MM-dd HH:mm:ss');
}

/**
* 用于日历控件转换
*/
function dateParser(s) {
    if(!s) return new Date();
    var ss = (s.split('-'));
    var y= parseInt(ss[0],10);
    var m= parseInt(ss[1],10);
    var d= parseInt(ss[2],10);
    if(!isNaN(y) && !isNaN(m) && !isNaN(d)) {
        return new Date(y,m-1,d);
    }
    else {
        return new Date();
    }
}

//重新登录
function relogin() {
    if(window.parent && window.parent != window && window.parent.relogin) {
        window.parent.reLogin();
    }
    else {
        window.location = openflow.root + 'login';
    }
}




