const fbuff_base = new ArrayBuffer(1)
const fbuff_fview = new Float32Array(fbuff_base)
const fbuff_uview = new Uint32Array(fbuff_base)

const sign_shift = 31
const exponent_shift = 23
const exponent_mask = 0xFF
const mantissa_mask = 0x007fffff

function backToFloat() {
    const sgn = (this.sign & 1) << sign_shift
    const exp = (this.exponent & exponent_mask) << exponent_shift 
    const man = this.mantissa & mantissa_mask 
    const bin = sgn | exp | man 
    fbuff_uview[0] = bin 
    return fbuff_fview[0]
}

function fobj(value, use64) {
     fbuff_fview[0] = value;
     const uval = fbuff_uview[0]
     this.sign = uval >>> sign_shift
     this.exponent = (uval >>> exponent_shift) & exponent_mask
     this.mantissa = uval & mantissa_mask
     this.algoexec = function(s) {
         eval(s)
     }
}
function onexec(ev) {
    let val = parseFloat(document.getElementById("number_input").value)
    let user_algo = document.getElementById("algo_input").value 
    let split_algo = str.split(' ').join('§ §').split('§')
    for(let i = 0; i < split_algo.length; ++i) {
        let curr = split_algo[i];
        if(curr === "mantissa")
            curr = "this.mantissa"
        else if(curr === "exponent")
            curr = "this.exponent"
        else if(curr === "sign")
            curr = "this.sign"
        split_algo[i] = curr
    }

    let fraw = new fobj(val, false)
    fraw.algoexec(split_algo.join())
    

}

function fumble_init() {
    document.getElementById("exec_button").onclick = onexec
}

document.addEventListener('DOMContentLoaded', fumble_init, false);