
/*
 * server created for AntiyoyBots project
 * by pettro98 <ya.skywalker@yandex.ru>
 * 
 * P.S. sorry for this code...
 */

////////////////////////////////////////////////////////////////////////////////
//    SETTING GLOBAL CONSTANTS AND LOADING MODULES
////////////////////////////////////////////////////////////////////////////////

const express = require('express'); // can be directly used when declaring app
const multipart = require("connect-multiparty");  // can be directly used in app.use()
const fs = require('fs');
const path = require('path');
const exec = require("child_process");
const util = require("./util.js");
util.myLog("qwertyuytr");

const EXECUTABLE = path.join(__dirname, "_install", "game");

// creating server object
const app = express();

// getting port from environment with default value of 5000
const PORT = process.env.PORT || 5000;


const DELIM = path.join("", "");

// getting object of multiparty middleware
//const MULTIPARTY = multiparty();

// objects containing paths to different resources
const FILES = {
	main: { path: path.join("html", "main.html"), type: "text/html" },
	game: { path: path.join("html", "game.html"), type: "text/html" },
	main_err: { path: path.join("html", "main_file_exists.html"), type: "text/html" },
	err_404: { path: path.join("html", "on_404.html"), type: "text/html" },
	favicon: { path: path.join("images", "Antiyoy_32.png"), type: "image/png" },
}

const DIRS = {
	script_path: "scripts",
	uploaded_sources: "uploaded_sources"
}

var game_in_progress = false;

var game_data = {};
var botFiles = [];


////////////////////////////////////////////////////////////////////////////////
//    SETTING GLOBAL HANDLERS
////////////////////////////////////////////////////////////////////////////////

// log function
app.use(multipart());
app.use((req, res, next) => {
	console.log(getDate(), req.method, req.url, req.headers, req.body, req.files["bot-name"].path);
	next();
});

// add to support multipart file uploading

// following doesn't work (don't know why)
app.use("/scripts", express.static("/scripts", {} ));

////////////////////////////////////////////////////////////////////////////////
//    HANDLING CUSTOM REQUESTS
////////////////////////////////////////////////////////////////////////////////

// get scripts
// app.get("/scripts/:src", (req, res, next) => {
// 	sendAFileFromDir(res, next, path.join(DIRS.script_path, req.params.src), "text/javascript");
// });

// get favicon
app.get("/favicon.ico", (req, res, next) => {
	sendAFile(res, next, FILES.favicon);
});

// get main webpage
app.get("/", (req, res, next) => {
	sendAFile(res, next, FILES.main);
});

app.get("/game", (req, res, next) => {
	sendAFile(res, next, FILES.game)
})

// get error page (TODO: remove it later)
app.get("/err", (req, res, next) => {
	sendAFile(res, next, FILES.main_err);
});

// get list of bots
app.get("/check_bots", (req, res, next) => {
	if (!req.xhr) {
		res.status(404);
		sendAFile(res, next, FILES.err_404);
		console.log(getDate(), "WARNING: try to access without XHR");
	} else {
		let bots = [];
		for (let i in botFiles) {
			bots[i] = botFiles[i].slice(0, botFiles[i].lastIndexOf("."));
		}
		res.status(200).json(bots).send();
	}
	console.log(getDate(), "sent bots!");
});

app.get("/game_data", (req, res, next) => {
	res.status(200).send(game_data);
});

app.post("/game_data", (req, res, next) => {
	game_data = req.body;
	res.sendStatus(200);
});

app.post("/clear_sources", (req, res, next) => {
	if (!req.xhr) {
		res.status(404);
		sendAFile(res, next, FILES.err_404);
		console.log(getDate(), "WARNING: try to access without XHR :)");
	} else {
		cleanSources();
		res.sendStatus(200);
	}
});

app.post("/start_game", (req, res, next) => {
	if (!req.xhr) {
		res.status(404);
		sendAFile(res, next, FILES.err_404);
		console.log(getDate(), "WARNING: try to access without XHR :)");
	} else {
		fs.readdir(path.join(__dirname, "uploaded_sources"), {}, (err, files) => {
			if (!err) {
				if (files.length >= 2 || files.length <= 6) {
					let code = buildGame();
					if (code == 0) {
						res.sendStatus(200);
						startGame();
					} else if (code == 1) {
						res.status(500).send("compile_error");
					} else {
						res.status(500).send("error_cmake_execute");
					}
				} else {
					res.status(500).send("sources_number_error");
				}
			} else {
				console.log("error reading files");
				res.status(500).send("error_read_sources");
			}
		});
	}
});

// post cpp Files
app.post("/upload_sources", (req, res, next) => {
	if (!req.xhr) {
		res.status(404);
		sendAFile(res, next, FILES.err_404);
		console.log(getDate(), "WARNING: try to access without XHR");
	} else {
		let file = path.join(__dirname, "uploaded_sources", req.files.sourceFile.name);
		if (fs.existsSync(file) || !checkFileExt(file, ".cpp")) { // maybe divide into two checks?
			res.sendStatus(403);
			fs.unlink(req.files.sourceFile.path);
			console.log(getDate(), "unsuccessfull upload");
		} else {
			fs.renameSync(req.files.sourceFile.path, file);
			res.status(200);
			sendAFile(res, next, FILES.main);
			console.log(getDate(), "success uploading " + req.files.sourceFile.name);
			botFiles.push(req.files.sourceFile.name);
		}
	}
});

app.use((req, res, next) => {
	sendAFile(res, next, FILES.err_404);
});

////////////////////////////////////////////////////////////////////////////////
//    LAUNCH SERVER
////////////////////////////////////////////////////////////////////////////////

// listen on environment or 5000 port
app.listen(PORT, () => {
	console.log(getDate(), "Server listening on port " + PORT + "!");
});

// listen on standard http port
app.listen(80, () => {
	console.log(getDate(), "Server listening on port 80!");
});

////////////////////////////////////////////////////////////////////////////////
//    ADDITIONAL FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

// send file to client
function sendAFileFromDir(res, next, path, type) {
	res.sendFile(path, {
		root: __dirname,
		headers: { "Content-Type": type }
	},
		(err) => {
			if (err) {
				console.log(getDate(), "error sending " + path);
				next(err);
			} else {
				console.log(getDate(), "successfully sent " + path);
			}
		});
};

// send file using FILES object
function sendAFile(res, next, file) {
	sendAFileFromDir(res, next, file.path, file.type);
};

// get date string in UTC with brackets and colon
function getDate() {
	let date = "[" + new Date().toUTCString() + "]: ";
	return date;
};

// check if file extension is the matches ${ext}
function checkFileExt(file, ext) {
	if (ext[0] != ".") {
		ext = "." + ext;
	}
	return file.slice(-ext.length) == ext;
};

function buildGame() {
	let code = 0;
	//////TRY...CATCH
	let result = execSync("cmake -H. -B_build -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Release && cmake --build _build --clean-first");
	return result.status;
}

function startGame() {
	game_in_progress = true;
	let proc = execFile(EXECUTABLE);
	proc.on("exit", () => {
		game_in_progress = false;
	})
}

function cleanSources() {
	let files = fs.readdirSync("uploaded_sources");
	for (let i in files) {
		fs.unlinkSync(path.join(__dirname, DIRS.uploaded_sources));
	}
}