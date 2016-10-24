#ifndef CHARACTERBANK_H
#define CHARACTERBANK_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std; 

struct CharacterBase {
   int u_id;
   int port_id;
   int atk;
   int def;
   string title;
};

class CharacterBank {
      public:
         CharacterBank();
         CharacterBank(string f_name);
         ~CharacterBank();
         
         static string path;
         
         bool saveToFile(string f_name);
         bool loadFromFile(string f_name);
         
         bool createCharacterBase(int port_id, int atk, int def, string title);
         
         CharacterBase *getCharBase(int u_id);
         CharacterBase *getCharBase(string title);
         
      protected:
      private:
         int next_id;
         vector<CharacterBase*> char_bank;
         
         
};

#endif
