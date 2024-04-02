import assert from "assert";

// Choose proper "import" depending on your PL.
// import { mancalaResult } from "./t2-as/build/release.js";
// import { mancala_result as mancalaResult } from "./t2_rust/pkg/t1_rust.js"
// [Write your own "import" for other PLs.]
import {mancalaResult} from "./glue.js"
import {mancalaResult_2} from "./glue2.js"
function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

let seq = []; // 生成 1 到 10 之间的随机整数
let flag = 0;
let size = 0;
let change = 0;

let case_15000 = 0;
let case_20000 = 0;
let case_30000 = 0;
let debug = false;
let usingGenerator = true;
function generateSeq() {
    seq = [];

    size = getRandomInt(0,100); // 0是否要测
    flag = getRandomInt(1,3);

    let base = 10;
    if (flag == 2) {
        base = 30 - base;
    }
    for(let i = 0; i < size; i++) {
        let number = getRandomInt(1,7);
        seq.push(number + base);
        change = getRandomInt(0,11);
        if (change !== 10)
        base = 30 - base;
    }
    if (debug) {
        console.log("生成测试用例:")
        console.log("flag:" + flag);
        console.log("seq: " + seq);
        console.log("size: " + size);
    }

}


if (usingGenerator) {
    for(let j = 0; j < 100000000; j++) {
        generateSeq()
        let res_1 = mancalaResult(flag,seq,size);
        let res_2 = mancalaResult_2(flag,seq,size);
        if (debug) {
            console.log("==================")
            console.log("Test: " + j);
            console.log("res_1: " + res_1);
            console.log("res_2: " + res_2);
            console.log("==================")
        }
        if (j % 100000 == 0) {
            console.log("Running Testcase : " + j);
        }
        if (res_1 !== res_2) {
            console.log("~~~~~~~ASSERT~~~~~FAILED~~~~~~~~~");
            console.log("==================");
            console.log("Test: " + j);
            console.log("res_1: " + res_1);
            console.log("res_2: " + res_2);
            console.log("==================")
            console.log("flag:" + flag);
            console.log("seq: " + seq);
            console.log("size: " + size);
            console.log("~~~~~~~~TEST~~~~~~END~~~~~~~~~~~");
            throw new Error("Assert Failed");
        }
        if (res_1 < 20000) {
            case_15000 += 1;
            console.log("~~~~~~~CAUGHT~~~~~CASE ONE~~~~~~~~~");
            console.log("==================");
            console.log("Test: " + j);
            console.log("res_1: " + res_1);
            console.log("res_2: " + res_2);
            console.log("==================")
            console.log("flag:" + flag);
            console.log("seq: " + seq);
            console.log("size: " + size);
            console.log("~~~~~~~~CAUGHT~~~~~~CASE ONE~~~~~~~~~~~");
        } else if (res_1 < 30000) {
            case_20000 += 1;
        } else {
            case_30000 += 1;
        }
    }


    console.log("Case_15000 :" + case_15000);
    console.log("Case_20000 :" + case_20000);
    console.log("Case_30000 :" + case_30000);
} else {
    seq = [16,22,21,13,16,15,21,11,25,16,14,22,16,15,24,14,23,16,15,16,13,16,12,23,16,15,14,16,25,24,15,26,11,21,13,23,14,12,22,14,25,24,13,25,15,21,16];
    flag = seq[1] % 10;
    size = seq.length;
    let res_1 = mancalaResult(flag,seq,size);
    let res_2 = mancalaResult_2(flag,seq,size);
    assert.strictEqual(res_1,res_2);
    seq = [16,22,21,13,16,15,22,16,12,25,12,24,13,16,15,16,11,16,14,25,26,23,11,26,24,14,22,16,21,12,26,25,23,16,15,22,16,13,23,14,26,25,26,24,26,25,16,15,16];
    flag = seq[1] % 10;
    size = seq.length;
    res_1 = mancalaResult(flag,seq,size);
    res_2 = mancalaResult_2(flag,seq,size);
    assert.strictEqual(res_1,res_2);
    seq = [16,22,21,13,16,15,22,16,12,25,12,24,13,16,15,16,11,16,14,23,14,26,24,16,21,13,22,12,24,14,23,13,24,14,25,16,11,26];
    flag = seq[1] % 10;
    size = seq.length;
    res_1 = mancalaResult(flag,seq,size);
    res_2 = mancalaResult_2(flag,seq,size);
    assert.strictEqual(res_1,res_2);
}

console.log("🎉 You have passed all the tests provided.");
