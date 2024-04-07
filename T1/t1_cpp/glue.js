import assert from "assert";
import Module from "./t1.ems.js";


let a = await Module()
const bocchiShutUp_cwrap = a.cwrap('bocchi_shut_up', 'number', ['number', 'number', 'number']);

function get_int32_pointer(inputArray) {
    let inputArrayPointer = a._malloc(inputArray.length * Int32Array.BYTES_PER_ELEMENT);
    a.HEAP32.set(inputArray, inputArrayPointer / Int32Array.BYTES_PER_ELEMENT);
    return inputArrayPointer;
}


export function bocchiShutUp(flag,array,size) {
    let flagPointer = get_int32_pointer([flag]);
    let arrayPointer = get_int32_pointer(array);
    let sizePointer = get_int32_pointer([size]);
    let ans = bocchiShutUp_cwrap(flagPointer,arrayPointer,sizePointer);
    a._free(flagPointer);
    a._free(arrayPointer);
    a._free(sizePointer);
    return ans;
}
