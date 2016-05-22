import test from 'ava';
import {spellcheck} from "./build/Release/hfst-ospell";

test("bindings work", t => {
    t.throws(() => spellcheck(), /wrong number of arguments/i);
    t.throws(() => spellcheck(12), /should be string/i);

    t.truthy(spellcheck("schinken"));
});
