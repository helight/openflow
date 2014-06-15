/**
* 工具栏处理
*/
openflow.editor.prototype.toolbar = (function () {
    /**
    * 工具栏对象
    */
    function toolbar() {
        this.items = [];
    }

    /**
    * 初始化工具栏
    */
    toolbar.prototype.init = function (option) {
        this.option = option;
        //如果没有指定容器。则创建一个，
        if (!this.option.container) {
            this.option.container = document.createElement('ul');
            $(this.option.container).attr('id','editor-component');
            document.getElementById('editor-body-right').appendChild(this.option.container);
        }
        //设定样式
        if (this.option.style) {
            openflow.css(this.option.container, this.option.style);
        }

        //当选中一个元素后，把所以工具栏都禁用
        var items = this.items;
        //添加到左边框s
        option.editor.leftToolbar.add('组件',this.option.container,true);
    }

    /**
    * 添加工具项
    * @method add
    * @param {String} icon 工具图标
    * @param {Number} w 宽度
    * @param {Number} h 高度
    * @param {Object} style 样式
    * @public
    */
    toolbar.prototype.add = function (icon, style, label,value,container) {
        var li = document.createElement('li');

        //确定拖放时的图片对象
        var styleInfo = this.option.editor.editor.styles[style] || style;
        var bigImg = document.createElement('img');
        bigImg.src = styleInfo.image || icon;
        bigImg.style['display'] = 'none';
        li.appendChild(bigImg);

        //li.style['background'] = 'url('+icon+') 0 50% no-repeat';
        //li.style['padding-left'] = '32px';
        var smallImg = document.createElement('div');

        smallImg.style['background'] = 'url('+icon+') 50% 50% no-repeat';
        li.appendChild(smallImg);

        this.option.editor.editor.regComponent(li,{style:style,value:value});
        var name = document.createElement('div');
        name.className = 'name';
        name.innerHTML = label;
        li.appendChild(name);
        container = container || this.option.container;
        container.appendChild(li);
        this.items.push({ img: bigImg });
        return bigImg;
    }

    //加载我的组件
    toolbar.prototype.loadMyComponents= function(style) {
        var banner= new openflow.expandBanner('#editor-body-left');
        var container = $('<ul class="mycomponent"><div class="jm_wait">loading...</div></ul>');
        banner.add('我的组件',container,true);
        var self = this;
        openflow.request('post', {
            url:'/servers/componentServer.js?cmd=search',
            page: 1,
            count:1000,
            type:1,
            orderby:'Name',
            creater:openflow.user
        }, function (data,err) {
            container.html('');
            if (data) {
               for(var i=0;i < data.data.length;i++) {
                    var com = data.data[i];

                    //为组件设定样式
                    style = jmUtils.clone(style);
                    style.image = '/content/primitive/icon/install.png';
                    style.resizable = true;
                    style.width = 105;
                    style.height = 65;
                    var stylename = 'component_' + com.Id;
                    self.option.editor.editor.regStyle(stylename, style);
                    self.option.editor.editor.regStyle(stylename + '-selected', style);
                    self.option.editor.editor.regStyle(stylename + '-loading', style);
                    self.option.editor.editor.regStyle(stylename + '-init', style);
                    self.option.editor.editor.regStyle(stylename + '-running', style);
                    self.option.editor.editor.regStyle(stylename + '-suspending', style);
                    self.option.editor.editor.regStyle(stylename + '-stopped', style);
                    self.option.editor.editor.regStyle(stylename + '-stopping', style);
                    self.option.editor.editor.regStyle(stylename + '-error', style);
                    self.option.editor.editor.regStyle(stylename + '-success', style);

                    self.add('/content/primitive/icon/install.png', stylename, com.Name,com.Name,container[0]);
               }
            }
            else if(err){
                container.html(err.toString());
            }
        });
    }
    return new toolbar();
})();

/**
=======
* 编辑器左工具栏
*/
openflow.editor.prototype.leftToolbar= (function() {
    var banner= new openflow.expandBanner('#editor-body-left');
    return banner;
})();

/**
* 显示浮动工具
*/
openflow.editor.prototype.showTools = function () {
    var content= $('#editor-toolbar');
    if(content.length == 0) {
        content = $('<div id="editor-toolbar"/>').appendTo('body');
    }

    var _this = this;
    //var tb = new mxToolbar(content[0]);
    var toolbarContainer= $('<ul class="toolbar_container"></ul>').appendTo(content);
    $('<div class="clear"></div>').appendTo(content);
    //当前编辑添加工具栏
    this.addTool = function (title, img, callback,container) {
        img = openflow.checkUrl(img);
        //var item = tb.addItem(title, openflow.checkUrl(mxContentPath, img), callback);
        //$(item).width(16).height(16);
        var item= $('<li></li>').appendTo(container || toolbarContainer);
        var itemimg= $('<img src="'+img+'" />').appendTo(item);
        var itemlnk= $('<a href="#">'+title+'</a>').appendTo(item);
        //itemlnk.hide();
        item.click(function() {
            callback.call(this);
            return false;
        });
        return item;
    }
    /**
    * 移除工具项
    */
    this.removeTool = function (item) {
        $(item).remove();
    }
    this.addTool('放大', 'content/images/toolbar/zoom_in.png', function (evt) {
        _this.editor.graph.zoomIn();
    });
    this.addTool('缩小', 'content/images/toolbar/zoom_out.png', function (evt) {
        _this.editor.graph.zoomOut();
    });
    this.addTool('复原', 'content/images/toolbar/original.png', function (evt) {
        _this.editor.graph.zoomActual();
    });

    if (openflow.editable && this.editor.isEnabled()) {
        this.addTool('撤消', 'content/images/toolbar/undo.png', function (evt) {
            _this.editor.execute('undo');
        });
        this.addTool('恢复', 'content/images/toolbar/redo.png', function (evt) {
            _this.editor.execute('redo');
        });

        /*this.addTool('左对齐', 'content/images/toolbar/left.png', function (evt) {
            _this.editor.align('left');
        });
        this.addTool('顶部对齐', 'content/images/toolbar/top.gif', function (evt) {
            _this.editor.align('top');
        });*/
        this.addTool('水平居中', 'content/images/toolbar/horizontal_center.png', function (evt) {
            _this.editor.align('center');
        });
         this.addTool('垂直居中', 'content/images/toolbar/vertical_center.png', function (evt) {
            _this.editor.align('middle');
        });

        /*this.addTool('右对齐', 'content/images/toolbar/right.png', function (evt) {
            _this.editor.align('right');
        });
        this.addTool('底部对齐', 'content/images/toolbar/bottom.png', function (evt) {
            _this.editor.align('bottom');
        });*/
    }

    this.addTool('打印', 'content/images/toolbar/print.png', function (evt) {
        window.print();
    });

    this.addTool('导出图片', 'content/images/toolbar/save_as_image.png', function (evt) {
        $('<div style="overflow:auto;"><img src="'+_this.editor.graph.toDataURL()+'" /></div>').window({
            width:600,
            height:500,
            title:'请右击另存为图片'
        });
    });

    this.addTool('描述', 'content/images/toolbar/xml.png', function (evt) {
        $('<div></div>').html(_this.toXml()).window({
            title:'描述',
            width:600,
            height:500,
            minimizable:false
        });
    });

    this.addTool('XML', 'content/images/toolbar/xml.png', function (evt) {
        /*var xmlwin = new $jm.win({
            title: 'xml', //标题
            //parent:this.container,
            modal: false,
            canMax: true, //禁止最大化
            canMin: false,
            resizeable: false,
            //top: 40,
            width: 800,
            height: 600,
            position: 'center',//位置，center=居中,top=顶部,left=左边
            bounds: { left: true, top: true, right: true, bottom: true }//边界锁定,//默认全锁定，此参数为一个json对象：如{left:false,top:true,right:true,bottom:false}//表示锁定顶部与右边，左边与底部可越界
        });
        xmlwin.show();  */
            var xmlwin = $('<div></div>');
            try {
                xmlwin.window({
                    title:'描述',
                    modal:true,
                    width:800,
                    height:600,
                    minimizable:false
                });
                var xml = _this.toTemplateXml(null,true,true);
                xml=xml.replace(/&/g,'&amp;');
                xmlwin.html($jm.code('xml', xml));//窗口内容
            }
            catch (ex) {
                xmlwin.html(ex.toString());
            }
    });

    if(this.editor.isEnabled()) {
        if (openflow.flag != 3) {
            this.addTool('编辑', 'content/images/toolbar/edit.png', function (evt) {
                _this.edit();
            });
        }
       if (openflow.editable) {
           this.addTool('保存', 'content/images/toolbar/save.png', function (evt) {
               _this.save();
            });
        }
    }



    //tb.addItem('Poster Print', openflow.checkUrl(mxContentPath, 'images/press32.png'), function (evt) {
    //    var pageCount = mxUtils.prompt('Enter maximum page count', '1');

    //    if (pageCount != null) {
    //        var scale = mxUtils.getScaleForPageCount(pageCount, editor.graph);
    //        var preview = new mxPrintPreview(editor.graph, scale);
    //        preview.open();
    //    }
    //});

    //全屏
    if (window.parent && window.parent.fullscreen) {
        this.addTool('全屏', 'content/images/toolbar/full_screen.png', function (evt) {
            window.parent.fullscreen();
        });
    }
    /*
    var left = openflow.editable ? 200 : 15;
    var w;
    if ($.browser.msie && $.browser.version <= 7) {
        w = 250;
    }

    var wnd = new mxWindow('工具栏', content, left, 25 ,w);//,250, 60, false
    wnd.setMaximizable(false);
    wnd.setMinimizable(true);
    wnd.setScrollable(false);
    wnd.setResizable(false);
    wnd.setVisible(true);
    return wnd;
    */
}

/**
* 显示全视图
*/
openflow.editor.prototype.showOutLine = function (outline) {
    if (!outline) {
        var outline = document.createElement('div');
        var outlinediv = document.createElement('div')
        outlinediv.className = 'sizeview';
        outlinediv.appendChild(outline);
        //$(outlinediv).appendTo($('#editor-body-left'));
        this.leftToolbar.add('缩略图',outlinediv,true);
    }

    if (mxClient.IS_IE) {
        new mxDivResizer(outline);
    }

    // Creates the outline (navigator, overview) for moving
    // around the graph in the top, right corner of the window.
    var outln = new mxOutline(this.graph, outline);
    return outln;
}
