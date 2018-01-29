"use strict";

////////////////////////////////////////////////////////////////////////////////
//  DEPENDENCIES
////////////////////////////////////////////////////////////////////////////////
const express = require("express");
const parser = require("body-parser");
const multiparty = require("connect-multiparty");
const fs = require("fs");
const cp = require("child_process");


////////////////////////////////////////////////////////////////////////////////
//  LOCAL DEPENDENCIES
////////////////////////////////////////////////////////////////////////////////
const utils = require("./utils");

////////////////////////////////////////////////////////////////////////////////
//  GLOBAL OBJECTS
////////////////////////////////////////////////////////////////////////////////
const main = express();
const lobbyData = {
    bots: fs.readdirSync("./bot_sources"),
    state: "idle"
};
const gameData = {
    bots: [],
    state: "idle"
};
const logger = utils.logger;
const lobbyPolls = {};
const gamePolls = {};


////////////////////////////////////////////////////////////////////////////////
//  INITIALIZATION
////////////////////////////////////////////////////////////////////////////////
fs.exists("./cmake_log.txt", (exists) => {
    if (exists) {
        fs.unlink("./cmake_log.txt", (err) => {
            if (err) {
                logger("ERROR: error while dleting cmake_log.txt");
            } else {
                logger("INFO: successfully deleted cmake_log.txt");
            }
        });
    } else {
        logger("INFO: no need to delete cmake_log.txt");
    }
});
fs.readdirSync("./uploaded").forEach((file) => {
    fs.unlink(`./uploaded/${file}`, (err) => {
        if (err) {
            logger(`ERROR: error while deleting ${file} file`);
        } else {
            logger(`INFO: successfuly deleted ${file} file`);
        }
    });
});

main.use(multiparty({ uploadDir: "./uploaded" }));
main.use(parser.json());
main.use(parser.urlencoded({
    extended: true
}));
main.use(utils.reqLogger);
main.use(require("serve-favicon")("./images/Antiyoy_32.png"));
main.use("/scripts", express.static("./scripts"));

////////////////////////////////////////////////////////////////////////////////
//  MAIN HANDLER
////////////////////////////////////////////////////////////////////////////////
main.get("/", (req, res, next) => {
    res.redirect("/lobby");
});
logger("INFO: redirect from '/' to '/lobby' is set");

main.get("/cmake_log", (req, res) => {
    fs.exists("./cmake_log.txt", (exists) => {
        if (exists) {
            res.download("./cmake_log.txt");
        } else {
            res.status(200).send("No cmake log found!");
        }
    });
});

////////////////////////////////////////////////////////////////////////////////
//  LOBBY
main.get("/lobby", (req, res) => {
    utils.sendPage(res, "lobby");
});

main.get("/lobby/data", (req, res) => {
    let bots = req.query.bots.split(",");
    if (bots[0] === "") {
        bots = [];
    }
    let state = req.query.state;
    if (utils.equalArrays(bots.sort(), lobbyData.bots.sort()) &&
        state === lobbyData.state) {
        let id;
        do {
            id = Math.random();
        } while (id in lobbyPolls);
        lobbyPolls[id] = res;
        req.on("close", () => {
            delete lobbyPolls[id];
            logger(`INFO: request ${id} was closed`);
        });
        logger(`INFO: registered request ${id}`);
    } else {
        res.status(200).json(lobbyData);
        logger("INFO: immediately sent data");
    }
});

main.post("/lobby/data", (req, res) => {
    let command = req.query.command;
    logger(`INFO: received ${command} command`);
    switch (command) {
        case "add": {
            let file = req.files.source;
            let { name, path } = file;
            fs.exists(`./bot_sources/${name}`, (exists) => {
                if (exists) {
                    res.status(500).send(`ERROR: file ${name} already exists`);
                    logger(`ERROR: cannot receive file ${name}: file already exists`);
                } else {
                    fs.rename(path, `./bot_sources/${name}`, (err) => {
                        if (err) {
                            res.status(500).send(`ERROR: error while receiving file ${name}`);
                            logger(`ERROR: error while moving file ${name} to bot_sources directory`);
                            console.log(utils.getUTCDateString(), err);
                            fs.unlink(path, (err) => {
                                if (err) {
                                    logger("ERROR: error while deleting uploaded file");
                                } else {
                                    logger("INFO: successfully deleted uploaded file");
                                }
                            });
                        } else {
                            res.sendStatus(200);
                            logger(`INFO: successfully received file ${name}`);
                            lobbyData.bots = fs.readdirSync("./bot_sources");
                            respondAll(lobbyPolls, lobbyData);
                        }
                    });
                }
            });
            break;
        }
        case "remove": {
            let bots = req.query.bots.split(",");
            bots.forEach((bot) => {
                if (fs.existsSync(`./bot_sources/${bot}`)) {
                    fs.unlinkSync(`./bot_sources/${bot}`)
                    logger(`INFO: successfully deleted bot ${bot}`);
                } else {
                    logger(`ERROR: bot ${bot} doesn't exist`);
                }
            });
            res.sendStatus(200);
            lobbyData.bots = fs.readdirSync("./bot_sources");
            respondAll(lobbyPolls, lobbyData);
            break;
        }
        case "startGame": {
            let bots = gameData.bots = req.query.bots.split(",");
            if (bots[0] === "") {
                bots = [];
            }
            let map = req.query.map;
            let count = req.query.count;
            let header = "";
            res.sendStatus(200);
            changeState("building");
            header += `#define BOTS_COUNT ${count}\n`;
            for (let i = 0; i < count; ++i) {
                if (i < bots.length) {
                    header += `#define BOT_${i} ${utils.cutExt(bots[i])}\n`;
                } else {
                    header += `#define BOT_${i} game_module::Bot\n`;
                    gameData.bots.push("-=< D3V310P3R >=-");
                }
            }
            header += `#define MAP_TYPE "${map}"\n`;
            fs.writeFileSync("./arch/dynamic.h", header);
            logger("INFO: wrote dynamic header");
            logger("INFO: build process started");
            bots.forEach((elem, index) => {
                bots[index] = "arch/" + elem;
            });
            if (fs.existsSync("./cmake_log.txt")) {
                fs.unlinkSync("./cmake_log.txt");
            }
            let logStream = fs.createWriteStream("./cmake_log.txt", { flags: "a" });
            process.env.BOT_SOURCES = bots.join(" ");
            createBuildFiles(logStream, res);
            break;
        }
        default:
            res.sendStatus(400);
    }
});

function createBuildFiles(logStream, res) {
    cp.spawn("cmake", ["-H.", "-B_build",
        "-DCMAKE_INSTALL_PREFIX=_install", "-DCMAKE_BUILD_TYPE=Release"],
        {
            env: process.env
        }).on("close", (code) => {
            if (!code) {
                logger("INFO: successfully created build files");
                buildGame(logStream, res);
            } else {
                logger("ERROR: error while creating build files");
                logStream.end();
                logger("INFO: cmake_log.txt file created");
                changeState("build_error");
            }
        }).stdout.pipe(logStream, { end: false });
}

function buildGame(logStream, res) {
    cp.spawn("cmake", ["--build", "_build"],
        {
            env: process.env
        }).on("close", (code) => {
            if (!code) {
                logger("INFO: successfully built game");
                installGame(logStream, res);
            } else {
                logger("ERROR: error while building game");
                logStream.end();
                logger("INFO: cmake_log.txt file created");
                changeState("build_error");
            }
        }).stdout.pipe(logStream, { end: false });
}

function installGame(logStream, res) {
    cp.spawn("cmake", ["--build", "_build",
        "--target", "install"],
        {
            env: process.env
        }).on("close", (code) => {
            if (!code) {
                logger("INFO: successfully installed");
            } else {
                logger("ERROR: error while installing");
                changeState("build_error");
            }
            logger("INFO: build process ended");
            logStream.end();
            logger("INFO: cmake_log.txt file created");
            if (!code) {
                execGame();
            }
        }).stdout.pipe(logStream, { end: false });
}

function execGame() {
    cp.execFile("./_install/bin/game")
        .on("close", (code) => {
            if (!code) {
                logger("INFO: game process closed");
                changeState("finished");
            } else {
                logger(`ERROR: game finished with error ${i}`);
                changeState("error");
            }
        });
    changeState("running");
}


////////////////////////////////////////////////////////////////////////////////
//  GAME
main.get("/game", (req, res) => {
    utils.sendPage(res, "game");
});

main.get("/game/data", (req, res) => {
    let turn = res.query.turn;
    let lastTurn = res.query.lastTurn;
    let state = req.query.state;
    if (state != gameData.state) {
        res.status(200).json({
            F: gameData.F[gameData.F.length - 1],
            S: gameData.S,
            CT: gameData.CT,
            state: gameData.state
        });
        logger("INFO: immediately sent data because of state");
        return;
    }
    if (turn == -1 && lastTurn < gameData.CT) {
        res.status(200).json({
            F: gameData.F[gameData.F.length - 1],
            S: gameData.S,
            CT: gameData.CT,
            state: gameData.state
        });
        logger("INFO: immediately sent data because of last turn number");
        return;
    }
    if (turn != -1) {
        res.status(200).json({
            F: gameData.F[turn],
            S: gameData.S,
            CT: gameData.CT,
            state: gameData.state
        });
        logger("INFO: immediately sent data because of last turn number");
        return;
    }
    let id;
    do {
        id = Math.random();
    } while (id in gamePolls);
    gamePolls[id] = res;
    req.on("close", () => {
        delete gamePolls[id];
        logger(`INFO: request ${id} was closed`);
    });
    logger(`INFO: registered request ${id}`);
});

main.post("/game/data", (req, res) => {
    gameData.CT = req.body.CT;
    gameData.S = req.body.S;
    gameData.F.push(req.body.F);
    res.sendStatus(200);
    respondAll(gamePolls, gameData);
});

logger("INFO: setting up main completed");

main.listen((process.env.PORT || 80), "localhost", () => {
    logger(`INFO: main is listening on port ${utils.PORT}`);
});

////////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
function respondAll(polls, json) {
    for (let i in polls) {
        polls[i].status(200).json(json);
        delete polls[i];
    }
}

function changeState(state) {
    lobbyData.state = gameData.state = state;
    respondAll(lobbyPolls, lobbyData);
    respondAll(gamePolls, gameData);
}