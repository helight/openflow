


/**
* 重置父级高度
*/
function resetParentHeight() {
    if (window.parent && window.parent != window && window.parent.frameAutoHeight) {
        window.parent.frameAutoHeight();//自增高
    }
}
/**
* 重新登录
*/
function reLogin() {
    if (window.parent && window.parent != window && window.parent.reLogin) {
        window.parent.reLogin();
    } else {
        window.location =openflow.checkUrl('/login');
    }
}
