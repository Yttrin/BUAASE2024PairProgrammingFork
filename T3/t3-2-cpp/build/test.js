// Choose proper "import" depending on your PL.
import { mancalaOperator as op1 } from "./release.js";

let status = [4,4,4,4,4,4,0,4,4,4,4,4,4,0];

let ans = op1(1, status);

console.log(ans);