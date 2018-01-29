"use strict";

const utils = module.exports = {};

const PORT = utils.PORT = process.env.PORT || 5000;
const PAGES = utils.PAGES = {
    lobby: __dirname + "/html/lobby.html",
    game: __dirname + "/html/game.html"
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

utils.getUTCDateString = getUTCDateString;
utils.getURLParams = getURLParams;
utils.concatURL = concatURL;
utils.deepLog = deepLog;
utils.logger = logger;
utils.reqLogger = reqLogger;
utils.sendPage = sendPage;
utils.equalArrays = equalArrays;
utils.cutExt = cutExt;

function getUTCDateString() {
    return `[ ${new Date().toUTCString()} ]: `;
}

function getURLParams(params) {
    let mas = [];
    for (let i in params) {
        mas.push(`${i}=${params[i]}`);
    }
    return mas.join("&");
}

function concatURL(url, params) {
    return `${url}?${getURLParams(params)}`;
}

function deepLog(...args) {
    let out = process.stdout;
    for (let i in args) {
        if (typeof args[i] === "object") {
            out.write(JSON.stringify(args[i], null, 2));
        } else {
            out.write("" + args[i]);
        }
        out.write(" ");
    }
    out.write("\n");
}

function logger(...args) {
    deepLog(getUTCDateString(), ...args);
}

function reqLogger(req, res, next) {
    logger(req.method, req.url, req.headers, (req.body || "{}"), (req.files || "{}"));
    next();
}

function sendPage(res, name) {
    res.sendFile(PAGES[name]);
}

function equalArrays(arr1, arr2) {
    return JSON.stringify(arr1) === JSON.stringify(arr2);
}

function cutExt(path) {
    return path.slice(0, path.lastIndexOf("."));
}