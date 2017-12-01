"use strict";

const table = document.getElementById("bots_table");
const errorMsg = document.getElementById("error_message");
const uploadElement = document.getElementById("uploadButton");
const errText = document.getElementById("err_text");
const botmsg = document.getElementById("botmsg")

const tableContents = { bots: [], checked: [] };
const XHR_TIMEOUT = 10000;
const REQ_REPEAT_TIME = 13000;

var is_requesting = false;

function addTableRow() {
	let row = document.createElement("tr");
	let mas = [row];
	for (let i in arguments) {
		row.innerHTML += "<td>" + arguments[i] + "</td>";
		mas.push(arguments[i]);
	};
	tableContents.bots.push(mas);
	table.appendChild(row);
	botmsg.innerHTML = "Currently uploaded bots: " + tableContents.bots.length;
};

function clearTable(){
	table.innerHTML = "";
	tableContents.bots = [];
};

function fillTable(container){
	for(let el in container){
		addTableRow(container[el]);
	};
};

function refillTable(data){
	if(data === null){
		// show error if needed
		return;
	};
	clearTable();
	fillTable(data);
	if(tableContents.bots.length >= 2 || tableContents.bots.length <= 6){
		document.getElementById("startButton").removeAttribute("disabled");
	}else{
		document.getElementById("startButton").setAttribute("disabled", "");
	}
};

function updateTable() {
	let xhr = new XMLHttpRequest();
	xhr.open("GET", "/check_bots", true);
	xhr.timeout = XHR_TIMEOUT;

	xhr.onload = function(){
		if(xhr.status == 200)
		refillTable(JSON.parse(xhr.responseText));
	};
	xhr.onerror = function(){
		// handle error
	};
	xhr.ontimeout = function (){
		// handle timeout
	};

	xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
	xhr.send();
};

// first load of table
updateTable();
setInterval(updateTable, 5000);

////////////////////////////////////////////////////////////////////////////////

function checkFileExt(file, ext){
	if(ext[0] != "."){
		ext = "." + ext;
	}
	return file.slice(-ext.length) == ext;
};

function upload(){
    let fdata = new FormData();
	let file = uploadElement.files[0];

    fdata.append("sourceFile", file);
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "upload_sources", true);
	xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
    xhr.onload = function(){
        updateTable();
    };
    xhr.send(fdata);
};

////////////////////////////////////////////////////////////////////////////////

function startGame(){
	let xhr = new XMLHttpRequest();
	xhr.open("POST", "/game", true);
	xhr.onload = function(){
		stopAnimation();
		if(xhr.status == 200){
			window.open("/game");
		} else {
			// show error
		}
	};


}
