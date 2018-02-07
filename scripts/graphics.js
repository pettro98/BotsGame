var my_panel = new jsgl.Panel(document.getElementById("panel"));

var lightPink = "rgb(255,182,193)";
var lightGreen = "rgb(144,238,144)";
var cyan = "rgb(0,255,255)";
var magenta = "rgb(255,0,255)";
var lightBlue = "rgb(173,216,230)";
var gold = "rgb(255,215,0)";
var r = 60;
var _x = 60;
var _y = 60;

function makeHex(x, y, color) {
    var cos30 = 0.5;
    var sin30 = 0.866;

    var hex = my_panel.createPolygon();

    hex.addPointXY(_x + x, _y + y - r);
    hex.addPointXY(_x + x + r * cos30, _y - r * sin30);
    hex.addPointXY(_x + x + r * cos30, _y + r * sin30);
    hex.addPointXY(_x + x, _y + y + r);
    hex.addPointXY(_x + x - r * cos30, _y + r * sin30);
    hex.addPointXY(_x + x - r * cos30, _y - r * sin30);

    hex.getStroke().setWeight(2);
    hex.getStroke().setColor("rgb(0,0,0)");

    hex.getFill().setColor(color);
    hex.getFill().setOpacity(1);

    return hex;
}

function addHex(x, y, color) {
    var posx = x
        my_panel.addElement(makeHex())
}