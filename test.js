import test from 'ava';
import {SpellChecker} from "./build/Debug/hfst-ospell";

test("constructor works", (t) => {
    const spellchecker = new SpellChecker("etc/se.zhfst");

    t.throws(() => new SpellChecker(), /wrong number of arguments/i);
    t.throws(() => new SpellChecker(2), /should be a string/i);
});

test("spell check signature", (t) => {
    const spellchecker = new SpellChecker("etc/se.zhfst");

    t.throws(() => spellchecker.check(), /wrong number of arguments/i);
    t.throws(() => spellchecker.check(2), /should be a string/i);
});

test("no suggestions for correct word", (t) => {
    const spellchecker = new SpellChecker("etc/se.zhfst");

    t.deepEqual(spellchecker.check("Lákku"), false);
});

test("spelling suggestions", (t) => {
    const spellchecker = new SpellChecker("etc/se.zhfst");

    t.deepEqual(spellchecker.check("akkusativa"),
        ['akkusatiivva', 'akkusatiiva', 'akkusatiivan']);
});

test("no spelling suggestions possible", (t) => {
    const spellchecker = new SpellChecker("etc/se.zhfst");

    t.deepEqual(spellchecker.check("apfelkuchen"), []);
});
