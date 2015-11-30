<?php
    $this->load->view("common/detail");
?>

<view1>
    <script src="../content/js/openflow.instance.js?ver=<%= version%>"></script>
    <script type="text/javascript">
        $(function () {
            //var pageindex = 1;//当前页码
            $('#pager').paging({
                showCount: 6, //当前页附近显示多少页码，比如，4或5表示前后显二个页码，6表示前后显示三个,都会除以2
                change: function (p) {
                    search(p);
                }
            });

            $('#tbInstances').datagrid({
                columns:[[{field:'Name',title:'名称',sortable:true},{field:'TemplateId',title:'模板',align:'left',sortable:true},{field:'Ip',title:'IP地址',sortable:true,width:100},{field:'State',title:'状态',sortable:true},{field:'CreateUser',title:'创建人'},{field:'CreateOn',title:'创建时间'}]],
                singleSelect:true,
                onSortColumn:function(sort,order) {
                    if(sort == 'Ip') sort = 'b.' + sort;
                    else sort = 'a.' + sort;
                    search(0,sort,order);
                }
            });

            openflow.root = '../';
            //$('#instances').html('<div class="jm_wait">加载中...</div>');
            $('#btnsearch').click(function() {
                //instanceTB.sort.fields= [];
                search(0);
            });
            //默认查询参数
            var params = openflow.getParams();
            var ip = params["ip"];
            if (ip) {
                ip = decodeURIComponent(ip);
                $('#txtip').val(ip);
            }
            search(0);
            resetParentHeight();

            for(var k in openflow.instanceStatus) {
                var v = openflow.instanceStatus[k];
                if(typeof v == 'number') {
                    $('<option value="'+v+'">'+openflow.instanceStatus.getStateName(v)+'</option>').appendTo('#txtstate');
                }
            }

        });
        function search(p,sort,order) {
                p = Number(p) || 1;
                var ip = $('#txtip').val();
                var key = $('#txtkey').val();
                var state= $('#txtstate').val();

                var wait = openflow.message.showWaiting('查询中...');
                var orderby = sort || '';//instanceTB.sort?instanceTB.sort.toString():'';
                if(sort && order) orderby = sort + ' ' + order;
                openflow.request('post', {
                    ip: ip,
                    page: p,
                    state:state,
                    count: 20,
                    orderby:orderby,
                    des: key,
                    url: openflow.root + 'servers/instanceServer.js?cmd=search'
                }, function (data,err) {
                    wait.close();
                    if (data) {
                        createTable(data.data,sort,order);

                        $('#pager').paging({ index: data.page, count: data.pageCount });
                    }
                    else if(err) {
                        $('#instances').html(err.toString());
                    }
                    //重置父页高度
                    resetParentHeight();
                });
            }

            /*var instanceTB = new $jm.table({
                    parent: 'instances',
                    id: 'tbinstances',
                    className: 'jm-table fullarea',
                    sortChange:function(sort) {
                        search(0);
                    },
                    header: [{value:"名称",name:'a.Name'}, {value:"模板",'name':'a.TemplateId'},{value:"IP地址",name:'b.Ip'},{value:'状态',name:'a.State'},"说明",{value:"创建人",name:'a.CreateUser'},{value:"创建时间",name:'a.CreateOn'}]
                });*/

            //生成模板表格
            function createTable(data,sort,order) {
                /*instanceTB.rows=[];
                if (data) {
                    for (var i = 0; i < data.length; i++) {
                        var opname='作废';
                        var mname='invalidInstance';
                        var opclass= 'cell_del';
                        if(data[i].Flag==1) {
                            opname= '启用';
                            mname='enableInstance';
                            opclass = 'cell_restore';
                        }
                        var namecell='';
                        var statename = openflow.instanceStatus.getStateName(data[i].State);
                        //未运行,停止，失败，成功 才可以更改状态
                        if(data[i].State == openflow.instanceStatus.Init ||
                            data[i].State == openflow.instanceStatus.Stopped ||
                            data[i].State == openflow.instanceStatus.Faild||
                            data[i].State == openflow.instanceStatus.Success) {
                            namecell+= '<a href="#" class="cell_op '+opclass+'" onclick="javascript:return '+
                                        mname +'('+data[i].Id+')" title="'+opname+'">&nbsp;</a>';
                        }
                        else {
                            opclass = 'cell_del_g';
                             namecell+= '<a title="'+statename+'状态下无法删除" class="cell_op '+opclass+'">&nbsp;</a>';
                        }
                        if('<%=user.permissions.check("admin")%>' == 'true') {
                             namecell += '<a  class="cell_op cell_setstate" onclick="javascript:return setInstanceState(this,'+data[i].Id+',\''+data[i].State+'\');" title="更改状态">&nbsp;</a>';
                        }

                        namecell += '&nbsp;<a href="/editor.js?mode=3&id=' + data[i].TemplateId + '&instance=' + data[i].Id + '"  target="_blank">(' +data[i].Id+ ')&nbsp;' +
                        (data[i].Name || '') + '</a>';
                        instanceTB.insertRow([
                        {
                            value: namecell,
                            style: 'width:200px;'
                        }, {
                            value: ('<a href="/editor.js?mode=0&id=' + data[i].TemplateId + '" target="_blank">' + data[i].TemplateName + '</a>') || '',
                            style: 'width:120px;'
                        }, {
                            value: data[i].Ip || '',
                            style: 'width:120px;'
                        },
                        {
                            value: statename,
                            style: 'width:60px'
                        },
                        data[i].Description || '',
                        data[i].CreateUser,
                        $jm.formatDate(data[i].CreateOn,'yyyy-MM-dd HH:mm:ss')]);
                    }
                }
                instanceTB.render();*/
                for (var i = 0; i < data.length; i++) {
                        var opname='作废';
                        var mname='invalidInstance';
                        var opclass= 'cell_del';
                        if(data[i].Flag==1) {
                            opname= '启用';
                            mname='enableInstance';
                            opclass = 'cell_restore';
                        }
                        var namecell='';
                        var statename = openflow.instanceStatus.getStateName(data[i].State);
                        //未运行,停止，失败，成功 才可以更改状态
                        if(data[i].State == openflow.instanceStatus.Init ||
                            data[i].State == openflow.instanceStatus.Stopped ||
                            data[i].State == openflow.instanceStatus.Faild||
                            data[i].State == openflow.instanceStatus.Success) {
                            namecell+= '<a href="#" class="cell_op '+opclass+'" onclick="javascript:return '+
                                        mname +'('+data[i].Id+')" title="'+opname+'">&nbsp;</a>';
                        }
                        else {
                            opclass = 'cell_del_g';
                             namecell+= '<a title="'+statename+'状态下无法删除" class="cell_op '+opclass+'">&nbsp;</a>';
                        }
                        if('<%=user.permissions.check("admin")%>' == 'true') {
                             namecell += '<a  class="cell_op cell_setstate" onclick="javascript:return setInstanceState(this,'+data[i].Id+',\''+data[i].State+'\');" title="更改状态">&nbsp;</a>';
                        }
                        if(data[i].Flag!=1) {
                           namecell += '<a class="cell_op cell_clock" onclick="javascript:turnclock(this);" target="clockframe" href="/clock.js?instance='+data[i].Id+'&instancename='+(data[i].Name || '')+'" title="设为定时任务">&nbsp;</a>';
                       }
                        namecell += '&nbsp;<a href="/editor.js?mode=3&id=' + data[i].TemplateId + '&instance=' + data[i].Id + '"  target="_blank">(' +data[i].Id+ ')&nbsp;' +
                        (data[i].Name || '') + '</a>';
                        data[i].Name = namecell;
                        data[i].State = statename;
                        data[i].TemplateId = '<a href="/editor.js?mode=0&id=' + data[i].TemplateId + '" target="_blank">(' +data[i].TemplateId + ')&nbsp;' + data[i].TemplateName + '</a>';
                        data[i].CreateOn = $jm.formatDate(data[i].CreateOn,'yyyy-MM-dd HH:mm:ss');
                    }
                $('#tbInstances').datagrid({data:data,sortName:sort,sortOrder:order});
            }

        //作废
        function invalidInstance(id) {
            if(confirm('确定作废?')){
                var wait = openflow.message.showWaiting('操作中...');
                openflow.request('post', {
                        id: id,
                        url: '/servers/instanceServer.js?cmd=invalid'
                    }, function (data) {
                        wait.close();
                        if (data) {
                            openflow.message.success('操作成功');
                            search(0);
                        }
                        else {
                            openflow.message.error('操作失败');
                        }
                });
            }
            return false;
        };
        //启用
        function enableInstance(id) {
            if(confirm('确定启用?')){
                var wait = openflow.message.showWaiting('操作中...');
                openflow.request('post', {
                        id: id,
                        url: '/servers/instanceServer.js?cmd=enable'
                    }, function (data) {
                        wait.close();
                        if (data) {
                            openflow.message.success('操作成功');
                            search(0);
                        }
                        else {
                            openflow.message.error('操作失败');
                        }
                });
            }
            return false;
        };
        //停止
        function setInstanceState(target,id,state) {
            if(!window.setStateContainer) {
                window.setStateCache = {};
                window.setStateCache.container = $('<div style="position:absolute;display:none;padding:6px 10px;border:1px solid black;background-color:white;height:60px;"></div>').appendTo('body');
                var opcontainer = $('<div style="margin:4px auto;"></div>').appendTo(window.setStateCache.container);
                var options = $('<select></select>').appendTo(opcontainer);
                for(var n in openflow.instanceStatus) {
                    var s = openflow.instanceStatus[n];
                    if(typeof s !== 'function') {
                        $('<option value="'+s+'">'+openflow.instanceStatus.getStateName(s)+'</option>').appendTo(options);
                    }
                }
                var buttons = $('<div style="margin:4px auto;"></div>').appendTo(window.setStateCache.container);
                $('<button>确定</button>').appendTo(buttons).click(function() {
                    var wait = openflow.message.showWaiting('操作中...');
                    var st = options.val();
                    openflow.request('post', {
                            id: window.setStateCache.id,
                            state:st,
                            url: '/servers/instanceServer.js?cmd=updatestate'
                        }, function (data) {
                            wait.close();
                            if (data) {
                                openflow.message.success('操作成功');
                                window.setStateCache.container.hide();
                                search(0);
                            }
                            else {
                                openflow.message.error('操作失败');
                            }
                    });
                });
                $('<button>取消</button>').appendTo(buttons).click(function(){
                    window.setStateCache.container.hide();
                });
                /*$(window.document).bind('mouseup',function(){
                    if(event.target == window.setStateCache.container[0]) return;
                    window.setStateCache.container.hide();
                });*/
            }
            window.setStateCache.id = id;
            window.setStateCache.container.find('select').val(state);
            var p = $(target).offset();
            window.setStateCache.container.css({
                top:p.top,
                left:p.left + $(target).width()
            });
            window.setStateCache.container.show();
            return false;
        };
        /**
    * 转换为定时任务
    */
    function turnclock(target) {
        var p = $(target).offset();
        var top = p.top;
        var left = p.left;
        $('#turnClock').window({title:'创建定时任务',width:600,height:420,top:top,left:left});
    }
    </script>
</view1>
<view2>
    <div id="searchwhere">
        <div>
        <label for="ip">IP地址</label>
        <input id="txtip" name="ip" type="text" value="" maxlength="20" placeholder="请输入要查询的ip地址" />
        </div>

        <div>
        <label for="des">关健词</label>
        <input id="txtkey" name="des" type="text" value="" maxlength="16" placeholder="关健词" />
        </div>
        <div>
        <label for="state">状态</label>
        <select id="txtstate" name="state" type="text">
            <option value="-1">全部</option>
        </select>
        </div>
        <div>
            <button value="查询" id="btnsearch" >查询</button>
            </div>
        <div class="clear"></div>
    </div>
    <div id="instances"><table id="tbInstances"></table></div>
    <div id="pager"></div>
    <div id="turnClock" class="easyui-widnow" data-options="iconCls:'icon-clock',modal:true">
        <iframe name="clockframe" frameborder="0" scrolling="no" height="98%" width="100%"></iframe>
    </div>
</view2>
