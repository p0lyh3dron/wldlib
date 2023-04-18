/*
 *    rand.h    --    Header for Terraria's random number generator emulation
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on April 17, 2023
 * 
 *    This file declares the functions used to emulate Terraria's random number
 *    generator. The functions are declared in the same order as they appear in
 *    the original game's source code.
 */
#ifndef WLD_RAND_H
#define WLD_RAND_H

/*
 *    Sets the seed for the random number generator.
 *
 *    @param int seed    The seed to use.
 */
void set_seed(int seed);

/*
 *    Source code def: Sample
 *
 *    @return double    A random number between 0 and 1?
 */
double sample(void);

/*
 *    Source code def: InternalSample
 *
 *    @return int    A random number between 0 and 2^32-1?
 */
int internal_sample(void);

/*
 *    Gets the sample for a larger range.
 *
 *    @return double    A random number between 0 and 1?
 */
double get_sample_for_large_range(void);

/*
 *    Gets the next random number in the sequence.
 *
 *    @return int    A random number between 0 and 2^32-1.
 */
int next(void);

/*
 *    Gets the next random number in the sequence.
 *
 *    @param int min    The minimum value to return.
 *    @param int max    The maximum value to return.
 *    @return int       A random number between minValue and maxValue.
 */
int next_minmax(int min, int max);

/*
 *    Gets the next random number in the sequence.
 *
 *    @param int max    The maximum value to return.
 *    @return int       A random number between 0 and maxValue.
 */
int next_max(int max);

/*
 *    Randomizes a byte array.
 *
 *    @param char *buffer    The buffer to randomize.
 *    @param int length      The length of the buffer.
 */
void next_bytes(char *buffer, int length);

/*
 *    Performs a CRC32 checksum on a byte array.
 *
 *    @param char *buffer    The buffer to checksum.
 *    @param int length      The length of the buffer.
 * 
 *    @return unsigned int    The CRC32 checksum of the buffer.
 */
unsigned int rand_crc32(char *buffer, int length);

#endif /* WLD_RAND_H  */