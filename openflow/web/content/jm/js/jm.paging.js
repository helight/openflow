/**
* 分页插件
* @param {Object} option 分页参数:
* showCount:{Number} 当前页附件显示多少个页码，默认为6
* className:{String} 分页样式名，默认为 jm_paging
* change:{Function} 页码改变回调，参数为当前页码
* pretext:{String} 上一页字符
* nexttext:{String} 下一页字符
* turntext:{String} 转到
* @namespace $jm
* @class $jm.paging
* @requires $jm
**/
$jm.paging = function (option) {
    /**
    * 当前页附件显示多少个页码
    * 默认为6
    * @property showCount
    * @type Number     
    **/
    this.showCount = option.showCount || 6;

    /**
    * 整个翻页区域元素    
    * @property _pagingBody
    * @type Object   
    * @private  
    **/
    this._pagingBody = $('<div class="jm_paging"></div>'); //翻页div

    /**
    * 承载分页对象的容器    
    * @property parent
    * @type Object
    **/
    this.parent = typeof option.parent == 'string' ? $('#' + option.parent) : option.parent;

    /**
    * 页码点击事件
    * @method _pageClick  
    * @private  
    **/
    this._pageClick = function (index) {
        if (option.change) {
            if (!index) {
                index = $(this).attr('jm_paging_index');
            }
            option.change(index);
        } //页码改变事件
    }

    /**
    * 转页输入事件
    * @method _turnKeypress  
    * @private  
    **/
    this._turnKeypress = function (evt) {
        var c = evt.keyCode ? evt.keyCode : evt.which;
        if (c == 13) {
            if (option.change && $.trim(this.value) != '') {
                option.change(this.value);
            }
        }
        else if ((c <= 57 && c >= 48) || c == 8 || c == 9 || c == 0) {
            //9是opera中的tab健
            return true;
        }
        return false;
    };

    /**
    * 生成页码
    * @method _createPages  
    * @param {Integer} index 当前页码
    * @param {Integer} count 总共有多少页
    * @private  
    * @return Array 所有需要显示的页码
    **/
    this._createPages = function (index, count) {
        //显示的页码
        var pageList = [];
        pageList.push(1); //加入第一页
        if (count > 1) pageList.push(2); //加入第二页
        var floor = Math.floor(this.showCount / 2);
        var showFirst = index - floor;
        var showLast = index + floor;
        if (showFirst < 0) showLast = showLast - showFirst;
        if (showLast > count) showLast = count;
        for (var i = showFirst; i <= showLast; i++) {
            //如果里面不存在则加入显示行列
            if (!$jm.contain(pageList, i) && i > 0) pageList.push(i);
        }

        var ltc = count - 1;
        //加上最后第二页
        if (ltc > 1 && !$jm.contain(pageList, ltc)) pageList.push(ltc);
        if (count > 1 && !$jm.contain(pageList, count)) pageList.push(count); //加上最后一页

        return pageList;
    }

    /**
    * 展现分页区域
    * @method render
    * @param {Object} params 参数json,{index:当前码码，count：总共多少页}
    * @return Object 分页区域元素div
    **/
    this.render = function (params) {
        this._pagingBody.html('');
        var index = parseInt(params.index) || 1; //当前页
        var count = parseInt(params.count) || 1; //总共多少页
        if (count <= 1) return this._pagingBody;

        var preText = option.pretext || "上一页";
        var nexttext = option.nexttext || "下一页";
        var turnpagetext = option.turntext || "转到";

        //生成页码集合
        var pageList = this._createPages(index, count);

        //写入上一页
        var preIndex = index > 1 ? index - 1 : 0;
        if (preIndex > 0) {
            var prelink = $('<a href="javascript:;" jm_paging_index="' + preIndex + '">' + preText + '</a>');
            prelink.bind('click', function () {
                if (option.change) {
                    var index = $(this).attr('jm_paging_index');
                    option.change(index);
                } //页码改变事件
            });
            prelink.appendTo(this._pagingBody);
        }

        var showindex = 0;
        //显示页码
        for (var i = 0; i < pageList.length; i++) {
            var pageindex = pageList[i];
            //中间空档
            if (pageindex - showindex > 1) {
                var sp = $('<span>&nbsp;...&nbsp;</span>');
                sp.appendTo(this._pagingBody);
            }
            var alink = $('<a href="javascript:;" jm_paging_index="' + pageindex + '">' + pageindex + '</a>');
            if (pageindex == index) alink.addClass('jm_curPage');
            else {
                //页码改变事件             
                alink.bind('click', function () {
                    if (option.change) {
                        var index = $(this).attr('jm_paging_index');
                        option.change(index);
                    } //页码改变事件
                });
            }
            alink.appendTo(this._pagingBody);
            showindex = pageindex;
        }
        //写入下一页
        var nextIndex = index < count ? index + 1 : 0;
        if (nextIndex > 0) {
            var nextlink = $('<a href="javascript:;" jm_paging_index="' + nextIndex + '">' + nexttext + '</a>');
            nextlink.bind('click', function () {
                if (option.change) {
                    var index = $(this).attr('jm_paging_index');
                    option.change(index);
                } //页码改变事件
            });
            nextlink.appendTo(this._pagingBody);
        }
        //转到第几页
        if (count > 1) {
            var txt = $('<input type="text" name="jm_PagerTxt" value="' + index + '" />');
            txt.bind('keypress', this._turnKeypress);
            txt.appendTo(this._pagingBody);
            var turnbtn = $('<a href="javascript:;">OK</a>');
            if (turnpagetext) turnbtn.text(turnpagetext);
            //页码改变事件
            turnbtn.bind('click', function () {
                if (option.change) {
                    var index = txt.val();
                    if (index > params.count) index = params.count;
                    else if (index < 1) index = 1;
                    txt.val(index);
                    option.change(index);
                }
            });
            turnbtn.appendTo(this._pagingBody);
        }
        if (this.parent) this._pagingBody.appendTo(this.parent);
        else this._pagingBody.appendTo('body');

        return this;
    }
};

/**
* 加载为jquery插件
**/
$.fn.paging = function (options) {
    var id = $(this).attr('id');
    options = $.extend({ parent: $(this) }, options);
    //获取翻页组件缓存
    var control = $jm.cache.set('jm.paging.' + id, $jm.cache.get('jm.paging.' + id) || new $jm.paging(options));
    control.render(options);

    //生成新菜单
    return control;
};