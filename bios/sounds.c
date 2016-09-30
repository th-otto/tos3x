#include "bios.h"

uint8_t const bellsnd[] = {
  0, 0x34,    /* channel A pitch */
  1, 0,
  2, 0,       /* no channel B */
  3, 0,
  4, 0,       /* no channel C */
  5, 0,
  6, 0,       /* no noise */
  7, 0xFE,    /* no sound or noise except channel A */
  8, 0x10,    /* channel A amplitude */
  9, 0,
  10, 0,
  11, 0,      /* envelope */
  12, 16,
  13, 9,
  0xFF, 0,    /* stop sound */
};

uint8_t const clicksnd[] = {
  0, 0x3B,
  1, 0,
  2, 0,
  3, 0,
  4, 0,
  5, 0,
  6, 0,
  9, 0,
  10, 0,
  7, 0xFE,
  8, 16,
  13, 3,
  11, 0x80,
  12, 1,
  0xFF, 0,
};
