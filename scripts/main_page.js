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

const Table = document.getElementById("table");


////////////////////////////////////////////////////////////////////////////////
//  WEBPAGE CONTROLLING

UpdateButton.onclick = updateData;
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

function updateData() {
    sendXHR("getBots", "POST", "/bots", createFormData({
        command: "get",
        content: JSON.stringify({
            bots: tableData.bots,
            state: gameState
        })
    }), 10000, function () {
        if (this.status == 200) {
            refreshData(JSON.parse(this.responseText));
            setTimeout(updateData, 0);
        } else {
            setTimeout(updateData, 10000);
        }
    }, function () {
        setTimeout(updateData, 0);
    });
}

updateData();

function refreshData(data) {
    if (data.state && data.state != gameState) {
        if (gameState == "building" && data.state == "running") {
            window.open("/game");
        }
        if (data.state == "error") {
            alert("game process finished with an error");
        }
        gameState = data.state;
        setState(gameState);
    }
    if (data.bots === undefined) {
        return;
    }
    let tmpBots = [];
    let tmpRows = [];
    for (let i in tableData.bots) {
        if (data.bots.indexOf(tableData.bots[i]) != -1) {
            tmpBots.push(tableData.bots[i]);
            tmpRows.push(tableData.rows[i])
        } else {
            Table.removeChild(tableData.rows[i].row);
        }
    }
    tableData.bots = tmpBots;
    tableData.rows = tmpRows;
    for (let i in data.bots) {
        if (tableData.bots.indexOf(data.bots[i]) == -1) {
            tableData.rows.push(addTableRow(data.bots[i]));
            tableData.bots.push(data.bots[i]);
        }
    }
}

function setState(state) {
    if (state == "running") {
        GameLink.removeAttribute("hidden");
        GameMsg.setAttribute("hidden", "");
        StartButton.setAttribute("disabled", "");
    } else if (state == "finished" || state == "idle") {
        GameLink.setAttribute("hidden", "");
        GameMsg.setAttribute("hidden", "");
        StartButton.removeAttribute("disabled");
    } else if (state == "building") {
        GameLink.setAttribute("hidden", "");
        GameMsg.removeAttribute("hidden");
        StartButton.setAttribute("disabled", "");
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

function sendXHR(id, method, url, data, timeout, onload, ontimeout, onerror) {
    let xhr = new XMLHttpRequest();
    xhr.open(method, url, true);
    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");

    xhr.timeout = +timeout;
    xhr.onload = onload;
    xhr.onerror = onerror;

    xhr.ontimeout = ontimeout;

    if (id in requests) {
        requests[id].abort();
    }
    requests[id] = xhr;

    xhr.send(data);
}

function sendSource() {
    let fdata = createFormData({
        command: "add",
        source: Source.files[0]
    });
    sendXHR("postBots", "POST", "/bots", fdata, 0, alertIfNotOK);
}

function createFormData(data) {
    let tmp = new FormData();
    for (let i in data) {
        tmp.append(i, data[i]);
    }
    return tmp;
}

function alertIfNotOK() {
    if (this.status != 200) {
        alert(this.responseText);
    }
}

function cleanSources() {
    let bots = getSelected();
    if (bots.length == tableData.bots.length || bots.length == 0) {
        if (!confirm("WARNING: This will clear all bots on server.\nContinue?")) {
            return;
        } else {
            bots = tableData.bots;
        }
    }
    let fdata = createFormData({
        command: "clean",
        content: JSON.stringify(bots)
    })
    sendXHR("postBots", "POST", "/bots", fdata, 0, alertIfNotOK);
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

function startGame() {
    startAnim();
    let bots = getSelected();
    let fdata = createFormData({
        command: "start",
        content: JSON.stringify({
            bots: bots,
            count: Count.value,
            map: getMap()
        })
    });
    sendXHR("postBots", "POST", "/bots", fdata, 0, function () {
        if (this.status != 200) {
            alert(this.responseText);
        }
        stopAnim();
    });
}

function startAnim() {
    GameMsg.removeAttribute("hidden");
}

function stopAnim() {
    GameMsg.setAttribute("hidden", "");
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