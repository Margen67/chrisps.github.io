#pragma once

//#define assume(x)   __builtin_assume(x)
#define assume(x)     if(!(x))  __builtin_unreachable()
#define noinline    __attribute__((noinline))
using ix_t = double;
using jnum_t = double;

template<typename T> 
constexpr jnum_t bitsizeof = sizeof(T) * 8u;