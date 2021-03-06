<?php
    $this->load->view("common/main");
?>

<view1>
    <script type="text/javascript">
        var frameHandle;
        function frameAutoHeight(frame) {
            try
            {
                frameHandle = frame || (frame = frameHandle) || (frame=document.getElementById('mainframe'));
                var doc = frame.contentDocument || frame.document;
                var h = Math.max($(doc).height(), 500);
                if (doc.body.offsetHeight) {
                    var tmp = doc.body.offsetHeight;
                    h = h < tmp ? tmp : h;
                }
                else {
                    var tmp = doc.body.scrollHeight;
                    h = h < tmp ? tmp : h;
                }
                //h = h < $('#main').height() ? $('#main').height() - 20 : h;
                frame.height = h + 40;
                //frameHandle = setTimeout(frameAutoHeight, 1000, frame);

            }
            finally {
                $(frame).show();
                $('#main .jm_wait').hide();
                if (layoutSizeChange) layoutSizeChange();
            }
        }

        //内容区域全屏
        function fullscreen() {
            var r = true;
            if ($('#main').attr('class') == 'fullscreen') {
                $('#main').attr('class', 'column rightSide left');
                $('#sidebar').show();
                r = false;
            }
            else {
                $('#main').attr('class', 'fullscreen');
                $('#sidebar').hide();
            }
            layoutSizeChange();
            return r;
        }
        $(function () {
            $('#txtquicksearch ').parent().bind('submit', function () {
                return redirect('<%=rootUrl %>/template.js?key=' + encodeURIComponent($('#txtquicksearch').val()), '模板');
            });
            var lnk = window.location.hash;
            if(lnk) {
                var src=$(lnk).attr('href');
                var title = $(lnk).html();
                redirect(src,title);
            }
        })
    </script>
</view1>

<view2>

    <aside id="sidebar" class="leftSide column left">
		<h3>流程管理</h3>
		<ul class="toggle">
			<li class="icn_create_template">
                 <a href="/flows/editor?mode=0" target="_blank">创建模板</a>
            </li>
			<li class="icn_template"><a href="<%=rootUrl %>/template.js" id="lnkTemplate" target="mainframe">模板</a></li>
			<li class="icn_instance"><a href="<%=rootUrl %>/instance.js" id="lnkInstance" target="mainframe">实例</a></li>
             <li class="icn_clock"><a href="<%=rootUrl %>/clockList.js" id="lnkClockList" target="mainframe">定时任务</a></li>
            <!--<li class="icn_clock"><a href="<%=rootUrl %>/clock.js" id="lnkclock" target="mainframe">创建定时任务</a></li>-->
             <li class="icn_setting">
                <a href="<%=rootUrl %>/profile/myTemplate.js" target="mainframe">我的模板</a>
            </li>

            <li class="icn_setting">
                <a id="mycategory" href="<%=rootUrl %>/profile/category.js" target="mainframe">分类管理</a>
            </li>
        </ul>
		<h3>系统管理</h3>
		<ul class="toggle">
			<li class="icn_container">
                  <a href="<%=rootUrl %>/container.js" target="mainframe">Master管理</a>
            </li>
			<li class="icn_container">
                  <a href="<%=rootUrl %>/container.js" target="mainframe">Agent管理</a>
            </li>
            <li class="icn_permission">
                  <a href="<%=rootUrl %>/system/permission.js" target="mainframe">用户权限</a>
            </li>
            <li class="icn_permission">
                 <a href="<%=rootUrl %>/system/userLog.js" target="mainframe">访问记录</a>
            </li>
            <li class="icn_setting">
                  <a href="<%=rootUrl %>/system/config.js" target="mainframe">系统设置</a>
            </li>
		</ul>

		<footer>
			<hr />
			<p><strong>&copy; 2012-
            <script type="text/javascript">
            var thisyear=(new Date()).getFullYear();document.write(thisyear);
            </script> OpenFlow</strong></p>
		</footer>
	</aside><!-- end of sidebar -->

	<section id="main" class="column rightSide left">
		<article class="module">
                <iframe id="mainframe" name="mainframe" frameborder="0" style="border:0;margin:0 4px;width:100%;display:block;overflow:hidden;background-color:transparent;"
                    src="/flows/instance" onload="frameAutoHeight(this)"></iframe>
            <div class="jm_wait">加载中...</div>
				<div class="clear"></div>
		</article><!-- end of stats article -->
	</section>
</view2>
