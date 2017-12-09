"use strict";

const fs = require("fs");
const path = require("path");
const util = require("./util.js");
const exec = require("child_process");

module.exports = botsSubapp;



function botsSubapp() {
    let bots = require("express")();

    let botList = {};

    cleanSources();

    function getBotNames(res, req, next) {
        let botList2 = {};
        for (let i in botList) {
            botList2[i].path = util.getShortName(botList[i].path);
        }
        res.status(200).json(botList2);
    };

    function addSource(req, res, next) {
        let fname = path.join(__dirname, util.DIRS.uploadedSrc,
            req.files[util.INPUT_NAME].name);
        if (fs.existsSync(fname)) {
            res.status(400).send("file_already_exists");
            fs.unlinkSync(req.files[util.INPUT_NAME].path);
            util.myLog("ERROR: file already exists");
            return;
        }
        fs.renameSync(req.files[util.INPUT_NAME].path, fname);
        botList[req.files["bot-name"]] = { path: fname };
        util.myLog("Received bot source successfully");
    };

    function cleanSources(req, res, next) {
        let toDel = [];

        for (let i in botList) {
            fs.unlink(botList[i].path, (err) => {
                if (err) {
                    util.myLog("error unlinking file " + botList[i].path);
                } else {
                    toDel.push(i);
                }
            });
        }

        for (let i in toDel) {
            if (botList[toDel[i]]) {
                delete botList[i];
            }
        }
    };

    function getSourcesList(dir) {
        let list = fs.readdirSync(dir);
        for (let i in list) {
            list[i] = path.join(dir, list[i]);
        }
        return list.join(" ");
    };

    function setHeader(mas) {
        let headerPath = path.join(__dirname, "arch", "dynamic.h");
        let header = "#define BOTS_COUNT " + mas[0] + "\n";
        let i = 1;
        for (; i < mas.length && i < 6; i++) {
            header += "#define BOT_" + i + " " +
                util.withoutExt(util.getShortName(botList[mas[i]].path)) + "\n";
        }
        for (; i < mas[0]; i++) {
            header += "#define BOT_" + i + " " + "game_module::Bot \n";
        }
        fs.writeFileSync(headerPath, header);
    };

    function buildGame() {
        let result = exec.spawnSync("cmake -H. -B_build -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Release && cmake --build _build --clean-first",
            { env: process.env });
        return result.status;
    };

    function startGame(req, res, next) {
        let mas = req.files["content"].split(" ");
        setHeader(mas);

        process.env.BOT_SOURCES = getSourcesList("arch");
        let buildCode = buildGame();
        if (buildCode == 1) {
            res.status(500).send("compile_error");
            return;
        } else if (buildCode == 127) {
            res.status(500).send("cmake_not_found");
            return;
        }

        res.sendStatus(200);
    };

    function postBots(req, res, next) {
        if (!req.xhr) {
            res.sendStatus(403);
            util.myLog("WARNING: tried to access without XHR");
            return;
        }
        if (req.files["command"] === "clean") {
            cleanSources();
            res.sendStatus(200);
        } else if (req.files["command"] === "add") {
            addSource(req, res, next);
            res.sendStatus(200);
        } else if (req.files["command"] === "start") {
            startGame(req, res, next);
        }
    };

    bots.route("/")
        .get(getBotNames)
        .post(postBots);

    return bots;
};