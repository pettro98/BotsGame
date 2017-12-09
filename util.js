"use strict";

const path = require("path");

function getCurrentDate() {
	let date = "[" + new Date().toUTCString() + "]: ";
	return date;
};

module.exports.getDate = getCurrentDate;

module.exports.checkExt = (file, ext) => {
	if (ext[0] != ".") {
		ext = "." + ext;
	}
	return file.slice(-ext.length) == ext;
};

module.exports.withoutExt = (file) => {
	return file.slice(0, file.lastIndexOf("."));
}

module.exports.getShortName = (file) => {
	return file.slice(lastIndexOf(PATH_DELIM));
}

module.exports.myLog = (...args) => {
	console.log(getCurrentDate(), ...args);
};

module.exports.logRequest = (req, res, next) => {
	module.exports.myLog(req.method, req.url);
	next();
};

// send file to client
module.exports.sendFromDir = function(res, next, path, type) {
	res.sendFile(path, {
		root: __dirname,
		headers: { "Content-Type": type }
	},
		(err) => {
			if (err) {
				module.exports.myLog("error sending " + path);
				next(err);
			} else {
				module.exports.myLog("successfully sent " + path);
			}
		});
};

// send file using FILES object
module.exports.sendAFile = function(res, next, file) {
	module.exports.sendFromDir(res, next, file.path, file.type);
};


module.exports.EXECUTABLE = path.join(__dirname, "_install", "game");
module.exports.PORT = process.env.PORT || 5000;
module.exports.HTTP_PORT = 80;
module.exports.PATH_DELIM = path.join("", "");
module.exports.FILES = {
	main: { path: path.join("html", "main.html"), type: "text/html" },
	game: { path: path.join("html", "game.html"), type: "text/html" },
	err404: { path: path.join("html", "on_404.html"), type: "text/html" },
	favicon: { path: path.join("images", "Antiyoy_32.png"), type: "image/png" },
}
module.exports.DIRS = {
	scriptsPath: "scripts",
	uploadedSrc: "uploaded_sources"
}
module.exports.INPUT_NAME = "uploadElem";