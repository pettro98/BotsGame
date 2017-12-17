"use strict";

var constants = module.exports;

constants.GAME_EXEC = __dirname + "/_install/bin/game";
constants.PORT = process.env.PORT || 80;
// constants.HTTP_PORT = 80;
constants.PATH_DELIM = "/";
constants.FILES = {
	main: { path: "html/main.html", type: "text/html" },
	game: { path: "html/game.html", type: "text/html" },
	err404: { path: "html/on_404.html", type: "text/html" },
	favicon: { path: "images/Antiyoy_32.png", type: "image/png" },
}
constants.DIRS = {
	scripts: "scripts",
	botSrc: "bot_sources"
}