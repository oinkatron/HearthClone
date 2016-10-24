#include "Card.h"

int Card::title_center_x = 126;
int Card::title_center_y = 170;
int Card::cost_center_x = 126;
int Card::cost_center_y = 16;
int Card::atk_center_x = 22;
int Card::atk_center_y = 377;
int Card::def_center_x = 220;
int Card::def_center_y = 377;
int Card::portrait_off_x = 18;
int Card::portrait_off_y = 16;

Card::Card()
{
   portrait = NULL;
   card_face = NULL;
   
   title = "";
   
   cost = 0;
   
   onDraw=NULL;
   onPlay=NULL;
   onDiscard=NULL;
}

Card::~Card()
{

}
