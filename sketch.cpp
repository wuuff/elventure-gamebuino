/*                                   __    
                                   /___/
                                   / /
   /-------------/              __/_/__
     |  |-------/       //===//__<*>_ //===//
     |  .----/                 / / /
     |  .---/                 / / /
     |  |------/             / / /
   /----------/ L + V + E + N + T + U + R + E *=-
   An Adventure Game for the Arduino/Video Game Shield 

   2011 - trodoss
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   bitmap_funcs.cpp used from "Parachute" sketch by pocketscience.
   For a full copy of that sketch, see:
   http://nootropicdesign.com/hackvision/downloads/games/Parachute.zip
   
   _____________________________________________________________________
   
   Elventure: http://http://www.team-arg.org/ELV-manual.html
 
   TV-out version 1.0 : http://www.team-arg.org/ELV-downloads.html
 
   PUBLISHED by TEAM a.r.g. : http://www.team-arg.org/About.html
 
   2013 - Trodoss
   
 */
 
//determine the game
#define GAME_ID 18 

#include <stdlib.h> 
#include <SPI.h>
#include <Gamebuino.h>
//#include <i2cmaster.h>
//#include <nunchuck.h>
//#include <TVout.h>
//#include <font4x6.h>
extern const uint8_t font3x5[];

#include "map.h"
#include "room.h"
#include "elf.h"
#include "sound.h"
#include "display.h"
#include "logo.h"
#include "other_bitmap.h"

#include "logo_bitmap.h"

//Require these for redrawing everything after pause menu
#include "bitmap_funcs.h"
#include "map_bitmap.h"
#include "elf_bitmap.h"
#define SIZEOF_ELF_RECORD 10
#define SIZEOF_MAP_RECORD 10

//Another example of the perils of Arduino IDE

void resetElf(bool reset_items);
void showElf();
void moveElf(unsigned char facing);
void throwSword();
RoomElement hitElf(RoomElement element);
Elf getElf();
void addElfItem(char type);
bool elfHasItem(char type);
char getElfState();

#define SCREENWIDTH     128
#define SCREENHEIGHT    96

#define PAUSE_BETWEEN_ACTIONS 100

#define GAME_TITLE   0
#define GAME_PLAYING 1
#define GAME_PAUSED  2
#define GAME_WON     3
#define GAME_OVER    4
//TVout TV;
Gamebuino gb;
//Nunchuck player1;
char game_state;

void start_game()
{
  //clear the contents of the screen
  gb.display.clear();
  gb.display.persistence = true;
  gb.setFrameRate(10);//100 ms per frame
  gb.display.setColor(BLACK,WHITE);

  gb.sound.command(1, 0, 0, 0);
  //TV.clear_screen();
  
  //start the game
  setMapRoom(0);
  showElf();
  updateDisplay(getElf());
  play_song(0);
  game_state = GAME_PLAYING;  
}

void start_title()
{
  //clear the contents of the screen
  gb.display.clear();
  gb.display.persistence = true;
  //gb.display.setColor(BLACK,WHITE);
  //TV.clear_screen();
  
  game_state = GAME_TITLE;
  drawLogo(0);
  play_song(4);
}

void start_game_over()
{
  //clear the contents of the screen
  gb.display.clear();
  //TV.clear_screen();
  
  game_state = GAME_OVER;
  drawLogo(4);
  play_song_once(2);
}

void start_game_won()
{
  //clear the contents of the screen
  gb.display.clear();
  //TV.clear_screen();
  
  game_state = GAME_WON;
  drawLogo(8);
  play_song(3);
}

void setup()
{
  gb.begin();
  //TV.begin(_NTSC, SCREENWIDTH, SCREENHEIGHT);
  //TV.begin(_PAL, SCREENWIDTH, SCREENHEIGHT);
  gb.display.setFont(font3x5);
  //TV.select_font(font4x6);
  
  randomSeed(analogRead(0));
  //player1.begin(0);

  
  gb.titleScreen(F("Original by TEAM ARG.\nPort by Wuff."),title_bitmap_unified);

  //Skip the original title screen since we used the Gamebuino title screen.
  start_game();
  //start_title();
  
  //TV.clear_screen();
  //TV.bitmap(18,32,team_arg_bitmap);
  //TV.delay(3000);
  
  //TV.clear_screen();
  //TV.bitmap(16,48,team_arg_bitmap);
  //TV.delay(1500);
}

void loop()
{  
  static long next_action = 0;
  switch (game_state)
  {
    case GAME_TITLE:
    case GAME_OVER:
    case GAME_WON:
       if( gb.update() )
       //if (TV.millis() >= next_action)
       {
         //player1.update();
         //wait for a button to be pressed to continue
         //if ((player1.button_c()) || (player1.button_z()))
         if (gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B))
         {
       
           //check to see what we are doing when we start the game
           if (game_state > GAME_TITLE)
           {
             if (game_state == GAME_WON)
             {
               //if the game has been won, then reset the inventory of the elf
               resetElf(true);
             } else {
               //if the game was just over (died) then do not reset the inventory
               resetElf(false);
             }
           }
       
           game_state = GAME_PLAYING;
           start_game();
         }
             
         //next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
         update_sound();
       }  
     break;
     
  case GAME_PLAYING:
       if( gb.update() )
       //if (TV.millis() >= next_action)
       {      
         //player1.update();

         if (gb.buttons.repeat(BTN_UP,1)) moveElf(FACING_UP);
         if (gb.buttons.repeat(BTN_DOWN,1)) moveElf(FACING_DOWN);
         if (gb.buttons.repeat(BTN_RIGHT,1)) moveElf(FACING_RIGHT);
         if (gb.buttons.repeat(BTN_LEFT,1)) moveElf(FACING_LEFT);
         if (gb.buttons.pressed(BTN_A)) throwSword();
         //if (gb.buttons.pressed(BTN_B)) game_state = GAME_PAUSED; //Remove pause state because menu works better

         if (gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Press \25 to resume."),title_bitmap_unified);
          gb.display.clear();
          gb.display.persistence = true;
          //For map tiles (not calling function because it loads room elements
          for (char y=0;y<6; y++)
          {
            for (char x=0;x<10; x++)
            {
              //determine the current block before trying to draw
              int curr_block = getMapBlock(x,y,-1);
              gb.display.drawBitmap(x * 8, y * 8, map_bitmap + (curr_block * SIZEOF_MAP_RECORD));
            }
          }
          updateDisplay(getElf()); //For hearts and items
          //For elf
          overlaybitmap(getElf().x, getElf().y, elf_bitmap + ( getElf().facing * SIZEOF_ELF_RECORD));
          overlaybitmap(getElf().x, getElf().y+8, elf_bitmap + ((getElf().facing + getElf().step) * SIZEOF_ELF_RECORD)); 
         }
         
         //if (player1.joy_up()) moveElf(FACING_UP);
         //if (player1.joy_down()) moveElf(FACING_DOWN);
         //if (player1.joy_right()) moveElf(FACING_RIGHT);
         //if (player1.joy_left()) moveElf(FACING_LEFT);
         //if (player1.button_z()) throwSword();
         //if (player1.button_c()) game_state = GAME_PAUSED;
    
         //next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
  
         handleRoomElements();
     
     //check the elf's state
     if (getElfState() != ELFSTATE_PLAYING)
     {
       if (getElfState() == ELFSTATE_DEAD)
       {
         start_game_over();
       } else {
         start_game_won();
       }
     }
      update_sound();
       }
       
       break;
       
   case GAME_PAUSED:
       if( gb.update() )
       //if (TV.millis() >= next_action)
       {
          gb.display.print("Paused");
         //player1.update();

         if (gb.buttons.pressed(BTN_B)) game_state = GAME_PLAYING;
         //if (player1.button_c()) game_state = GAME_PLAYING;
         
         //next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
       }
       break;
  }
}
