#include <cstdio>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include <sstream>
#include <list>

#include "Image.h"
#include "CardBank.h"
#include "Card.h"
#include "CharacterBank.h"
#include "Character.h"

using namespace std;

enum SelectMode {
   FRIEND_HAND=0,
   FRIEND_BOARD,
   ENEMY_BOARD
};

SelectMode sel_mode = FRIEND_HAND;
int sel_pos = 0;

//SDL VARS
SDL_Surface *screen = NULL;
SDL_Event event;

//SCREEN SETTINGS
int scr_w = 1024;
int scr_h = 768;
int scr_bpp = 32;
int scr_flags = (SDL_HWSURFACE | SDL_DOUBLEBUF);

//TMP CONSTS
int CARD_WIDTH = 254;
int CARD_HEIGHT = 416;

template <class V>
string toString(V val)
{
   stringstream ss;
   ss << val;
   return ss.str();
}

//Renderer Settings
TTF_Font *game_font;

bool finished = false;

//GLOBAL IMAGES
Image *card_face = NULL;
Image *background_image = NULL;
Image *char_face = NULL;

//Player stuff
list<Card*> friendly_hand;
list<int> friendly_deck;

list<Card*> enemy_hand;
list<int> enemy_deck;

//TIMING VARS
long strt_time = 0, last_time = 0, frame_time =0;
const int MAX_FPS = 60;

//system functions
bool init();
void onUpdate();
void onRender();

//RENDERER FUCNTIONS
void drawString(TTF_Font *f, string msg, int x, int y, SDL_Color c);
void drawImage(Image *i, int x, int y, SDL_Rect *clip=NULL);
void drawCard(Card *c, int x, int y);
void drawCharacter(Character *ch, int x, int y);

//CARD FUNCIONS
Card *cardFromBase(CardBase *cb);
Character *charFromBase(CharacterBase *cb);

int main(int argn, char *args[])
{
   finished = init();
   
   TTF_Font *t_f = NULL;
   game_font = TTF_OpenFont("main.ttf", 16);
   t_f = TTF_OpenFont("main.ttf", 8);
   
   Image::loadImage("background.png");
   Image::loadImage("cardFace01.png");
   Image::loadImage("minionPortrait01.png");
   Image::loadImage("minionPortrait02.png");
   Image::loadImage("charBorder01.png");
   Image::loadImage("playerBorder01.png");
   Image::loadImage("playerPort01.png");
   Image::loadImage("playerPort02.png");
   Image::loadImage("healthBorder01.png");
   Image::loadImage("curser.png");
   
   card_face = Image::getImage("cardFace01");
   background_image = Image::getImage("background");
   char_face = Image::getImage("charBorder01");
   
   if (!background_image)
   {
      clog << "Error loading Image: background" << endl;
      return 1;
   }
   
   CardBank *c_b = new CardBank("cbank01.cb");
   CharacterBank *char_b = new CharacterBank("charBank01.chb");
   
   friendly_hand.push_back(cardFromBase(c_b->getBase(1)));
   friendly_hand.push_back(cardFromBase(c_b->getBase(1)));
   friendly_hand.push_back(cardFromBase(c_b->getBase(2)));
   
   enemy_hand.push_back(cardFromBase(c_b->getBase(0)));
   
   while (!finished)
   {
      last_time = strt_time;
      strt_time = SDL_GetTicks();
      frame_time = strt_time - last_time;
   
      while (SDL_PollEvent(&event))
      {
         switch (event.type)
         {
            case SDL_QUIT:
            {
               finished = true;
               break;
            }
            case SDL_MOUSEMOTION:
            {
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                   case SDLK_LEFT:
                        switch (sel_mode)
                        {
                            case FRIEND_HAND:
                               sel_pos--;
                               if (sel_pos < 0)
                               {
                                   sel_pos = friendly_hand.size()-1;
                               }
                               break;
                            case FRIEND_BOARD:
                               break;
                            case ENEMY_BOARD:
                               break;
                        }
                        
                        break;
                   case SDLK_RIGHT:
                        switch (sel_mode)
                        {
                            case FRIEND_HAND:
                               sel_pos++;
                               if (sel_pos == friendly_hand.size())
                               {
                                   sel_pos = 0;
                               }
                               break;
                            case FRIEND_BOARD:
                               break;
                            case ENEMY_BOARD:
                               break;
                        }
                        break;
                   case SDLK_UP:
                        switch (sel_mode)
                        {
                            case FRIEND_HAND:
                               sel_mode = FRIEND_BOARD;
                               if (sel_pos == friendly_hand.size())
                               {
                                   sel_pos = 0;
                               }
                               break;
                            case FRIEND_BOARD:
                               sel_mode = ENEMY_BOARD;
                               break;
                            case ENEMY_BOARD:
                               sel_mode = FRIEND_HAND;
                               break;
                        }
                        break;
                   case SDLK_DOWN:
                        switch (sel_mode)
                        {
                            case FRIEND_HAND:
                               sel_mode = ENEMY_BOARD;
                               if (sel_pos == friendly_hand.size())
                               {
                                   sel_pos = 0;
                               }
                               break;
                            case FRIEND_BOARD:
                               sel_mode = FRIEND_HAND;
                               break;
                            case ENEMY_BOARD:
                               sel_mode = FRIEND_BOARD;
                               break;
                        }
                        break;
                   case SDLK_SPACE:
                        switch(sel_mode)
                        {
                           case FRIEND_HAND:
                                if (!friendly_hand.empty())
                                {
                                   list<Card*>::iterator it = friendly_hand.begin();
                                   advance(it, sel_pos);
                                   friendly_hand.erase(it);
                                   if (sel_pos == friendly_hand.size())
                                   {
                                      sel_pos -= 1;
                                   }
                                }
                                break;
                        }
                }
                break; 
            }
         }
      }
      
      onUpdate();
      onRender();
      
      if (SDL_GetTicks() - strt_time < 1000/MAX_FPS)
      {
         SDL_Delay((1000/MAX_FPS) - (SDL_GetTicks() - strt_time));
      } 
   }
   
   return 0;
}

bool init()
{
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      clog << "Error initing SDL :(\n";
      return true;
   }
   
   if (TTF_Init() < 0)
   {
      clog << "Error initing SDL_ttf :(\n";
      return true;
   }
   
   screen = SDL_SetVideoMode(scr_w, scr_h, scr_bpp, scr_flags);
   if (!screen)
   {
      clog << "Error setting video mode D:\n";
      return true;
   }
   
   return false;
}

void onUpdate()
{
     
}

void onRender()
{
     //draw Background
     //SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));
     drawImage(background_image, 0, 0);
     
     //Draw Player and Enemy Hands
     if (!friendly_hand.empty())
     {
         int strt_x = (scr_w/2) - ((CARD_WIDTH*friendly_hand.size())/2);
         int cnt = 0;
         for (list<Card*>::iterator it = friendly_hand.begin(); it != friendly_hand.end(); ++it)
         {
            if (sel_mode == FRIEND_HAND && sel_pos == cnt)
            {
               drawCard(*it, strt_x + (CARD_WIDTH*cnt++), 700 - CARD_HEIGHT);
            }
            else 
            {
               drawCard(*it, strt_x + (CARD_WIDTH*cnt++), 700);
            }
            
         }
             
     }
     
     if (!enemy_hand.empty())
     {
        int start_x = (scr_w/2) - ((CARD_WIDTH*enemy_hand.size())/2);
        int cnt = 0;
        for (list<Card*>::iterator it = enemy_hand.begin(); it != enemy_hand.end(); ++it)
        {
           drawCard(*it, start_x + (CARD_WIDTH*cnt++), 68 - CARD_HEIGHT);
        }
     }
     
      //Draw Player Icons, Borders and HP
      drawImage(Image::getImage("healthBorder01"), 68, 376);
      
      SDL_Rect tmp;
      for (int i = 0; i < 30; i++)
      {
          tmp.x = 71;
          tmp.y = 495 - (i*4);
          tmp.w = 26;
          tmp.h = 2;
          SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 228, 228, 288));
      }
      
      drawImage(Image::getImage("playerPort01"), 68, 506);
      drawImage(Image::getImage("playerBorder01"), 64, 500);
      
      drawImage(Image::getImage("healthBorder01"), 924, 268);
      
      for (int i = 0; i < 30; i++)
      {
         tmp.x = 927;
         tmp.y = 272 + (i * 4);
         tmp.w = 26;
         tmp.h = 2;
         
         SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 228, 228, 288));
      }
      
      drawImage(Image::getImage("playerPort02"), 892, 200);
      drawImage(Image::getImage("playerBorder01"), 888, 196);
  
      //Draw Cursor
      switch (sel_mode)
      {
         case FRIEND_HAND:
            if (!friendly_hand.empty())
            {
               drawImage(Image::getImage("curser"), (scr_w/2)-((CARD_WIDTH*friendly_hand.size())/2) + (sel_pos*CARD_WIDTH) + (CARD_WIDTH/2), 612 - CARD_HEIGHT);
            }
      }
  
      SDL_Flip(screen);
}

void drawImage(Image *i, int x, int y, SDL_Rect *clip)
{
   if (!i || !screen)
      return;
   
   SDL_Rect r;
   r.x = x;
   r.y = y;
   r.w = 0;
   r.h = 0;
   
   SDL_BlitSurface(i->getRaw(), clip, screen, &r);
}

void drawCard(Card *c, int x, int y)
{
   drawImage(c->cardFaceImage(), x, y);
   drawImage(c->portraitImage(), x + Card::portrait_off_x, y + Card::portrait_off_y);
   
   int w, h;
   
   TTF_SizeText(game_font, c->cardTitle().c_str(), &w, &h);
   drawString(game_font, c->cardTitle(), (Card::title_center_x - (w/2)) + x, (Card::title_center_y  - (h/2))+ y, (SDL_Color){0, 0, 0, 255});
   
   TTF_SizeText(game_font, toString(c->cardCost()).c_str(), &w, &h);
   drawString(game_font, toString(c->cardCost()), (Card::cost_center_x - (w/2)) + x, (Card::cost_center_y - (h/2)) + y, (SDL_Color){0, 255, 255, 255});
}

void drawString(TTF_Font *f, string txt, int x, int y, SDL_Color c)
{
     if (!f || !screen)
        return;
     
     SDL_Surface *msg = NULL;
     SDL_Rect r;
     
     msg = TTF_RenderText_Blended(f, txt.c_str(), c);
     if (!msg)
        return;
     
     r.x = x;
     r.y = y;
     r.w = msg->w;
     r.h = msg->h;
     
     SDL_BlitSurface(msg, NULL, screen, &r);
     SDL_FreeSurface(msg);

     return;
}

void drawCharacter(Character *c, int x, int y)
{
   if (!c)
      return;
      
   drawImage(c->portraitImage(), x, y);
   drawImage(char_face, x-12, y-16);
   
}

Card *cardFromBase(CardBase *cb)
{
   Card *new_c = new Card;
   new_c->cardFaceImage(card_face);
   if (!cb)
   {
      new_c->cardTitle("Err Card");
      new_c->cardCost(0);
   }
   else
   {
      new_c->portraitImage(Image::getImage(cb->port_id));
      new_c->cardTitle(cb->title);
      new_c->cardCost(cb->cost);
   }
   
   return new_c;
}

Character *charFromBase(CharacterBase *cb)
{
   Character *c = new Character;
   
   if (cb)
   {
       c->portraitImage(Image::getImage(cb->port_id));
       c->charAtk(cb->atk);
       c->charDef(cb->def);
       c->charTitle(cb->title);
   }
   else
   {
       c->charAtk(0);
       c->charDef(0);
       c->charTitle("Errnion");
   }
       
   return c;
}
