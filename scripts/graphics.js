var my_panel = new jsgl.Panel(document.getElementById("panel"));

var black = "rgb(0,0,0)";
var white = "rgb(255,255,255)";
var lightPink = "rgb(255,182,193)";
var lightGreen = "rgb(144,238,144)";
var cyan = "rgb(0,255,255)";
var magenta = "rgb(255,0,255)";
var lightBlue = "rgb(173,216,230)";
var gold = "rgb(255,215,0)";
var colors = [white, lightPnk, lightGreen, cyan, magenta, lightBlue, gold];
var r = 60;
var _x = 60;
var _y = 60;
var cos30 = 0.5;
var sin30 = 0.866;

function makeHex(x, y, color) {
    var hex = my_panel.createPolygon();

    hex.addPointXY(_x + x, _y + y - r);
    hex.addPointXY(_x + x + r * cos30, _y - r * sin30);
    hex.addPointXY(_x + x + r * cos30, _y + r * sin30);
    hex.addPointXY(_x + x, _y + y + r);
    hex.addPointXY(_x + x - r * cos30, _y + r * sin30);
    hex.addPointXY(_x + x - r * cos30, _y - r * sin30);

    hex.getStroke().setWeight(2);
    hex.getStroke().setColor();

    hex.getFill().setColor(colors[coor]);
    hex.getFill().setOpacity(1);

    return hex;
}

function placeHex(x, y, color, contents) {
    var posx = _x + x * 2 * r + ((y + 1) % 2) * r;
    var posy = _y + y * 2 * cos30;
    my_panel.addElement(makeHex(posx, posy, color));

    // add showing of contents
}

function updateField(field) {
    var i, j;
    for (i in field) {
        for (j in field) {
            var par = field[i, j].split("_");
            var cont = par[0];
            var own = par[1];
            if(own == -1) continue;
            placeHex(i, j, own, cont);
        }
    }
}