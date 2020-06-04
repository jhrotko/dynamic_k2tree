#ifndef __UTILS_H__
#define __UTILS_H__

#include <cmath>

#define EPS 0.25
#define TAU(m) log2(log2(m))
#define MAXSZ(m, i) floor(((m) << 1) / pow(log2(m), 2 - (i)*EPS))

typedef unsigned int uint;

uint max(uint a, uint b)
{
    return a >= b? a : b;
}

#endif