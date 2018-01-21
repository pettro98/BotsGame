"use strict";

const Back = document.getElementById("back");
const Forward = document.getElementById("forward");
var StatTable = document.getElementById("statTable");
const Count = document.getElementById("count");


////////////////////////////////////////////////////////////////////////////////

Back.onclick = showPrev;
Forward.onclick = showNext;


////////////////////////////////////////////////////////////////////////////////

var gameState = "";
var request = null;
var showingTurn = 0;
var lastTurn = 0;
var botFiles = null;
var stat = {};
var field = [];

function sendReq(turn) {
    let xhr = new XMLHttpRequest();
    xhr.open("GET", "/game?" + "lastTurn=" + lastTurn + "&gameState=" + gameState + "&turn=" + (turn || -1), true);
    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");

    xhr.timeout = 3000;

    xhr.ontimeout = function () {
        setTimeout(function () {
            sendReq(turn);
        }, 0);
    }

    xhr.onload = function () {
        if (this.status == 200) {
            updateData(xhr);
        }
        if (gameState == "running") {
            setTimeout(function () {
                sendReq(turn);
            }, 0);
        }
    }

    let data = {
        state: gameState,
        lastTurn: lastTurn,
        turn: turn || -1
    }

    if (request !== null) {
        request.abort();
    }
    request = xhr;

    // fdata.append("content", JSON.stringify(data));
    xhr.send();
}

function updateData(xhr) {
    let data = JSON.parse(xhr.responseText);
    if (gameState == "running" && data.state == "finished") {
        Back.removeAttribute("disabled");
        Forward.removeAttribute("disabled");
    }
    if(data.state == "idle" || data.state == "building"){
        alert("Someone started a new game or you wanted to open the tab without a game running. This tab will be closed");
        window.close();
    }
    if(data.state == "error"){
        alert("game process finished with an error. this tab will be closed");
        window.close();
    }
    gameState = data.state || gameState;
    stat = data.stat || stat;
    showingTurn = data.turn || data.lastTurn;
    lastTurn = data.lastTurn || lastTurn;
    field = data.field;
    botFiles = botFiles || data.bots;

    fillTable();
    Count.innerHTML = showingTurn;
    /////////////////////////////

    myStage.sendMessage("gameData", field);
}

sendReq();

function showPrev(){
    if(showingTurn == 0){
        return;
    }
    showingTurn--;
    sendReq(showingTurn);
}

function showNext(){
    if(showingTurn == lastTurn){
        return;
    }
    showingTurn++;
    sendReq(showingTurn);
}

////////////////////////////////////////////////////////////////////////////////

var statRowCount = 0;
const colors = ["grey", "white",
    "lightpink", "lightgreen",
    "cyan", "magenta",
    "lightblue", "gold"
];

function addTableRow() {
    statRowCount += 1;
    let row = document.createElement("tr");
    row.setAttribute("style", "background:" + colors[statRowCount] + ";");
    let mas = [row];
    for (let i in arguments) {
        row.innerHTML += "<td>" + arguments[i] + "</td>";
        mas.push(arguments[i]);
    }
    StatTable.appendChild(row);
}

function clearTable() {
    StatTable.innerHTML = "";
    statRowCount = 0;
    addTableRow("Name", "points", "last turn", "armies", "farms", "towers", "moves");
}

function fillTable() {
    clearTable();
    for (let i in botFiles) {
        addTableRow(botFiles[i], stat.points[i], stat.last_turn[i],
            stat.built_armies[i], stat.built_farms[i], stat.built_towers[i], stat.moves[i]);
    }
}