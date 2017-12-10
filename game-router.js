"use strict";

const express = require("express");
const util = require("./util.js");

module.exports = gameSubapp;

function gameSubapp() {
    let game = require("express")();

    let gameInProgress = false;
    let gameData = [];
    let stats = {};

    function getGame(req, res, next) {
        util.sendAFile(res, next, util.FILES.game);
    };

    function getData(req, res, next) {
        if (req.body == "") {
            res.status(200).json({
                stat: stats,
                progress: util.gameInProgress,
                bots: util.botList,
                data: gameData[gameData.length - 1]
            });
        } else {
            res.status(200).json({
                stat: stats,
                progress: util.gameInProgress,
                bots: util.botList,
                data: gameData[req.body]
            });
        }
    };

    function receiveData(req, res, next) {
        let received = JSON.parse(req.body);
        gameData.push(received.data);
        stats = received.stat;
        res.sendStatus(200);
    }

    game.get("/", getGame);
    game.get("/data", getData);
    game.post("/data", receiveData);

    return game;
}