
/**
* 消息提示
*/
openflow.message = (function () {
    if(window.parent && window.parent != window && window.parent.openflow && window.parent.openflow.message) {
        return window.parent.openflow.message;
    }
    var msg = function () {
        this.cache = {};
    }

    /**
    * 弹出提示信息
    **/
    msg.prototype.show = function (msg, key, css, ind,modal,canclose) {
        var box = ind ? (this.cache[key] || (this.cache[key] = $('<div></div>'))) : $('<div></div>');

        //如果为模式窗口
        if (modal) {
            if (!box.bg) box.bg = $('<div class="modal" />');
            box.bg.remove();
            box.bg.appendTo('body').show();
        }

        box.appendTo('body');
        box.attr('class', 'alert ' + css);
        box.html(msg);
        if(canclose) {
            $('<a href="#" class="close">X</a>').appendTo(box).click(function() {
                this.parentElement.style.display='none';
                return false;
            });
        }

        /*
        *关闭当前弹框
        */
        box.close = function () {
            this.hide();
            if (this.bg) this.bg.remove();
        };
        box.show();
        var onsize = function () {
            openflow.message.position(box);
            if (box.bg) {
                var w = Math.max($(window).width(), $(window.document).width());
                var h = Math.max($(window).height(), $(window.document).height());
                box.bg.width(w);
                box.bg.height(h);
            }
        };
        onsize();
        $(window).bind('resize', onsize);
        if (ind != true) {
            setTimeout(function () {
                box.animate({
                    //'top': 0,
                    'opacity': 0
                }, 2000, function () {
                    $(window).unbind('resize', onsize);
                    box.close();
                })
            }, 5000);
        }
        openflow.log(msg);//调试日志
        return box;
    };

    /**
    * 隐藏
    */
    msg.prototype.close = function (key) {
        var box = this.cache[key];
        if (box) {
            box.hide();
            if (box.bg) box.bg.remove();
        }
    }

    //定位消息框
    msg.prototype.position=function (box) {
        var l = ($(window.document).width() - box.width()) / 2;
        box.css({ 'left': l, 'opacity': 1, 'top': 10 });
    };

    /**
    * 出错信息
    **/
    msg.prototype.error= function (msg, ind) {
        this.show(msg, 'openflow.error', 'alert_error', ind,false,true);
    };

    /**
    * 警告信息
    **/
    msg.prototype.warning= function (msg, ind) {
        this.show(msg, 'openflow.warning', 'alert_warning', ind,false,true);
    }

    /**
    * 成功信息
    **/
    msg.prototype.success = function (msg, ind) {
        this.show(msg, 'openflow.warning', 'alert_success', ind,false,true);
    }

    /**
    * 提示信息
    **/
    msg.prototype.tip= function (msg, ind) {
        return this.show(msg, 'openflow.tooltip', 'alert_info', ind,false,true);
    }

    /*
    * 弹出等待框
    */
    msg.prototype.showWaiting = function (msg,key)    {
        return this.show('<div class="jm_wait">'+msg+'</div>', key || 'openflow.waiting', 'alert_info', true, true,false);
    }
    return new msg();
})();
