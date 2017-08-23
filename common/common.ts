
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

enum FieldType {
    i8 = 0,
    u8 = 1,
    i16 = 2,
    u16 = 3,
    i32 = 4,
    u32 = 5,
    f32 = 6,
    f64 = 7
}

class StructField {
    public name : PackedString_t;
    public typ : FieldType;
    public setter : any;
    public getter : any;
    public sizeof : size_t;
    public elements : size_t;
    public arrayType : Optional<any>;
    public isString : bool_t;
    constructor(name : string, typ : FieldType, elements : size_t = 1, isstring : bool_t = false) {
        this.name = PackedString(name);
        this.typ = typ;
        this.sizeof = this.sizeof_() * elements;
        this.elements = elements;
        this.setter = this.setter_();

        this.getter = this.getter_();
        if(this.elements > 1)
            this.arrayType = this.arraytype_();
        this.isString = isstring;

    }
    public sizeof_() : size_t {
        const typ : FieldType = this.typ;
        switch(typ) {
            case FieldType.i8:
            case FieldType.u8:
                return 1;
            case FieldType.i16:
            case FieldType.u16:
                return 2;
            case FieldType.i32:
            case FieldType.u32:
            case FieldType.f32:
                return 4;
            case FieldType.f64:
                return 8;
        }
    }
    private getter_() : any {
        const typ : FieldType = this.typ;
        switch(typ) {
            case FieldType.i8:
                return DataView.prototype.getInt8;
            case FieldType.u8:
                return DataView.prototype.getUint8;
            case FieldType.i16:
                return DataView.prototype.getInt16;
            case FieldType.u16:
                return DataView.prototype.getUint16;
            case FieldType.i32:
                return DataView.prototype.getInt32;
            case FieldType.u32:
                return DataView.prototype.getUint32;
            case FieldType.f32:
                return DataView.prototype.getFloat32;
            case FieldType.f64:
                return DataView.prototype.getFloat64;
        }
    }
    private setter_() : any {
        const typ : FieldType = this.typ;
        switch(typ) {
            case FieldType.i8:
                return DataView.prototype.setInt8;
            case FieldType.u8:
                return DataView.prototype.setUint8;
            case FieldType.i16:
                return DataView.prototype.setInt16;
            case FieldType.u16:
                return DataView.prototype.setUint16;
            case FieldType.i32:
                return DataView.prototype.setInt32;
            case FieldType.u32:
                return DataView.prototype.setUint32;
            case FieldType.f32:
                return DataView.prototype.setFloat32;
            case FieldType.f64:
                return DataView.prototype.setFloat64;
        }
    }
    private arraytype_() : any {
        const typ : FieldType = this.typ;
        switch(typ) {
            case FieldType.i8:
                return Int8Array;
            case FieldType.u8:
                return Uint8Array;
            case FieldType.i16:
                return Int16Array;
            case FieldType.u16:
                return Uint16Array;
            case FieldType.i32:
                return Int32Array;
            case FieldType.u32:
                return Uint32Array;
            case FieldType.f32:
                return Float32Array;
            case FieldType.f64:
                return Float64Array;
        }
    }
}

export class StructFactory {
    private fields : Array<StructField>;
    private sizeof : size_t;
    constructor(...fields : Array<StructField>) {
        let sizeof = 0;
        for(let field of fields)
            sizeof = addu(sizeof, field.sizeof);
        //this.data = new DataView(new ArrayBuffer(sizeof));
        this.sizeof = sizeof;
        this.fields = fields;
    }

    public construct() {
        let resultdata =  new DataView(new ArrayBuffer(this.sizeof));
        let result = {
            data : resultdata
        };
        let fields : Array<StructField> = this.fields;
        let offset : size_t = 0;
        for(let field of fields) {
            let name = unpackString(field.name);

            if(field.elements == 1) {
                let getter = field.getter.bind(resultdata, offset);
                let setter = field.setter.bind(resultdata, offset);
                Object.defineProperty(
                    result,
                    name,
                    {
                        get: function () {
                            return getter();
                        },
                        set: function (value: numeric_t) {
                            setter(value);
                        }
                    }
                );

            } else {
                let ctor = field.arrayType;
                let implname = `${name}_`;
                result[implname] = new ctor(resultdata.buffer, offset, field.elements);
                Object.defineProperty(
                    result,
                    name,
                    {
                        get: function () {
                            return result[implname];
                        },
                        set: (field.isString) ? function(iterable : string) {
                            const iterations = iterable.length;
                            const arr = result[implname];
                            for(let i : idx_t = 0; i < iterations; ++i) {
                                arr[i] = iterable.charCodeAt(i);
                            }
                        } : function(iterable) {
                            const iterations = iterable.length;
                            const arr = result[implname];
                            for(let i : idx_t = 0; i < iterations; ++i) {
                                arr[i] = iterable[i];
                            }
                        }
                    }
                );
            }
            offset = addu(offset, field.sizeof);
        }
        return result;
    }

}