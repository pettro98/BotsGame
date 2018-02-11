var my_panel = new jsgl.Panel(document.getElementById("panel"));

var black = "rgb(0,0,0)";
var white = "rgb(255,255,255)";
var lightPink = "rgb(255,182,193)";
var lightGreen = "rgb(144,238,144)";
var cyan = "rgb(0,255,255)";
var magenta = "rgb(255,0,255)";
var lightBlue = "rgb(173,216,230)";
var gold = "rgb(255,215,0)";
var color_str = [white, lightPink, lightGreen, cyan, magenta, lightBlue, gold];
var r = 10;
var _x = 20;
var _y = 20;
var sin30 = 0.5;
var cos30 = Math.cos(Math.PI / 6);

function makeHex(x, y, color) {
    var hex = my_panel.createPolygon();

    hex.addPointXY(+x, +y - r * 1.15);
    hex.addPointXY(+x + r * 1.15 * cos30, +y - r * 1.15 * sin30);
    hex.addPointXY(+x + r * 1.15 * cos30, +y + r * 1.15 * sin30);
    hex.addPointXY(+x, +y + r * 1.15);
    hex.addPointXY(+x - r * 1.15 * cos30, +y + r * 1.15 * sin30);
    hex.addPointXY(+x - r * 1.15 * cos30, +y - r * 1.15 * sin30);

    hex.getStroke().setWeight(1);
    hex.getStroke().setColor(black);

    hex.getFill().setColor(color_str[color]);
    hex.getFill().setOpacity(1);

    return hex;
}

function placeHex(group, x, y, color, contents) {
    var posx = _x + +x * 2 * r + ((+y) % 2) * r * 1.15 * cos30;
    var posy = _y + +y * 2 * r * cos30;
    group.addElement(makeHex(posx, posy, color));

    var text, shift = 1;
    switch(contents){
        case "a": text = "C"; break;
        case "b": text = "F"; break;
        case "c": text = "G"; break;
        case "d": text = "Pi"; shift = 2; break;
        case "e": text = "Pa"; shift = 2; break;
        case "f": text = "t1"; shift = 2; break;
        case "g": text = "t2"; shift = 2; break;
        case "h": text = "a1"; shift = 2; break;
        case "i": text = "a2"; shift = 2; break;
        case "j": text = "a3"; shift = 2; break;
        case "k": text = "a4"; shift = 2; break;
    }

    var l = my_panel.createLabel();
    l.setX(posx-r*0.3*shift);
    l.setY(posy-r*0.7);
    l.setText(text);
    l.setFontSize(r * 1.1);
    group.addElement(l);
    // add showing of contents
}

function updateField(field) {
    var i, j;
    newGroup = my_panel.createGroup();
    for (i in field) {
        for (j in field) {
            var par = field[i][j].split("_");
            var cont = par[0];
            var own = par[1];
            if (own == "/") continue;
            placeHex(newGroup, j, i, own, cont);
        }
    }
    my_panel.clear();
    my_panel.addElement(newGroup)
}