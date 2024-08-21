#include <stdio.h>
#include <math.h>
#include <sys/timeb.h>
#include <malloc.h>
#include "Hex.h"
#include "Base32.h"
#include "HMac.h"

char* timeToHex(time_t millis, double period) {
    long long time = (long long) floor(round((double) millis / 1000.0) / period);

    char* buffer = malloc(32);
    if (!buffer)
        return NULL;

    sprintf_s(buffer, 32, "%016llx", time);
    return buffer;
}

int generateCode(const char* secret, char* time, int digits) {
    char* decodedSecret;
    size_t length;
    b32_decode(&decodedSecret, &length, secret, strlen(secret));
    if (!decodedSecret) {
        fprintf(stderr, "Cannot decode secret\n");
        return -1;
    }
    decodedSecret[length] = '\0';

    size_t timeLength, keyLength;
    const char* hexSecret = hexEncode(decodedSecret);
    if (!hexSecret) {
        fprintf(stderr, "Cannot encode secret to hex string\n");
        return -1;
    }

    unsigned char* timeBytes = hexStringToBytes(time, &timeLength);
    if (!timeBytes) {
        fprintf(stderr, "Cannot convert time hex to bytes\n");
        return -1;
    }

    unsigned char* keyBytes = hexStringToBytes(hexSecret, &keyLength);
    if (!keyBytes) {
        fprintf(stderr, "Cannot convert secret hex to bytes\n");
        free(timeBytes);
        return -1;
    }

    unsigned int hashLength;
    unsigned char* hash = hmac("SHA1", keyBytes, (int) keyLength, timeBytes, timeLength, &hashLength); // Вычислям HMAC-SHA1 от времени и ключа
    free(timeBytes);
    free(keyBytes);
    if (!hash) {
        fprintf(stderr, "Cannot get hash HMAC-SHA1\n");
        return -1;
    }

    int offset = hash[hashLength - 1] & 0xF; // Эквивалентно сдвигу вправо на 4 бита
    int binary = ((hash[offset] & 0x7F) << 24)
                 | ((hash[offset + 1] & 0xFF) << 16)
                 | ((hash[offset + 2] & 0xFF) << 8)
                 | (hash[offset + 3] & 0xFF); // Значение из четырёх байтов

    int code = binary % ((int) pow(10, digits)); // Получаем остаток деления на число 10 в степени длины кода (обычно 10⁶)

    free(hash);
    return code;
}

int main(void) {
    struct _timeb timeBuffer;
    _ftime64_s(&timeBuffer);
    time_t millis = (timeBuffer.time * 1000) + timeBuffer.millitm;

    char* time = timeToHex(millis, 30.0);
    if (!time) {
        fprintf(stderr, "An error occured when converting milliseconds to hex\n");
        return 1;
    }

    char secret[256];
    gets_s(secret, sizeof(secret));

    int code = generateCode(secret, time, 6);

    printf("%06d\n", code);
    free(time);
    return 0;
}
