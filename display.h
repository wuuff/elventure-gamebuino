#ifndef __DISPLAY__
#define __DISPLAY__
#include "elf.h"

#define MAX_HEARTS 4



//Arduino IDE is a farce

#define FACING_DOWN  0
#define FACING_UP    3
#define FACING_LEFT  6
#define FACING_RIGHT 9

#define STEP_LENGTH  4

#define MAX_ITEMS 3

#define ELFSTATE_PLAYING 0
#define ELFSTATE_DEAD 1
#define ELFSTATE_WON 2

typedef struct ElfStruct
{
  unsigned char facing;
  unsigned char step;
  unsigned char x;
  unsigned char y;
  unsigned char hearts;
  unsigned char items[4];
  unsigned char state;
} Elf;

void updateDisplay(Elf elf);

#endif __DISPLAY__
