#ifndef CHARACTER_H
#define CHARACTER_H

#include "Image.h"

class Character
{
   public:
      Character();
      ~Character();
      
      Image *portraitImage() { return portrait; }
      void portraitImage(Image *val) { portrait = val; }
      
      string charTitle() { return title; }
      void charTitle(string val) { title = val; }
      
      int charAtk() { return atk; }
      void charAtk(int val) { atk = val; }
      
      int charDef() { return def; }
      void charDef(int val) { def = val; } 
      
   protected:
   private:
      Image *portrait;
      string title;
      int atk;
      int def;
      
};

#endif
