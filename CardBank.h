#ifndef CARDBANK_H
#define CARDBANK_H

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct CardBase {
   int u_id;
   int m_id;
   int port_id;
   int cost;
   string title;
};


class CardBank {
  public:
     CardBank();
     CardBank(string f_name);
     ~CardBank();
     
     static string path;
     
     bool saveToFile(string f_name);
     bool loadFromFile(string f_name);
     bool createCardBase(int m_id, int port_id, int cost, string title);
     
     CardBase *getBase(int u_id);
     CardBase *getBase(string title);
     
  private:
     vector<CardBase*> card_bank;
     int next_id;
     
};

#endif
