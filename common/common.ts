
export type numeric_t = number;
export type int_t = number;
export type uint_t = number;
export type size_t = uint_t;

export type bool_t = boolean;

export type idx_t = int_t;
export type udx_t = uint_t;

export type flags_t = uint_t;
export type flag_t = uint_t;
export type bitidx_t = uint_t;
export type bit_t  = uint_t;
export type mask_t = uint_t;
export type real_t = number;

export type Nullable<T> = null | T;

export type Optional<T> = undefined | T;

export const INT_BITS : uint_t = uint(32);

export const NONE : int_t = int(-1);

export type PackedString_t = Uint8Array;

export interface FlagTestable {
    testFlag(flag : flag_t) : bool_t;
    extractFlag(bitIdx : bitidx_t) : bit_t;
}

export interface FlagSettable {
    setFlag(flag : flag_t) : void;
    complementFlag(flag : flag_t) : bool_t;
}

export interface Flagged extends FlagSettable, FlagTestable {

}

export function uint(value : any) : uint_t {
    return value >>> 0;
}

export function int(value : any) : int_t {
    return value >> 0;
}

export function bool(value : any) : bool_t {
    return !!value;
}

export function real(value : any) : real_t {
    return +value;
}

export function addu(lhs : uint_t, rhs : uint_t) : uint_t {
    return uint(uint(lhs) + uint(rhs));
}

export function subu(lhs : uint_t, rhs : uint_t) : uint_t {
  return uint(uint(lhs) - uint(rhs));
}

export function mulu(lhs : uint_t, rhs : uint_t) : uint_t {
  return uint(uint(lhs) * uint(rhs));
}

export function divu(lhs : uint_t, rhs : uint_t) : uint_t {
  return uint(uint(lhs) / uint(rhs));
}

export function modu(lhs : uint_t, rhs : uint_t) : uint_t {
  return uint(uint(lhs) % uint(rhs));
}

export function shlu(lhs : uint_t, rhs : uint_t) : uint_t {
  return uint(uint(lhs) << rhs);
}

export function shru(lhs : uint_t, rhs : uint_t) : uint_t {
  return uint(lhs >>> rhs);
}

export function addi(lhs : int_t, rhs : int_t) : int_t {
    return int(int(lhs) + int(rhs));
}

export function subi(lhs : int_t, rhs : int_t) : int_t {
    return int(int(lhs) - int(rhs));
}

export function muli(lhs : int_t, rhs : int_t) : int_t {
    return int(int(lhs) * int(rhs));
}

export function divi(lhs : int_t, rhs : int_t) : int_t {
    return int(int(lhs) / int(rhs));
}

export function modi(lhs : int_t, rhs : int_t) : int_t {
    return int(int(lhs) % int(rhs));
}

export function shli(lhs : int_t, rhs : int_t) : int_t {
    return int(int(lhs) << rhs);
}

export function shri(lhs : int_t, rhs : int_t) : int_t {
    return int(lhs >>> rhs);
}

export function isOdd(value : numeric_t) : bool_t {
    return bool(value&0x1);
}

export function popcount(value : uint_t) : size_t {
    let count = 0;
    for(let i : bitidx_t = 0; i < INT_BITS; ++i) {
        if(bool(value & shlu(1, i)))
            count++;
    }
    return count;
}

export function makeMask(bitcount : size_t) : mask_t {
    let value : mask_t = 0x0;
    for(let i : bitidx_t = 0; i < bitcount; ++i) {
        value |= shlu(0x1, i);
    }
    return value;
}

export function BitPacker(start_pos : bitidx_t, end_pos : bitidx_t) : (value : uint_t) => uint_t {
    const mask = shlu(makeMask(end_pos - start_pos), start_pos);
    return (value : uint_t) : uint_t => {
        return shlu(value, start_pos) & mask;
    };
}

export function BitUnpacker(start_pos : bitidx_t, end_pos : bitidx_t) : (value : uint_t) => uint_t {
    const mask = shlu(makeMask(end_pos - start_pos), start_pos);
    return (value : uint_t) : uint_t => {
        return shru(value & mask, start_pos);
    };
}

export function PackedString(str : string) : PackedString_t {
    const slen : size_t = str.length;
    const result : PackedString_t = new Uint8Array(slen);
    for(let i = 0; i < slen; ++i) {
        result[i] = str.charCodeAt(i);
    }
    return result;
}

export function unpackString(str : PackedString_t) : string {
    return String.fromCharCode.apply(null, str);
}

export function isNONE(value : int_t) : bool_t {
    return !~value;
}

export function Fixer(fractional_bits : size_t) : (value : real_t) => int_t {
    const fixed_one : real_t = real(shlu(1, fractional_bits));
    return (value : real_t) : int_t => {
        return int(value*fixed_one);
    }
}
export function Unfixer(fractional_bits : size_t) : (value : int_t) => real_t {
    const fixed_one : real_t = real(shlu(1, fractional_bits));
    return (value : int_t) : real_t => {
        return real(value) / fixed_one;
    }
}