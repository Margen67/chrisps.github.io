/*
uint128_t.h
An unsigned 128 bit integer type for C++

Copyright (c) 2013 - 2017 Jason Lee @ calccrypto at gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

With much help from Auston Sterling

Thanks to Stefan Deigm�ller for finding
a bug in operator*.

Thanks to Fran�ois Dessenne for convincing me
to do a general rewrite of this class.
*/
#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <utility>

class uint128_t{
    private:
        uint64_t UPPER, LOWER;

    public:
        // Constructors
        constexpr uint128_t();
        constexpr uint128_t(const uint128_t & rhs);
        constexpr uint128_t(uint128_t && rhs);

        template <typename T> 
        constexpr uint128_t(const T & rhs)
            : UPPER(0), LOWER(rhs)
        {
            static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
        }

        template <typename S, typename T> 
        constexpr uint128_t(const S & upper_rhs, const T & lower_rhs)
            : UPPER(upper_rhs), LOWER(lower_rhs)
        {
            static_assert(std::is_integral <S>::value &&
                          std::is_integral <T>::value
                          , "Input argument types must be integers.");
        }

        //  RHS input args only

        // Assignment Operator
        constexpr uint128_t operator=(const uint128_t & rhs);
        constexpr uint128_t operator=(uint128_t && rhs);

        template <typename T> 
        constexpr uint128_t operator=(const T & rhs){
            static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
            UPPER = 0;
            LOWER = rhs;
            return *this;
        }

        // Typecast Operators
        constexpr operator bool() const;
        constexpr operator uint8_t() const;
        constexpr operator uint16_t() const;
        constexpr operator uint32_t() const;
        constexpr operator uint64_t() const;

        // Bitwise Operators
        constexpr uint128_t operator&(const uint128_t & rhs) const;

        template <typename T> 
        constexpr uint128_t operator&(const T & rhs) const{
            return uint128_t(0, LOWER & (uint64_t) rhs);
        }

       constexpr  uint128_t & operator&=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator&=(const T & rhs){
            UPPER = 0;
            LOWER &= rhs;
            return *this;
        }

        constexpr uint128_t operator|(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator|(const T & rhs) const{
            return uint128_t(UPPER, LOWER | (uint64_t) rhs);
        }

        constexpr uint128_t & operator|=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator|=(const T & rhs){
            LOWER |= (uint64_t) rhs;
            return *this;
        }

        constexpr uint128_t operator^(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator^(const T & rhs) const{
            return uint128_t(UPPER, LOWER ^ (uint64_t) rhs);
        }

        constexpr uint128_t & operator^=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator^=(const T & rhs){
            LOWER ^= (uint64_t) rhs;
            return *this;
        }

        constexpr uint128_t operator~() const;

        // Bit Shift Operators
        constexpr uint128_t operator<<(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator<<(const T & rhs) const{
            return *this << uint128_t(rhs);
        }

        constexpr uint128_t & operator<<=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator<<=(const T & rhs){
            *this = *this << uint128_t(rhs);
            return *this;
        }

        constexpr uint128_t operator>>(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator>>(const T & rhs) const{
            return *this >> uint128_t(rhs);
        }

        constexpr uint128_t & operator>>=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator>>=(const T & rhs){
            *this = *this >> uint128_t(rhs);
            return *this;
        }

        // Logical Operators
        constexpr bool operator!() const;
        constexpr bool operator&&(const uint128_t & rhs) const;
        constexpr bool operator||(const uint128_t & rhs) const;

        template <typename T> constexpr bool operator&&(const T & rhs){
            return static_cast <bool> (*this && rhs);
        }

        template <typename T> constexpr bool operator||(const T & rhs){
            return static_cast <bool> (*this || rhs);
        }

        // Comparison Operators
        constexpr bool operator==(const uint128_t & rhs) const;

        template <typename T> bool operator==(const T & rhs) const{
            return (!UPPER && (LOWER == (uint64_t) rhs));
        }

        constexpr bool operator!=(const uint128_t & rhs) const;

        template <typename T> constexpr bool operator!=(const T & rhs) const{
            return (UPPER | (LOWER != (uint64_t) rhs));
        }

        constexpr bool operator>(const uint128_t & rhs) const;

        template <typename T> constexpr bool operator>(const T & rhs) const{
            return (UPPER || (LOWER > (uint64_t) rhs));
        }

        constexpr bool operator<(const uint128_t & rhs) const;

        template <typename T> constexpr bool operator<(const T & rhs) const{
            return (!UPPER)?(LOWER < (uint64_t) rhs):false;
        }

        constexpr bool operator>=(const uint128_t & rhs) const;

        template <typename T> bool operator>=(const T & rhs) const{
            return ((*this > rhs) | (*this == rhs));
        }

        constexpr bool operator<=(const uint128_t & rhs) const;

        template <typename T> constexpr bool operator<=(const T & rhs) const{
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        constexpr uint128_t operator+(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator+(const T & rhs) const{
            return uint128_t(UPPER + ((LOWER + (uint64_t) rhs) < LOWER), LOWER + (uint64_t) rhs);
        }

        constexpr uint128_t & operator+=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator+=(const T & rhs){
            UPPER = UPPER + ((LOWER + rhs) < LOWER);
            LOWER = LOWER + rhs;
            return *this;
        }

        constexpr uint128_t operator-(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator-(const T & rhs) const{
            return uint128_t((uint64_t) (UPPER - ((LOWER - rhs) > LOWER)), (uint64_t) (LOWER - rhs));
        }

        constexpr uint128_t & operator-=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator-=(const T & rhs){
            *this = *this - rhs;
            return *this;
        }

        constexpr uint128_t operator*(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator*(const T & rhs) const{
            return *this * uint128_t(rhs);
        }

        constexpr uint128_t & operator*=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator*=(const T & rhs){
            *this = *this * uint128_t(rhs);
            return *this;
        }

    private:
        std::pair <uint128_t, uint128_t> divmod(const uint128_t & lhs, const uint128_t & rhs) const;

    public:
		constexpr uint128_t operator/(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator/(const T & rhs) const{
            return *this / uint128_t(rhs);
        }

        constexpr uint128_t & operator/=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator/=(const T & rhs){
            *this = *this / uint128_t(rhs);
            return *this;
        }

        constexpr uint128_t operator%(const uint128_t & rhs) const;

        template <typename T> constexpr uint128_t operator%(const T & rhs) const{
            return *this - (rhs * (*this / rhs));
        }

        constexpr uint128_t & operator%=(const uint128_t & rhs);

        template <typename T> constexpr uint128_t & operator%=(const T & rhs){
            *this = *this % uint128_t(rhs);
            return *this;
        }

        // Increment Operator
        constexpr uint128_t & operator++();
        constexpr uint128_t operator++(int);

        // Decrement Operator
        constexpr uint128_t & operator--();
        constexpr uint128_t operator--(int);

        // Nothing done since promotion doesn't work here
        constexpr uint128_t operator+() const;

        // two's complement
        constexpr uint128_t operator-() const;

        // Get private values
        constexpr const uint64_t & upper() const;
        constexpr const uint64_t & lower() const;

        // Get bitsize of value
        constexpr uint8_t bits() const;

        // Get string representation of value
        std::string str(uint8_t base = 10, const unsigned int & len = 0) const;
};

// Give uint128_t type traits
namespace std {  // This is probably not a good idea
    template <> struct is_arithmetic <uint128_t> : std::true_type {};
    template <> struct is_integral   <uint128_t> : std::true_type {};
    template <> struct is_unsigned   <uint128_t> : std::true_type {};
};

// useful values
//extern const uint128_t uint128_0;
//extern const uint128_t uint128_1;

// lhs type T as first arguemnt
// If the output is not a bool, casts to type T

// Bitwise Operators
template <typename T> constexpr uint128_t operator&(const T & lhs, const uint128_t & rhs){
    return rhs & lhs;
}

template <typename T> constexpr T & operator&=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs & lhs);
}

template <typename T> constexpr uint128_t operator|(const T & lhs, const uint128_t & rhs){
    return rhs | lhs;
}

template <typename T> constexpr T & operator|=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs | lhs);
}

template <typename T> constexpr uint128_t operator^(const T & lhs, const uint128_t & rhs){
    return rhs ^ lhs;
}

template <typename T> constexpr T & operator^=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs ^ lhs);
}

// Bitshift operators
constexpr uint128_t operator<<(const bool     & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const uint8_t  & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const uint16_t & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const uint32_t & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const uint64_t & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const int8_t   & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const int16_t  & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const int32_t  & lhs, const uint128_t & rhs);
constexpr uint128_t operator<<(const int64_t  & lhs, const uint128_t & rhs);

template <typename T> constexpr T & operator<<=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) << rhs);
}

constexpr uint128_t operator>>(const bool     & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const uint8_t  & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const uint16_t & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const uint32_t & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const uint64_t & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const int8_t   & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const int16_t  & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const int32_t  & lhs, const uint128_t & rhs);
constexpr uint128_t operator>>(const int64_t  & lhs, const uint128_t & rhs);

template <typename T> constexpr T & operator>>=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) >> rhs);
}

// Comparison Operators
template <typename T> constexpr bool operator==(const T & lhs, const uint128_t & rhs){
    return (!rhs.upper() && ((uint64_t) lhs == rhs.lower()));
}

template <typename T> constexpr bool operator!=(const T & lhs, const uint128_t & rhs){
    return (rhs.upper() | ((uint64_t) lhs != rhs.lower()));
}

template <typename T> constexpr bool operator>(const T & lhs, const uint128_t & rhs){
    return (!rhs.upper()) && ((uint64_t) lhs > rhs.lower());
}

template <typename T> constexpr bool operator<(const T & lhs, const uint128_t & rhs){
    if (rhs.upper()){
        return true;
    }
    return ((uint64_t) lhs < rhs.lower());
}

template <typename T> constexpr bool operator>=(const T & lhs, const uint128_t & rhs){
    if (rhs.upper()){
        return false;
    }
    return ((uint64_t) lhs >= rhs.lower());
}

template <typename T> constexpr bool operator<=(const T & lhs, const uint128_t & rhs){
    if (rhs.upper()){
        return true;
    }
    return ((uint64_t) lhs <= rhs.lower());
}

// Arithmetic Operators
template <typename T> constexpr uint128_t operator+(const T & lhs, const uint128_t & rhs){
    return rhs + lhs;
}

template <typename T> constexpr T & operator+=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs + lhs);
}

template <typename T> constexpr uint128_t operator-(const T & lhs, const uint128_t & rhs){
    return -(rhs - lhs);
}

template <typename T> constexpr T & operator-=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (-(rhs - lhs));
}

template <typename T> constexpr uint128_t operator*(const T & lhs, const uint128_t & rhs){
    return rhs * lhs;
}

template <typename T> constexpr T & operator*=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs * lhs);
}

template <typename T> constexpr uint128_t operator/(const T & lhs, const uint128_t & rhs){
    return uint128_t(lhs) / rhs;
}

template <typename T> constexpr T & operator/=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) / rhs);
}

template <typename T> constexpr uint128_t operator%(const T & lhs, const uint128_t & rhs){
    return uint128_t(lhs) % rhs;
}

template <typename T> constexpr T & operator%=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) % rhs);
}

// IO Operator
std::ostream & operator<<(std::ostream & stream, const uint128_t & rhs);

//const uint128_t uint128_0(0);
//const uint128_t uint128_1(1);

constexpr uint128_t::uint128_t()
    : UPPER(0), LOWER(0)
{}

constexpr uint128_t::uint128_t(const uint128_t & rhs)
    : UPPER(rhs.UPPER), LOWER(rhs.LOWER)
{}

constexpr uint128_t::uint128_t(uint128_t && rhs)
    : UPPER(std::move(rhs.UPPER)), LOWER(std::move(rhs.LOWER))
{
    if (this != &rhs){
        rhs.UPPER = 0;
        rhs.LOWER = 0;
    }
}
constexpr uint128_t uint128_0 = 0;
constexpr uint128_t uint128_1 = 1;
constexpr uint128_t uint128_t::operator=(const uint128_t & rhs){
    UPPER = rhs.UPPER;
    LOWER = rhs.LOWER;
    return *this;
}

constexpr uint128_t uint128_t::operator=(uint128_t && rhs){
    if (this != &rhs){
        UPPER = std::move(rhs.UPPER);
        LOWER = std::move(rhs.LOWER);
        rhs.UPPER = 0;
        rhs.LOWER = 0;
    }
    return *this;
}

constexpr uint128_t::operator bool() const{
    return (bool) (UPPER | LOWER);
}

constexpr uint128_t::operator uint8_t() const{
    return (uint8_t) LOWER;
}

constexpr uint128_t::operator uint16_t() const{
    return (uint16_t) LOWER;
}

constexpr uint128_t::operator uint32_t() const{
    return (uint32_t) LOWER;
}

constexpr uint128_t::operator uint64_t() const{
    return (uint64_t) LOWER;
}

constexpr uint128_t uint128_t::operator&(const uint128_t & rhs) const{
    return uint128_t(UPPER & rhs.UPPER, LOWER & rhs.LOWER);
}

constexpr uint128_t & uint128_t::operator&=(const uint128_t & rhs){
    UPPER &= rhs.UPPER;
    LOWER &= rhs.LOWER;
    return *this;
}

constexpr uint128_t uint128_t::operator|(const uint128_t & rhs) const{
    return uint128_t(UPPER | rhs.UPPER, LOWER | rhs.LOWER);
}

constexpr uint128_t & uint128_t::operator|=(const uint128_t & rhs){
    UPPER |= rhs.UPPER;
    LOWER |= rhs.LOWER;
    return *this;
}

constexpr uint128_t uint128_t::operator^(const uint128_t & rhs) const{
    return uint128_t(UPPER ^ rhs.UPPER, LOWER ^ rhs.LOWER);
}

constexpr uint128_t & uint128_t::operator^=(const uint128_t & rhs){
    UPPER ^= rhs.UPPER;
    LOWER ^= rhs.LOWER;
    return *this;
}

constexpr uint128_t uint128_t::operator~() const{
    return uint128_t(~UPPER, ~LOWER);
}

constexpr uint128_t uint128_t::operator<<(const uint128_t & rhs) const{
    const uint64_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) || (shift >= 128)){
        return uint128_0;
    }
    else if (shift == 64){
        return uint128_t(LOWER, 0);
    }
    else if (shift == 0){
        return *this;
    }
    else if (shift < 64){
        return uint128_t((UPPER << shift) + (LOWER >> (64 - shift)), LOWER << shift);
    }
    else if ((128 > shift) && (shift > 64)){
        return uint128_t(LOWER << (shift - 64), 0);
    }
    else{
        return uint128_0;
    }
}

constexpr uint128_t & uint128_t::operator<<=(const uint128_t & rhs){
    *this = *this << rhs;
    return *this;
}

constexpr uint128_t uint128_t::operator>>(const uint128_t & rhs) const{
    const uint64_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) || (shift >= 128)){
        return uint128_0;
    }
    else if (shift == 64){
        return uint128_t(0, UPPER);
    }
    else if (shift == 0){
        return *this;
    }
    else if (shift < 64){
        return uint128_t(UPPER >> shift, (UPPER << (64 - shift)) + (LOWER >> shift));
    }
    else if ((128 > shift) && (shift > 64)){
        return uint128_t(0, (UPPER >> (shift - 64)));
    }
    else{
        return uint128_0;
    }
}

constexpr uint128_t & uint128_t::operator>>=(const uint128_t & rhs){
    *this = *this >> rhs;
    return *this;
}

constexpr bool uint128_t::operator!() const{
    return !(bool) (UPPER | LOWER);
}

constexpr bool uint128_t::operator&&(const uint128_t & rhs) const{
    return ((bool) *this && rhs);
}

constexpr bool uint128_t::operator||(const uint128_t & rhs) const{
     return ((bool) *this || rhs);
}

constexpr bool uint128_t::operator==(const uint128_t & rhs) const{
    return ((UPPER == rhs.UPPER) && (LOWER == rhs.LOWER));
}

constexpr bool uint128_t::operator!=(const uint128_t & rhs) const{
    return ((UPPER != rhs.UPPER) | (LOWER != rhs.LOWER));
}

constexpr bool uint128_t::operator>(const uint128_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER > rhs.LOWER);
    }
    return (UPPER > rhs.UPPER);
}

constexpr bool uint128_t::operator<(const uint128_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER < rhs.LOWER);
    }
    return (UPPER < rhs.UPPER);
}

constexpr bool uint128_t::operator>=(const uint128_t & rhs) const{
    return ((*this > rhs) | (*this == rhs));
}

constexpr bool uint128_t::operator<=(const uint128_t & rhs) const{
    return ((*this < rhs) | (*this == rhs));
}

constexpr uint128_t uint128_t::operator+(const uint128_t & rhs) const{
    return uint128_t(UPPER + rhs.UPPER + ((LOWER + rhs.LOWER) < LOWER), LOWER + rhs.LOWER);
}

constexpr uint128_t & uint128_t::operator+=(const uint128_t & rhs){
    UPPER += rhs.UPPER + ((LOWER + rhs.LOWER) < LOWER);
    LOWER += rhs.LOWER;
    return *this;
}

constexpr uint128_t uint128_t::operator-(const uint128_t & rhs) const{
    return uint128_t(UPPER - rhs.UPPER - ((LOWER - rhs.LOWER) > LOWER), LOWER - rhs.LOWER);
}

constexpr uint128_t & uint128_t::operator-=(const uint128_t & rhs){
    *this = *this - rhs;
    return *this;
}

constexpr uint128_t uint128_t::operator*(const uint128_t & rhs) const{
    // split values into 4 32-bit parts
    uint64_t top[4] = {UPPER >> 32, UPPER & 0xffffffff, LOWER >> 32, LOWER & 0xffffffff};
    uint64_t bottom[4] = {rhs.UPPER >> 32, rhs.UPPER & 0xffffffff, rhs.LOWER >> 32, rhs.LOWER & 0xffffffff};
    uint64_t products[4][4] = {};

    // multiply each component of the values
    for(int y = 3; y > -1; y--){
        for(int x = 3; x > -1; x--){
            products[3 - x][y] = top[x] * bottom[y];
        }
    }

    // first row
    uint64_t fourth32 = (products[0][3] & 0xffffffff);
    uint64_t third32  = (products[0][2] & 0xffffffff) + (products[0][3] >> 32);
    uint64_t second32 = (products[0][1] & 0xffffffff) + (products[0][2] >> 32);
    uint64_t first32  = (products[0][0] & 0xffffffff) + (products[0][1] >> 32);

    // second row
    third32  += (products[1][3] & 0xffffffff);
    second32 += (products[1][2] & 0xffffffff) + (products[1][3] >> 32);
    first32  += (products[1][1] & 0xffffffff) + (products[1][2] >> 32);

    // third row
    second32 += (products[2][3] & 0xffffffff);
    first32  += (products[2][2] & 0xffffffff) + (products[2][3] >> 32);

    // fourth row
    first32  += (products[3][3] & 0xffffffff);

    // move carry to next digit
    third32  += fourth32 >> 32;
	second32 += third32  >> 32;
	first32  += second32 >> 32;

    // remove carry from current digit
	fourth32 &= 0xffffffff;
    third32  &= 0xffffffff;
    second32 &= 0xffffffff;
    first32  &= 0xffffffff;

    // combine components
    return uint128_t((first32 << 32) | second32, (third32 << 32) | fourth32);
}

constexpr uint128_t & uint128_t::operator*=(const uint128_t & rhs){
    *this = *this * rhs;
    return *this;
}

std::pair <uint128_t, uint128_t> uint128_t::divmod(const uint128_t & lhs, const uint128_t & rhs) const{
    // Save some calculations /////////////////////
    if (rhs == uint128_0){
        assert(false);//throw std::domain_error("Error: division or modulus by 0");
    }
    else if (rhs == uint128_1){
        return std::pair <uint128_t, uint128_t> (lhs, uint128_0);
    }
    else if (lhs == rhs){
        return std::pair <uint128_t, uint128_t> (uint128_1, uint128_0);
    }
    else if ((lhs == uint128_0) || (lhs < rhs)){
        return std::pair <uint128_t, uint128_t> (uint128_0, lhs);
    }

    std::pair <uint128_t, uint128_t> qr (uint128_0, uint128_0);
    for(uint8_t x = lhs.bits(); x > 0; x--){
        qr.first  <<= uint128_1;
        qr.second <<= uint128_1;

        if ((lhs >> (x - 1U)) & 1){
            qr.second++;
        }

        if (qr.second >= rhs){
            qr.second -= rhs;
            qr.first++;
        }
    }
    return qr;
}

constexpr uint128_t uint128_t::operator/(const uint128_t & rhs) const{
    return divmod(*this, rhs).first;
}

constexpr uint128_t & uint128_t::operator/=(const uint128_t & rhs){
    *this = *this / rhs;
    return *this;
}

constexpr uint128_t uint128_t::operator%(const uint128_t & rhs) const{
    return divmod(*this, rhs).second;
}

constexpr uint128_t & uint128_t::operator%=(const uint128_t & rhs){
    *this = *this % rhs;
    return *this;
}

constexpr uint128_t & uint128_t::operator++(){
    return *this += uint128_1;
}

constexpr uint128_t uint128_t::operator++(int){
    uint128_t temp(*this);
    ++*this;
    return temp;
}

constexpr uint128_t & uint128_t::operator--(){
    return *this -= uint128_1;
}

constexpr uint128_t uint128_t::operator--(int){
    uint128_t temp(*this);
    --*this;
    return temp;
}

constexpr uint128_t uint128_t::operator+() const{
    return *this;
}

constexpr uint128_t uint128_t::operator-() const{
    return ~*this + uint128_1;
}

constexpr const uint64_t & uint128_t::upper() const{
    return UPPER;
}

constexpr const uint64_t & uint128_t::lower() const{
    return LOWER;
}

constexpr uint8_t uint128_t::bits() const{
    uint8_t out = 0;
    if (UPPER){
        out = 64;
        uint64_t up = UPPER;
        while (up){
            up >>= 1;
            out++;
        }
    }
    else{
        uint64_t low = LOWER;
        while (low){
            low >>= 1;
            out++;
        }
    }
    return out;
}

std::string uint128_t::str(uint8_t base, const unsigned int & len) const{
    if ((base < 2) || (base > 16)){
        assert(false);//throw std::invalid_argument("Base must be in the range [2, 16]");
    }
    std::string out = "";
    if (!(*this)){
        out = "0";
    }
    else{
        std::pair <uint128_t, uint128_t> qr(*this, uint128_0);
        do{
            qr = divmod(qr.first, base);
            out = "0123456789abcdef"[(uint8_t) qr.second] + out;
        } while (qr.first);
    }
    if (out.size() < len){
        out = std::string(len - out.size(), '0') + out;
    }
    return out;
}

constexpr uint128_t operator<<(const bool & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const uint8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const uint16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const uint32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const uint64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const int8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const int16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const int32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator<<(const int64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

constexpr uint128_t operator>>(const bool & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const uint8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const uint16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const uint32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const uint64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const int8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const int16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const int32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

constexpr uint128_t operator>>(const int64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

std::ostream & operator<<(std::ostream & stream, const uint128_t & rhs){
    if (stream.flags() & stream.oct){
        stream << rhs.str(8);
    }
    else if (stream.flags() & stream.dec){
        stream << rhs.str(10);
    }
    else if (stream.flags() & stream.hex){
        stream << rhs.str(16);
    }
    return stream;
}
