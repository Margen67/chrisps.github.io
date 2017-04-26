const fbuff_base = new ArrayBuffer(4)
const fbuff_fview = new Float32Array(fbuff_base)
const fbuff_uview = new Uint32Array(fbuff_base)

const sign_shift = 31

const exponent_shift = 23
const exponent_mask = 0xFF
const exponent_bias = 127

const mantissa_mask = 0x007fffff
function getPreviousAlgo() {
    return localStorage.getItem("last_algo")
}
function setLastAlgo(s) {
    return localStorage.setItem("last_algo", s)
}
function backToFloat(sign, exponent, mantissa) {
    const sgn = (sign & 1) << sign_shift
    const exp = ((exponent+exponent_bias) & exponent_mask) << exponent_shift 
    const man = mantissa & mantissa_mask 
    const bin = sgn | exp | man 
    fbuff_uview[0] = bin 
    return fbuff_fview[0]
}

function outp(s) {
    if(typeof s !== 'string')
        s = s.toString()
    let outparea = document.getElementById("output_area")
    if(outparea.value != null && outparea.value.length > 1)
        outparea.value = s.concat("\n", outparea.value)
    else 
        outparea.value = s
}

function clear_outp(ev) {
    document.getElementById("output_area").value = null
}

function fobj(value_, use64) {
     fbuff_fview[0] = value_;
     const uval = fbuff_uview[0]
     this.sign = uval >>> sign_shift
     this.exponent = ((uval >>> exponent_shift) & exponent_mask) - exponent_bias
     this.original = value_
     this.mantissa = uval & mantissa_mask
     this.algoexec = function(s) {
         var sign = this.sign 
         var exponent = this.exponent 
         var mantissa = this.mantissa 
         const originalValue = this.original
         var value = originalValue
         fbuff_fview[0] = value 
         var uvalue = fbuff_uview[0]
         var updatea = function() {
             let tempfobj = new fobj(value, false)
             sign = tempfobj.sign 
             exponent = tempfobj.exponent
             mantissa = tempfobj.mantissa
         }
         var updatef = function() {
             value = backToFloat(sign, exponent, mantissa)
         }
         var updateu = function() {
             fbuff_fview[0] = backToFloat(sign, exponent, mantissa)
             uvalue = fbuff_uview[0]
         }
         var fact = function(v) {
             let result = v
             v--
             while(v != 0) {
                result *= v
                --v
                
             }
             return result
         }
         var log = function(v) {
             outp(v)
         }

         let stmts = s.split(/[\n;]+/)
         for(let stmtIterator = 0; stmtIterator < stmts.length; ++stmtIterator) {
             let TEMPSIGN = sign
             let TEMPEXP = exponent 
             let TEMPMAN = mantissa 
             let TEMPVALUE = value 
             let TEMPUVALUE = uvalue 
             eval(stmts[stmtIterator])
             if(TEMPSIGN != sign || TEMPEXP != exponent || TEMPMAN != mantissa) {
                updatef()
                updateu()
             }
             else if(TEMPVALUE != value) {
                 updatea()
                 updateu()
             }
             else if(TEMPUVALUE != uvalue) {
                 fbuff_uview[0] = uvalue 
                 value = fbuff_fview[0]
                 updatea()
             }
         }
        // eval(s)
         return backToFloat(sign, exponent, mantissa)
     }
}
function onexec(ev) {
    let val = parseFloat(document.getElementById("number_input").value)
    if(isNaN(val)) {
        alert("Please enter a valid value.")
        return
    }
    let user_algo = document.getElementById("algo_input").value 
    if(user_algo == null || user_algo.length == 0) {
        alert("Please enter some code before clicking Execute. For more information, click Help.")
        return
    }
    setLastAlgo(user_algo)


    let fraw = new fobj(val, false)
    let algo_result = fraw.algoexec(user_algo)
    outp(algo_result)

}

function show_help(ev) {
    document.getElementById("help_modal").style.display = "block"
}

function fumble_init() {
    document.getElementById("exec_button").onclick = onexec
    document.getElementById("clear_output_button").onclick = clear_outp
    document.getElementById("help_button").onclick = show_help
    document.getElementsByClassName("close")[0].onclick = function() {
        document.getElementById("help_modal").style.display = "none"
    }
    window.onclick = function(event) {
        if (event.target == modal) {
            modal.style.display = "none"
        }
    }
    document.getElementById("algo_input").value = getPreviousAlgo()
}

document.addEventListener('DOMContentLoaded', fumble_init, false);