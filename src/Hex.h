#pragma once

#include <string.h>
#include <stdlib.h>

char* hexEncode(const char* input) {
    size_t hexStringLength = strlen(input);
    char* hexString = malloc(hexStringLength * 2 + 1);
    if (!hexString)
        return NULL;

    char* string = hexString;
    for (size_t i = 0; i < hexStringLength; i++) {
        sprintf_s(string, 3, "%02hhx", (unsigned char) input[i]);
        string += 2;
    }

    *string = '\0';
    return hexString;
}

unsigned char* hexStringToBytes(const char* hexString, size_t* outLength) {
    size_t hexStringLength = strlen(hexString);
    if (hexStringLength % 2 != 0)
        return NULL;

    *outLength = hexStringLength / 2;
    unsigned char* buffer = malloc(*outLength);
    if (!buffer)
        return NULL;

    char hexBuffer[3];
    for (size_t i = 0; i < hexStringLength; i += 2) {
        strncpy_s(hexBuffer, sizeof(hexBuffer), hexString + i, 2);
        hexBuffer[2] = '\0';

        buffer[i / 2] = (unsigned char) strtol(hexBuffer, NULL, 16);
    }

    return buffer;
}
