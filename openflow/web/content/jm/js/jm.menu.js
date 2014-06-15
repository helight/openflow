/**
* ======================= <br />
* 右健菜单 <br />
* ======================= <br />
* @namespace $jm
* @class $jm.menu
* @param {Object} option 菜单参数，如：option.target:触发菜单的对象，items:菜单项,parent 菜单事件触发对象,button:0=左健，2=右健
* @requires $jm
**/

$jm.menu = function (option) {
    this._target = option.target;

    /**
    * 当前菜单对象 DIV
    * @property _menu
    * @for menu
    * @type Object 
    * @private
    **/
    this._menu = $('<div class="jm_menu"></div>');
    /**
    * 当前菜单主体 UL
    * @property  __body
    * @for menu
    * @type Object 
    * @private
    **/
    this.__body = $('<ul></ul>');
    this.__body.appendTo(this._menu);

    /**
    * 当前菜单ID
    * @property id
    * @for menu 
    * @type Number   
    **/
    this.id;
    /**
    * 所有菜单项
    * @property items
    * @for menu
    * @type Array 
    * @private
    **/
    this.items = {};

    /**
    * 菜单项个数
    * @method itemCount
    * @return {Int} 菜单项个数
    **/
    this.itemCount = function () {
        var i = 0; for (var k in this.items) { i++; } return i;
    };

    /**
    * 当前菜单索引
    * @property index
    * @for menu
    * @type Number     
    **/
    this.index = 0;

    /**
    * 当前菜单项索引
    * @property __itemIndex
    * @for menu
    * @type Number 
    * @private
    **/
    this.__itemIndex = 0;

    /**
    * 单击事件句柄
    * @property _eventHander
    * @for menu
    * @private
    **/
    var _eventHander = null;
    var _frameHandler = null;

    /**
    * 菜单句柄
    * @event IContextMenuHander
    * @for menu    
    * @private
    **/
    this.IContextMenuHander = null;

    /**
    * document 菜单弹出父容器
    * @property _backGround
    * @for menu
    * @type Object 
    * @private
    **/
    var _backGround = option.target || $(document);

    /**
    * 事件取消
    * @method _cancelfun
    * @for menu
    * @private
    **/
    var _cancelfun = function () { return false; };

    /**
    * 获取或设置当前菜单的层级
    * @method zIndex
    * @for menu
    * @param {Int} index 设置当前菜单层级,为空则只返回当前层级
    * @return {Int} 返回当前菜单层级
    **/
    this.zIndex = function (index) {
        if (!$jm.isNull(index)) {
            this._menu.css('z-index', index);
        }
        else return this._menu.css('z-index');
    }

    /**
    * 弹出当前菜单
    * @method show
    * @for menu
    * @param {Object} mu 当前菜单class   
    * @private
    **/
    this.show = function (mu, e) {        
        _eventHander = function (evt) {
            if (mu)
                mu.hide();
        };
        $(document).unbind('mouseup', _eventHander);
        $(document).bind('mouseup', _eventHander);

        var position = $jm.getMouseLocation(e, mu._target); //获取鼠标位置
        var targetoffset = $jm.getOffset(mu._target);

        //如果菜单超出容器则从左弹出
        var muwidth = mu._menu.width();
        if (position.x >= muwidth && e.offsetX > $(window.document).width() - muwidth) {
            position.x = position.x - muwidth;
        }

        mu._menu.css('left', position.x);
        mu._menu.css('top', position.y);

        mu._menu.show(200);
    };

    /**
    * 隐藏当前菜单,并取消事件绑定
    * @method hide
    * @for menu    
    * @private
    **/
    this.hide = function () {
        _backGround.unbind('mouseup', _eventHander);
        this._menu.hide(200);
    };

    /**
    * 当前菜单的缓存,如果mu参数不为空则写入当前菜单到缓存中
    * @method _menucache
    * @for menu
    * @param {Int} menuid 菜单ID
    * @param {Menu} mu 当前菜单对象
    * @private
    * @return {Menu} 如果mu参数为空,则返回当前菜单
    **/
    var _menucache = function (menuid, mu) {
        if (menuid) {
            if (mu) $jm.cache.set('jm_menu_' + menuid, mu);
            else return $jm.cache.get('jm_menu_' + menuid);
        }
    }

    /**
    * 当前菜单的索引,如果index参数不为空则更改当前菜单索引
    * @method _menuindex
    * @for menu
    * @param {Int} index 索引,可为空    
    * @private
    * @return {Int} 如果index参数为空,则返回当前菜单的索引,否为为空
    **/
    var _menuindex = function (index) {
        if (!$jm.isNull(index)) $jm.cache.set('jm_menu___index', index);
        else {
            index = $jm.cache.get('jm_menu___index');
            index = Number(index) > 0 ? index : 0;
            return index;
        }
    }

    /**
    * 添加菜单项
    * @method addItem
    * @for menu
    * @param {Object} item 菜单项,例如:{text:'菜单一',tag:'测试',click:function(){alert('点击了菜单一')}}
    **/
    this.addItem = function (item) {
        //得到当前菜单
        var menu = _menucache(this.id);
        //当前菜单项唯一标识
        this.__itemIndex++;
        var itemmark = 'jm_menuitem_' + this.index + '_' + this.__itemIndex;
        var mitem = $('<li jm_menuitem="' + itemmark + '" jm_menuid="' + this.id + '">' + item.text + '</li>');

        if (item.disabled) mitem.attr('disabled', 'disabled'); //禁用
        if (!$jm.isNull(item.tag)) mitem.attr('tag', item.tag);

        //缓存菜单项
        this.items[itemmark] = item;

        //绑定单击事件
        mitem.bind('mousedown', function (evt) {
            evt.stopPropagation();
            var curmenuid = $(this).attr('jm_menuid');
            var curmenu = _menucache(curmenuid);

            var curitemmark = $(this).attr('jm_menuitem');
            var curitem = curmenu.items[curitemmark]; //获取菜单项
            if (curitem.disabled == true) return;
            if (curitem && curitem.click)
            { curitem.click(); }

            if (curmenu) curmenu.hide(); //关闭菜单
            return false;
        });

        //菜单加入到页面中
        mitem.appendTo(this.__body);
    }

    /**
    * 初始化菜单,生成菜单项,绑定菜单事件和菜单弹出对象
    * @method init
    * @for menu
    **/
    this.init = function () {
        this.index = _menuindex() + 1;
        _menuindex(this.index); //更新索引
        //去除默认菜单事件
        _backGround.bind('contextmenu', _cancelfun);

        var button = option.button;
        //如果按健设置不正确则默认为右健
        if ($jm.isNull(button) || Number(button) < 0) button = 2;

        var menudown = function (obj) {
            if (obj.button == button || (button == 0 && obj.button == 1)) //按健不配匹不理会
            {
                //首先禁用系统右健菜单
                $(document).bind('contextmenu', _cancelfun);
                var target = $(obj.srcElement || obj.target);
                var menuid = target.attr('jm_menuid');
                var menu = _menucache(menuid); //从缓存中获取菜单
                if (menu) {
                    menu.show(menu, obj); //弹出菜单
                    obj.stopPropagation();
                }
            }
        };
        this._target.unbind('mouseup', menudown);
        this._target.bind('mouseup', menudown);

        //生成其ID
        this.id = 'jm_menu_id_' + this.index;
        this._target.attr('jm_menuid', this.id); //把菜单ID写到需要弹出的母板上

        //添加菜单项
        if (this.items) {
            for (var i in option.items) {
                this.addItem(option.items[i]);
            }
        }

        _menucache(this.id, this); //把菜单加到缓存中
        this._menu.appendTo(this._target);
        //this._menu.appendTo('body');
    };

    this.init();
    return this;
};

/**
* 加载为jquery插件
**/
$.fn.menu = function (options) {   
    var options = $.extend({
        target: $(this)       
    }, options);
    //生成新菜单
    return new $jm.menu(options);
};