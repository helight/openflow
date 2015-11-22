/**
* ======================= <br />
* 脚本样式文件的异步加载 <br />
* ======================= <br />
* @namespace $jm
* @require $jm
**/

/**
* 异步加载脚本
* @method load
* @param {String} url 脚本文件URL地址
* @param {Function} callback 加载后回调
* @for $jm
* @return void
**/
$jm.load = function (url, callback) {
    $.ajax({
        url: url,
        type: "get",
        success: function (js) {            
            if (callback) callback(js); //调用回调
        }
    });
}

/**
* 异步加载样式
* @method loadCss
* @param {String} url 样式URL地址
* @param {String} mark 唯一标识,如果相同会覆盖
* @param {Function} callback 加载后回调
* @return void
* @for $jm
**/
$jm.loadCss = function (url, mark, callback) {
    $.ajax({
        url: url,
        type: "get",
        success: function (css) {
            var style = mark ? $('#' + mark) : $('div[url="' + url + '"]');
            if (!style || style.length == 0) {
                style = $('<div style="display:none;"></div>');
                if (mark) style.attr('id', mark);
                style.attr('url', url);
                style.appendTo('body');
            }
            style.html('<style>' + css.replace(/~\//ig, $jm.rootUrl) + '</style>');
            if (callback) callback(); //调用回调
        }
    });
};

/**
* 异步加载图片
* @method loadImg
* @param {String} url 图片URL地址
* @param {Function} callback 加载后回调
* @return void
* @for $jm
**/
$jm.loadImg = function (url, callback) {
    var img = new Image();
    img.onload = callback;
    img.src = url;
};