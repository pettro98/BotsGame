"use strict";

const Source = document.getElementById("source");
const SourceName = document.getElementById("sourceName");

const SendButton = document.getElementById("sendButton");
const CleanButton = document.getElementById("cleanButton");
const StartButton = document.getElementById("startButton");
const UpdateButton = document.getElementById("updateButton");
const LogButtton = document.getElementById("logButton");

const GameMsg = document.getElementById("gameMsg");
const GameLink = document.getElementById("gameLink");

const Duel = document.getElementById("duel");
const Classic = document.getElementById("classic");
const Random = document.getElementById("random");
const Count = document.getElementById("count");
const Cooldown = document.getElementById("cooldown");

const Table = document.getElementById("table");


////////////////////////////////////////////////////////////////////////////////
//  WEBPAGE CONTROLLING

UpdateButton.onclick = subscribe;
SendButton.onclick = sendSource;
CleanButton.onclick = cleanSources;
StartButton.onclick = startGame;
LogButtton.onclick = function () {
    window.open("/cmake_log");
}
Source.onchange = function () {
    SourceName.innerText = Source.files[0].name;
}

if (0 in Source.files) {
    SourceName.innerText = Source.files[0].name;
}

function setCounter(enable, val) {
    if (val !== undefined) {
        Count.value = val;
    }
    if (enable) {
        Count.removeAttribute("disabled");
    } else {
        Count.setAttribute("disabled", "");
    }
}

setCounter(Random.checked);

Random.onclick = function () {
    setCounter(true)
}
Duel.onclick = function () {
    setCounter(false, "2")
}
Classic.onclick = function () {
    setCounter(false, "4")
}

////////////////////////////////////////////////////////////////////////////////
//  DATA STRUCTURES

var tableData = {
    bots: [],
    rows: []
};
var gameState = "idle";
var requests = {};

//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  

function subscribe() {
    let xhr = new XMLHttpRequest();
    xhr.open("GET", "/lobby/data?state=" + gameState + "&bots=" +
        tableData.bots.join(","), true);
    xhr.onload = function () {
        if (this.status != 200) {
            alert(this.responseText);
        } else {
            let data = JSON.parse(this.responseText);
            if (data.state != gameState) {
                gameState = data.state;
                setState(gameState);
            }
            let newBots = [];
            let newRows = [];
            tableData.bots.forEach(function (bot, i) {
                if (data.bots.indexOf(bot) == -1) {
                    Table.removeChild(tableData.rows[i].row);
                } else {
                    newBots.push(tableData.bots[i]);
                    newRows.push(tableData.rows[i]);
                }
            });
            data.bots.forEach(function (bot, i) {
                if (newBots.indexOf(bot) == -1) {
                    newBots.push(bot);
                    newRows.push(addTableRow(bot));
                }
            });
            tableData.bots = newBots;
            tableData.rows = newRows;
        }
        setTimeout(subscribe, 0);
    }
    xhr.ontimeout = function () {
        setTimeout(subscribe, 0);
    }
    xhr.timeout = 30000;
    if (requests.get) {
        requests.get.abort();
    }
    xhr.send(null);
    requests.get = xhr;
}

subscribe();

function sendSource() {
    let file = Source.files[0];
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "/lobby/data?command=add", true);
    let fdata = new FormData();
    fdata.append("source", file);
    xhr.onload = function () {
        if (this.status != 200) {
            alert(this.responseText);
        } else {
            subscribe()
        }
    }
    xhr.onerror = function () {
        alert("Error connecting to server");
    }
    if (requests.send) {
        requests.send.abort();
    }
    requests.send = xhr;
    xhr.send(fdata);
}

function cleanSources() {
    let bots = getSelected().join(",");
    if (bots == "") {
        return;
    }
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "/lobby/data?command=remove&bots=" + bots, true);
    xhr.onload = function () {
        if (this.status != 200) {
            alert(responseText);
        } else {
            subscribe()
        }
    }
    xhr.onerror = function () {
        alert("Error connecting to server");
    }
    if (requests.clean) {
        requests.clean.abort();
    }
    requests.clean = xhr;
    xhr.send(null);
}

function startGame() {
    let bots = getSelected().join(",");
    let map = getMap();
    let count = Count.value;
    let cooldown = Cooldown.value;
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "/lobby/data?command=startGame&map=" + map +
        "&count=" + count + "&bots=" + bots + "&cooldown=" + cooldown, true);
    xhr.onload = function () {
        if (this.status != 200) {
            alert(responseText);
        } else {
            subscribe()
        }
    }
    xhr.onerror = function () {
        alert("Error connecting to server");
    }
    if (requests.start) {
        requests.start.abort();
    }
    requests.start = xhr;
    xhr.send(null);
}

function setState(state) {
    if (state == "running") {
        GameLink.removeAttribute("hidden");
        GameMsg.setAttribute("hidden", "");
        StartButton.setAttribute("disabled", "");
        window.open("/game");
    } else if (state == "finished" || state == "idle" ||
        state == "error" || state == "build_error") {
        GameLink.setAttribute("hidden", "");
        GameMsg.setAttribute("hidden", "");
        StartButton.removeAttribute("disabled");
    } else if (state == "building") {
        GameLink.setAttribute("hidden", "");
        GameMsg.removeAttribute("hidden");
        StartButton.setAttribute("disabled", "");
    }
    if (state == "error") {
        alert("Game finished with an error");
    }
    if(state == "build_error"){
        alert("Build process finished with an error");
    }
}

function cleanTable() {
    table.innerHTML = "<tr><td>Name</td><td>Select</td></tr>";
}

function addTableRow(name) {
    let row = document.createElement("tr");
    row.innerHTML = "<td>" + name + "</td>";
    let cell = document.createElement("td");
    let check = document.createElement("input");
    check.setAttribute("type", "checkbox");
    cell.appendChild(check);
    row.appendChild(cell);
    Table.appendChild(row);
    return {
        check: check,
        row: row
    };
}

function alertIfNotOK() {
    if (this.status != 200) {
        alert(this.responseText);
    }
}

function getSelected() {
    let tmp = [];
    for (let i in tableData.rows) {
        if (tableData.rows[i].check.checked) {
            tmp.push(tableData.bots[i]);
        }
    }
    return tmp;
}

function getMap() {
    if (Random.checked) {
        return "random";
    } else if (Classic.checked) {
        return "classic";
    } else if (Duel.checked) {
        return "duel";
    } else {
        return "undefined";
    }
}