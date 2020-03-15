#ifndef __UTILS_H__
#define __UTILS_H__

#define EPS 0.25
#define TAU(m) log2(log2(m))
#define MAXSZ(m, i) floor(((m) << 1) / pow(log2(m), 2 - (i)*EPS))

typedef unsigned int uint;

/*!
         *  \param base base of exponential.
         *  \param pow 
         *  \returns Calculates the exponential with a base
         */
uint exp_pow(uint base, uint pow)
{
    uint i, result = 1;
    for (i = 0; i < pow; i++)
        result *= base;

    return result;
}

uint max(uint a, uint b)
{
    return a >= b? a : b;
}

#endif