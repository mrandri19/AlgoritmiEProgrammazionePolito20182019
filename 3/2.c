#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void stampaCodifica(void *number, unsigned long size, bool bigEndian);
void get_bits(void *number, unsigned char bits[], unsigned long size);
void print_bits(unsigned char bits[], unsigned long size);

// number: a generic floating point number
// bits[]: an array of unsigned chars which will be set to the number's bits
// size: the length of bits
// es: get_bits((unsigned int) 42, ..., 4)
// -> 0 0 0 0  0 0 0 0  0 0 1 0  1 0 1 0
void get_bits(void *number, unsigned char bits[], unsigned long size) {
  printf("\n");
  printf("hex: ");

  // Iterate through the number byte-by-byte
  for (unsigned long i = 0; i < size / CHAR_BIT; ++i) {
    unsigned char byte = *(((unsigned char *)number) + i);
    printf("%02x ", byte);

    unsigned long j = 0;
    while (byte > 0) {
      // FIXME: doesnt work with big endian
      bits[size - (i * CHAR_BIT) - j - 1] = byte % 2;
      byte /= 2;
      j++;
    }
  }
  printf("\n");
}

void print_bits(unsigned char bits[], unsigned long size) {
  printf("bits: ");
  for (unsigned long i = 0; i < size; ++i) {
    printf("%u", bits[i]);
  }
  printf("\n");
}

void stampaCodifica(void *number, unsigned long size, bool bigEndian) {
  // FIXME: doesnt work with big endian
  if (size == 4) {
    // We want to allocate an unsigned char foreach bit of the input, and size
    // is in bytes so we multiply bt CHAR_BIT (bits in a byte)
    unsigned char *bits = calloc(sizeof(unsigned char), size * CHAR_BIT);
    get_bits(number, bits, size * CHAR_BIT);
    print_bits(bits, size * CHAR_BIT);

    // Print sign bit, exponent bits and mantissa bits in binary form.
    printf("sign bit: %u\n", bits[0]);
    printf("exponent bits: ");
    for (unsigned long i = 0 + 1; i < 8 + 1; ++i) {
      printf("%u", bits[i]);
    }
    printf("\n");
    printf("mantissa bits: ");
    for (unsigned long i = 8 + 1; i < (size * CHAR_BIT); ++i) {
      printf("%u", bits[i]);
    }
    printf("\n");

    free(bits);
    return;
  }
  if (size == 8) {
    unsigned char *bits = calloc(sizeof(unsigned char), size * CHAR_BIT);
    get_bits(number, bits, size * CHAR_BIT);
    print_bits(bits, size * CHAR_BIT);

    // Print sign bit, exponent bits and mantissa bits in binary form.
    printf("sign bit: %u\n", bits[0]);
    printf("exponent bits: ");
    for (unsigned long i = 0 + 1; i < 11 + 1; ++i) {
      printf("%u", bits[i]);
    }
    printf("\n");
    printf("mantissa bits: ");
    for (unsigned long i = 11 + 1; i < (size * CHAR_BIT); ++i) {
      printf("%u", bits[i]);
    }
    printf("\n");

    free(bits);
    return;
  }
  if (size == 16) {
    unsigned char *bits = calloc(sizeof(unsigned char), size * CHAR_BIT);
    get_bits(number, bits, size * CHAR_BIT);
    print_bits(bits, size * CHAR_BIT);

    // Print sign bit, exponent bits and mantissa bits in binary form.
    printf("sign bit: %u\n", bits[0]);
    printf("exponent bits: ");
    for (unsigned long i = 0 + 1; i < 15 + 1; ++i) {
      printf("%u", bits[i]);
    }
    printf("\n");
    printf("mantissa bits: ");
    for (unsigned long i = 15 + 1; i < (size * CHAR_BIT); ++i) {
      printf("%u", bits[i]);
    }
    printf("\n");

    free(bits);
    return;
  }
}

int main() {
  float af;
  double ad;
  long double ald;
  printf("Insert a real number: ");

  // Scanf-ing to the long double to keep the max precision, we'll reduce it
  // later
  scanf("%Lf", &ald);

  // This casts don't change ald, right?
  af = (float)ald;
  ad = (double)ald;

  // Understand if pc is big endian or little endian
  bool bigEndian;

  // How check is represented in memory:

  // Big Endian (MSB first)
  // 0x12 0x34 0x56 0x78

  // Little Endian (MSB last)
  // 0x78 0x56 0x34 0x12
  // /\
  // ||
  // Using the & we will have the address pointing here, then casting it to an
  // unsigned char* will let us examine only the first byte, which will be
  // either 0x12 if big endian or 0x78 if little endian

  unsigned int check = 0x12345678;
  unsigned char c = *(unsigned char *)&check;
  bigEndian = (c == 0x12);

  // Display sizes in bits and bytes of all vars, CHAR_BIT was found in limits.h
  // and contains how many bytes in a bit for this architecture.
  printf("Length of a float: %lu (Bytes), %lu (bits)\n", sizeof(af),
         sizeof(af) * CHAR_BIT);
  printf("Length of a double: %lu (Bytes), %lu (bits)\n", sizeof(ad),
         sizeof(ad) * CHAR_BIT);
  printf("Length of a long double: %lu (Bytes), %lu (bits)\n", sizeof(ald),
         sizeof(ald) * CHAR_BIT);
  printf("\n");

  // Display codification of the variables
  stampaCodifica((void *)&af, sizeof(af), bigEndian);
  stampaCodifica((void *)&ad, sizeof(ad), bigEndian);
  stampaCodifica((void *)&ald, sizeof(ald), bigEndian);

  return 0;
}
