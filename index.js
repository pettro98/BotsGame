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
    botSources: [],
    currentStats: {},
    fieldHistory: []
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
app.use(u.logReq);
app.use(favicon(c.FILES.favicon.path));
app.use("/scripts", express.static(c.DIRS.scripts));


////////////////////////////////////////////////////////////////////////////////
//  CUSTOM REQUEST HANDLERS
////////////////////////////////////////////////////////////////////////////////

app.get("/", getMain);

app.get("/bots", getBots);
app.get("/bots/log", getCMakeLog);
app.post("/bots", postBots);

app.get("/game", getGame);
app.get("/game/data", getGameData);
app.post("/game/data", postGameData);

app.use(notFound);


////////////////////////////////////////////////////////////////////////////////
//  LAUNCH SERVER
////////////////////////////////////////////////////////////////////////////////

// listen on environment or 80 port
app.listen(c.PORT, () => {
    u.log("INFO: Server listening on port " + c.PORT);
});


////////////////////////////////////////////////////////////////////////////////
//  WEBPAGE AND FILE SENDING
////////////////////////////////////////////////////////////////////////////////

function getMain(req, res, next) {
    u.sendFile(res, next, "main");
}

function getGame(req, res, next) {
    u.sendFile(res, next, "game");
}

function getCMakeLog(req, res, next) {
    if(fs.existsSync("./CMakeLog.txt")){
        u.sendFileX(res, next, "./CMakeLog.txt", "text/plain");
    } else { 
        res.sendStatus(403);
    }
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

function getBots(req, res, next) {
    if (deniedWithoutXHR(req, res)) return;
    u.log("INFO: Received getBots request");
    registerBotsRequest(req, res);
}

function postBots(req, res, next) {
    if (deniedWithoutXHR(req, res)) return;
    u.log("INFO: Received postBots request");
    let command = req.body.command;
    switch (command) {
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
//  /game/data HANGLER FUNCTIONS

function getGameData(req, res, next) {
    if (deniedWithoutXHR(req, res)) return;
    u.log("INFO: Received getGameData request");
    registerGameRequest(req, res);
}

function postGameData(req, res, next) {
    let body = JSON.parse(req.body);
    data.currentStats = body.stats;
    data.lastTurn = body.lastTurn;
    data.fieldHistory.push(body.field);
    sendGameData();
}

//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  OTHER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  LONG POLL FUNCTIONS

function registerBotsRequest(req, res) {
    let body = JSON.parse(req.body);
    let bots = getBotsList();
    if (body.length != bots.length) {
        res.status(200).json(bots);
        u.log("INFO: Immediately sent bots list (different count)");
        return;
    }
    for (let i in body) {
        if (bots.indexOf(body[i]) == -1) {
            res.status(200).json(bots);
            u.log("INFO: Immediately sent bots list (different content)");
            return;
        }
    }
    req.on("close", () => {
        let pos = polls.bots.req.indexOf(this);
        polls.bots.res.splice(pos, 1);
        polls.bots.req.splice(pos, 1);
    });
    polls.bots.req.push(req);
    polls.bots.res.push(res);
    u.log("INFO: Registered bots request");
}

function registerGameRequest(req, res) {
    let body = JSON.parse(req.body);
    if (body.state != data.state) {
        res.status(200).json({
            state: data.state,
            stats = data.stats
        });
        u.log("INFO: Immediately sent state");
        return;
    }
    if (body.turn > 0) {
        if (body.turn > data.lastTurn) {
            res.sendStatus(400);
            u.log("ERROR: Wrong turn number");
            return;
        }
        let tmp = {
            field: data.fieldHistory[body.turn - 1]
        }
        if (body.lastTurn != data.lastTurn) {
            tmp.lastTurn = data.lastTurn;
            tmp.stats = data.stats;
        }
        res.status(200).json(tmp);
        u.log("INFO: Immediately sent data on turn " + body.turn);
        return;
    } else {
        if (body.lastTurn > data.lastTurn) {
            res.sendStatus(400);
            u.log("ERROR: Wrong lastTurn number");
            return;
        }
        if (body.lastTurn < data.lastTurn) {
            res.status(200).json({
                lastTurn: data.lastTurn,
                field: data.fieldHistory[data.lastTurn - 1],
                stats: data.currentStats
            });
            u.log("INFO: Immediately sent data on last turn");
            return;
        }
    }
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
        field: data.fieldHistory[data.lastTurn - 1]
    }
    for (let i in resp) {
        resp.status(200).json(tmp);
    }
    resp = [];
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
    gRes = [];
    polls.game.req = [];
    let bRes = polls.bots.res;
    for (let i in bRes) {
        bRes[i].status(200).json(tmp);
    }
    bRes = [];
    polls.bots.req = [];
    u.log("INFO: Sent updated game state");
}

function sendBots(){
    let resp = polls.bots.res;
    let tmp = {
        state: data.state,
        bots: getBotsList()
    }
    for (let i in resp) {
        resp.status(200).json(tmp);
    }
    resp = [];
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
    if (fs.existsSync(fName)) {
        fs.unlinkSync(path);
        res.status(400).send("ERROR: File " + name + " already exists.");
        u.log("ERROR: Cannot receive " + name + ". File already exists.");
        return;
    }
    fs.renameSync(path, fName);
    u.log("INFO: Received bot " + name + " successfully!");
    sendBots();
    res.sendStatus(200);
}

function cleanBots(req, res) {
    let bots = JSON.parse(req.body.content);
    if (bots.length == 0) {
        bots = getBotsList();
    }
    for (let i in bots) {
        fs.unlinkSync(c.DIRS.botSrc + "/" + bots[i]);
    }
    u.log("INFO: Cleaned bots ", ...bots, " successfully!");
    sendBots();
    res.sendStatus(200);
}

function startGame(req, res) {
    let content = JSON.parse(req.body.content);
    data.playingBots = data.botSources = content.bots;
    setDynamicHeader(content);
    let code = buildGame(bots);
    if (code == 1) {
        res.status(500).send("ERROR: Compilation failed. Check CMakeLog");
        u.log("ERROR: Compilation failed. Check CMakeLog");
    } else if (code == 127 || code === null) {
        res.status(500).send("ERROR: Compilation failed. CMake not found. Check CMake installation");
        u.log("ERROR: Compilation failed. CMake not found. Check CMake installation");
    }
    execGame();
    u.log("INFO: Started game process");
    res.sendStatus(200);
}

function setDynamicHeader(content) {
    let headerPath = __dirname + "/arch/dynamic.h";
    let header = "#define BOTS_COUNT " + content.count + "\n";
    let i = 0;
    for (; i < content.bots.length && i < 6; i++) {
        header += "#define BOT_" + i + " " + u.cutPathExt(content.bots[i]) + "\n";
    }
    for (; i <= content.count; i++) {
        header += "#define BOT_" + i + " " + "game_module::Bot \n";
        data.playingBots.push("-= D3V310P3R =-");
    }
    header += "define MAP_TYPE \"" + content.map + "\"";
    fs.writeFileSync(headerPath, header);
    u.log("INFO: Dynamic header created");
}

function buildGame() {
    u.log("INFO: Build process started");
    for(let i in data.botSources){
        data.botSources[i] = c.DIRS.botSrc + "/" + data.botSources[i];
    }
    process.env.BOT_SOURCES = data.botSources.join(" ");
    let result = exec.spawnSync("cmake -H. -B_build " +
        "-DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Release && " +
        "cmake --build _build --clean-first", {
            env: process.env
        });
    u.log("INFO: Build process finished");
    fs.writeFileSync("./CMakeLog.txt", result.output);
    u.log("INFO: CMakeLog file created");
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
    u.log("INFO: Game process started!");
}