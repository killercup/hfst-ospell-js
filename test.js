import test from 'ava';
import {hello} from "./build/release/hfst-ospell";

test("bindings work", t => {
    t.is(hello("world"), "hello, world");
    t.throws(() => hello(), /wrong number of arguments/i);
    t.throws(() => hello(12), /should be string/i);
});
