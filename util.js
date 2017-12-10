"use strict";

const path = require("path");

function getCurrentDate() {
	let date = "[" + new Date().toUTCString() + "]: ";
	return date;
};

var util = module.exports;
util.getDate = getCurrentDate;

util.checkExt = (file, ext) => {
	if (ext[0] != ".") {
		ext = "." + ext;
	}
	return file.slice(-ext.length) == ext;
};

util.withoutExt = (file) => {
	return file.slice(0, file.lastIndexOf("."));
}

util.getShortName = (file) => {
	return file.slice(file.lastIndexOf(util.PATH_DELIM) + 1);
}

util.myLog = (...args) => {
	console.log(getCurrentDate(), ...args);
};

util.logRequest = (req, res, next) => {
	util.myLog(req.method, req.url, req.files);
	next();
};

// send file to client
util.sendFromDir = function(res, next, path, type) {
	res.sendFile(path, {
		root: __dirname,
		headers: { "Content-Type": type }
	},
		(err) => {
			if (err) {
				util.myLog("error sending " + path);
				next(err);
			} else {
				util.myLog("successfully sent " + path);
			}
		});
};

// send file using FILES object
util.sendAFile = function(res, next, file) {
	util.sendFromDir(res, next, file.path, file.type);
};


util.EXECUTABLE = path.join(__dirname, "_install", "game");
util.PORT = process.env.PORT || 5000;
util.HTTP_PORT = 80;
util.PATH_DELIM = path.join("a", "a")[1];
util.FILES = {
	main: { path: path.join("html", "main.html"), type: "text/html" },
	game: { path: path.join("html", "game.html"), type: "text/html" },
	err404: { path: path.join("html", "on_404.html"), type: "text/html" },
	favicon: { path: path.join("images", "Antiyoy_32.png"), type: "image/png" },
}
util.DIRS = {
	scriptsPath: "scripts",
	uploadedSrc: "uploaded_sources"
}
util.INPUT_NAME = "uploadElem";

util.gameInProgress = false;

util.botList = [];