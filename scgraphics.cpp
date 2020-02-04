


#include "scgraphics.h"
#include "font8x8_basic.h"

using namespace std;


#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
///extern font8x8_basic[];


    mycolor TheColor[10] = {
                                    {255,255,255},  //white 0
                                    {0,0,0},        //black 1
                                    {255,0,0},      //red 2
                                    {0,255,0},      //green 3
                                    {0,0,255},      //blue 4
                                    {255,255,0},    //yellow 5
                                    {255,0,255},    //magenta 6
                                    {200,200,200},  //greyish 7
                                    {100,200,0},    // 8
                                    {50,150,255}    // 9
                                };

mygraphics::mygraphics(uint16_t x, uint16_t y, uint16_t w, uint16_t h): WIDTH(w), HEIGHT(h)
{
    _width    = WIDTH;
    _height   = HEIGHT;
    position_x  = position_y = 0;
    fontColor = 0x000;
    bgColor = 0xffff;
    fontName = NULL;
    fc = TheColor[yellow];
    bgc = TheColor[magenta];
    initbuff( "/dev/fb0");
    drawbuffer(0,0,100, 500, magenta);
    this->fontsize = 16;
    const char* usefont = "FreeSerifItalic.ttf";
    ///const char* usefont = "FreeSerif.ttf";
    ///const char* usefont = "FreeSerifBoldItalic.ttf";
    this->face = NULL;
    initfreetype(usefont);
    this->fontName=usefont;
}


mygraphics::mygraphics(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* ft): WIDTH(w), HEIGHT(h)
{
    _width    = WIDTH;
    _height   = HEIGHT;
    position_x  = position_y = 0;
    fontColor = 0x000;
    bgColor = 0xffff;
    fontName = ft;
    fc = TheColor[yellow];
    bgc = TheColor[magenta];
    initbuff( "/dev/fb0");
    drawbuffer(0,0, 800, 480, magenta);
    this->fontsize = 16;
    ///const char* usefont = "FreeSerifItalic.ttf";
    ///const char* usefont = "FreeSerif.ttf";
    ///const char* usefont = "FreeSerifBoldItalic.ttf";
    this->face = NULL;
    initfreetype(ft);

}

mygraphics::~mygraphics()
{
    munmap(this->fbp, this->screensize);
    close(this->fbfd);
    FT_Done_FreeType(this->library);
    printf("destructor done\n");
}

int mygraphics::initbuff(const char* bfile )
{
    this->fbfd = open(bfile, O_RDWR);
    if (fbfd == -1)
    {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    /// Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        perror("Error reading fixed information");
        exit(2);
    }

    /// Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        perror("Error reading variable information");
        exit(3);
    }
    printf("%dx%d, %dbpp,%dbytes\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.line_length);
    /// 1680x1050 32bpp 6720 bytes

    /// Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    cout<<"screensize: "<<screensize<<endl;
    /// screensize: 7056000

    /// Map the device to memory
    this->fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((int)fbp == -1)
    {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    cout<<"The framebuffer device was mapped to memory successfully"<<endl;
    return 0;

/// ================frame buffer set up complete ==========================

}

///===========================
/// this writes the color directly into the frame buffer memory
///===========================
void mygraphics::drawbuffer(uint16_t x, uint16_t y, uint16_t w, uint16_t h, enum colors buffColor)
{
    int current_y=100; ///force to value
    uint16_t cred = 0;
    uint16_t cgreen = 0;
    uint16_t cblue = 0;

    for (h = 100; h < 500; h++)
    {
        if(h>current_y)
        {
            ///int v1 = rand() % 10;
            cred = TheColor[magenta].red;
            cgreen = TheColor[magenta].green;
            cblue = TheColor[magenta].blue;
            current_y = current_y+50;
        }

        for (w = 100; w < 1400; w++) /// 600
        {
            location = (w + vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (h + vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32)
                {
                *(fbp + location) = cblue;
                *(fbp + location + 1) = cgreen;
                *(fbp + location + 2) = cred;
                *(fbp + location + 3) = 0;                  /// No transparency
                }
        }
    }
}

void mygraphics::putpixel(int x, int y, enum colors sdc)
{
    uint16_t red = TheColor[sdc].red;
    uint16_t green = TheColor[sdc].green;
    uint16_t blue = TheColor[sdc].blue;

    location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y + vinfo.yoffset) * finfo.line_length;

    if (vinfo.bits_per_pixel == 32)
        {
            *(fbp + location) = blue;
            *(fbp + location + 1) = green;
            *(fbp + location + 2) = red;
            *(fbp + location + 3) = 0;                  /// No transparency
        }
}

void mygraphics::drawChar(int16_t x, int16_t y, unsigned char c, enum colors sdc)
{
    for(int8_t j=0; j<8; j++ )
        {
            uint8_t f_byte = font8x8_basic[c][j]; /// get the byte
            ///uint8_t f_byte = font8x8_hiragana[c][j]; /// get the byte
            ///cout<<"f_byte byte "<<(int)f_byte<<endl;

            for(int8_t i=0; i<8; i++, f_byte >>= 1) // >>=
            {
                if(f_byte & 1)        // &1
                {
                    putpixel(x+i, y+j, sdc);
                }

            }
        }
}


int mygraphics::initfreetype(const char* fontname)
{
    int err=0;
    ///const char* thenname = "FreeSerifItalic.ttf";
/// ==== Set up freetype font rasterizer =========================
/// ======================================================
/// STEP ONE ============================== Init FreeType lib, load font ======
	if((err = FT_Init_FreeType(&library)))
    {
		fprintf(stderr, "FreeType init error: %d", err);
		return err;
	}
/// STEP TWO ============================ set the new face name data ======
	if((err = FT_New_Face(library, fontname, 0, &face)))
    {
		fprintf(stderr, "Font load error: %d", err);
		FT_Done_FreeType(library);
		return err;
	}

/// STEP THREE =========================== Set a face size ========

    /// << 6 because '26dot6' fixed-point format
	///FT_Set_Char_Size(face, size << 6, 0, DPI, 0);
	///FT_Set_Char_Size(face, 16*64, 0, 300, 0);

///set a size BEFORE loading a glyph
    ///if(err=FT_Set_Char_Size(face, 16*64, 0, 300, 0))

    if((err= FT_Set_Pixel_Sizes(face, 0,fontsize)))
    {
		fprintf(stderr, "pixel size error: %d", err);
		FT_Done_FreeType(library);
		return err;
	}


	bitmap = &face->glyph->bitmap;
    FT_GlyphSlot  slot = face->glyph;  /// a small shortcut


    return 0;
}

const char* mygraphics::getfontname() const
{
    return this->fontName;

}

void mygraphics::textWriteff(const char* buffer, uint16_t len, int16_t x, int16_t y,enum colors sdc)
{
  if (len == 0)
    len = strlen(buffer);


  for (uint16_t i=0;i<len;i++)
    {

        if(buffer[i] == '\n')
        {
            x=x+this->fontsize/4;
        }
        drawCharff(x,y, buffer[i], sdc);

        int H_advance = (face->glyph->metrics.horiAdvance)/64;
        int X_Bear = (face->glyph->metrics.horiBearingX)/64;

 /*************************************
        cout<<"1 bitmap horiz advance: "<<(face->glyph->metrics.horiAdvance)/64<<endl;
        cout<<"2 bitmap bearing x: "<<(face->glyph->metrics.horiBearingX)/64<<endl;
        cout<<"3 Width "<<face->glyph->bitmap.width<<endl;
        ///cout<<"4 rows "<<face->glyph->bitmap.rows<<endl;
        cout<<"5 char "<<buffer[i]<<endl;
        ///x=x+120;
        ///x += face->glyph->advance.x >> 6;
******************************************************/

        x=x+H_advance+X_Bear; ///+(face->glyph->metrics.width/64);face->glyph->bitmap.width

        //cout<<"H adv "<<H_advance<<end;
        //cout<<"X bear "<<X_Bear<<endl;
    }
}
int mygraphics::drawCharff(int16_t x, int16_t y, unsigned char c, enum colors sdc)
{
    int err;

    FT_UInt glyph_index = FT_Get_Char_Index(face, c);
	if((err=FT_Load_Glyph(face, glyph_index,FT_LOAD_DEFAULT)))
    {
		fprintf(stderr, "FT_Load_Glyph error: %d", err);
		FT_Done_FreeType(library);
        return err;
    }

    if(err=FT_Render_Glyph(face->glyph,FT_RENDER_MODE_MONO))
    {
		fprintf(stderr, "render error: %d", err);
		FT_Done_FreeType(library);
        return err;
    }

    if((err=FT_Get_Glyph(face->glyph, &glyph)))
    {
		fprintf(stderr, "FT_Get_Glyph error: %d", err);
		FT_Done_FreeType(library);
        return err;
    }



    int font_pitch = bitmap->pitch;

 /*********************************
    cout<<endl;
    cout<<"1 char is: "<<c<<endl;
    cout<<"2 bitmap horiz advance: "<<(face->glyph->metrics.horiAdvance)/64<<endl;
    cout<<"2a bitmap horiz advance: "<<(face->glyph->advance.x)/64<<endl;
    cout<<"3 bitmap bearing x: "<<(face->glyph->metrics.horiBearingX)/64<<endl;
    cout<<"4 Width BMap "<<face->glyph->bitmap.width<<endl;
    cout<<"4a Width BB "<<(face->glyph->metrics.width/64)<<endl;
 ***********************************/


    int H_advance = (face->glyph->metrics.horiAdvance)/64;
    int X_Bear = (face->glyph->metrics.horiBearingX)/64;


    for(uint8_t i=0; i<face->glyph->bitmap.rows; i++)
        {
            uint8_t lineupper = bitmap->buffer[(i*font_pitch)];
            uint8_t linelower = bitmap->buffer[(i*font_pitch)+1];
            uint16_t line = (lineupper <<8) | linelower;

            for(int8_t j=0; j<face->glyph->bitmap.width; j++) /// for(int8_t j=0; j<8; j++, line >>= 1)
                {
                    if(line & 0x8000) ///was 1
                        {
                        /// cout<<"X";
                        putpixel(j+x, (210+y)-face->glyph->bitmap_top, sdc);
                        /// x= x+face->glyph->bitmap.width
                        }
                        else
                        {
                            int y=0; ///cout<<" ";

                        }
                        line <<=1;

                }
                ///cout<<endl;

                y=y+1;
        }
    return 0;

}
void mygraphics::setfont(const char* thefont)
{
    assert(thefont!=NULL);

    this->fontName = thefont;
    initfreetype(thefont);
}
