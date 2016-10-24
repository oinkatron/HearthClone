#include "CharacterBank.h"

string CharacterBank::path = "res/";

CharacterBank::CharacterBank()
{
   next_id = 0;
}

CharacterBank::CharacterBank(string f_name)
{
   loadFromFile(f_name);
}

CharacterBank::~CharacterBank()
{

}

bool CharacterBank::saveToFile(string f_name)
{
   ofstream o_file((path + f_name).c_str());
   if (!o_file.is_open())
   {
      clog << "Error opening file: " << f_name << endl;
      return false;
   }
   
   int t_len;
   
   for (int i = 0; i < char_bank.size(); i++)
   {
      o_file.write((char*)&char_bank[i]->u_id, sizeof(int));
      o_file.write((char*)&char_bank[i]->port_id, sizeof(int));
      o_file.write((char*)&char_bank[i]->atk, sizeof(int));
      o_file.write((char*)&char_bank[i]->def, sizeof(int));
      
      t_len = char_bank[i]->title.length();
      o_file.write((char*)&t_len, sizeof(int));
      o_file.write(char_bank[i]->title.c_str(), t_len);
      
      if (o_file.bad())
      {
         clog << "Error writing to CharacterBank: " << f_name << endl;
         o_file.close();
         return false;
      }
   }

   o_file.close();
   return true;
}

bool CharacterBank::loadFromFile(string f_name)
{
   ifstream i_file((path+f_name).c_str());
   if (!i_file.is_open())
   {
      clog << "Error opening: " << f_name << endl; 
      return false;
   }
   
   char *t_buff;
   int t_len;
   CharacterBase *cb;
   
   while (!i_file.eof())
   {
      cb = new CharacterBase;
      i_file.read((char*)&cb->u_id, sizeof(int));
      i_file.read((char*)&cb->port_id, sizeof(int));
      i_file.read((char*)&cb->atk, sizeof(int));
      i_file.read((char*)&cb->def, sizeof(int));
      
      i_file.read((char*)&t_len, sizeof(int));
      
      t_buff = new char[t_len+1];
      i_file.read(t_buff, t_len);
      t_buff[t_len] = '\0';
      
      cb->title = string(t_buff);
      delete t_buff;
      
      if (i_file.good())
      {
         char_bank.push_back(cb);
         clog << "Loaded Character: " << cb->title << endl;
      }
      else if (i_file.bad())
      {
         clog << "Error loading CharacterBank: " << f_name << endl;
         delete cb;
         i_file.close();
         char_bank.clear();
         return false;
      }
   }
   
   next_id = char_bank.size();
   i_file.close();
   
   return true;   
}

CharacterBase *CharacterBank::getCharBase(int u_id)
{
   for (int i = 0; i < char_bank.size(); i++)
   {
      if (char_bank[i]->u_id == u_id)
      {
         return char_bank[i];
      }
   }
   
   return NULL;
}

CharacterBase *CharacterBank::getCharBase(string title)
{
   for (int i = 0; i < char_bank.size(); i++)
   {
      if (char_bank[i]->title == title)
      {
         return char_bank[i];
      }
   }
   
   return NULL;
}

bool CharacterBank::createCharacterBase(int port_id, int atk, int def, string title)
{
   CharacterBase *cb = new CharacterBase;
   next_id++;
   
   cb->u_id = next_id;
   cb->port_id = port_id;
   cb->atk = atk;
   cb->def = def;
   cb->title = title;
   
   char_bank.push_back(cb);
   
   return true;
}
