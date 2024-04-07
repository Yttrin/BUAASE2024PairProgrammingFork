// test.js
import assert from "assert";
import {bocchiShutUp} from "./t1_cpp/glue.js";

let seq = [];
let size = 0;
let test_flag = 0;
let generate_ans = 0;
let debug = false;
let map = new Map();

function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

function init() {
    map.clear();
    for(let i = 11; i <= 16; i++) {
        map.set(i,0);
    }
    for(let i = 21; i <= 26; i++) {
        map.set(i,0);
    }
}

function generateSeq() {
    seq = [];
    let _10_cnt = 0;
    let _20_cnt = 0;
    size = getRandomInt(0,10000); // 0是否要测
    test_flag = getRandomInt(1,3);

    for(let i = 0; i < size; i++) {
        let flag = getRandomInt(1,3);
        let base = 10;
        if (flag === 2) {
            base = 30 - base;
            _20_cnt += 1;
        } else {
            _10_cnt += 1;
        }
        let number = getRandomInt(1,7);
        seq.push(number + base);
        map.set(number + base,map.get(number + base) + 1);
    }
    if (debug) {
        console.log("生成测试用例:")
        console.log("flag:" + test_flag);
        console.log("seq: " + seq);
        console.log("size: " + size);
    }
    generate_ans = checkMax(test_flag);
}

function checkMax(flag) {
    let base = flag * 10;
    let bound = base + 7;
    let max_cnt = 0;
    let max_num = 0;
    map.forEach((value, key, map) => {
        if (value > max_cnt && base < key && key < bound) {
            max_cnt = value;
            max_num = key;
        }
    })
    map.forEach((value, key, map) => {
        if (value === max_cnt && max_num !== key  && base < key && key < bound) {
            max_num = 10;
        }
    })
    return max_num;
}


for(let i = 0; i < 1000000; i++) {
    init();
    if (debug) {
        console.log("第 " + i + " 次测试")
    } else if (i % 10000 === 0) {
        console.log("Running Testcase : " + i);
    }
    generateSeq();
    let ans = bocchiShutUp(test_flag,seq,size);
    if (ans !== generate_ans) {
        console.log("Assert Failed!");
        console.log("flag:" + test_flag);
        console.log("seq: " + seq);
        console.log("size: " + size);
        console.log("generate_ans: " + generate_ans);
        console.log("your_ans: " + ans);
        break;
    }
    if (debug) {
        console.log("generate_ans: " + generate_ans);
        console.log("your_ans: " + ans);
    }
}

console.log("🎉 You have passed all the tests provided.");




