<?php
    $this->load->view("common/flowside.php");
?>
    <div id="editor-toolbar"></div>
    <div id="editor-body">
        <div id="editor-body-left">
            <div class="right_shadow"></div>
        </div>
        <div id="editor-body-right">
            <div id="editor-parent">
                 <div id="editor" class="left"></div>
            </div>
        </div>
    </div>

    <div id="taskServerContainer" class="task-server-container" style="display:none;">
        <ul class="statusMark">
            <li class="stoped">被停止</li>
            <li class="suspend">暂停中</li>
            <li class="success">运行成功</li>
            <li class="failed">运行失败</li>
            <div class="clear"></div>
        </ul>
        <ul id="task-servers"></ul>
    </div>
</body>
