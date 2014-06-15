
/**
* 正方体shape
*/
openflow.BoxShape = function (graph) {
    /*
			The example shape is a "3D box" that looks like this:
		               ____
		              /   /|
		             /___/ |
		             |   | /
		             |___|/

		   	The code below defines the shape. The BoxShape function
		   	it the constructor which creates a new object instance.
		*/
    function shape() { };

    /*
        The next lines use an mxCylinder instance to augment the
        prototype of the shape ("inheritance") and reset the
        constructor to the topmost function of the c'tor chain.
    */
    shape.prototype = new mxCylinder();
    shape.prototype.constructor = shape;

    // Requests crisp rendering in SVG for this shape
    shape.prototype.crisp = true;

    // Defines the extrusion of the box as a "static class variable"
    shape.prototype.extrude = 10;

    /*
        Next, the mxCylinder's redrawPath method is "overridden".
        This method has a isForeground argument to separate two
        paths, one for the background (which must be closed and
        might be filled) and one for the foreground, which is
        just a stroke.

         Foreground:       /
                     _____/
                          |
                          |
                       ____
         Background:  /    |
                     /     |
                     |     /
                     |____/
    */
    shape.prototype.redrawPath = function (path, x, y, w, h, isForeground) {
        var dy = this.extrude * this.scale;
        var dx = this.extrude * this.scale;

        if (isForeground) {
            path.moveTo(0, dy);
            path.lineTo(w - dx, dy);
            path.lineTo(w, 0);
            path.moveTo(w - dx, dy);
            path.lineTo(w - dx, h);
            path.end();
        }
        else {
            path.moveTo(0, dy);
            path.lineTo(dx, 0);
            path.lineTo(w, 0);
            path.lineTo(w, h - dy);
            path.lineTo(w - dx, h);
            path.lineTo(0, h);
            path.lineTo(0, dy);
            path.lineTo(dx, 0);
            path.close();
            path.end();
        }
    };

    return shape;
}

/**
* 流程图圆形
*/
openflow.CircleShape = function (graph) {

    function shape() { };

    shape.prototype = new mxCylinder();
    shape.prototype.constructor = shape;

    //shape.prototype.crisp = true;
    //shape.prototype.extrude = 10;

    shape.prototype.redrawPath = function (path, x, y, w, h, isForeground) {
        //var dy = this.extrude * this.scale;
        //var dx = this.extrude * this.scale;

        var len = Math.max(w, h) * 2;
        var hlen = len / 2;
        path.ellipse(0, 0, len , len);
        path.end();
    };

    return shape;
}

/**
* 结束流程图
*/
openflow.EndShape = function (graph) {

    function shape() { };

    shape.prototype = new mxCylinder();
    shape.prototype.constructor = shape;

    shape.prototype.step = 1;
    //shape.prototype.crisp = true;
    //shape.prototype.extrude = 10;

    shape.prototype.redrawPath = function (path, x, y, w, h, isForeground) {
        //var dy = this.extrude * this.scale;
        //var dx = this.extrude * this.scale;
        var wtp = this.step;
        var hlen = w / 2;
        var center = { x: w / 2 - hlen, y: h / 2 - hlen };
        //path.ellipse(w / 2 - len / 2, h / 2 - len / 2, len, len);
        var lrlen = hlen / 2;//二边圆弧宽度
        var cr1 = Math.acos(wtp / 2 / hlen);
        var cr2 = Math.asin(lrlen / hlen);
        var cr = cr1 - cr2;
        path.end();
    };

    return shape;
}

/**
* 并行组件
*/
openflow.ParallelShape = function (graph) {

    function shape() { };

    shape.prototype = new mxCylinder();
    shape.prototype.constructor = shape;
    shape.prototype.crisp = true;
    shape.prototype.extrude = 10;

    shape.prototype.redrawPath = function (path, x, y, w, h, isForeground) {
        var dy = this.extrude * this.scale;
        var dx = this.extrude * this.scale;

        if (isForeground) {
            path.moveTo(0, dy);
            path.lineTo(w - dx, dy);
            path.lineTo(w, 0);
            path.moveTo(w - dx, dy);
            path.lineTo(w - dx, h);
            path.end();
        }
        else {
            path.moveTo(0, dy);
            path.lineTo(dx, 0);
            path.lineTo(w, 0);
            path.lineTo(w, h - dy);
            path.lineTo(w - dx, h);
            path.lineTo(0, h);
            path.lineTo(0, dy);
            path.lineTo(dx, 0);
            path.close();
            path.end();
        }
    };

    return shape;
}
