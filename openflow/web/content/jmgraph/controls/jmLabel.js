
/**
 * 显示文字控件
 * params参数:style=样式，value=显示的文字
 *
 * @class jmLabel
 * @for jmGraph
 * @module jmGraph
 * @param {jmGraph} graph 当前画布
 * @param {object} params 文字控件参数
 */
var jmLabel = (function() {
	function jmLabel(graph,params) {
		if(!params) params = {};		
		var style = params.style || {};
		style.font = style.font || "15px Arial";
		this.type = 'jmLabel';
		// 显示不同的 textAlign 值
		//文字水平对齐
		style.textAlign = style.textAlign || 'left';
		//文字垂直对齐
		style.textBaseline = style.textBaseline || 'middle',
		
		this.graph = graph;
		this.value = params.value;
		this.position(params.position || {x:0,y:0});
		this.width(params.width || 0);
		this.height(params.height  || 0);
		this.initializing(graph.context,style);
	}
	jmUtils.extend(jmLabel,jmControl);//jmPath
	return jmLabel;	
})();

/**
 * 初始化图形点,主要用于限定控件边界。
 *
 * @method initPoints
 * @return {array} 所有边界点数组
 * @private
 */
jmLabel.prototype.initPoints = function() {	
	var location = this.getLocation();
	
	var w = location.width;
	var h = location.height;
	this.points = [{x:location.left,y:location.top}];
	this.points.push({x:location.left + location.width,y:location.top});
	this.points.push({x:location.left + location.width,y:location.top + location.height});
	this.points.push({x:location.left,y:location.top+ location.height});
	return this.points;
}

/**
 * 根据位置偏移画字符串
 * 
 * @method draw
 */
jmLabel.prototype.draw = function() {	
	if(this.value) {
		//获取当前控件的绝对位置
		var bounds = this.parent && this.parent.absoluteBounds?this.parent.absoluteBounds:this.absoluteBounds;
		var textSize = this.context.measureText?this.context.measureText(this.value):{width:15};
		
		var location = this.getLocation();
		var x = location.left + bounds.left;
		var y = location.top + bounds.top;
		//通过文字对齐方式计算起始X位置
		switch(this.style.textAlign) {
			case 'right': {
				x += location.width;
				break;
			}
			case 'center': {
				x += location.width / 2;
				break;
			}
		}
		//通过垂直对齐方式计算起始Y值
		switch(this.style.textBaseline) {
			case 'bottom': {
				y += location.height;
				break;
			}
			case 'hanging':
			case 'alphabetic':
			case 'middle' : {
				y += location.height/2;
				break;
			}

		}
		if(this.style.fill && this.context.fillText) {
			if(this.style.maxWidth) {
				this.context.fillText(this.value,x,y,this.style.maxWidth);
			}
			else {
				this.context.fillText(this.value,x,y);
			}
		}
		else if(this.context.strokeText) {
			if(this.style.maxWidth) {
				this.context.strokeText(this.value,x,y,this.style.maxWidth);
			}
			else {
				this.context.strokeText(this.value,x,y);
			}
		}

		//如果有指定边框，则画出边框
		if(this.style.border) {
			this.context.moveTo(this.points[0].x + bounds.left,this.points[0].y + bounds.top);
			if(this.style.border.top) {
				this.context.lineTo(this.points[1].x + bounds.left,this.points[1].y + bounds.top);
			}
			
			if(this.style.border.right) {
				this.context.moveTo(this.points[1].x + bounds.left,this.points[1].y + bounds.top);
				this.context.lineTo(this.points[2].x + bounds.left,this.points[2].y + bounds.top);
			}
			
			if(this.style.border.bottom) {
				this.context.moveTo(this.points[2].x + bounds.left,this.points[2].y + bounds.top);
				this.context.lineTo(this.points[3].x + bounds.left,this.points[3].y + bounds.top);
			}
			
			if(this.style.border.left) {
				this.context.moveTo(this.points[3].x + bounds.left,this.points[3].y + bounds.top);	
				this.context.lineTo(this.points[0].x + bounds.left,this.points[0].y + bounds.top);
			}	
		}
		
	}	
}
