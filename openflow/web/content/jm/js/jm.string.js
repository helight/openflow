//去除字符串前后的字符
$jm.trim = function (s) {
    return s.replace(/(^\s*)|(\s*$)/g, "");
}
//是否为空
$jm.isNullorEmpty = function (s) {
    if (!s) return true;
    if (typeof (s) == 'string') {
        s = this.trim(s); //去除前后空格
        return s == '';
    }
    return false;
}

//转换json对象
$jm.json = function (data) {
    if (typeof (data) == 'string') data = eval(data);
    return data;
}

//检 查地址是否带参数
//如果已带则直接加上&号..否则加上?
$jm.checkUrlParMark = function (url) {
    if (url) {
        url = url.indexOf('?') > -1 ? url + '&' : url + '?';
    }
    return url;
};

//解析json为url参数
//@js 为json字符串或json对象
$jm.jsToUrl = function (js) {
    if (!js) return '';
    js = $jm.json(js);
    var re = '';
    //遍历组合参数
    for (var j in js) {
        re += j + '=' + encodeURI(js[j]) + '&';
    }
    //去除最后的&符号
    if (re.length > 0) {
        re = re.substring(0, re.length - 2);
    }
    return re;
};
