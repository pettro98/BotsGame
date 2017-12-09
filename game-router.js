"use strict";

const express = require("express");
const util = require("./util.js");

module.exports = gameSubapp;

function gameSubapp() {
    let game = require("express")();

    let gameInProgress = false;
    let gameData = [];

    function getGame(req, res, next) {
        util.sendAFile(res, next, util.FILES.game);
    };

    function getData(req, res, next) {
        if (req.body == "") {
            res.status(200).json(gameData[gameData.length - 1]);
        } else {
            res.status(200).json(gameData[res.body]);
        }
    };

    function receiveData(req, res, next) {
        gameData.push(JSON.parse(req.body));
        res.sendStatus(200);
    }

    game.get("/", getGame);
    game.get("/data", getData);
    game.post("/data", receiveData);

    return game;
}