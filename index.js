var SpellChecker;

try {
    SpellChecker = require("./build/Debug/hfst-ospell").SpellChecker;
} catch (e) {
    SpellChecker = require("./build/Release/hfst-ospell").SpellChecker;
}

module.exports = {
    SpellChecker: SpellChecker
};
