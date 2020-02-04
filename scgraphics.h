#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <cstdio>
#include <ctype.h>
#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>   /// for mmap() and munmap()
#include <sys/ioctl.h>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <cassert>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


enum colors{white, black, red, green, blue, yellow, magenta, grey, pink,other};


struct mycolor
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

extern mycolor TheColor[10];



class mygraphics
{
public:
    mygraphics(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    mygraphics(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* ft);
    ~mygraphics();
    int16_t getheight(void) const;
    int16_t getwidth(void) const;
    int16_t getPositionX(void) const;
    int16_t getPositionY(void) const;
    void putpixel(int x, int y, enum colors sdc);
    void drawChar(int16_t x, int16_t y, unsigned char c, enum colors sdc);
    int drawCharff(int16_t x, int16_t y, unsigned char c, enum colors sdc);
    void textWriteff(const char* buffer, uint16_t len, int16_t x, int16_t y, enum colors sdc);
    void textWrite(const char* buffer, uint16_t len, int16_t x, int16_t y, enum colors sdc);
    void drawbuffer(uint16_t x, uint16_t y, uint16_t w, uint16_t h, enum colors buffColor);
    const char* getfontname() const;
    void setfont(const char*);


private:
    uint16_t fontColor, bgColor;
    uint16_t _width, _height;
    uint16_t position_x, position_y;
    const uint16_t WIDTH, HEIGHT;
    const char* fontName;
    FT_UInt fontsize;
    mycolor fc,bgc;
/// ====== frame buffer items
    int initbuff(const char* bfile);
    int fbfd;
    long int screensize;
    char *fbp;
    long int location;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
/// ====== free type items
    int initfreetype(const char*);
    FT_Library library;
	FT_Face face;
	FT_Glyph  glyph;
	FT_Bitmap  *bitmap;
	FT_BitmapGlyphRec *g;
	int font_pitch;

};
#endif
