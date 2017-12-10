/*
 * server created as part of AntiyoyBots project
 * by pettro98 <ya.skywalker@yandex.ru>
 * 
 * P.S. sorry for this code...
 */

"use strict";

////////////////////////////////////////////////////////////////////////////////
//  IMPORTING NPM MODULES
////////////////////////////////////////////////////////////////////////////////

const express = require("express");
const multipart = require("connect-multiparty");
const fs = require('fs');
const favicon = require("serve-favicon");
const exec = require("child_process");

////////////////////////////////////////////////////////////////////////////////
//	IMPORTING CUSTOM MODULES
////////////////////////////////////////////////////////////////////////////////

const u = require("./util.js");
const c = require("./constants.js");
const botsSub = require("./bots-router.js");
const gameSub = require("./game-router.js");

////////////////////////////////////////////////////////////////////////////////
//  GLOBAL VARIABLES & CONSTANTS
////////////////////////////////////////////////////////////////////////////////

const app = express();

var fullData = {
	running: false,
	stat: {},
	turn: 0,
	data: []
};

////////////////////////////////////////////////////////////////////////////////
//  GENERAL REQUEST HANDLERS
////////////////////////////////////////////////////////////////////////////////

app.use(multipart());
app.use(u.logReq);
app.use(favicon(c.FILES.favicon.path));
app.use("/scripts", express.static(c.DIRS.scripts));

////////////////////////////////////////////////////////////////////////////////
//  CUSTOM REQUEST HANDLERS
////////////////////////////////////////////////////////////////////////////////

app.get("/", getMain);

app.get("/bots", getBots);
app.post("/bots", postBots);

app.get("/game", getGame);
app.get("/game/data", getGameData);
app.post("/game/data", postGameData);

app.use(notFound);

////////////////////////////////////////////////////////////////////////////////
//	LAUNCH SERVER
////////////////////////////////////////////////////////////////////////////////

// listen on environment or 5000 port
app.listen(c.PORT, () => {
	u.log("Server listening on port " + c.PORT + "!");
});

// listen on standard http port
app.listen(c.HTTP_PORT, () => {
	u.log("Server listening on port " + c.HTTP_PORT + "!");
});

////////////////////////////////////////////////////////////////////////////////
//	WEBPAGE SENDING
////////////////////////////////////////////////////////////////////////////////

function getMain(req, res, next) {
	u.sendFile(res, next, c.FILES.main);
	u.log("sent main page");
}

function getGame(req, res, next) {
	u.sendFile(res, next, c.FILES.game);
	u.log("sent game page");
}

function notFound(req, res, next) {
	res.sendStatus(404);
}

////////////////////////////////////////////////////////////////////////////////
//	HANDLER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

function getBots(req, res, next) {
	if (deniedWithoutXHR(req, res)) return;
	let tmpBots = fs.readdirSync(c.DIRS.botSrc);
	for (let i in tmpBots) {
		tmpBots[i] = u.cutPath(tmpBots[i]);
	}
	res.status(200).json(tmpBots);
}

function postBots(req, res, next) {
	if (deniedWithoutXHR(req, res)) return;
	let command = req.body.command;
	if (command == "add") {
		addBotSource(req, res);
	} else if (command == "clean") {
		cleanBotSource(req, res);
	} else if (command == "start") {
		startGame(req, res);
	} else {
		res.sendStatus(400);
	}
}

function getGameData(req, res, next) {
	if (deniedWithoutXHR(req, res)) return;
	let tmpBots = fs.readdirSync(c.DIRS.botSrc);
	for (let i in tmpBots) {
		tmpBots[i] = u.cutExt(u.cutPath(tmpBots[i]));
	}
	let resData = {
		bots: tmpBots,
		stat: fullData.stat,
		turn: fullData.turn,
		running: fullData.running,
	};
	if (req.body == "") {
		resData.data = fullData.data[turn - 1];
	} else {
		resData.data = fullData.data[req.body];
	}
	res.status(200).json(resData);
}

function postGameData(req, res, next) {
	if (denyWithoutXHR(req, res)) return;
	let reqData = JSON.parse(req.body);
	fullData.stat = reqData.stat;
	fullData.turn = reqData.turn;
	fullData.data.push(reqData.data);
	res.sendStatus(200);
}

////////////////////////////////////////////////////////////////////////////////
//	OTHER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

function deniedWitoutXHR(req, res) {
	if (!req.xhr) {
		res.sendStatus(403);
		return true;
	} else return false;
}

function addBotSource(req, res, next) {
	let name = req.files.source.name;
	let path = req.files.source.path;
	let fName = c.DIRS.botSrc + "/" + name;
	if (fs.existsSync(fname)) {
		res.status(400).send("ERROR: file " + name + " already exists");
		fs.unlinkSync(path);
		u.log("ERROR: file " + name + " already exists");
		return;
	}
	fs.renameSync(path, fName);
	u.log("Received bot source successfully");
	res.sendStatus(200);
}

function cleanBotSource(req, res) {
	let bots = [];
	if (req.body.bots == "") {
		bots = fs.readdirSync(c.DIRS.botSrc);
	} else {
		bots = req.body.bots.split(" ");
	}
	for (let i in bots) {
		fs.unlinkSync(c.DIRS.botSrc + "/" + bots[i]);
	}
	res.sendStatus(200);
}

function startGame(req, res) {
	let count = +req.body.count;
	let bots = req.body.bots.split(" ");
	let map = req.body.map;
	setDynamicHeader(count, bots, map);
	let code = buildGame(bots);
	if (code == 1) {
		res.status(500).send("ERROR: Compilation failed. Check your source files.");
	} else if (code == 127 || code === null) {
		res.status(500).send("ERROR: CMake not found. Check CMake installation.");
	}
	execGame();
	res.sendStatus(200);
}

////////////////////////////////////////////////////////////////////////////////
//	FOR startGame()

function setDynamicHeader(count, bots, map) {
	let headerPath = __dirname + "/arch/dynamic.h";
	let header = "#define BOTS_COUNT " + count + "\n";
	let i = 0;
	for (; i < mas.length && i < 6; i++) {
		header += "#define BOT_" + i + " " + u.cutPathExt(bots[i]) + "\n";
	}
	for (; i <= mas[0]; i++) {
		header += "#define BOT_" + i + " " + "game_module::Bot \n";
	}
	header += "define MAP_TYPE \"" + map + "\"";
	fs.writeFileSync(headerPath, header);
}

function buildGame() {
	process.env.BOT_SOURCES = bots.join(" " + c.DIRS.botSrc + "/");
	let result = exec.spawnSync("cmake -H. -B_build " +
		"-DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Release && " +
		"cmake --build _build --clean-first", { env: process.env });
	return result.status;
}

function execGame() {
	exec.spawn("./_install/bin/game")
		.on("close", (code, signal) => {
			fullData.running = false;
		});
	fullData.running = true;
}

//
////////////////////////////////////////////////////////////////////////////////