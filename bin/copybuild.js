const fs = require("fs");
const path = require("path");

const addonSrc = path.resolve(__dirname + "/../build/Release/addon.node");
const addonDest = path.resolve(__dirname + "/../dist/addon.node");

fs.copyFileSync(addonSrc, addonDest);

const typingsSrc = path.resolve(__dirname + "/../lib/addon.d.ts");
const typingsDest = path.resolve(__dirname + "/../dist/addon.d.ts");

fs.copyFileSync(typingsSrc, typingsDest);