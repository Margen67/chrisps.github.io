#pragma once

#define assume(x)   __builtin_assume(x)
#define noinline    __attribute__((noinline))
using ix_t = double;
using jnum_t = double;