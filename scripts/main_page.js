"use strict";

const botsTable = document.getElementById("botsTable");
const errorMsg = document.getElementById("errorMsg");
const uploadElem = document.getElementById("uploadElem");
const errText = document.getElementById("errText");
const botMsg = document.getElementById("botMsg")

const tableContents = { bots: [] };
const XHR_TIMEOUT = 10000;
const REQ_REPEAT_TIME = 13000;

var is_requesting = false;

function checkFileExt(file, ext) {
	if (ext[0] != ".") {
		ext = "." + ext;
	}
	return file.slice(-ext.length) == ext;
}

function sendXHR(method, url, data, onload, onerror, ontimeout) {
	let xhr = new XMLHttpRequest();
	xhr.open(method, url, true);
	xhr.timeout = XHR_TIMEOUT;

	xhr.onload = onload(xhr);
	if (onerror) {
		xhr.onerror = onerror(xhr);
	}
	if (ontimeout) {
		xhr.ontimeout = ontimeout(xhr);
	}

	xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
	xhr.send(data);
}

function addTableRow() {
	let row = document.createElement("tr");
	let mas = [row];
	for (let i in arguments) {
		row.innerHTML += "<td>" + arguments[i] + "</td>";
		mas.push(arguments[i]);
	};
	tableContents.bots.push(mas);
	botsTable.appendChild(row);
	botMsg.innerHTML = "Currently uploaded bots: " + tableContents.bots.length;
}

function clearTable() {
	botsTable.innerHTML = "";
	tableContents.bots = [];
}

function fillTable(container) {
	for (let el in container) {
		addTableRow(container[el]);
	};
}

function refillTable(data) {
	if (data === null) {
		// show error if needed
		return;
	};
	clearTable();
	fillTable(data);
	let botCount = tableContents.bots.length;
	if (botCount == 2 || botCount == 4 || botCount == 6) {
		document.getElementById("startButton").removeAttribute("disabled");
	} else {
		document.getElementById("startButton").setAttribute("disabled", "");
	}
}

function updateTable() {
	sendXHR("GET", "/check_bots", "", function (xhr) {
		return function () {
			if (xhr.status == 200) {
				refillTable(JSON.parse(xhr.responseText));
			}
		}
	});
}

// first load of table
updateTable();
setInterval(updateTable, 5000);

////////////////////////////////////////////////////////////////////////////////


function upload() {
	let fdata = new FormData();
	let file = uploadElem.files[0];

	fdata.append("sourceFile", file);

	sendXHR("POST", "/upload_sources", fdata, function () { return updateTable });
}


function startGame() {
	startAnimation();
	sendXHR("POST", "/game", "start", function (xhr) {
		return function () {
			stopAnimation();
			if (xhr.status == 200) {
				window.open("/game");
			} else {
				alert(xhr.responseText);
			}
		}
	});
}

function cleanSources() {
	sendXHR("POST", "/game", "clean", function () { return updateTable });
}