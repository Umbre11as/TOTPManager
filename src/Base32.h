#pragma once

// Credits to https://gist.github.com/jkiv/5efcdd0e378c59a864be87adaec35957

#include <stdio.h>
#include <stdlib.h>

char b32_decode_char(char c);
void b32_decode(char** dst, size_t* dstlen, const char* src, size_t srclen);

void b32_decode(char** dst, size_t* dstlen, const char* src, size_t srclen)
{
    size_t padlen = 0;   // Number of ='s in padding
    size_t lastlen = 0;  // Length of last quantum in characters

    *dst = NULL;
    *dstlen = 0;

    // Check padding
    for (size_t i = 1; i < srclen; i++)
    {
        if (src[srclen-i] == '=')
            padlen++;
        else
            break;
    }

    // Check source material
    for (size_t i = 0; i < srclen - padlen; i++)
    {
        if (b32_decode_char(src[i]) > 0x1F)
        {
            // ERROR: one or more characters cannot be decoded
            return;
        }
    }

    // Calculate the length of the last quantum in src
    lastlen = (srclen - padlen) % 8;

    // How many quantums do we have?
    size_t qmax = (srclen - padlen) / 8;

    if (lastlen > 0)
    {
        // Last quantum is a partial quantum
        // ... qmax rounded down
        qmax += 1;
    }
    else
    {
        // Last quantum is a full quantum
        // ... length of last quantum is 8, not 0
        lastlen = 8;
    }

    // Calculate dst buffer size
    *dstlen = ((srclen - padlen) / 8) * 5;

    switch(lastlen)
    {
        case 8:
            break;
        case 7:
            *dstlen += 4;
            break;
        case 5:
            *dstlen += 3;
            break;
        case 4:
            *dstlen += 2;
            break;
        case 2:
            *dstlen += 1;
            break;
        default:
            // ERROR: Not a multiple of a byte.
            *dstlen = 0;
            break;
    }

    if (dstlen == 0)
    {
        // Either empty src, or an error occurred
        return;
    }

    // Allocate dst buffer
    *dst = (char*) malloc(sizeof(char) * (*dstlen));

    // Loop variables
    size_t qlen;
    char* pdst = *dst;
    const char* psrc = src;

    // Decode each quantum
    for (size_t q = 0; q < qmax; q++)
    {
        // Are we on the last quantum?
        if (q == qmax - 1)
            qlen = lastlen;
        else
            qlen = 8;

        // dst       0           1          2          3           4
        //      [11111 111][11 11111 1][1111 1111][1 11111 11][111 11111]
        // src     0      1      2      3       4      5      6      7

        switch(qlen)
        {
            // 8 = 5 bytes in quantum
            case 8:
                pdst[4]  = b32_decode_char(psrc[7]);
                pdst[4] |= b32_decode_char(psrc[6]) << 5;
                // 7 = 4 bytes in quantum
            case 7:
                pdst[3]  = b32_decode_char(psrc[6]) >> 3;
                pdst[3] |= (b32_decode_char(psrc[5]) & 0x1F) << 2;
                pdst[3] |= b32_decode_char(psrc[4]) << 7;
                // 5 = 3 bytes in quantum
            case 5:
                pdst[2] = b32_decode_char(psrc[4]) >> 1;
                pdst[2] |= b32_decode_char(psrc[3]) << 4;
                // 4 = 2 bytes in quantum
            case 4:
                pdst[1]  = b32_decode_char(psrc[3]) >> 4;
                pdst[1] |= (b32_decode_char(psrc[2]) & 0x1F) << 1;
                pdst[1] |= b32_decode_char(psrc[1]) << 6;
                // 2 = 1 byte in quantum
            case 2:
                pdst[0]  = b32_decode_char(psrc[1]) >> 2;
                pdst[0] |= b32_decode_char(psrc[0]) << 3;
                break;
            default:
                break; // TODO error
        }

        // Move quantum pointers forward
        psrc += 8;
        pdst += 5;
    }
}

char b32_decode_char(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    else if (c >= '2' && c <= '7')
        return c - '2' + 26;
        // ... handle lowercase here???
    else if (c >= 'a' && c <= 'z')
        return c - 'a';
    else
        return 0xFF; // ERROR
}
