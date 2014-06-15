/**
* ======================= <br />
* 家猫js类库 <br />
* 依赖jquery <br />
* Copyright (c) 2011, jiamaocode.com, All rights reserved <br />
* ======================= <br />
* @version: 2.0.1.5
* @module jmcore
* @class $jm
* @static
**/
var $jm = {

    /**
    * 标识为调试
    * @private
    * @type Boolean 
    */
    debug: true,

    /**
    * 缓存队列
    * @private
    */
    _hash: {},

    /**
    * 基础缓存
    * @class cache
    * @namespace $jm
    * @requires $jm
    */
    cache: {

        /**
        * 添加缓存
        * @method set
        * @for cache
        * @param {String} key 缓存KEY值,如果已存在则被覆盖
        * @param {Object} value 缓存对象
        * 
        **/
        set: function (key, value) {
            $jm._hash[key] = value;
            return value;
        },

        /**
        * 获取缓存
        * @method get
        * @for cache
        * @param {String} key 缓存KEY值
        * @return {Object} return 从set加入缓存的对象,不存在则返回空
        * 
        **/
        get: function (key) {
            return $jm._hash[key];
        },

        /**
        * 缓存个数
        * @method count
        * @for cache   
        * @return {Int} return 当前缓存队列中数据条数
        * 
        **/
        count: function () { var i = 0; for (var k in $jm._hash) { i++; } return i; },

        /**
        * 移除缓存
        * @method remove
        * @for cache
        * @param {String} key 缓存KEY值   
        * 
        **/
        remove: function (key) { delete $jm._hash[key]; },

        /**
        * 获取缓存
        * @method contains
        * @for cache
        * @param {String} key 缓存KEY值
        * @return {Boolean} return 存在此缓存则返回true,否则返回false
        * 
        **/
        contains: function (key) { return !$jm.isNull($jm._hash[key]); }
    },

    /**
    * 浏览器信息
    * @class browser
    * @namespace $jm
    * @requires $jm
    * @static
    */
    browser: {
        /**
        * 浏览器头信息
        * @property userAgent
        * @for $jm.brower
        * @type String
        **/
        userAgent: navigator.userAgent,

        /**
        * 初始化浏览器信息
        * 判断浏览器版本等信息，移动版等
        * @method init
        * @for $jm.brower
        * @type Boolean
        * @private
        **/
        _init: function () {
            //            if (/Webkit/i.test(this.userAgent)) {
            //                this.webkit = true;
            //            }
            if (/Mobile/i.test(this.userAgent)) {
                this.mobile = true;
            }
            if (/iPad/i.test(this.userAgent)) {
                this.iPad = true;
            }
            else if (/iPhone/i.test(this.userAgent)) {
                this.iPhone = true;
            }
            else if (/Android/i.test(this.userAgent)) {
                this.android = true;
            }

            if (/msie/i.test(this.userAgent)) {
                this.msie = true;
            }
            else if (/Chrome/i.test(this.userAgent)) {
                this.chrome = true;
            }
            else if (/Safari/i.test(this.userAgent)) {
                this.safari = true;
            }
            else if (/Firefox/i.test(this.userAgent)) {
                this.firefox = true;
            }

            //            if (/Mozilla/i.test(this.userAgent)) {
            //                this.mozilla = true;
            //            }
            for (var i in $.browser) {
                this[i] = $.browser[i];
            }
        }
    },

    /**
    * 当前部署相对于网站的路径,
    * 比如放置在网站的os目录下,则设置为/os/
    * @property rootUrl
    * @for $jm
    * @type String 
    * 
    **/
    rootUrl: '/',

    /**
    * 检查当前路径,替换其中的~/根路径标识    
    * @method checkUrl
    * @for $jm
    * @param {String} url 待处理的url
    * @return String 处理后的url
    * 
    **/
    checkUrl: function (url) {
        return url.replace(/~\//ig, $jm.rootUrl);
    },

    /**
    * 打印消息类型
    * 主要用于console打印调试消息    
    *  log: 0,error: 1,info: 2,warn: 3
    * @property msgType
    * @for $jm
    * @type Enum 
    * 
    **/
    msgType: {
        log: 0,
        error: 1,
        info: 2,
        warn: 3
    },

    /**
    * 打印console消息
    * @method out   
    * @param {String} msg 消息
    * @param {$jm.msgType} t 消息类型   
    * @for $jm
    * 
    **/
    out: function (msg, t) {
        if (this.debug && window.console) {
            if (window.console.log && t == $jm.msgType.log) window.console.log(msg);
            else if (window.console.error && t == $jm.msgType.error) window.console.error(msg);
            else if (window.console.warn && t == $jm.msgType.warn) window.console.warn(msg);
            else if (window.console.info) window.console.info(msg); //默认info
        }
    },

    /**
    * 检查对象是否为null或undefined
    * @method isNull    
    * @param {Objec} s 要检查的对象   
    * @return {Boolean} return 如果为空则返回true,否则返回false
    * @for $jm
    * 
    **/
    isNull: function (s) {
        return s == null || typeof s == 'undefined';
    },

    /**
    * 当前URL所传的参数哈希表    
    * @property __urlParams
    * @for $jm
    * @type Array 
    * @private
    **/
    __urlParams: null,

    /**
    * 获取URL指定KEY的参数值
    * @method getParam
    * @param {String} key 需要获取的参数的key值
    * @return {String} return 返回KEY健对应的值,不存在则返回空
    * @for $jm
    * 
    **/
    getParam: function (key) {
        //如果参数为空,则分解URL参数
        if (this.isNull(__urlParams)) {
            var __urlParams = {};
            var pindex = window.location.href.indexOf('?');
            if (pindex > 0) {
                var purl = window.location.href.substring(pindex + 1);
                var ps = purl.split('&');
                var len = ps.length;
                for (var i = 0; i < len; i++) {
                    var pk = ps[i].split('=');
                    __urlParams[pk[0]] = decodeURI(pk[1]);
                }
            }
        }
        return __urlParams[key];
    },

    /**
    * 获取元素的坐标偏移量
    * @method getOffset
    * @param {Object} target 元素对象
    * @for $jm
    **/
    getOffset: function (target) {
        target = $(target)[0];
        var x = y = 0;
        do {
            x += target.offsetLeft;
            y += target.offsetTop;
        } while (target = target.offsetParent);
        return {
            'left': x,
            'top': y
        };
    },

    /**
    * 获取鼠标坐标
    * @method getMouseLocation
    * @param {Object} ev 事件对象
    * @param {Object} target 可选,事件发生对象
    * @for $jm
    **/
    getMouseLocation: function (ev, target) {
        if (!target) target = $(document.body);
        return {
            x: ev.clientX - target.offset().left,
            y: ev.clientY - target.offset().top
        };
    },

    /**
    * 把JSON时间转为datetime,
    * 主要是后台序列化过来的时间格式
    * @method parseDate
    * @param {String} jsDate 序列化的时间字符 \/Date 564646 +8000\/
    * @for $jm
    * @return Date js时间对象
    **/
    parseDate: function (jsDate) {
        if (typeof jsDate == 'string') {
            jsDate = jsDate.replace("/Date(", "").replace(")/", "");
            var aindex = jsDate.indexOf("+");
            var vindex = jsDate.indexOf("-");
            if (aindex > 0) {
                jsDate = jsDate.substring(0, aindex);
                return new Date(parseInt(jsDate, 10));
            }
        }
        return new Date(jsDate);           
    }, 
    /**
    * 格式化时间
    */
    formatDate:function(date,format) {
        if(!date || date == '-') return '-';
        if(typeof date == 'string' || typeof date == 'number') date = this.parseDate(date);

        date = date || new Date();
        format = format || 'yyyy-MM-dd HH:mm:ss';
        var result = format.replace('yyyy', date.getFullYear().toString())
        .replace('MM', (date.getMonth()< 9?'0':'') + (date.getMonth() + 1).toString())
        .replace('dd', (date.getDate()<9?'0':'')+date.getDate().toString())
        .replace('HH', (date.getHours() < 9 ? '0' : '') + date.getHours().toString())
        .replace('mm', (date.getMinutes() < 9 ? '0' : '') + date.getMinutes().toString())
        .replace('ss', (date.getSeconds() < 9 ? '0' : '') + date.getSeconds().toString());

        return result;
    },
    /**
    * 转化毫秒数为时间
    */
    timeSpan: function(date) {
        var result= 
        {
            toString:function() {
                var fstr= '';
                if(this.days) {
                    fstr += this.days + '天';
                }
                if(this.hours) {
                    fstr += this.hours + '小时';
                }
                if(this.minutes) {
                    fstr += this.minutes + '分';
                }
                if(this.seconds) {
                    fstr += this.seconds + '秒';
                }
                return fstr;
            }
        };
        result.seconds = Math.ceil(date / 1000);
        result.minutes = Math.floor(result.seconds / 60);
        result.seconds = result.seconds % 60;
        result.hours = Math.floor(result.minutes / 60);
        result.minutes = result.minutes % 60;
        result.days = Math.floor(result.hours / 24);
        result.hours = result.hours % 24;
        return result;
    },
    /**
    * 一些简单的元素操作
    * @class element
    * @namespace $jm
    * @requires $jm
    * @static
    **/
    element: {
        /**
        * 获取元系的类型，比如:div,p,body等
        * @method type
        * @param {Object} el html元素
        * @return String 元素的类型字符
        **/
        type: function (el) {
            if (el) el = $(el);
            if (el.length > 0) {
                el = el.get(0);
                switch (el) {
                    case window:
                        {
                            return 'window';
                        }
                    case document: return 'document';
                    default:
                        {
                            return el.tagName.toLowerCase();
                        }
                }
            }
        }
    },
    /**
    * 继承
    * @method extend
    * @param superclass {Class} 原类
    * @param subclass {Class} 派生类
    **/
    extend: function (superclass, subclass) {
        var F = function () { };
        F.prototype = superclass.prototype;
        subclass.prototype = new F();
        subclass.prototype.constructor = subclass

        subclass.superclass = superclass;
        if (superclass.prototype.constructor == Object.prototype.constructor) {
            superclass.prototype.constructor = superclass;
        }
    }
};

//初始化浏览器信息
$jm.browser._init();

/**
* 判断集合中是否包含指定的元素
* @method parseDate
* @param {Array} arr 对象集合
* @param {Object} obj 需要判断的元素
* @for $jm
* @return Boolean 包含与否
**/
$jm.contain = function (arr, obj) {
    if (arr) {
        for (var i in arr) {
            if (arr[i] == obj) return true;
        }
    }
    return false;
};