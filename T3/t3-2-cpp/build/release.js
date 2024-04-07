import Module from "./t3_2.ems.js";


let a = await Module()
const mancala_operator_cwarp = a.cwrap('mancala_operator', 'number', ['number', 'number']);

function get_int32_pointer(inputArray) {
    let inputArrayPointer = a._malloc(inputArray.length * Int32Array.BYTES_PER_ELEMENT);
    a.HEAP32.set(inputArray, inputArrayPointer / Int32Array.BYTES_PER_ELEMENT);
    return inputArrayPointer;
}


export function mancalaOperator(flag,status) {
    let flagPointer = get_int32_pointer([flag]);
    let statusPointer = get_int32_pointer(status);
    let ans = mancala_operator_cwarp(flagPointer,statusPointer);
    a._free(flagPointer);
    a._free(statusPointer);
    return ans;
}
