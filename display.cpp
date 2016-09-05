#include <avr/pgmspace.h>
//#include <TVout.h>
#include "display_bitmap.h"
#include "bitmap_funcs.h"
#include "elf.h"
#include "display.h"
#include "room.h"

//extern TVout TV;
#include <Gamebuino.h>
extern Gamebuino gb;

#define SIZEOF_DISPLAY_RECORD 6

//For now, let's just move our icons under the battery icon and leave it enabled.

//In contrast to the original game, which draws the hearts horizontally,
//since we have less space, let's try drawing them vertically along
//the right side...

void updateDisplay(Elf elf)
{
  //draw hearts
  char x_offset = 80; //4 less than maximum                  //98/2;
  char y_offset = 8;
  for (char i=1; i <= MAX_HEARTS; i++)
  {
    if (i <= elf.hearts)
    {
      //draw hearts
      //gb.display.cursorX = x_offset;
      //gb.display.cursorY = 0;
      //gb.display.print("hearts");
      gb.display.drawBitmap(x_offset, y_offset, display_bitmap + SIZEOF_DISPLAY_RECORD);
      //TV.bitmap(x_offset, 4, display_bitmap + SIZEOF_DISPLAY_RECORD);
    } else {
      //draw blanks
      eraseBitmapRect(x_offset,y_offset,display_bitmap);
      //gb.display.drawBitmap(x_offset, 4, display_bitmap);
      //TV.bitmap(x_offset, 4, display_bitmap);
    }	
    y_offset += 5;
    //x_offset += 5;
  }
   
  //draw crystals (inventory)
  x_offset = 80; //4 less than maximum                               //98/2;
  y_offset = 28; //After the hearts
  for (char j=0; j < MAX_ITEMS; j++)
  {
    switch (elf.items[j])
    {
	  case ITEM_CRYSTAL:
        //draw crystals
        gb.display.drawBitmap(x_offset, y_offset, display_bitmap + (SIZEOF_DISPLAY_RECORD * 2 ));
        //TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 2 ));
		break;
		
	  case ITEM_ARMOR:
        //draw crystals
        gb.display.drawBitmap(x_offset, y_offset, display_bitmap + (SIZEOF_DISPLAY_RECORD * 3 ));
        //TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 3 ));
		break;

	  case ITEM_ORB:
        //draw crystals
        gb.display.drawBitmap(x_offset, y_offset, display_bitmap + (SIZEOF_DISPLAY_RECORD * 4 ));
        //TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 4 ));
		break;

	  case ITEM_STAFF:
        //draw crystals
        gb.display.drawBitmap(x_offset, y_offset, display_bitmap + (SIZEOF_DISPLAY_RECORD * 5 ));
        //TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 5 ));
		break;
		
      default:
        //draw blanks
        eraseBitmapRect(x_offset,y_offset,display_bitmap);
        //gb.display.drawBitmap(x_offset, 12, display_bitmap);
        //TV.bitmap(x_offset, 12, display_bitmap);
        break;
    }	
    y_offset += 5;
  }   
}

