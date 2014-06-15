/**
* 表格插件
* 
* @param {Object} option json格式参数，
className:表示当前表格采用的样式,
parent:表示表格被加载到哪个容器
id:为当前表格的ID，
header:一个字符串数组，表示当当前表格的表头
* 
* @namespace $jm
* @class $jm.table
**/
$jm.table = function (option) {
    /**
    * 当前表格的ID
    * @property id 
    * @type String
    **/
    this.id = option.id;

    this.option = option;

    //当前表格对象
    //var table = $jm.table.createTable(option.header, option.className);

    /**
    * 表格行的集合
    * 集合中每个行对象都是一个数组，里面为当前行的所有单元格值
    * @property rows 
    * @type Array
    **/
    this.rows = [];

    /**
    * 添加行
    * @function createTable 
    * @param {Object} params 行信息的json格式数据,id为当前行的id,cells为一个表示单元格的数组
    * @param {String} css 表格的样式名，默认为 jm-table
    * @for $jm.table
    * @return 当前行
    **/
    this.insertRow = function (params) {
        if (params) {
            this.rows.push(params);
        }
    };

    /**
    * 展显当前表格
    * @function render 
    * @param {Object} params 行信息的json格式数据,id为当前行的id,cells为一个表示单元格的数组
    * @param {String} css 表格的样式名，默认为 jm-table
    * @for $jm.table
    * @return Object 当前生成的表格对象
    **/
    this.render = function () {
        var table = this.createTable(this.option);
        
        table += '<tbody>';
        for (var i in this.rows) {
            var r = this.rows[i];
            var row = '<tr ';//document.createElement('tr');
           
            if (i % 2 == 0) row += 'class ="even" '; //偶数样式
            if (r.id) row += 'id="' + r.id + '" ';
            row += '>';
            if (r && r.length > 0) {
                for (var i in r) {
                    if(r[i]){
                        row += '<td ';
                        if(r[i].style)row += 'style="' + r[i].style + '" ';
                        if (r[i].className) row += 'class="' + r[i].className + '" ';
                        if (r[i].attr) row += r[i].attr + ' ';
                        row += '>' + (r[i].value == undefined ? r[i] : r[i].value) + '</td>';
                    }
                    else{
                        row += '<td></td>';
                    }
                }
            }
            //table.appendChild(row);
            row += '</tr>';
            table += row;
        }
        table += '</tbody></table>';
        if (option.parent) {
            var p = typeof option.parent === 'string'?document.getElementById(option.parent):option.parent;
            if (p) {
                //加入到父容器中
                //$(table).appendTo(p);
                $(p).html(table);
            }
            this.initEvent();//初始化事件
        }

        return table;    
    };
}

/**
* 初始化一些事件，比较排序事件
*/
$jm.table.prototype.initEvent= function() {
    //获取包含名称的表头.绑定点击事件
    this.sortChange = this.sortChange || this.option.sortChange;
    if(!this.sort) 
        {
            this.sort= {fields:[]};
            this.sort.toString= function() {
                var str='';
                var len= this.fields.length;
                for(var i =len-1; i>=0;i--) {
                    if(typeof this.fields[i] == 'object') {
                        if(str)str += ',';
                        str += this.fields[i].name + ' ' + this.fields[i].type;
                    }
                }
                return str;
            };
        }
    if(this.sortChange) {
        var _this= this;
        $('th[data-name]').click(function(evt) {
            var name = $(this).attr('data-name');            
            var type= 'desc';
            for(var i in _this.sort.fields) {
                if(_this.sort.fields[i].name == name) {                                    
                    type= _this.sort.fields[i].type == 'asc'?'desc':'asc';  
                    delete _this.sort.fields[i];
                    break;
                }
            }

            var sortinfo= {name:name,type:type};
            _this.sort.fields.push(sortinfo);             
            _this.sortChange(_this.sort);
        });
    }
}

/**
* 创建一个表格对象
* @function createTable 
* @param {Array} header 表头的数组
* @param {String} css 表格的样式名，默认为 jm-table
* @for $jm.table
**/
$jm.table.prototype.createTable = function (option) {
    var table = '<table class="'+(option.className || 'jm-table')+'" '+(option.style?'style="'+option.style+'"':'')+'>';
    if (this.option.header && this.option.header.length > 0) {
            //var head = tb.createTHead();
            table += '<thead>';
            for (var i in this.option.header) {
                //var h = document.createElement('th');
                if(this.option.header[i].name)  {
                        table += '<th style="cursor:pointer;" data-name="'+this.option.header[i].name+'">';
                    }
                else {
                    table += '<th>';
                }
                //h.innerHTML = this.option.header[i];
                var thValue = this.option.header[i].value || this.option.header[i];
                table += thValue;
                //head.appendChild(h);
                table += '</th>';
            }
            table += '</thead>';
    }
    return table;
};
