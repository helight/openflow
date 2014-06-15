
/**
 * 模板操作
 */
openflow.template= {
    /**
    * 作废
    */
    invalidTemplate: function(id,callback) {
        if(confirm('确定废弃当前模板?')) {
            var wait = openflow.message.showWaiting('操作中...');
            openflow.request('post', {
                url:'/servers/templateServer.js?cmd=invalid',
                id: id
            }, function (data,err) {
                wait.close();
                if (data) {
                    openflow.message.success('操作成功');
                    callback(1);
                }
                else if(err) {
                    openflow.message.error(err || '操作失败');
                    callback(0);
                }
            });
        }
        return false;
    },
    /**
    * 恢复
    */
    enableTemplate: function (id,callback) {
        var wait = openflow.message.showWaiting('操作中...');
        openflow.request('post', {
            url:'/servers/templateServer.js?cmd=enable',
            id: id
        }, function (data,err) {
            wait.close();
            if (data) {
                openflow.message.success('操作成功');
                callback(1);
            }
            else if(err){
                openflow.message.error(err || '操作失败');
                callback(0);
            }
        });
        return false;
    },

    /**
    * 复制当前模板
    */
    copyTemplate: function(id,name,des,callback) {
        var editcontent = $('<ul class="edit-container" />');
        var lblname = $('<span for="txtname">名称</span>');
        var txtname = $('<input type="text" maxlength="32" />');
        txtname.val(name + '_副本');

        var lbldes = $('<span for="txtdes">描述</span>');
        var txtdes = $('<textarea type="text" maxlength="256"/>');
        txtdes.val(des);

        var nameli = $('<li></li>').appendTo(editcontent);
        lblname.appendTo(nameli);
        txtname.appendTo(nameli);
        var desli = $('<li></li>').appendTo(editcontent);
        lbldes.appendTo(desli);
        txtdes.appendTo(desli);
        editcontent.dialog({
                title: '复制模板', //标题
                modal:true,
                width: 600,
                top:$(this).offset().top,
                buttons:[
                    {
                        text: '确定',
                        handler: function () {
                            var nv = $.trim(txtname.val());
                            if (nv == '') {
                                openflow.message.warning('名称不可为空');
                                txtname.focus();
                                return;
                            }
                            var des = $.trim(txtdes.val());
                            if (des == '') {
                                openflow.message.warning('描述不可为空');
                                txtdes.focus();
                                return;
                            }

                            var wait = openflow.message.showWaiting('复制中...');
                            openflow.request('post', {
                                url:'/servers/templateServer.js?cmd=copy',
                                id: id,
                                name: nv,
                                des:des
                            }, function (data,err) {
                                wait.close();
                                if (data) {
                                    editcontent.dialog('close');
                                    openflow.message.success('复制成功');
                                    callback(1);
                                }
                                else if(err) {
                                    openflow.message.error(err || '复制失败');
                                    callback(0);
                                }
                            });
                        }
                    }, {
                        text: '取消',
                        handler: function () {
                            editcontent.dialog('close');
                        }
                    }
                ]
            });
        return false;
    }
};

/**
 * 模板选择器
 */
openflow.templateSelecter = function(params,callback) {
    if(typeof params === 'function') {
        callback = params;
        params = null;
    }
    this.callback = callback;
    var container = $('<div></div>');
    var tb = $('<div></div>').appendTo(container);
    var pager = $('<div id="template_pager2"></div>').appendTo(container);
    this.params = params;

    var _this = this;
    window.selecter_linkTemplate = function(id,name,template) {
        if(_this.callback) _this.callback(id,name,template);
        container.window('close');
        return false;
    };

    /**
     * 查询实例
     */
    function search(p) {
        p = Number(p) || 1;
        openflow.request('post', {
            page: p,
            flag: 0,
            count: 10,
            templateId:_this.params?_this.params.template:'',
            user:openflow.user,
            url: openflow.root + 'servers/instanceServer.js?cmd=search'
        }, function (data,err) {
            if (data) {
                createTable(data.data);
                pager.paging({ index: data.page, count: data.pageCount });
            }
            else if(err) {
                container.html(err.toString());
            }
            //重置父页高度
            resetParentHeight();
        });
    }

    var instanceTB = new $jm.table({
            parent: tb,
            id: 'tbinstances',
            className: 'jm-table fullarea',
            sortChange:function(sort) {
                search(0);
            },
            header: [{value:"名称",name:'a.Name'}, {value:"模板",'name':'a.TemplateId'},{value:"IP地址",name:'b.Ip'}]
        });
    pager.paging({
        showCount: 6, //当前页附近显示多少页码，比如，4或5表示前后显二个页码，6表示前后显示三个,都会除以2
        change: function (p) {
            search(p);
        }
    });
    //生成模板表格
    function createTable(data) {
        instanceTB.rows=[];
        if (data) {
            for (var i = 0; i < data.length; i++) {

                var namecell = '<a href="#" onclick="javascript:return selecter_linkTemplate(' + data[i].Id +',\''+data[i].Name+'\','+data[i].TemplateId+');">' +
                (data[i].Name || '') + '</a>';
                instanceTB.insertRow([
                {
                    value: namecell,
                    style: 'width:200px;'
                }, {
                    value: data[i].TemplateName || '',
                    style: 'width:120px;'
                }, {
                    value: data[i].Ip || '',
                    style: 'width:120px;'
                }]);
            }
        }
        instanceTB.render();
    }
    this.show = function(left,top,callback) {
        if(typeof left === 'function') {
            callback = left;
            left = 100;
        }
        if(callback) this.callback = callback;

        left = left || 100;
        top = top || 100;
        search(0);
        //instancewin.show();
        container.window({title:'选择模板',left:left,top:top,width:600,height:350,modal:true});
    }
}
