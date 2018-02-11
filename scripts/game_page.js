"use strict";

const Back = document.getElementById("back");
const Forward = document.getElementById("forward");
var StatTable = document.getElementById("statTable");
const Count = document.getElementById("count");


////////////////////////////////////////////////////////////////////////////////

Back.onclick = showPrev;
Forward.onclick = showNext;


////////////////////////////////////////////////////////////////////////////////

var gameState = "running";
var request = null;
var showingTurn = 0;
var lastTurn = 0;
var botFiles = null;
var stat = {};
var field = [];

function sendReq(turn) {
    let xhr = new XMLHttpRequest();
    if(turn === undefined) turn = -1;
    xhr.open("GET", "/game/data?" + "lastTurn=" + lastTurn + "&gameState=" + gameState + "&turn=" + turn , true);
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
    if (data.state == "finished") {
        Back.removeAttribute("disabled");
        Forward.removeAttribute("disabled");
    }
    if(data.state == "idle" || data.state == "building"){
        alert("There is no game running now. This tab will be closed");
        window.close();
    }
    if(data.state == "error"){
        alert("Game process finished with an error. This tab will be closed");
        window.close();
    }
    gameState = data.state || state;
    stat = data.S || stat;
    showingTurn = data.turn || showingTurn;
    lastTurn = data.CT || lastTurn;
    field = data.F;
    botFiles = data.bots || botFiles;

    fillTable();
    Count.innerHTML = showingTurn;
    /////////////////////////////

    updateField(field);
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
    addTableRow("Name", "last turn", "armies", "farms", "towers", "moves");
}

function fillTable() {
    clearTable();
    for (let i in botFiles) {
        addTableRow(botFiles[i], stat.LT[i],
            stat.BA[i], stat.BF[i], stat.BT[i], stat.M[i]);
    }
}