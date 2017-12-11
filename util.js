"use strict";

const c = require("./constants.js");

var util = module.exports;

util.getDate = getDate;
util.checkExt = checkExt;
util.cutExt = cutExt;
util.cutPath = cutPath;
util.cutPathExt = cutPathExt;
util.log = log;
util.logReq = logReq;
util.sendFileX = sendFileX;
util.sendFile = sendFile;

function getDate() {
	let date = "[" + new Date().toUTCString() + "]: ";
	return date;
};

function checkExt(file, ext) {
	if (ext[0] != ".") {
		ext = "." + ext;
	}
	return file.slice(-ext.length) == ext;
};

function cutExt(file) {
	let pos = file.lastIndexOf(".");
	if (pos == -1) return file;
	return file.substr(0, file.lastIndexOf("."));
}

function cutPath(file) {
	return file.substr(file.lastIndexOf(PATH_DELIM) + 1);
}

function cutPathExt(file) {
	return cutPath(cutext(file));
}

function log(...args) {
	console.log(getDate(), ...args);
};

function logReq(req, res, next) {
	log(req.method, req.url, req.files);
	next();
};

function sendFileX(res, next, path, type) {
	res.status(200).sendFile(path, {
		root: __dirname,
		headers: { "Content-Type": type }
	}, (err) => {
		if (err) {
			util.log("ERROR: Failed to send " + path);
			next(err);
		} else {
			util.log("INFO: Successfully sent " + path);
		}
	});
};

function sendFile(res, next, file) {
	util.sendFileX(res, next, c.FILES[file].path, c.FILES[file].type);
};