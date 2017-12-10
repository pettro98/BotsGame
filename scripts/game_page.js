"use strict";

const back = document.getElementById("back");
const forward = document.getElementById("forward");

function enableButtons(){
    back.removeAttribute("disabled");
    forward.removeAttribute("disabled")
}

var gameInProgress = true;
var botFiles = [];
var stat = {};
var turnNum = 0;

function getData(turn) {
    let xhr = new XMLHttpRequest();
    xhr.open("GET", "/game", true);
    xhr.timeout = 10000;

    xhr.onload = function () {
        let data = JSON.parse(xhr.responseText);
        myStage.sendMessage("gameData", data.data);
        stat = data.stat;
        if (turn !== undefined) {
            turnNum = turn;
        } else {
            turnNum = stat.turn;
        }
        gameInProgress = data.progress;
        botFiles = data.bots;
        if (!data.progress) {
            clearInterval(timerID);

        }
    };

    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
    if (turn !== undefined) {
        xhr.send(turn);
    } else {
        xgr.send();
    }
}

getData();
var timerID = setInterval(function () { getData(); fillTable() }, 1000);

////////////////////////////////////////////////////////////////////////////////

var statTable = document.getElementById("statTable");
var statRowCount = 0;
const colors = ["grey", "white",
    "lightpink", "lightgreen",
    "cyan", "magenta",
    "lightblue", "gold"];

function addTableRow() {
    statRowCount += 1;
    let row = document.createElement("tr");
    row.setAttribute("style", "background:" + colors[statRowCount] + ";");
    let mas = [row];
    for (let i in arguments) {
        row.innerHTML += "<td>" + arguments[i] + "</td>";
        mas.push(arguments[i]);
    };
    statTable.appendChild(row);
}

function clearTable() {
    statTable.innerHTML = "";
}

function fillTable() {
    clearTable();
    addTableRow("Name", "points", "last turn", "armies", "farms", "towers", "moves");
    for (let i in botFiles) {
        addTableRow(botFiles[i], stat.points[i], stat.last_turn[i], stat.built_armies[i], stat.built_farms[i], stat.built_towers[i], stat.moves[i]);
    }
}

