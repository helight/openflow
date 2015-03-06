
/**
 * 分类
 *
 */
openflow.category = {
	data:null,
	get:function(id) {
		if(id && this.data) {
			for(var i =0;i<this.data.length;i++) {
				if(this.data[i].Id == id) {
					return this.data[i];
				}
			}
		}
	},
    //检查分类名是否已存在
    checkName : function(name) {
        if(this.data) {
            for(var i =0;i<this.data.length;i++) {
                if(this.data[i].Name == name) {
                    return true;
                }
            }
        }
    },
	// 生成树控件所需的数据结构
    createDataTree: function (data,parent,deep) {
            deep = deep || 0;
            deep ++;//深度
            parent = parent || 0;
            var ds = [];
            for(var i=0;i<data.length;i++) {
                var d = data[i];
                if(d.Parent == parent) {
                    d.deep = deep;
                    ds.push({
                        'id':d.Id,
                        'text':d.Name,
                        //递归子节点
                        'children':this.createDataTree(data,d.Id,deep)
                    });
                }
            }
            return ds;
        },
    search: function(callback) {
        openflow.request('post', {
                url: openflow.root + 'servers/categoryServer.js?cmd=search',
                user: openflow.user,
                count:0
            }, function (data) {
                if(callback) callback(data?data.data:null);
        });
    },
    add:function(name,parent,callback) {
        openflow.request('post', {
                url: openflow.root + 'servers/categoryServer.js?cmd=insert',
                name: encodeURIComponent(name),
                parent: parent
            }, function (data) {
                if(callback) callback(data);
        });
    },
    update:function(id,name,parent,callback) {
        openflow.request('post', {
                url: openflow.root + 'servers/categoryServer.js?cmd=update',
                id:id,
                name: encodeURIComponent(name),
                parent: parent
            }, function (data) {
                if(callback) callback(data);
        });
    },
    remove:function(id,callback) {
    	openflow.request('post', {
                url: openflow.root + 'servers/categoryServer.js?cmd=remove',
                id: id
            }, function (data) {
                if(callback) callback(data);
        });
    },
    /**
     * 分类选择器
     */
    selecter: {
        container:$('<div style="padding:6px;"><div class="jm_wait"></div></div>'),
        get : function(id,callback) {
            if(this.data) {
                for(var i=0;i<this.data.length;i++) {
                    if(this.data[i].Id == id) {
                        callback(this.data[i]);
                        return;
                    }
                }
                callback(null);
            }
            else {
                openflow.category.search(function(data) {
                    openflow.category.selecter.data = data;
                    if(data) {
                        openflow.category.selecter.get(id,callback);
                    }
                    else {
                        callback(null);
                    }
                });
            }
        } ,
        show: function(left,top,callback) {
            this.container.window({
                title:'选择分类',
                left:left || 100,
                to:top||100,
                modal:true,
                minimizable:false,
                width:500,
                height:320
            });
            function showCategory(data) {
                data = data || openflow.category.selecter.data;
                openflow.category.selecter.data = data;
                if(data && data.length > 0) {
                    data = openflow.category.createDataTree(data,0)
                    var body = $('<ul></ul>').tree({
                        data:data,
                        onSelect: function(node) {
                            if(callback) {
                                openflow.category.selecter.get(node.id,function(category) {
                                    callback(category);
                                });
                            }
                            openflow.category.selecter.container.window('close');
                        }
                    });
                    openflow.category.selecter.container.html('');
                    body.appendTo(openflow.category.selecter.container);
                }
                else {
                    openflow.category.selecter.container.html('<a href="'+openflow.root+'default#mycategory" target="_blank">您还没有分类，请点击此处编辑您的分类。</a>');
                }
            };

            if(this.data) {
                showCategory();
            }
            else {
                //获取当前用户分类
                openflow.category.search(showCategory);
            }
        }
    }
}
