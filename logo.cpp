#include <avr/pgmspace.h>
//#include <TVout.h>
#include <Gamebuino.h>
#include "logo_bitmap.h"
extern Gamebuino gb;
//extern TVout TV;

#define SIZEOF_LOGO_RECORD 10

void drawLogo(char start)
{
  //draw hearts
  char x_offset = 26; //84/2-16                 //48;
  for (char i=start; i < (start+4); i++)
  {
      //draw logo element
      gb.display.drawBitmap(x_offset,16, logo_bitmap + (SIZEOF_LOGO_RECORD * i));
      //TV.bitmap(x_offset, 16, logo_bitmap + (SIZEOF_LOGO_RECORD * i));
   
    x_offset += 8;
  }   
}

