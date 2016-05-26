var SpellChecker;

try {
  SpellChecker = require("./build/Debug/hfst-ospell").SpellChecker;
} catch (e) {
  SpellChecker = require("./build/Release/hfst-ospell").SpellChecker;
}


if (typeof Promise === 'undefined') {
  throw new Error("hfst-ospell-js requires Promise support, but `Promise` was not found in global namespace.");
}

var callbackSuggestions = SpellChecker.prototype.suggestions;

SpellChecker.prototype.suggestions = function suggestions(word, cb) {
  var self = this;

  return new Promise(function (resolve, reject) {
    callbackSuggestions.call(self, word, function (error, res) {
      if (typeof cb === 'function') {
        cb(error, res);
      }

      if (error) {
        reject(error);
      } else {
        resolve(res);
      }
    });
  });
};

module.exports = {
  SpellChecker: SpellChecker
};
