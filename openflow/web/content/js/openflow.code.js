if (!openflow) openflow = {};

/**
* 高亮代码类
*/
openflow.code = function () {

}

/**
* 一次把代码转为html
*/
openflow.textToCode=function (code,mode,callback) {
	var tabSize = CodeMirror.defaults.tabSize;
	var col = 0;
	CodeMirror.runMode(code, mode,
     function(text,style){
     	var node=null;
		if (text == "\n") {
	        node=document.createElement("br");
	        col = 0;
	      }
	      else{
	      var content = "";
	      // replace tabs
	      for (var pos = 0;;) {
	        var idx = text.indexOf("\t", pos);
	        if (idx == -1) {
	          content += text.slice(pos);
	          col += text.length - pos;
	          break;
	        } else {
	          col += idx - pos;
	          content += text.slice(pos, idx);
	          var size = tabSize - col % tabSize;
	          col += size;
	          for (var i = 0; i < size; ++i) content += " ";
	          pos = idx + 1;
	        }
	      }

	      if (style) {
	        node = document.createElement("span");
	        node.className = "cm-" + style.replace(/ +/g, " cm-");
	        node.appendChild(document.createTextNode(content));
	      } else {
	        node=document.createTextNode(content);
	      }
		}
		if(callback)callback(node);
	 });
}
