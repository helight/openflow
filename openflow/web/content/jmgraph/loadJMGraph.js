/**
 * 加载整个组件依赖
 *
 * @method load
 * @for jmGraph
 * @param {function} [callback] 回调函数callback为成功或失败后回调
 */
loadJMGraph = function(callback) {
	var jmgraphRoot = '';
	//获取当前graph路径
    var sc = document.getElementsByTagName('script');
    for(var i in sc) {
        var src = sc[i].src;
        var graphindex = src.indexOf('loadJMGraph.js');
        if(graphindex >= 0) {
            jmgraphRoot = src.substring(0,graphindex);
            break;
        }
    }
	require([
			jmgraphRoot + 'common/jmUtils.js',
			jmgraphRoot + 'common/jmModels.js',
			jmgraphRoot + 'common/jmProperty.js',
			jmgraphRoot + 'common/jmEvents.js',
			jmgraphRoot + 'common/jmControl.js',
			jmgraphRoot + 'common/jmSvg.js',
			jmgraphRoot + 'shapes/jmShape.js',
			jmgraphRoot + 'shapes/jmPath.js',
			jmgraphRoot + 'shapes/jmLine.js',
			jmgraphRoot + 'shapes/jmCircle.js',
			jmgraphRoot + 'shapes/jmArc.js',
			jmgraphRoot + 'shapes/jmHArc.js',
			jmgraphRoot + 'shapes/jmPrismatic.js',
			jmgraphRoot + 'shapes/jmBezier.js',
			jmgraphRoot + 'shapes/jmRect.js',
			jmgraphRoot + 'shapes/jmArraw.js',					
			jmgraphRoot + 'controls/jmLabel.js',
			jmgraphRoot + 'controls/jmImage.js',
			jmgraphRoot + 'controls/jmResize.js',
			jmgraphRoot + 'controls/jmArrowLine.js',
			jmgraphRoot + 'jmGraph.js',
			jmgraphRoot + 'editor/jmCell.js',
			jmgraphRoot + 'editor/jmEditor.js'
			],function() {				
				if(callback) setTimeout(callback,10);
			});
	/**
	 * 加载js文件
	 * 
	 * @method require
	 * @for jmGraph
	 * @param {string} js 需要加载的JS的路径
	 * @param {function} [callback] 回调函数callback为成功或失败后回调
	 */
	function require(js,callback) {
	    if(typeof(js) !== 'string') {
	        var loaded = js.length;	        
	        if(loaded > 0) {
	        	var jsindex = 0;
	        	function requireCallback(j,err) {
	        		if(err) {
	                    if(callback) callback(j,err);
	                }
	                else {
	                    jsindex++;
	                    if(loaded == jsindex) {
	                        if(callback) callback(loaded);
	                    }
	                    else {
	                    	require(js[jsindex],requireCallback);
	                    }
	                }
	        	}
	        	require(js[jsindex],requireCallback);
	        }	        
	    }
	    else {
	        //获取所有已加载的js标记
	        var sc = document.getElementsByTagName('script');
	        for(var i in sc) {
	            //如果已加载则直接返回成功
	            if(sc[i].src === js) {
	                if(callback) callback(js);
	                return;
	            }
	        }
	        //创建script，加载js
	        sc = document.createElement('script');
	        sc.type= 'text/javascript';
	        sc.charset = 'utf-8';
	        sc.src = js;
	        //append到head中
	        var head = document.getElementsByTagName('head')[0];
	        head.appendChild(sc);

	        if(callback) {
	        	function loadCallback(e) {
		            if(this.readyState && this.readyState !== 'loaded' && this.readyState !== 'complete') {
		                return;
		            }
		            if(callback) callback(js);
		        }
		        //加载回调
		        if(sc.readyState) {
		        	sc.onreadystatechange = loadCallback;
		        }
	        	else {
	        		sc.onload = loadCallback;
	        	}		        
	        }
	        
	        //加载失败
	        sc.onerror = function() {
	            head.removeChild(sc);
	            if(callback) callback(js,'load faild');
	        }        
	    }
	}
}