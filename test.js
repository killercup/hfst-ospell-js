import test from 'ava';
import {spellcheck} from "./build/Release/hfst-ospell";

test("bindings work", (t) => {
    t.throws(() => spellcheck(), /wrong number of arguments/i);
    t.throws(() => spellcheck(12), /should be a string/i);
});

test("no suggestions for correct word", (t) => {
    t.deepEqual(spellcheck("Lákku"), false);
});

test("spelling suggestions", (t) => {
    t.deepEqual(spellcheck("akkusativa"),
        ['akkusatiivva', 'akkusatiiva', 'akkusatiivan']);
});

test("no spelling suggestions possible", (t) => {
    t.deepEqual(spellcheck("apfelkuchen"), []);
});
