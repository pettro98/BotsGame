"use strict";

const botsTable = document.getElementById("botsTable");
const errorMsg = document.getElementById("errorMsg");
const uploadElem = document.getElementById("uploadElem");
const errText = document.getElementById("errText");
const botMsg = document.getElementById("botMsg");
const botName = document.getElementById("botName");
const countElem = document.getElementById("countElem");

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
	if (arguments.length != 0) {
		let row = document.createElement("tr");
		let mas = [row];
		for (let i in arguments) {
			row.innerHTML += "<td>" + arguments[i] + "</td>";
			mas.push(arguments[i]);
		};
		tableContents.bots.push(mas);
		botsTable.appendChild(row);
	}
	botMsg.innerHTML = "Currently uploaded bots: " + tableContents.bots.length;
}

function clearTable() {
	botsTable.innerHTML = "";
	tableContents.bots = [];
	botMsg.innerHTML = "Currently uploaded bots: " + tableContents.bots.length;
}

function fillTable(container) {
	for (let el in container) {
		addTableRow(el, container[el].path);
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
	if (botCount <= 6) {
		document.getElementById("startButton").removeAttribute("disabled");
	} else {
		document.getElementById("startButton").setAttribute("disabled", "");
	}
}

function updateTable() {
	sendXHR("GET", "/bots", "", function (xhr) {
		return function () {
			if (xhr.status == 200) {
				refillTable(JSON.parse(xhr.responseText));
			}
		}
	});
}

updateTable();
setInterval(updateTable, REQ_REPEAT_TIME);

////////////////////////////////////////////////////////////////////////////////

function startAnimation() {
	errText.textContent = "Preparing game. Please, wait...";
	errText.removeAttribute("hidden");
}

function stopAnimation() {
	errText.setAttribute("hidden", "");
}

function upload() {
	let fdata = new FormData();
	let file = uploadElem.files[0];

	for (let i in tableContents.bots) {
		if (tableContents.bots[i][1] == botName.value) {
			alert("friendly_name_exists");
			return;
		}
	}

	fdata.append("command", "add");
	fdata.append("uploadElem", file);
	fdata.append("bot-name", botName.value);

	sendXHR("POST", "/bots", fdata, function (xhr) {
		return function () {
			if (xhr.status == 200) {
				updateTable();
			} else {
				alert(xhr.responseText);
			}
		}
	});
}


function startGame() {
	startAnimation();
	let form = new FormData();
	form.append("command", "start");

	if (countElem.value > 6 || countElem.value < tableContents.bots.length) {
		alert("wrong number of bots in the textfield");
		return;
	}

	let mas = [countElem.value];
	for (let i in tableContents.bots) {
		mas.push(tableContents.bots[i][1]);
	}

	form.append("content", mas.join(" "));

	sendXHR("POST", "/bots", form, function (xhr) {
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
	let fdata = new FormData();
	fdata.append("command", "clean");
	sendXHR("POST", "/bots", fdata, function (xhr) {
		return function () {
			if (xhr.status == 200) {
				updateTable();
			} else {
				alert(xhr.responseText);
			}
		}
	});
}