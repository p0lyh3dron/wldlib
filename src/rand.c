/*
 *    rand.c    --    Source for Terraria's random number generator emulation
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on April 17, 2023
 * 
 *    This file defines the functions used to emulate Terraria's random number
 *    generator. The functions are declared in the same order as they appear in
 *    the original game's source code.
 */
#include "rand.h"

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))

const int _rand_max = 2147483647;
const int _seed     = 161803398;
int       _i_next;
int       _i_next_p;
int       _seed_array[56];

/*
 *    Sets the seed for the random number generator.
 *
 *    @param int seed    The seed to use.
 */
void set_seed(int seed) {
    unsigned long i;
    for (i = 0; i < ARR_LEN(_seed_array); ++i)
        _seed_array[i] = 0;

    /*
     *    No idea what these num variables are, 
     *    so anyone who stumbles across this can suffer.
     */
    int num = (seed == -2147483648) ? 2147483647 : abs(seed);
    int num2 = 161803398 - num;
    _seed_array[55] = num2;
    int num3 = 1;

    for (i = 1; i < 55; ++i) {
        unsigned long num4 = (21 * i) % 55;
        _seed_array[num4] = num3;
        num3 = num2 - num3;
        if (num3 < 0)
            num3 += 2147483647;

        num2 = _seed_array[num4];
    }

    for (i = 1; i < 5; ++i) {
        unsigned long j;
        for (j = 1; j < 56; ++j) {
            _seed_array[j] -= _seed_array[1 + (j + 30) % 55];
            if (_seed_array[j] < 0)
                _seed_array[j] += 2147483647;
        }
    }

    _i_next   = 0;
    _i_next_p = 21;
}

/*
 *    Source code def: Sample
 *
 *    @return double    A random number between 0 and 1?
 */
double sample(void) {
    return (double)internal_sample() * 4.656612875245797e-10;
}

/*
 *    Source code def: InternalSample
 *
 *    @return int    A random number between 0 and 2^32-1?
 */
int internal_sample(void) {
    int num = _i_next;
    int num2 = _i_next_p;

    if (++num >= 56)
        num = 1;

    if (++num2 >= 56)
        num2 = 1;
    
    int num3 = _seed_array[num] - _seed_array[num2];

    if (num3 == 2147483647)
        num3--;

    if (num3 < 0)
        num3 += 2147483647;

    _seed_array[num] = num3;
    _i_next = num;
    _i_next_p = num2;

    return num3;
}

/*
 *    Gets the sample for a larger range.
 *
 *    @return double    A random number between 0 and 1?
 */
double get_sample_for_large_range(void) {
    int num = internal_sample();

    if ((internal_sample() % 2) == 0)
        num = -num;

    return ((double)num + 2147483646.0) / 4294967293.0;
}

/*
 *    Gets the next random number in the sequence.
 *
 *    @return int    A random number between 0 and 2^32-1.
 */
int next(void) {
    return internal_sample();
}

/*
 *    Gets the next random number in the sequence.
 *
 *    @param int min    The minimum value to return.
 *    @param int max    The maximum value to return.
 *    @return int       A random number between minValue and maxValue.
 */
int next_minmax(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    long num2 = (long)(max - min);
    if (num2 <= 2147483647)
        return (int)(sample() * (double)num2) + min;

    return (int)((long)(get_sample_for_large_range() * (double)num2) + min);
}

/*
 *    Gets the next random number in the sequence.
 *
 *    @param int max    The maximum value to return.
 *    @return int       A random number between 0 and maxValue.
 */
int next_max(int max) {
    return (int)(sample() * (double)max);
}

/*
 *    Randomizes a byte array.
 *
 *    @param char *buffer    The buffer to randomize.
 *    @param int length      The length of the buffer.
 */
void next_bytes(char *buffer, int length) {
    if (buffer == (char *)0x0)
        return;

    unsigned long i;
    for (i = 0; i < length; ++i)
        buffer[i] = (char)(internal_sample() % 256);
}

/*
 *    Performs a CRC32 checksum on a byte array.
 *
 *    @param char *buffer    The buffer to checksum.
 *    @param int length      The length of the buffer.
 * 
 *    @return unsigned int    The CRC32 checksum of the buffer.
 */
unsigned int rand_crc32(char *buffer, int length) {
    unsigned int crc = 0xFFFFFFFF;
    unsigned int i;

    for (i = 0; i < length; ++i) {
        crc = crc ^ buffer[i];
        unsigned int j;
        for (j = 8; j > 0; --j) {
            if ((crc & 1) == 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }

    return ~crc;
}