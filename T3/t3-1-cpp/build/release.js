import Module from "./t3_1.ems.js";


let a = await Module()
const mancala_board_cwarp = a.cwrap('mancala_board', 'number', ['number', 'number', 'number', 'number']);

function get_int32_pointer(inputArray) {
    let inputArrayPointer = a._malloc(inputArray.length * Int32Array.BYTES_PER_ELEMENT);
    a.HEAP32.set(inputArray, inputArrayPointer / Int32Array.BYTES_PER_ELEMENT);
    return inputArrayPointer;
}

let result = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15];
export function mancalaBoard(flag,array,size) {
    let flagPointer = get_int32_pointer([flag]);
    let arrayPointer = get_int32_pointer(array);
    let sizePointer = get_int32_pointer([size]);
    let resultPointer = get_int32_pointer(result);
    mancala_board_cwarp(flagPointer,arrayPointer,sizePointer,resultPointer);
    result = [];
    for(let i = 0; i < 15; i++) {
        result.push(a.HEAP32[resultPointer / 4 + i]);
    }
    a._free(flagPointer);
    a._free(arrayPointer);
    a._free(sizePointer);
    return result;
}
