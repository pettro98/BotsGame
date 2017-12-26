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
const bodyparser = require("body-parser");


////////////////////////////////////////////////////////////////////////////////
//  IMPORTING CUSTOM MODULES
////////////////////////////////////////////////////////////////////////////////

const u = require("./serv_util");
const c = require("./serv_constants");


////////////////////////////////////////////////////////////////////////////////
//  GLOBAL VARIABLES & CONSTANTS
////////////////////////////////////////////////////////////////////////////////

const app = express();

var data = {
    state: "idle",
    lastTurn: 0,
    playingBots: [],
    currentStats: {},
    fieldHistory: [],
    botSources: []
};

var polls = {
    bots: {
        req: [],
        res: []
    },
    game: {
        req: [],
        res: []
    }
}


////////////////////////////////////////////////////////////////////////////////
//  GENERAL REQUEST HANDLERS
////////////////////////////////////////////////////////////////////////////////

app.use(multipart());
app.use(bodyparser.json());
app.use(bodyparser.urlencoded({
    extended: true
}));
app.use(u.logReq);
app.use(favicon(c.FILES.favicon.path));
app.use("/scripts", express.static(c.DIRS.scripts));


////////////////////////////////////////////////////////////////////////////////
//  CUSTOM REQUEST HANDLERS
////////////////////////////////////////////////////////////////////////////////

app.get("/", getMain);

app.post("/bots", postBots);

app.get("/cmake_log", getCMakeLog);

app.get("/game", getGame);

app.post("/game", postGame);

app.post("/data", postData);

app.use(notFound);


////////////////////////////////////////////////////////////////////////////////
//  LAUNCH SERVER
////////////////////////////////////////////////////////////////////////////////

fs.exists("./cmake_log.txt", (exists) => {
    if (exists) {
        fs.unlink("./cmake_log.txt", (err) => {
            u.log("ERROR: Error unlinking cmake_log.txt")
        });
        u.log("INFO: cmake_log.txt deleted successfully");
    }
})

// listen on environment or 80 port
app.listen(c.PORT, () => {
    u.log("INFO: Server listening on port " + c.PORT);
});


////////////////////////////////////////////////////////////////////////////////
//  WEBPAGE AND FILE SENDING
////////////////////////////////////////////////////////////////////////////////

function getMain(req, res, next) {
    u.sendFILE(res, next, "main");
}

function getGame(req, res, next) {
    u.sendFILE(res, next, "game");
}

function getCMakeLog(req, res, next) {
    let log = "./cmake_log.txt";
    fs.exists(log, (exists) => {
        if (exists) {
            res.download(log);
        } else {
            res.status(200).send("No cmake log found!");
        }
    });
}

function notFound(req, res, next) {
    u.log("ERROR: Page " + req.url + " not found");
    res.sendStatus(404);
}


////////////////////////////////////////////////////////////////////////////////
//  HANDLER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  /bots HANDLER FUNCTIONS

function postBots(req, res, next) {
    if (deniedWithoutXHR(req, res)) return;
    u.log("INFO: Received postBots request");

    let command = req.body.command;
    switch (command) {
        case "get":
            {
                u.log("INFO: Received 'get' command");
                processBotsReq(req, res);
                break;
            }
        case "add":
            {
                u.log("INFO: Received 'add' command");
                addBots(req, res);
                break;
            }
        case "clean":
            {
                u.log("INFO: Received 'clean' command");
                cleanBots(req, res);
                break;
            }
        case "start":
            {
                u.log("INFO: Received 'start' command");
                startGame(req, res);
                break;
            }
        default:
            {
                u.log("ERROR: Bad command: " + command);
                res.sendStatus(400);
            }
    }
}

//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  /game/data HANDLER FUNCTIONS

function postGame(req, res, next) {
    if (deniedWithoutXHR(req, res)) return;
    u.log("INFO: Received postGame request");
    processGameReq(req, res);
}

function postData(req, res, next) {
    u.log("INFO: Received postData request");
    let body = JSON.parse(req.body);
    data.currentStats = body.stats;
    data.lastTurn = body.lastTurn;
    data.fieldHistory.push(body.field);
    res.sendStatus(200);
    sendGameData();
}

//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  OTHER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  LONG POLL FUNCTIONS

function processBotsReq(req, res) {
    let content = JSON.parse(req.body.content);
    let tmp = getBotsList();
    if (content.state != data.state) {
        res.status(200).json({
            state: data.state
        });
        u.log("INFO: Immediately sent game state");
        return;
    }
    if (content.bots.length != tmp.length) {
        res.status(200).json({
            bots: tmp
        });
        u.log("INFO: Immediately sent bots list (different count)");
        return;
    }
    for (let i in content.bots) {
        if (tmp.indexOf(content.bots[i]) == -1) {
            res.status(200).json(tmp);
            u.log("INFO: Immediately sent bots list (different content)");
            return;
        }
    }
    registerBotsReq(req, res);
}

function registerBotsReq(req, res) {
    req.on("close", () => {
        let pos = polls.bots.req.indexOf(this);
        polls.bots.res.splice(pos, 1);
        polls.bots.req.splice(pos, 1);
    });
    polls.bots.req.push(req);
    polls.bots.res.push(res);
    u.log("INFO: Registered bots request");
}

function processGameReq(req, res) {
    let content = JSON.parse(req.body.content);
    if (content.state != data.state) {
        res.status(200).json({
            state: data.state,
            stat: data.stats,
            bots: data.playingBots
        });
        u.log("INFO: Immediately sent state");
        return;
    }
    if (content.turn > 0) {
        if (content.turn > data.lastTurn) {
            res.sendStatus(400);
            u.log("ERROR: Wrong turn number");
            return;
        }
        let tmp = {
            field: data.fieldHistory[content.turn - 1],
            turn: content.turn,
            bots: data.playingBots
        }
        if (content.lastTurn != data.lastTurn) {
            tmp.lastTurn = data.lastTurn;
            tmp.stats = data.stats;
        }
        res.status(200).json(tmp);
        u.log("INFO: Immediately sent data on turn " + content.turn);
        return;
    } else {
        if (content.lastTurn > data.lastTurn) {
            res.sendStatus(400);
            u.log("ERROR: Wrong lastTurn number");
            return;
        }
        if (content.lastTurn < data.lastTurn) {
            res.status(200).json({
                lastTurn: data.lastTurn,
                field: data.fieldHistory[data.lastTurn - 1],
                stats: data.currentStats,
                bots: data.playingBots
            });
            u.log("INFO: Immediately sent data on last turn");
            return;
        }
    }
    registerGameReq(req, res);
}

function registerGameReq(req, res) {
    req.on("close", () => {
        let pos = polls.bots.req.indexOf(this);
        polls.bots.res.splice(pos, 1);
        polls.bots.req.splice(pos, 1);
    });
    polls.bots.req.push(req);
    polls.bots.res.push(res);
    u.log("INFO: Registered game request");
}

function sendGameData() {
    let resp = polls.game.res;
    let tmp = {
        stats: data.currentStats,
        lastTurn: data.lastTurn,
        field: data.fieldHistory[data.lastTurn - 1],
        bots: data.playingBots
    }
    for (let i in resp) {
        resp[i].status(200).json(tmp);
    }
    polls.game.res = [];
    polls.game.req = [];
    u.log("INFO: Sent updated game data");
}

function sendState() {
    let tmp = {
        state: data.state
    }
    let gRes = polls.game.res;
    for (let i in gRes) {
        gRes[i].status(200).json(tmp);
    }
    polls.game.res = [];
    polls.game.req = [];
    let bRes = polls.bots.res;
    for (let i in bRes) {
        bRes[i].status(200).json(tmp);
    }
    polls.bots.res = [];
    polls.bots.req = [];
    u.log("INFO: Sent updated game state");
}

function sendBots() {
    let resp = polls.bots.res;
    let tmp = JSON.stringify({
        state: data.state,
        bots: getBotsList()
    });
    for (let i in resp) {
        resp[i].status(200).send(tmp);
    }
    polls.bots.res = [];
    polls.bots.req = [];
    u.log("INFO: Sent updated bots list");
}

//
////////////////////////////////////////////////////////////////////////////////

function deniedWithoutXHR(req, res) {
    if (!req.xhr) {
        res.sendStatus(403);
        u.log("ERROR: Received non-XHR request.");
        return true;
    }
    return false;
}

function getBotsList() {
    let bots = fs.readdirSync(c.DIRS.botSrc);
    for (let i in bots) {
        bots[i] = u.cutPath(bots[i]);
    }
    return bots;
}

function addBots(req, res) {
    let name = req.files.source.name;
    let path = req.files.source.path;
    let fName = c.DIRS.botSrc + "/" + name;
    fs.exists(fName, (exists) => {
        if (exists) {
            res.status(400).send("ERROR: File " + name + " already exists.");
            u.log("ERROR: Cannot receive " + name + ". File already exists.");
            fs.unlink(path, (err) => {
                if (err) {
                    u.log("ERROR: Unlink file error: \n" + err.stack);
                }
            });
        } else {
            fs.rename(path, fName, (err) => {
                if (err) {
                    u.log("ERROR: Rename/Move file error: \n" + err.stack);
                    res.sendStatus(500);
                } else {
                    u.log("INFO: Received bot " + name + " successfully!");
                    res.sendStatus(200);
                    sendBots();
                }
            });
        }
    });
}

function cleanBots(req, res) {
    let bots = JSON.parse(req.body.content);
    if (bots.length == 0) {
        bots = getBotsList();
    }
    for (let i in bots) {
        fs.unlink(c.DIRS.botSrc + "/" + bots[i], (err) => {
            if (err) {
                u.log("ERROR: Unlink file error \n" + err.stack);
            }
        });
    }
    u.log("INFO: Cleaned bots ", ...bots, " successfully!");
    res.sendStatus(200);
    sendBots();
}

function startGame(req, res) {
    let content = JSON.parse(req.body.content);
    data.botSources = content.bots;
    data.playingBots = [];
    for (let i in data.botSources) {
        data.playingBots[i] = data.botSources[i]
    }
    setDynamicHeader(content);
    let code = buildGame(content.bots);
    if (code == 127 || code === null) {
        res.status(500).send("ERROR: Compilation failed. CMake not found. Check CMake installation");
        u.log("ERROR: Compilation failed. CMake not found. Check CMake installation");
    
    } else if (code != 0) {
        res.status(500).send("ERROR: Compilation failed. Check CMakeLog");
        u.log("ERROR: Compilation failed. Check CMakeLog");
    } else {
        execGame();
        u.log("INFO: Started game process");
        res.sendStatus(200);
    }
}

function setDynamicHeader(content) {
    let headerPath = __dirname + "/arch/dynamic.h";
    let header = "#define BOTS_COUNT " + content.count + "\n";
    let i = 0;
    for (; i < content.bots.length && i < 6; i++) {
        header += "#define BOT_" + i + " " + u.cutPathExt(content.bots[i]) + "\n";
    }
    for (; i < content.count; i++) {
        header += "#define BOT_" + i + " " + "game_module::Bot \n";
        data.playingBots.push("-= D3V310P3R =-");
    }
    header += "#define MAP_TYPE \"" + content.map + "\"";
    fs.writeFileSync(headerPath, header);
    // , (err) => {
    //     if (err) {
    //         u.log("ERROR: Failed creating dynamic header")
    //     } else {
    u.log("INFO: Dynamic header created");
    //     }
    // });
}

function buildGame(bots) {
    u.log("INFO: Build process started");
    data.state = "building";
    sendState();
    for (let i in bots) {
        bots[i] = c.DIRS.botSrc + "/" + bots[i];
    }
    if (fs.existsSync("./cmake_log.txt")) {
        fs.unlinkSync("./cmake_log.txt");
    }
    // let logStream = fs.createWriteStream("./cmake_log.txt", { flags: "a" });
    process.env.BOT_SOURCES = bots.join(" ");
    let result = exec.spawnSync("cmake", ["-H.", "-B_build",
        "-DCMAKE_INSTALL_PREFIX=_install", "-DCMAKE_BUILD_TYPE=Release"], {
            env: process.env,
            maxBuffer: 1024*1024
        });
    fs.writeFileSync("./cmake_log.txt", result.output, { flag: "a" });
    result = exec.spawnSync("cmake", ["--build", "_build"], {
        env: process.env,
        maxBuffer: 1024*1024
    });
    fs.writeFileSync("./cmake_log.txt", result.output, { flag: "a" });
    result = exec.spawnSync("cmake", ["--build", "_build", "--target", "install"], {
        env: process.env,
        maxBuffer: 1024*1024
    });
    // logStream.end();
    fs.writeFileSync("./cmake_log.txt", result.output, { flag: "a" });
    u.log("INFO: Build process finished");
    u.log("INFO: CMakeLog file created");
    data.state = "idle";
    sendState();
    return result.status;
}

function execGame() {
    exec.spawn("./_install/bin/game")
        .on("close", (code, signal) => {
            if (code == 0) {
                data.state = "finished";
                u.log("INFO: Game process closed");
                sendState();
            } else {
                data.state = "error";
                u.log("ERROR: Game process closed with error");
                sendState();
            }
        });
    data.state = "running";
    sendState();
    u.log("INFO: Game process started!");
}
