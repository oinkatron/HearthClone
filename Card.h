#ifndef CARD_H
#define CARD_H

#include "Image.h"

     
class Card {
   public:
      Card();
      ~Card();
      
      static int title_center_x, title_center_y;
      static int cost_center_x, cost_center_y;
      static int atk_center_x, atk_center_y;
      static int def_center_x, def_center_y;
      static int portrait_off_x, portrait_off_y;
      
      void setPlayAction(void (*val)(void*)) { onPlay = val; }      
      
      Image *portraitImage() { return portrait; }
      void portraitImage(Image* val) { portrait = val; }
      
      Image *cardFaceImage() { return card_face; }
      void cardFaceImage(Image *val) { card_face = val; }
      
      string cardTitle() { return title; }
      void cardTitle(string val) { title = val; }
      
      int cardCost() { return cost; }
      void cardCost(int val) { cost = val; }
      
   protected:
   private:
      Image *portrait;
      Image *card_face;
      
      string title;
      int cost;
      
      void (*onDraw)(void*);
      void (*onPlay)(void*);
      void (*onDiscard)(void*);
};

#endif
