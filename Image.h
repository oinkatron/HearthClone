#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Image
{
    public:
        Image();
        virtual ~Image();

        int getW() { return raw->w; }
        int getH() { return raw->h; }

        static Image* getImage(string name);
        static Image* getImage(int uid);
        static bool loadImage(string filename);

        //Getters & Setters
        SDL_Surface *getRaw() { return raw; }
        void setRaw (SDL_Surface *val) { raw = val; }
        void setRaw(string filename);
        
        string resName() { return res_name; }
        void resName(string s) { res_name = s; }
        
        int uID() { return u_id; }

        string getFileName() { return f_name; }
        void setFileName(string val) { f_name = val; }

        //Public vars
        //static int num_images;
        static int next_id;
        static string path;
        static vector<Image*> image_bank;

    protected:
    private:
        SDL_Surface *raw;
        string f_name;
        string res_name;
        int u_id;
};

#endif // IMAGE_H
