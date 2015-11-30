<!DOCTYPE html>

<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>openflow</title>

        <link href="../content/css/base.css" rel="stylesheet" />
        <link href="../content/css/layout.css" rel="stylesheet" />

        <link href="../content/jm/css/jm.win.css"
            rel="stylesheet" />
</head>
<body>
    <!--[if lt IE 9]>
       <div style="background-color:#F7FCD1;border:1px solid #EDF5A0;padding:6px;color:#1A436C;font-size:16px;text-align:center;">您的浏览器为ie,当前系统不支持ie9以下的版本。请使用chrome,firefox或ie9及以上版本<a href="#" onclick="javascript:this.parentElement.style.display='none';" style="color:blue;margin:10px;">X</a></div>
        <![endif]-->
    <!-- header view -->
     <script src="../content/js/jquery.min.js"></script>
        <script src="../content/js/openflow.js?v=2"></script>
        <script src="../content/js/openflow.message.js"></script>
        <script src="../content/js/openflow.base.js?ver=<%= version%>"></script>
    <view1>
    </view1>

    <header id="header">
        <hgroup>
            <h1 class="leftSide site_title"><a href="#">OpenFlow</a></h1>
            <div class="rightSide left">
                <h2 class="section_title">图形化程序语言/分布式工作流引擎</h2>
                <div class="btn_view_site"><a href="#" class="help">帮助中心</a></div>
            </div>
        </hgroup>
    </header>
    <!-- end of header bar -->

    <section id="secondary_bar">
        <div class="leftSide user">
            <p>Helight</p>
            <!-- <a class="logout_user" href="#" title="Logout">Logout</a> -->
        </div>
        <div class="breadcrumbs_container rightSide left">
            <article class="breadcrumbs">
                <div class="breadcrumb_divider"></div>
                <a class="current">openflow</a>
            </article>
        </div>
        <div class="clear"></div>
    </section>
    <!-- end of secondary bar -->

    <!--content view-->
    <view2></view2>

    <script type="text/javascript">
        openflow.version = '1.0';
        /**
        * 排版
        */
        function layoutSizeChange() {
            var w = Math.max($(window).width(),1138) - $('.leftSide').width() - 4;
            $('.rightSide').width(w);
            if ($('#main').attr('class') == 'fullscreen') {
                $('#main').width($(window).width() - 6);
            }

            var h = $(document).height() - $('#header').height() - $('#secondary_bar').height();
            var columns = $('.column');
            if (columns.get(0).style['min-height'] != undefined) {
                columns.css('min-height', h);
            }
            else {
                columns.height(h);
            }
        }

        //跳转页面
        function redirect(url,title) {
            $('#mainframe').hide();
            $('#main .jm_wait').show();
            $('#mainframe').attr('src', url);
            if (title) $('.breadcrumbs .current').html(title);
            return false;
        }

        $(function () {
            layoutSizeChange();
            $(window).bind('resize', layoutSizeChange);
            //setInterval(layoutSizeChange, 100);
            //左边菜单点击事件
            $('ul[class=toggle] li a').click(function () {
                var href = $(this).attr('href');
                href += (href.indexOf('?') > -1 ? '&ver=' : '?ver=') + openflow.version + '&rnd=' + Math.random();
                //跳转
                if($(this).attr('target') == '_blank') {
                    return true;
                }
                redirect(href, $(this).html());
                $('ul[class=toggle] li').toggleClass('current', false);
                $(this).parent().toggleClass('current', true);
                return false;
            });
            $('#main .jm_wait').hide();
        })
    </script>
</body>
</html>
