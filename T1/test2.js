// test.js
import assert from "assert";
import {bocchiShutUp} from "./t1_cpp/glue.js";

// Test cases
assert.strictEqual(bocchiShutUp(1, [13, 14, 15, 21, 11, 16], 6), 10);
assert.strictEqual(bocchiShutUp(2, [13, 14, 13, 11, 21, 13, 21, 22], 8), 21);

console.log("🎉 You have passed all the tests provided.");