#include "Cardbank.h"

string CardBank::path = "res/";

CardBank::CardBank()
{
   next_id = 0;
}

CardBank::CardBank(string f_name)
{
   next_id = 0;
   loadFromFile(f_name);
}

CardBank::~CardBank()
{

}

bool CardBank::saveToFile(string f_name)
{
   ofstream o_file((path+f_name).c_str());
   if (!o_file.is_open())
   {
      clog << "Error Opening: " << f_name << endl;
      return false;
   }
   
   for (int i = 0; i < card_bank.size(); i++)
   {
      o_file.write((char*)(&card_bank[i]->u_id), sizeof(int));
      o_file.write((char*)&card_bank[i]->m_id, sizeof(int));
      o_file.write((char*)&card_bank[i]->port_id, sizeof(int));
      o_file.write((char*)&card_bank[i]->cost, sizeof(int));
      
      int t_size = card_bank[i]->title.length();
      o_file.write((char*)&t_size, sizeof(int));
      o_file.write(card_bank[i]->title.c_str(), t_size);
      
      if (o_file.bad())
      {
         clog << "Error Writing to CardBank: " << f_name << endl;
         o_file.close();
         return false;
      }
   }
   
   return true;
}

bool CardBank::loadFromFile(string f_name)
{
   ifstream i_file((path+f_name).c_str());
   if (!i_file.is_open())
   {
      clog << "Error Opening: " << f_name << endl;
      return false;
   }
   
   CardBase *bse;
   char *tmp_text;
   int t_size;
   
   while (!i_file.eof())
   {
       bse = new CardBase;
       i_file.read((char*)&bse->u_id, sizeof(int));
       i_file.read((char*)&bse->m_id, sizeof(int));
       i_file.read((char*)&bse->port_id, sizeof(int));
       i_file.read((char*)&bse->cost, sizeof(int));
       
       i_file.read((char*)&t_size, sizeof(int));
       tmp_text = new char[t_size+1];
       
       i_file.read(tmp_text, t_size);
       tmp_text[t_size] = '\0';
       bse->title = string(tmp_text);
       delete tmp_text;
       
       if (i_file.good())
       {
          card_bank.push_back(bse);
          clog << "Loaded Card: " << bse->title << endl;
       }
       else if (i_file.bad())
       {
          clog << "Error Loading card bank: " << f_name << endl;
          delete bse;        
          card_bank.clear();
          i_file.close();
          return false;
       }
   }
   
   next_id = card_bank.size();
   i_file.close();
   
   return true;
}

bool CardBank::createCardBase(int m_id, int port_id, int cost, string title)
{
    CardBase *b = new CardBase;
    next_id++;
    b->u_id = next_id;
    b->m_id = m_id;
    b->port_id = port_id;
    b->cost = cost;
    b->title = title;
    
    card_bank.push_back(b);
    return true;
}

CardBase *CardBank::getBase(int u_id)
{
   for (int i = 0; i < card_bank.size(); i++)
   {
      if (card_bank[i]->u_id == u_id)
         return card_bank[i];
   }
   
   return NULL;
}

CardBase *CardBank::getBase(string title)
{
   for (int i = 0;i < card_bank.size(); i++)
   {
      if (card_bank[i]->title == title)
         return card_bank[i];
   }
   
   return NULL;
}
