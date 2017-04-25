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
#include "commonstuff.hpp"
using bigint_u32type = double;
#define BIGINT_CONSTEXPR   constexpr noinline 
class uint128_t{
    private:
        uint64_t UPPER, LOWER;

    public:
        // Constructors
        BIGINT_CONSTEXPR uint128_t();
        BIGINT_CONSTEXPR uint128_t(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint128_t(uint128_t && rhs);

        template <typename T> 
        BIGINT_CONSTEXPR uint128_t(const T & rhs)
            : UPPER(0), LOWER(rhs)
        {
            //static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
        }

        template <typename S, typename T> 
        BIGINT_CONSTEXPR uint128_t(const S & upper_rhs, const T & lower_rhs)
            : UPPER(upper_rhs), LOWER(lower_rhs)
        {
            /*static_assert(std::is_integral <S>::value &&
                          std::is_integral <T>::value
                          , "Input argument types must be integers.");*/
        }

        //  RHS input args only

        // Assignment Operator
        BIGINT_CONSTEXPR uint128_t operator=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint128_t operator=(uint128_t && rhs);

        template <typename T> 
        BIGINT_CONSTEXPR uint128_t operator=(const T & rhs){
           // static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
            UPPER = 0;
            LOWER = rhs;
            return *this;
        }

        // Typecast Operators
        BIGINT_CONSTEXPR operator bool() const;
        BIGINT_CONSTEXPR operator uint8_t() const;
        BIGINT_CONSTEXPR operator uint16_t() const;
        BIGINT_CONSTEXPR operator uint32_t() const;
        BIGINT_CONSTEXPR operator uint64_t() const;

        // Bitwise Operators
        BIGINT_CONSTEXPR uint128_t operator&(const uint128_t & rhs) const;

        template <typename T> 
        BIGINT_CONSTEXPR uint128_t operator&(const T & rhs) const{
            return uint128_t(0, LOWER & (uint64_t) rhs);
        }

       BIGINT_CONSTEXPR  uint128_t & operator&=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator&=(const T & rhs){
            UPPER = 0;
            LOWER &= rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator|(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator|(const T & rhs) const{
            return uint128_t(UPPER, LOWER | (uint64_t) rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator|=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator|=(const T & rhs){
            LOWER |= (uint64_t) rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator^(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator^(const T & rhs) const{
            return uint128_t(UPPER, LOWER ^ (uint64_t) rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator^=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator^=(const T & rhs){
            LOWER ^= (uint64_t) rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator~() const;

        // Bit Shift Operators
        BIGINT_CONSTEXPR uint128_t operator<<(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator<<(const T & rhs) const{
            return *this << uint128_t(rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator<<=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator<<=(const T & rhs){
            *this = *this << uint128_t(rhs);
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator>>(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator>>(const T & rhs) const{
            return *this >> uint128_t(rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator>>=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator>>=(const T & rhs){
            *this = *this >> uint128_t(rhs);
            return *this;
        }

        // Logical Operators
        BIGINT_CONSTEXPR bool operator!() const;
        BIGINT_CONSTEXPR bool operator&&(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator||(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator&&(const T & rhs){
            return static_cast <bool> (*this && rhs);
        }

        template <typename T> BIGINT_CONSTEXPR bool operator||(const T & rhs){
            return static_cast <bool> (*this || rhs);
        }

        // Comparison Operators
        BIGINT_CONSTEXPR bool operator==(const uint128_t & rhs) const;

        template <typename T> bool operator==(const T & rhs) const{
            return (!UPPER && (LOWER == (uint64_t) rhs));
        }

        BIGINT_CONSTEXPR bool operator!=(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator!=(const T & rhs) const{
            return (UPPER | (LOWER != (uint64_t) rhs));
        }

        BIGINT_CONSTEXPR bool operator>(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator>(const T & rhs) const{
            return (UPPER || (LOWER > (uint64_t) rhs));
        }

        BIGINT_CONSTEXPR bool operator<(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator<(const T & rhs) const{
            return (!UPPER)?(LOWER < (uint64_t) rhs):false;
        }

        BIGINT_CONSTEXPR bool operator>=(const uint128_t & rhs) const;

        template <typename T> bool operator>=(const T & rhs) const{
            return ((*this > rhs) | (*this == rhs));
        }

        BIGINT_CONSTEXPR bool operator<=(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator<=(const T & rhs) const{
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        BIGINT_CONSTEXPR uint128_t operator+(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator+(const T & rhs) const{
            return uint128_t(UPPER + ((LOWER + (uint64_t) rhs) < LOWER), LOWER + (uint64_t) rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator+=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator+=(const T & rhs){
            UPPER = UPPER + ((LOWER + rhs) < LOWER);
            LOWER = LOWER + rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator-(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator-(const T & rhs) const{
            return uint128_t((uint64_t) (UPPER - ((LOWER - rhs) > LOWER)), (uint64_t) (LOWER - rhs));
        }

        BIGINT_CONSTEXPR uint128_t & operator-=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator-=(const T & rhs){
            *this = *this - rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator*(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator*(const T & rhs) const{
            return *this * uint128_t(rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator*=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator*=(const T & rhs){
            *this = *this * uint128_t(rhs);
            return *this;
        }

    private:
        BIGINT_CONSTEXPR std::pair <uint128_t, uint128_t> divmod(const uint128_t & lhs, const uint128_t & rhs) const;

    public:
		BIGINT_CONSTEXPR uint128_t operator/(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator/(const T & rhs) const{
            return *this / uint128_t(rhs);
        }

        BIGINT_CONSTEXPR uint128_t & operator/=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator/=(const T & rhs){
            *this = *this / uint128_t(rhs);
            return *this;
        }

        BIGINT_CONSTEXPR uint128_t operator%(const uint128_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint128_t operator%(const T & rhs) const{
            return *this - (rhs * (*this / rhs));
        }

        BIGINT_CONSTEXPR uint128_t & operator%=(const uint128_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint128_t & operator%=(const T & rhs){
            *this = *this % uint128_t(rhs);
            return *this;
        }

        // Increment Operator
        BIGINT_CONSTEXPR uint128_t & operator++();
        BIGINT_CONSTEXPR uint128_t operator++(int);

        // Decrement Operator
        BIGINT_CONSTEXPR uint128_t & operator--();
        BIGINT_CONSTEXPR uint128_t operator--(int);

        // Nothing done since promotion doesn't work here
        BIGINT_CONSTEXPR uint128_t operator+() const;

        // two's complement
        BIGINT_CONSTEXPR uint128_t operator-() const;

        // Get private values
        BIGINT_CONSTEXPR const uint64_t & upper() const;
        BIGINT_CONSTEXPR const uint64_t & lower() const;

        // Get bitsize of value
        BIGINT_CONSTEXPR bigint_u32type bits() const;

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
template <typename T> BIGINT_CONSTEXPR uint128_t operator&(const T & lhs, const uint128_t & rhs){
    return rhs & lhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator&=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs & lhs);
}

template <typename T> BIGINT_CONSTEXPR uint128_t operator|(const T & lhs, const uint128_t & rhs){
    return rhs | lhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator|=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs | lhs);
}

template <typename T> BIGINT_CONSTEXPR uint128_t operator^(const T & lhs, const uint128_t & rhs){
    return rhs ^ lhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator^=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs ^ lhs);
}

// Bitshift operators
BIGINT_CONSTEXPR uint128_t operator<<(const bool     & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const uint8_t  & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const uint16_t & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const uint32_t & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const uint64_t & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const int8_t   & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const int16_t  & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const int32_t  & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator<<(const int64_t  & lhs, const uint128_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator<<=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) << rhs);
}

BIGINT_CONSTEXPR uint128_t operator>>(const bool     & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const uint8_t  & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const uint16_t & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const uint32_t & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const uint64_t & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const int8_t   & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const int16_t  & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const int32_t  & lhs, const uint128_t & rhs);
BIGINT_CONSTEXPR uint128_t operator>>(const int64_t  & lhs, const uint128_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator>>=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) >> rhs);
}

// Comparison Operators
template <typename T> BIGINT_CONSTEXPR bool operator==(const T & lhs, const uint128_t & rhs){
    return (!rhs.upper() && ((uint64_t) lhs == rhs.lower()));
}

template <typename T> BIGINT_CONSTEXPR bool operator!=(const T & lhs, const uint128_t & rhs){
    return (rhs.upper() | ((uint64_t) lhs != rhs.lower()));
}

template <typename T> BIGINT_CONSTEXPR bool operator>(const T & lhs, const uint128_t & rhs){
    return (!rhs.upper()) && ((uint64_t) lhs > rhs.lower());
}

template <typename T> BIGINT_CONSTEXPR bool operator<(const T & lhs, const uint128_t & rhs){
    if (rhs.upper()){
        return true;
    }
    return ((uint64_t) lhs < rhs.lower());
}

template <typename T> BIGINT_CONSTEXPR bool operator>=(const T & lhs, const uint128_t & rhs){
    if (rhs.upper()){
        return false;
    }
    return ((uint64_t) lhs >= rhs.lower());
}

template <typename T> BIGINT_CONSTEXPR bool operator<=(const T & lhs, const uint128_t & rhs){
    if (rhs.upper()){
        return true;
    }
    return ((uint64_t) lhs <= rhs.lower());
}

// Arithmetic Operators
template <typename T> BIGINT_CONSTEXPR uint128_t operator+(const T & lhs, const uint128_t & rhs){
    return rhs + lhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator+=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs + lhs);
}

template <typename T> BIGINT_CONSTEXPR uint128_t operator-(const T & lhs, const uint128_t & rhs){
    return -(rhs - lhs);
}

template <typename T> BIGINT_CONSTEXPR T & operator-=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (-(rhs - lhs));
}

template <typename T> BIGINT_CONSTEXPR uint128_t operator*(const T & lhs, const uint128_t & rhs){
    return rhs * lhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator*=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (rhs * lhs);
}

template <typename T> BIGINT_CONSTEXPR uint128_t operator/(const T & lhs, const uint128_t & rhs){
    return uint128_t(lhs) / rhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator/=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) / rhs);
}

template <typename T> BIGINT_CONSTEXPR uint128_t operator%(const T & lhs, const uint128_t & rhs){
    return uint128_t(lhs) % rhs;
}

template <typename T> BIGINT_CONSTEXPR T & operator%=(T & lhs, const uint128_t & rhs){
    return lhs = static_cast <T> (uint128_t(lhs) % rhs);
}

// IO Operator
std::ostream & operator<<(std::ostream & stream, const uint128_t & rhs);


BIGINT_CONSTEXPR uint128_t::uint128_t()
    : UPPER(0), LOWER(0)
{}

BIGINT_CONSTEXPR uint128_t::uint128_t(const uint128_t & rhs)
    : UPPER(rhs.UPPER), LOWER(rhs.LOWER)
{}

BIGINT_CONSTEXPR uint128_t::uint128_t(uint128_t && rhs)
    : UPPER(std::move(rhs.UPPER)), LOWER(std::move(rhs.LOWER))
{
    if (this != &rhs){
        rhs.UPPER = 0;
        rhs.LOWER = 0;
    }
}
BIGINT_CONSTEXPR uint128_t uint128_0 = 0;
BIGINT_CONSTEXPR uint128_t uint128_1 = 1;

BIGINT_CONSTEXPR uint128_t uint128_64(64);
BIGINT_CONSTEXPR uint128_t uint128_128(128);
BIGINT_CONSTEXPR uint128_t uint128_256(256);
BIGINT_CONSTEXPR uint128_t uint128_t::operator=(const uint128_t & rhs){
    UPPER = rhs.UPPER;
    LOWER = rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator=(uint128_t && rhs){
    if (this != &rhs){
        UPPER = std::move(rhs.UPPER);
        LOWER = std::move(rhs.LOWER);
        rhs.UPPER = 0;
        rhs.LOWER = 0;
    }
    return *this;
}

BIGINT_CONSTEXPR uint128_t::operator bool() const{
    return (bool) (UPPER | LOWER);
}

BIGINT_CONSTEXPR uint128_t::operator uint8_t() const{
    return (uint8_t) LOWER;
}

BIGINT_CONSTEXPR uint128_t::operator uint16_t() const{
    return (uint16_t) LOWER;
}

BIGINT_CONSTEXPR uint128_t::operator uint32_t() const{
    return (uint32_t) LOWER;
}

BIGINT_CONSTEXPR uint128_t::operator uint64_t() const{
    return (uint64_t) LOWER;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator&(const uint128_t & rhs) const{
    return uint128_t(UPPER & rhs.UPPER, LOWER & rhs.LOWER);
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator&=(const uint128_t & rhs){
    UPPER &= rhs.UPPER;
    LOWER &= rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator|(const uint128_t & rhs) const{
    return uint128_t(UPPER | rhs.UPPER, LOWER | rhs.LOWER);
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator|=(const uint128_t & rhs){
    UPPER |= rhs.UPPER;
    LOWER |= rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator^(const uint128_t & rhs) const{
    return uint128_t(UPPER ^ rhs.UPPER, LOWER ^ rhs.LOWER);
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator^=(const uint128_t & rhs){
    UPPER ^= rhs.UPPER;
    LOWER ^= rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator~() const{
    return uint128_t(~UPPER, ~LOWER);
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator<<(const uint128_t & rhs) const{
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

BIGINT_CONSTEXPR uint128_t & uint128_t::operator<<=(const uint128_t & rhs){
    *this = *this << rhs;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator>>(const uint128_t & rhs) const{
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

BIGINT_CONSTEXPR uint128_t & uint128_t::operator>>=(const uint128_t & rhs){
    *this = *this >> rhs;
    return *this;
}

BIGINT_CONSTEXPR bool uint128_t::operator!() const{
    return !(bool) (UPPER | LOWER);
}

BIGINT_CONSTEXPR bool uint128_t::operator&&(const uint128_t & rhs) const{
    return ((bool) *this && rhs);
}

BIGINT_CONSTEXPR bool uint128_t::operator||(const uint128_t & rhs) const{
     return ((bool) *this || rhs);
}

BIGINT_CONSTEXPR bool uint128_t::operator==(const uint128_t & rhs) const{
    return ((UPPER == rhs.UPPER) && (LOWER == rhs.LOWER));
}

BIGINT_CONSTEXPR bool uint128_t::operator!=(const uint128_t & rhs) const{
    return ((UPPER != rhs.UPPER) | (LOWER != rhs.LOWER));
}

BIGINT_CONSTEXPR bool uint128_t::operator>(const uint128_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER > rhs.LOWER);
    }
    return (UPPER > rhs.UPPER);
}

BIGINT_CONSTEXPR bool uint128_t::operator<(const uint128_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER < rhs.LOWER);
    }
    return (UPPER < rhs.UPPER);
}

BIGINT_CONSTEXPR bool uint128_t::operator>=(const uint128_t & rhs) const{
    return ((*this > rhs) | (*this == rhs));
}

BIGINT_CONSTEXPR bool uint128_t::operator<=(const uint128_t & rhs) const{
    return ((*this < rhs) | (*this == rhs));
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator+(const uint128_t & rhs) const{
    return uint128_t(UPPER + rhs.UPPER + ((LOWER + rhs.LOWER) < LOWER), LOWER + rhs.LOWER);
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator+=(const uint128_t & rhs){
    UPPER += rhs.UPPER + ((LOWER + rhs.LOWER) < LOWER);
    LOWER += rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator-(const uint128_t & rhs) const{
    return uint128_t(UPPER - rhs.UPPER - ((LOWER - rhs.LOWER) > LOWER), LOWER - rhs.LOWER);
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator-=(const uint128_t & rhs){
    *this = *this - rhs;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator*(const uint128_t & rhs) const{
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

BIGINT_CONSTEXPR uint128_t & uint128_t::operator*=(const uint128_t & rhs){
    *this = *this * rhs;
    return *this;
}

std::pair <uint128_t, uint128_t> 
BIGINT_CONSTEXPR uint128_t::divmod(const uint128_t & lhs, const uint128_t & rhs) const{
    // Save some calculations /////////////////////
    if (rhs == uint128_0){
        assume(false);
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

BIGINT_CONSTEXPR uint128_t uint128_t::operator/(const uint128_t & rhs) const{
    return divmod(*this, rhs).first;
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator/=(const uint128_t & rhs){
    *this = *this / rhs;
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator%(const uint128_t & rhs) const{
    return divmod(*this, rhs).second;
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator%=(const uint128_t & rhs){
    *this = *this % rhs;
    return *this;
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator++(){
    return *this += uint128_1;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator++(int){
    uint128_t temp(*this);
    ++*this;
    return temp;
}

BIGINT_CONSTEXPR uint128_t & uint128_t::operator--(){
    return *this -= uint128_1;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator--(int){
    uint128_t temp(*this);
    --*this;
    return temp;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator+() const{
    return *this;
}

BIGINT_CONSTEXPR uint128_t uint128_t::operator-() const{
    return ~*this + uint128_1;
}

BIGINT_CONSTEXPR const uint64_t & uint128_t::upper() const{
    return UPPER;
}

BIGINT_CONSTEXPR const uint64_t & uint128_t::lower() const{
    return LOWER;
}

BIGINT_CONSTEXPR bigint_u32type uint128_t::bits() const{
    bigint_u32type out = 0;
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
        assume(false);
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

BIGINT_CONSTEXPR uint128_t operator<<(const bool & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const uint8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const uint16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const uint32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const uint64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const int8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const int16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const int32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator<<(const int64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const bool & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const uint8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const uint16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const uint32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const uint64_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const int8_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const int16_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const int32_t & lhs, const uint128_t & rhs){
    return uint128_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t operator>>(const int64_t & lhs, const uint128_t & rhs){
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
class uint256_t{
    private:
        uint128_t UPPER, LOWER;

    public:
        // Constructors
        BIGINT_CONSTEXPR uint256_t();
        BIGINT_CONSTEXPR uint256_t(const uint256_t & rhs);
        BIGINT_CONSTEXPR uint256_t(uint256_t && rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t(const T & rhs)
            : UPPER(uint128_0), LOWER(rhs)
        {
            //static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
        }

        template <typename S, typename T> BIGINT_CONSTEXPR uint256_t(const S & upper_rhs, const T & lower_rhs)
            : UPPER(upper_rhs), LOWER(lower_rhs)
        {
            /*static_assert(std::is_integral <S>::value &&
                          std::is_integral <T>::value
                          , "Input argument types must be integers.");*/
        }

        template <typename R, typename S, typename T, typename U>
        BIGINT_CONSTEXPR uint256_t(const R & upper_lhs, const S & lower_lhs, const T & upper_rhs, const U & lower_rhs)
            : UPPER(upper_lhs, lower_lhs), LOWER(upper_rhs, lower_rhs)
        {
            /*static_assert(std::is_integral <R>::value &&
                          std::is_integral <S>::value &&
                          std::is_integral <T>::value &&
                          std::is_integral <U>::value
                          , "Input argument types must be integers.");*/
        }

        //  RHS input args only

        // Assignment Operator
        BIGINT_CONSTEXPR uint256_t operator=(const uint256_t & rhs);
        BIGINT_CONSTEXPR uint256_t operator=(uint256_t && rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t operator=(const T & rhs){
          //  static_assert(std::is_integral <T>::value, "Input argument type must be an integer.");
            UPPER = uint128_0;
            LOWER = rhs;
            return *this;
        }

        // Typecast Operators
        BIGINT_CONSTEXPR operator bool      () const;
        BIGINT_CONSTEXPR operator uint8_t   () const;
        BIGINT_CONSTEXPR operator uint16_t  () const;
        BIGINT_CONSTEXPR operator uint32_t  () const;
        BIGINT_CONSTEXPR operator uint64_t  () const;
        BIGINT_CONSTEXPR operator uint128_t () const;

        // Bitwise Operators
        BIGINT_CONSTEXPR uint256_t operator&(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator&(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator&(const T & rhs) const{
            return uint256_t(uint128_0, LOWER & (uint128_t) rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator&=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator&=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator&=(const T & rhs){
            UPPER = uint128_0;
            LOWER &= rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator|(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator|(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator|(const T & rhs) const{
            return uint256_t(UPPER, LOWER | uint128_t(rhs));
        }

        BIGINT_CONSTEXPR uint256_t & operator|=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator|=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator|=(const T & rhs){
            LOWER |= (uint128_t) rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator^(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator^(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator^(const T & rhs) const{
            return uint256_t(UPPER, LOWER ^ (uint128_t) rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator^=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator^=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator^=(const T & rhs){
            LOWER ^= (uint128_t) rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator~() const;

        // Bit Shift Operators
        BIGINT_CONSTEXPR uint256_t operator<<(const uint128_t & shift) const;
        BIGINT_CONSTEXPR uint256_t operator<<(const uint256_t & shift) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator<<(const T & rhs) const{
            return *this << uint256_t(rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator<<=(const uint128_t & shift);
        BIGINT_CONSTEXPR uint256_t & operator<<=(const uint256_t & shift);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator<<=(const T & rhs){
            *this = *this << uint256_t(rhs);
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator>>(const uint128_t & shift) const;
        BIGINT_CONSTEXPR uint256_t operator>>(const uint256_t & shift) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator>>(const T & rhs) const{
            return *this >> uint256_t(rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator>>=(const uint128_t & shift);
        BIGINT_CONSTEXPR uint256_t & operator>>=(const uint256_t & shift);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator>>=(const T & rhs){
            *this = *this >> uint256_t(rhs);
            return *this;
        }

        // Logical Operators
        BIGINT_CONSTEXPR bool operator!() const;

        BIGINT_CONSTEXPR bool operator&&(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator&&(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator&&(const T & rhs) const{
            return ((bool) *this && rhs);
        }

        BIGINT_CONSTEXPR bool operator||(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator||(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator||(const T & rhs) const{
            return ((bool) *this || rhs);
        }

        // Comparison Operators
        BIGINT_CONSTEXPR bool operator==(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator==(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator==(const T & rhs) const{
            return (!UPPER && (LOWER == uint128_t(rhs)));
        }

        BIGINT_CONSTEXPR bool operator!=(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator!=(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator!=(const T & rhs) const{
            return ((bool) UPPER | (LOWER != uint128_t(rhs)));
        }

        BIGINT_CONSTEXPR bool operator>(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator>(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator>(const T & rhs) const{
            return ((bool) UPPER | (LOWER > uint128_t(rhs)));
        }

        BIGINT_CONSTEXPR bool operator<(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator<(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator<(const T & rhs) const{
            return (!UPPER)?(LOWER < uint128_t(rhs)):false;
        }

        BIGINT_CONSTEXPR bool operator>=(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator>=(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator>=(const T & rhs) const{
            return ((*this > rhs) | (*this == rhs));
        }

        BIGINT_CONSTEXPR bool operator<=(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR bool operator<=(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR bool operator<=(const T & rhs) const{
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        BIGINT_CONSTEXPR uint256_t operator+(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator+(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator+(const T & rhs) const{
            return uint256_t(UPPER + ((LOWER + (uint128_t) rhs) < LOWER), LOWER + (uint128_t) rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator+=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator+=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator+=(const T & rhs){
            UPPER = UPPER + ((LOWER + rhs) < LOWER);
            LOWER = LOWER + rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator-(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator-(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator-(const T & rhs) const{
            return uint256_t(UPPER - ((LOWER - rhs) > LOWER), LOWER - rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator-=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator-=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator-=(const T & rhs){
            *this = *this - rhs;
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator*(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator*(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator*(const T & rhs) const{
            return *this * uint256_t(rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator*=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator*=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator*=(const T & rhs){
            *this = *this * uint256_t(rhs);
            return *this;
        }

    private:
        BIGINT_CONSTEXPR std::pair <uint256_t, uint256_t> divmod(const uint256_t & lhs, const uint256_t & rhs) const;

    public:
        BIGINT_CONSTEXPR uint256_t operator/(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator/(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator/(const T & rhs) const{
            return *this / uint256_t(rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator/=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator/=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator/=(const T & rhs){
            *this = *this / uint256_t(rhs);
            return *this;
        }

        BIGINT_CONSTEXPR uint256_t operator%(const uint128_t & rhs) const;
        BIGINT_CONSTEXPR uint256_t operator%(const uint256_t & rhs) const;

        template <typename T> BIGINT_CONSTEXPR uint256_t operator%(const T & rhs) const{
            return *this % uint256_t(rhs);
        }

        BIGINT_CONSTEXPR uint256_t & operator%=(const uint128_t & rhs);
        BIGINT_CONSTEXPR uint256_t & operator%=(const uint256_t & rhs);

        template <typename T> BIGINT_CONSTEXPR uint256_t & operator%=(const T & rhs){
            *this = *this % uint256_t(rhs);
            return *this;
        }

        // Increment Operators
        BIGINT_CONSTEXPR uint256_t & operator++();
        BIGINT_CONSTEXPR uint256_t operator++(int);

        // Decrement Operators
        BIGINT_CONSTEXPR uint256_t & operator--();
        BIGINT_CONSTEXPR uint256_t operator--(int);

        // Nothing done since promotion doesn't work here
        BIGINT_CONSTEXPR uint256_t operator+() const;

        // two's complement
        BIGINT_CONSTEXPR uint256_t operator-() const;

        // Get private values
        BIGINT_CONSTEXPR const uint128_t & upper() const;
        BIGINT_CONSTEXPR const uint128_t & lower() const;

        // Get bitsize of value
        BIGINT_CONSTEXPR bigint_u32type bits() const;

        // Get string representation of value
        std::string str(uint8_t base = 10, const unsigned int & len = 0) const;
};

// Give uint256_t type traits
namespace std {  // This is probably not a good idea
    template <> struct is_arithmetic <uint256_t> : std::true_type {};
    template <> struct is_integral   <uint256_t> : std::true_type {};
    template <> struct is_unsigned   <uint256_t> : std::true_type {};
};



// Bitwise Operators
BIGINT_CONSTEXPR uint256_t operator&(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator&(const T & lhs, const uint256_t & rhs){
    return rhs & lhs;
}

BIGINT_CONSTEXPR uint128_t & operator&=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator&=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs & lhs);
}

BIGINT_CONSTEXPR uint256_t operator|(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator|(const T & lhs, const uint256_t & rhs){
    return rhs | lhs;
}

BIGINT_CONSTEXPR uint128_t & operator|=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator|=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs | lhs);
}

BIGINT_CONSTEXPR uint256_t operator^(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator^(const T & lhs, const uint256_t & rhs){
    return rhs ^ lhs;
}

BIGINT_CONSTEXPR uint128_t & operator^=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator^=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs ^ lhs);
}

// Bitshift operators
BIGINT_CONSTEXPR uint256_t operator<<(const bool      & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const uint8_t   & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const uint16_t  & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const uint32_t  & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const uint64_t  & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const uint128_t & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const int8_t    & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const int16_t   & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const int32_t   & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator<<(const int64_t   & lhs, const uint256_t & rhs);

BIGINT_CONSTEXPR uint128_t & operator<<=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator<<=(T & lhs, const uint256_t & rhs){
    lhs = static_cast <T> (uint256_t(lhs) << rhs);
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const bool      & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const uint8_t   & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const uint16_t  & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const uint32_t  & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const uint64_t  & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const uint128_t & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const int8_t    & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const int16_t   & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const int32_t   & lhs, const uint256_t & rhs);
BIGINT_CONSTEXPR uint256_t operator>>(const int64_t   & lhs, const uint256_t & rhs);

BIGINT_CONSTEXPR uint128_t & operator>>=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator>>=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (uint256_t(lhs) >> rhs);
}

// Comparison Operators
BIGINT_CONSTEXPR bool operator==(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR bool operator==(const T & lhs, const uint256_t & rhs){
    return (!rhs.upper() && ((uint64_t) lhs == rhs.lower()));
}

BIGINT_CONSTEXPR bool operator!=(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR bool operator!=(const T & lhs, const uint256_t & rhs){
    return (rhs.upper() | ((uint64_t) lhs != rhs.lower()));
}

BIGINT_CONSTEXPR bool operator>(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR bool operator>(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?false:((uint128_t) lhs > rhs.lower());
}

BIGINT_CONSTEXPR bool operator<(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR bool operator<(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?true:((uint128_t) lhs < rhs.lower());
}

BIGINT_CONSTEXPR bool operator>=(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR bool operator>=(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?false:((uint128_t) lhs >= rhs.lower());
}

BIGINT_CONSTEXPR bool operator<=(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR bool operator<=(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?true:((uint128_t) lhs <= rhs.lower());
}

// Arithmetic Operators
BIGINT_CONSTEXPR uint256_t operator+(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator+(const T & lhs, const uint256_t & rhs){
    return rhs + lhs;
}

BIGINT_CONSTEXPR uint128_t & operator+=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator+=(T & lhs, const uint256_t & rhs){
    lhs = static_cast <T> (rhs + lhs);
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator-(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator-(const T & lhs, const uint256_t & rhs){
    return -(rhs - lhs);
}

BIGINT_CONSTEXPR uint128_t & operator-=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator-=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (-(rhs - lhs));
}

BIGINT_CONSTEXPR uint256_t operator*(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator*(const T & lhs, const uint256_t & rhs){
    return rhs * lhs;
}

BIGINT_CONSTEXPR uint128_t & operator*=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator*=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (rhs * lhs);
}

BIGINT_CONSTEXPR uint256_t operator/(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator/(const T & lhs, const uint256_t & rhs){
    return uint256_t(lhs) / rhs;
}

BIGINT_CONSTEXPR uint128_t & operator/=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator/=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (uint256_t(lhs) / rhs);
}

BIGINT_CONSTEXPR uint256_t operator%(const uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR uint256_t operator%(const T & lhs, const uint256_t & rhs){
    return uint256_t(lhs) % rhs;
}

BIGINT_CONSTEXPR uint128_t & operator%=(uint128_t & lhs, const uint256_t & rhs);

template <typename T> BIGINT_CONSTEXPR T & operator%=(T & lhs, const uint256_t & rhs){
    return lhs = static_cast <T> (uint256_t(lhs) % rhs);
}

// IO Operator
std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs);

BIGINT_CONSTEXPR uint256_t::uint256_t()
    : UPPER(uint128_0), LOWER(uint128_0)
{}

BIGINT_CONSTEXPR uint256_t::uint256_t(const uint256_t & rhs)
    : UPPER(rhs.UPPER), LOWER(rhs.LOWER)
{}

BIGINT_CONSTEXPR uint256_t::uint256_t(uint256_t && rhs)
    : UPPER(std::move(rhs.UPPER)), LOWER(std::move(rhs.LOWER))
{
    if (this != &rhs){
        rhs.UPPER = uint128_0;
        rhs.LOWER = uint128_0;
    }
}



BIGINT_CONSTEXPR uint256_t uint256_t::operator=(const uint256_t & rhs){
    UPPER = rhs.UPPER;
    LOWER = rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator=(uint256_t && rhs){
    if (this != &rhs){
        UPPER = std::move(rhs.UPPER);
        LOWER = std::move(rhs.LOWER);
        rhs.UPPER = uint128_0;
        rhs.LOWER = uint128_0;
    }
    return *this;
}

BIGINT_CONSTEXPR uint256_t::operator bool() const{
    return (bool) (UPPER | LOWER);
}

BIGINT_CONSTEXPR uint256_t::operator uint8_t() const{
    return (uint8_t) LOWER;
}

BIGINT_CONSTEXPR uint256_t::operator uint16_t() const{
    return (uint16_t) LOWER;
}

BIGINT_CONSTEXPR uint256_t::operator uint32_t() const{
    return (uint32_t) LOWER;
}

BIGINT_CONSTEXPR uint256_t::operator uint64_t() const{
    return (uint64_t) LOWER;
}

BIGINT_CONSTEXPR uint256_t::operator uint128_t() const{
    return LOWER;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator&(const uint128_t & rhs) const{
    return uint256_t(uint128_0, LOWER & rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator&(const uint256_t & rhs) const{
    return uint256_t(UPPER & rhs.UPPER, LOWER & rhs.LOWER);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator&=(const uint128_t & rhs){
    UPPER  = uint128_0;
    LOWER &= rhs;
    return *this;
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator&=(const uint256_t & rhs){
    UPPER &= rhs.UPPER;
    LOWER &= rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator|(const uint128_t & rhs) const{
    return uint256_t(UPPER , LOWER | rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator|(const uint256_t & rhs) const{
    return uint256_t(UPPER | rhs.UPPER, LOWER | rhs.LOWER);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator|=(const uint128_t & rhs){
    LOWER |= rhs;
    return *this;
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator|=(const uint256_t & rhs){
    UPPER |= rhs.UPPER;
    LOWER |= rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator^(const uint128_t & rhs) const{
    return uint256_t(UPPER, LOWER ^ rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator^(const uint256_t & rhs) const{
    return uint256_t(UPPER ^ rhs.UPPER, LOWER ^ rhs.LOWER);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator^=(const uint128_t & rhs){
    LOWER ^= rhs;
    return *this;
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator^=(const uint256_t & rhs){
    UPPER ^= rhs.UPPER;
    LOWER ^= rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator~() const{
    return uint256_t(~UPPER, ~LOWER);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator<<(const uint128_t & rhs) const{
    return *this << uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator<<(const uint256_t & rhs) const{
    const uint128_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) || (shift >= uint128_256)){
        return 0;
    }
    else if (shift == uint128_128){
        return uint256_t(LOWER, uint128_0);
    }
    else if (shift == uint128_0){
        return *this;
    }
    else if (shift < uint128_128){
        return uint256_t((UPPER << shift) + (LOWER >> (uint128_128 - shift)), LOWER << shift);
    }
    else if ((uint128_256 > shift) && (shift > uint128_128)){
        return uint256_t(LOWER << (shift - uint128_128), uint128_0);
    }
    else{
        return 0;
    }
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator<<=(const uint128_t & shift){
    return *this <<= uint256_t(shift);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator<<=(const uint256_t & shift){
    *this = *this << shift;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator>>(const uint128_t & rhs) const{
    return *this >> uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator>>(const uint256_t & rhs) const{
    const uint128_t shift = rhs.LOWER;
    if (((bool) rhs.UPPER) | (shift >= uint128_256)){
        return 0;
    }
    else if (shift == uint128_128){
        return uint256_t(UPPER);
    }
    else if (shift == uint128_0){
        return *this;
    }
    else if (shift < uint128_128){
        return uint256_t(UPPER >> shift, (UPPER << (uint128_128 - shift)) + (LOWER >> shift));
    }
    else if ((uint128_256 > shift) && (shift > uint128_128)){
        return uint256_t(UPPER >> (shift - uint128_128));
    }
    else{
        return 0;
    }
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator>>=(const uint128_t & shift){
    return *this >>= uint128_t(shift);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator>>=(const uint256_t & shift){
    *this = *this >> shift;
    return *this;
}

BIGINT_CONSTEXPR bool uint256_t::operator!() const{
    return ! (bool) *this;
}

BIGINT_CONSTEXPR bool uint256_t::operator&&(const uint128_t & rhs) const{
    return (*this && uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator&&(const uint256_t & rhs) const{
    return ((bool) *this && (bool) rhs);
}

BIGINT_CONSTEXPR bool uint256_t::operator||(const uint128_t & rhs) const{
    return (*this || uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator||(const uint256_t & rhs) const{
    return ((bool) *this || (bool) rhs);
}

BIGINT_CONSTEXPR bool uint256_t::operator==(const uint128_t & rhs) const{
    return (*this == uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator==(const uint256_t & rhs) const{
    return ((UPPER == rhs.UPPER) && (LOWER == rhs.LOWER));
}

BIGINT_CONSTEXPR bool uint256_t::operator!=(const uint128_t & rhs) const{
    return (*this != uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator!=(const uint256_t & rhs) const{
    return ((UPPER != rhs.UPPER) | (LOWER != rhs.LOWER));
}

BIGINT_CONSTEXPR bool uint256_t::operator>(const uint128_t & rhs) const{
    return (*this > uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator>(const uint256_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER > rhs.LOWER);
    }
    if (UPPER > rhs.UPPER){
        return true;
    }
    return false;
}

BIGINT_CONSTEXPR bool uint256_t::operator<(const uint128_t & rhs) const{
    return (*this < uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator<(const uint256_t & rhs) const{
    if (UPPER == rhs.UPPER){
        return (LOWER < rhs.LOWER);
    }
    if (UPPER < rhs.UPPER){
        return true;
    }
    return false;
}

BIGINT_CONSTEXPR bool uint256_t::operator>=(const uint128_t & rhs) const{
    return (*this >= uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator>=(const uint256_t & rhs) const{
    return ((*this > rhs) | (*this == rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator<=(const uint128_t & rhs) const{
    return (*this <= uint256_t(rhs));
}

BIGINT_CONSTEXPR bool uint256_t::operator<=(const uint256_t & rhs) const{
    return ((*this < rhs) | (*this == rhs));
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator+(const uint128_t & rhs) const{
    return *this + uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator+(const uint256_t & rhs) const{
    return uint256_t(UPPER + rhs.UPPER + (((LOWER + rhs.LOWER) < LOWER)?uint128_1:uint128_0), LOWER + rhs.LOWER);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator+=(const uint128_t & rhs){
    return *this += uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator+=(const uint256_t & rhs){
    UPPER = rhs.UPPER + UPPER + ((LOWER + rhs.LOWER) < LOWER);
    LOWER = LOWER + rhs.LOWER;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator-(const uint128_t & rhs) const{
    return *this - uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator-(const uint256_t & rhs) const{
    return uint256_t(UPPER - rhs.UPPER - ((LOWER - rhs.LOWER) > LOWER), LOWER - rhs.LOWER);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator-=(const uint128_t & rhs){
    return *this -= uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator-=(const uint256_t & rhs){
    *this = *this - rhs;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator*(const uint128_t & rhs) const{
    return *this * uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator*(const uint256_t & rhs) const{
    // split values into 4 64-bit parts
    uint128_t top[4] = {UPPER.upper(), UPPER.lower(), LOWER.upper(), LOWER.lower()};
    uint128_t bottom[4] = {rhs.upper().upper(), rhs.upper().lower(), rhs.lower().upper(), rhs.lower().lower()};
    uint128_t products[4][4];

    // multiply each component of the values
    for(int y = 3; y > -1; y--){
        for(int x = 3; x > -1; x--){
            products[3 - y][x] = top[x] * bottom[y];
        }
    }

    // first row
    uint128_t fourth64 = uint128_t(products[0][3].lower());
    uint128_t third64  = uint128_t(products[0][2].lower()) + uint128_t(products[0][3].upper());
    uint128_t second64 = uint128_t(products[0][1].lower()) + uint128_t(products[0][2].upper());
    uint128_t first64  = uint128_t(products[0][0].lower()) + uint128_t(products[0][1].upper());

    // second row
    third64  += uint128_t(products[1][3].lower());
    second64 += uint128_t(products[1][2].lower()) + uint128_t(products[1][3].upper());
    first64  += uint128_t(products[1][1].lower()) + uint128_t(products[1][2].upper());

    // third row
    second64 += uint128_t(products[2][3].lower());
    first64  += uint128_t(products[2][2].lower()) + uint128_t(products[2][3].upper());

    // fourth row
    first64  += uint128_t(products[3][3].lower());

    // combines the values, taking care of carry over
    return uint256_t(first64 << uint128_64, uint128_0) +
           uint256_t(third64.upper(), third64 << uint128_64) +
           uint256_t(second64, uint128_0) +
           uint256_t(fourth64);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator*=(const uint128_t & rhs){
    return *this *= uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator*=(const uint256_t & rhs){
    *this = *this * rhs;
    return *this;
}

BIGINT_CONSTEXPR std::pair <uint256_t, uint256_t> uint256_t::divmod(const uint256_t & lhs, const uint256_t & rhs) const{
    // Save some calculations /////////////////////
    if (rhs == 0){
        assume(false);
    }
    /*
    else if (rhs == 1){
        return std::pair <uint256_t, uint256_t> (lhs, 0);
    }
    else if (lhs == rhs){
        return std::pair <uint256_t, uint256_t> (1, 0);
    }
    else if ((lhs == 0) || (lhs < rhs)){
        return std::pair <uint256_t, uint256_t> (0, lhs);
    }*/

    std::pair <uint256_t, uint256_t> qr(0, lhs);
    uint256_t copyd = rhs << (lhs.bits() - rhs.bits());
    uint256_t adder = 1 << static_cast<uint32_t>(lhs.bits() - rhs.bits());
    if (copyd > qr.second){
        copyd >>= 1;
        adder >>= 1;
    }
    while (qr.second >= rhs){
        if (qr.second >= copyd){
            qr.second -= copyd;
            qr.first |= adder;
        }
        copyd >>= 1;
        adder >>= 1;
    }
    return qr;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator/(const uint128_t & rhs) const{
    return *this / uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator/(const uint256_t & rhs) const{
    return divmod(*this, rhs).first;
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator/=(const uint128_t & rhs){
    return *this /= uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator/=(const uint256_t & rhs){
    *this = *this / rhs;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator%(const uint128_t & rhs) const{
    return *this % uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator%(const uint256_t & rhs) const{
    return *this - (rhs * (*this / rhs));
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator%=(const uint128_t & rhs){
    return *this %= uint256_t(rhs);
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator%=(const uint256_t & rhs){
    *this = *this % rhs;
    return *this;
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator++(){
    *this += 1;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator++(int){
    uint256_t temp(*this);
    ++*this;
    return temp;
}

BIGINT_CONSTEXPR uint256_t & uint256_t::operator--(){
    *this -= 1;
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator--(int){
    uint256_t temp(*this);
    --*this;
    return temp;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator+() const{
    return *this;
}

BIGINT_CONSTEXPR uint256_t uint256_t::operator-() const{
    return ~*this + 1;
}

BIGINT_CONSTEXPR const uint128_t & uint256_t::upper() const {
    return UPPER;
}

BIGINT_CONSTEXPR const uint128_t & uint256_t::lower() const {
    return LOWER;
}

BIGINT_CONSTEXPR bigint_u32type uint256_t::bits() const{
    bigint_u32type out = 0;
    if (UPPER){
        out = 128;
        uint128_t up = UPPER;
        while (up){
            up >>= uint128_1;
            out++;
        }
    }
    else{
        uint128_t low = LOWER;
        while (low){
            low >>= uint128_1;
            out++;
        }
    }
    return out;
}

std::string uint256_t::str(uint8_t base, const unsigned int & len) const{
    if ((base < 2) || (base > 16)){
        assume(false);
    }
    std::string out = "";
    if (!(*this)){
        out = "0";
    }
    else{
        std::pair <uint256_t, uint256_t> qr(*this, 0);
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

BIGINT_CONSTEXPR uint256_t operator&(const uint128_t & lhs, const uint256_t & rhs){
    return rhs & lhs;
}

BIGINT_CONSTEXPR uint128_t & operator&=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (rhs & lhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator|(const uint128_t & lhs, const uint256_t & rhs){
    return rhs | lhs;
}

BIGINT_CONSTEXPR uint128_t & operator|=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (rhs | lhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator^(const uint128_t & lhs, const uint256_t & rhs){
    return rhs ^ lhs;
}

BIGINT_CONSTEXPR uint128_t & operator^=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (rhs ^ lhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const bool & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const uint8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const uint16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const uint32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const uint64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const uint128_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const int8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const int16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const int32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint256_t operator<<(const int64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) << rhs;
}

BIGINT_CONSTEXPR uint128_t & operator<<=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (uint256_t(lhs) << rhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const bool & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const uint8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const uint16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const uint32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const uint64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const uint128_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const int8_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const int16_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const int32_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint256_t operator>>(const int64_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) >> rhs;
}

BIGINT_CONSTEXPR uint128_t & operator>>=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (uint256_t(lhs) >> rhs).lower();
    return lhs;
}

// Comparison Operators
BIGINT_CONSTEXPR bool operator==(const uint128_t & lhs, const uint256_t & rhs){
    return rhs == lhs;
}

BIGINT_CONSTEXPR bool operator!=(const uint128_t & lhs, const uint256_t & rhs){
    return rhs != lhs;
}

BIGINT_CONSTEXPR bool operator>(const uint128_t & lhs, const uint256_t & rhs){
    return rhs < lhs;
}

BIGINT_CONSTEXPR bool operator<(const uint128_t & lhs, const uint256_t & rhs){
    return rhs > lhs;
}

BIGINT_CONSTEXPR bool operator>=(const uint128_t & lhs, const uint256_t & rhs){
    return rhs <= lhs;
}

BIGINT_CONSTEXPR bool operator<=(const uint128_t & lhs, const uint256_t & rhs){
    return rhs >= lhs;
}

// Arithmetic Operators
BIGINT_CONSTEXPR uint256_t operator+(const uint128_t & lhs, const uint256_t & rhs){
    return rhs + lhs;
}

BIGINT_CONSTEXPR uint128_t & operator+=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (rhs + lhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator-(const uint128_t & lhs, const uint256_t & rhs){
    return -(rhs - lhs);
}

BIGINT_CONSTEXPR uint128_t & operator-=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (-(rhs - lhs)).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator*(const uint128_t & lhs, const uint256_t & rhs){
    return rhs * lhs;
}

BIGINT_CONSTEXPR uint128_t & operator*=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (rhs * lhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator/(const uint128_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) / rhs;
}

BIGINT_CONSTEXPR uint128_t & operator/=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (uint256_t(lhs) / rhs).lower();
    return lhs;
}

BIGINT_CONSTEXPR uint256_t operator%(const uint128_t & lhs, const uint256_t & rhs){
    return uint256_t(lhs) % rhs;
}

BIGINT_CONSTEXPR uint128_t & operator%=(uint128_t & lhs, const uint256_t & rhs){
    lhs = (uint256_t(lhs) % rhs).lower();
    return lhs;
}

std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs){
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
