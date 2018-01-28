"use strict";

////////////////////////////////////////////////////////////////////////////////
//  DEPENDENCIES
////////////////////////////////////////////////////////////////////////////////
const express = require("express");
const parser = require("body-parser");
const multiparty = require("connect-multiparty");
const fs = require("fs");


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
            logger(`ERROR: error while deleting ${m[i]} file`);
        } else {
            logger(`INFO: successfuly deleted ${m[i]} file`);
        }
    });
});

main.use(multiparty({ uploadDir: "./uploaded" }));
main.use(parser.json());
main.use(utils.reqLogger);


////////////////////////////////////////////////////////////////////////////////
//  MAIN HANDLER
////////////////////////////////////////////////////////////////////////////////
main.get("/", (req, res, next) => {
    res.redirect("/lobby");
});
logger("INFO: redirect from '/' to '/lobby' is set");

////////////////////////////////////////////////////////////////////////////////
//  LOBBY
main.get("/lobby", (req, res) => {
    utils.sendPage(res, "lobby");
});

main.get("/lobby/data", (req, res) => {
    let bots = req.query.bots.split(",");
    let state = req.query.state;
    if (utils.equalArrays(bots.sort(), lobbyData.bots.sort()) &&
        state === lobbyData.state) {
        let id;
        do {
            id = Math.random();
        } while (id in polls);
        polls[id] = res;
        req.on("close", () => {
            delete polls[id];
            logger(`INFO: request ${id} was closed`);
        });
        logger(`INFO: registered request ${id}`);
    } else {
        utils.sendJSON(res, 200, lobbyData);
        logger("INFO: immediately sent data");
    }
});

main.post("/lobby/data", (req, res) => {
    let command = req.query.command;
    logger(`INFO: received ${command} command`);
    switch (commmand) {
        case "add":
            let file = req.files.source;
            let { name, path } = file;
            fs.exists(`./bot_sources/${name}`, (exists) => {
                if (exists) {
                    res.status(500).send(`ERROR: file ${name} already exists`);
                    logger(`ERROR: cannot receive file ${name}: file already exists`);
                } else {
                    fs.rename(path, `./${name}`, (err) => {
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
        case "remove":
            let bots = req.query.bots.split(",");
            bots.forEach((bot) => {
                fs.exists(`./bot_sources/${bot}`, (exists) => {
                    if (exists) {
                        fs.unlink(`./bot_sources/${bot}`, (err) => {
                            if (err) {
                                logger(`ERROR: error while deleting bot ${bot}`);
                            } else {
                                logger(`INFO: successfully deleted bot ${bot}`);
                            }
                        });
                    } else {
                        logger(`ERROR: bot ${bot} doesn't exist`);
                    }
                });
            });
            res.sendStatus(200);
            respondAll(lobbyPolls, lobbyData);
            break;
        case "startGame":
            let bots = req.query.bots.split(",");
            let map = req.query.map;
            let count = req.query.count;
            let header = fs.createWriteStream("./arch/dynamic.h", "w");
            lobbyData.state = gameData.state = "building";
            respondAll(lobbyPolls, lobbyData);
            respondAll(gamePolls, gameData);
            header.write(`#define BOTS_COUNT ${count}\n`);
            for (let i = 0; i < count; ++i) {
                if (i < bots.length) {
                    header.write(`#define BOT_${i} ${utils.cutExt(bots[i])}\n`);
                } else {
                    header.write(`#define BOT_${i} game_module::Bot\n`);
                }
            }
            logger("INFO: wrote dynamic header");
             
            break;
        default:
            res.sendStatus(403);
    }
})


////////////////////////////////////////////////////////////////////////////////
//  GAME
main.get("/game", (req, res) => {
    utils.sendPage(res, "game");
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