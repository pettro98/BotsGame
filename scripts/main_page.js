"use strict";

const Source = document.getElementById("source");
const SourceName = document.getElementById("sourceName");

const SendButton = document.getElementById("sendButton");
const CleanButton = document.getElementById("cleanButton");
const StartButton = document.getElementById("startButton");
const UpdateButton = document.getElementById("updateButton");

const GameMsg = document.getElementById("gameMsg");
const GameLink = document.getElementById("gameLink");
const Duel = document.getElementById("duel");
const Classic = document.getElementById("classic");
const Random = document.getElementById("random");
const Count = document.getElementById("count");
const Table = document.getElementById("table");

////////////////////////////////////////////////////////////////////////////////

var timeoutID = null;
var tableData = {};
var xhrRequests = {};
var running = false;

////////////////////////////////////////////////////////////////////////////////

UpdateButton.onclick = updateData;
SendButton.onclick = sendSource;
CleanButton.onclick = cleanSources;
StartButton.onclick = startGame;
Source.onchange = function () {
    SourceName.innerText = Source.files[0].name;
}

////////////////////////////////////////////////////////////////////////////////

function updateData() {
    function onload(xhr) {
        let data = JSON.parse(xhr.responseText)
        refreshTable(data.bots);
        setRunning(data.runnning);
        if(timeoutID !== null){
            clearTimeout(timeoutID);
        }
        timeoutID = setTimeout(function () {
            sendXHR("getBots", "GET", "/bots", null, 5000, onload);
        }, 2000);
    }

    sendXHR("getBots", "GET", "/bots", null, 5000, onload);
}

updateData();

function sendSource() {
    let file = Source.files[0];

    if (!file) {
        alert("nothing to upload");
        return;
    }

    if (file.name in tableData) {
        alert("bot already exists");
        return;
    }

    let fdata = createFormData({
        command: "add",
        source: file
    });

    sendXHR("addBotSource", "POST", "/bots", fdata, 5000, function (xhr) {
        if (xhr.status == 200) {
            let data = JSON.parse(xhr.responseText)
            updateData(data);
        } else if (xhr.status == 400) {
            alert(xhr.responseText);
        }
    });
}

function cleanSources() {
    let bots = getSelected();
    if (bots.length == 0) {
        if (confirm("You haven't selected any bots. \n" +
            "WARNING: If you press OK it will clean all.")) {
            var fdata = createFormData({
                command: "clean"
            });
        } else return;
    } else {
        var fdata = createFormData({
            command: "clean",
            bots: bots.join(" ")
        });
    }
    sendXHR("cleanBotSource", "POST", "/bots", fdata, 5000, function () {
        updateData();
    });
}

function startGame() {
    startAnim();

    let bots = getSelected();

    if (bots.length > Count.value) {
        alert("ERROR: Wrong number of bots selected");
        return;
    }

    if (bots.length != 0) {
        if (!confirm("You haven't selected any bots. \n" +
            "Press OK to start battle between developers' bots.")) return;
    }

    let fdata = createFormData({
        command: "start",
        count: Count.value,
        bots: bots.join(" "),
        map: getMap()
    });

    sendXHR("POST", "/bots", form, function (xhr) {
        return function () {
            stopAnim();
            if (xhr.status == 200) {
                window.open("/game");
            } else {
                alert(xhr.responseText);
            }
        }
    });
}

////////////////////////////////////////////////////////////////////////////////

function clearTable() {
    Table.innerHTML = "<tr><td>Name</td><td></td></tr>";
}

function addTableRow(name) {
    let id = "rowID_" + tableData.length;
    let row = document.createElement("tr");
    let checkCell = document.createElement("td");
    let checkBox = document.createElement("input");
    checkBox.setAttribute("type", "checkbox");
    checkCell.appendChild(checkBox);
    row.innerHTML = "<td>" + name + "</td>";
    row.id = id;
    row.appendChild(checkCell);
    Table.appendChild(row);
    tableData[name] = [row, checkBox];
}

function refreshTable(bots) {
    if (tableData.length === 0) {
        for (let i in bots) {
            addTableRow(bots[i]);
        }
    } else {
        for (let i in bots) {
            if (!(bots[i] in tableData)) {
                addTableRow(bots[i]);
            }
        }
        for (let i in tableData) {
            if (bots.indexOf(i) == -1) {
                Table.removeChild(tableData[i][0]);
                delete tableData[i];
            }
        }
    }
}

function sendXHR(id, method, url, data, timeout, onload, ontimeout, onerror) {
    let xhr = new XMLHttpRequest();
    xhr.open(method, url, true);
    xhr.timeout = timeout;

    if (onload) {
        xhr.onload = function (event) {
            onload(xhr, event);
        }
    }

    if (ontimeout) {
        xhr.ontimeout = function (event) {
            ontimeout(xhr, event);
        }
    }

    if (onerror) {
        xhr.onerror = function (event) {
            onerror(xhr, event);
        }
    }

    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
    xhr.send(data);

    if (xhrRequests[id]) {
        xhrRequests[id].abort();
        delete xhrRequests[id];
    }
    xhrRequests[id] = xhr;
}

function createFormData(data) {
    if (data === undefined) return;
    let fdata = new FormData();
    for (let i in data) {
        fdata.append(i, data[i]);
    }
    return fdata;
}

function getMap() {
    if (Duel.checked) {
        return "duel";
    } else if (Classic.checked) {
        return "classic";
    } else if (Random.checked) {
        return "random";
    }
}

function setRunning(running) {
    if (running) {
        StartButton.setAttribute("disabled", "");
        GameLink.removeAttribute("hidden");
    } else {
        StartButton.removeAttribute("disabled");
        GameLink.setAttribute("hidden", "");
    }
}

function getSelected() {
    let bots = [];
    for (let i in tableData) {
        if (tableData[i][1].checked) {
            bots.push(i);
        }
    }
    return bots;
}

function startAnim() {
    GameMsg.removeAttribute("hidden");
}

function stopAnim() {
    GameMsg.setAttribute("hidden", "");
}