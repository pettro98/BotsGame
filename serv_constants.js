"use strict";

var constants = module.exports;

constants.GAME_EXEC = __dirname + "/_install/bin/game";
constants.PORT = process.env.PORT || 80;
constants.PATH_DELIM = "/";
constants.FILES = {
	main: { path: __dirname + "/html/main.html", type: "text/html" },
	game: { path: __dirname + "/html/game.html", type: "text/html" },
	err404: { path: __dirname + "/html/on_404.html", type: "text/html" },
    favicon: { path: __dirname + "/images/Antiyoy_32.png", type: "image/png" },
    cmake_log: { path: __dirname + "/cmake_log.txt", type: "text/plain"}
}
constants.DIRS = {
	scripts: __dirname + "/scripts",
	botSrc: __dirname + "/bot_sources"
}