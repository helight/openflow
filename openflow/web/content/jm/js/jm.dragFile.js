
/**
* 文件上传库
*/
$jm.file = (function () {
    var drag = function (option) {
        this.option = option;
        option.target.get(0).addEventListener('dragleave', this.leave);
        option.target.get(0).addEventListener('dragover', this.over);
        option.target.get(0).addEventListener('dragenter', this.enter);
        option.target.get(0).addEventListener('dragleave', this.exit);
        option.target.get(0).addEventListener('drop', this.drop);
    };
    drag.prototype.leave = function (e) {
        e.preventDefault();
        if (this.option.leave) this.option.leave.call(this.option.target);
    };
    drag.prototype.enter = function (e) {
        e.preventDefault();
        if (this.option.enter) this.option.enter.call(this.option.target);
    };
    drag.prototype.over = function (e) {
        e.preventDefault();
        if (this.option.over) this.option.over.call(this.option.target);
    };
    drag.prototype.exit = function (e) {
        e.preventDefault();
        if (this.option.exit) this.option.exit.call(this.option.target);
    };
    drag.prototype.drop = function (e) {
        e.preventDefault();
        if (this.option.drop) {
            var data = new FormData();
            for (var i in e.dataTransfer.files) {
                data.append('file', e.dataTransfer.files[i]);
            }
            this.option.drop.call(this.option.target, data);
        }
    };
    return drag;
})();