
#include <iostream>
#include <sstream>
#include <chrono>		//chrono::milliseconds(1000); need scope chrono
#include <thread>		// for chrono sleep this_thread::sleep_for(chrono::seconds(1));
#include "scgraphics.h"
#include "mycurl.h"


using namespace std;
using namespace std::chrono;

static unsigned long interval = 1000;
static unsigned long previousMillis = 0;

ostream& operator<<(ostream& os, const mywx& wx)
{
    os << wx.temp << '/' << wx.humid << '/' << wx.alt << endl;
    return os;
}


int main()
{
    cout << "Hello world!" << endl;
    mywx testforcurl;


    mygraphics sctest(0,0,23,230, "FreeSerifBoldItalic.ttf");
    ///mygraphics sctest(23,230, "FreeSerifItalic.ttf");
    ///mygraphics sctest(23,230, "FreeMonoOblique.ttf"); /// mono font
    /// mygraphics sctest(0,0,23,230, "FreeMonoBoldOblique.ttf"); /// mono font
    ///mygraphics sctest(23,230, "FreeMonoBold.ttf");

    sctest.drawChar(250, 259, '@', yellow );

    for(int i=0; i<(126-32); i++)
    {
        sctest.drawChar(200+(8*i), 280, (0x21+i), blue);
    }

    const char* thename = sctest.getfontname();
    cout<<"font name "<<thename<<endl;

    const char* myname ="sdc SDC Steven D Clark";
    const char* fontrun="**TWrun Hello from Steve Clark, 3745 BG ";
    sctest.textWriteff(myname, 0, 250, 270, yellow);
    ///sctest.setfont("FreeSerifBoldItalic.ttf");
    ///sctest.textWriteff(myname, 0, 250, 250, yellow);

    mycurl(testforcurl);

    /// sctest.setfont("FreeSerifBoldItalic.ttf");
    ostringstream oss;
    oss << "Temp is: " << testforcurl.temp;

    /// cout<<testforcurl;
    sctest.textWriteff(testforcurl.datetime, 0, 250, 100, yellow);
    sctest.textWriteff(oss.str().c_str(), 0, 250, 115, yellow);

    oss.str("");    /// reset & clear stringstream
    oss.clear();
    oss << "Humidity is: " << testforcurl.humid;
    sctest.textWriteff(oss.str().c_str(), 0, 250, 130, yellow);
    sctest.textWriteff("Temp as: 65.23", 0, 250, 150, black);

    for(int i=0; i<800; i++)
    {
        sctest.putpixel(999, i, yellow);
        sctest.putpixel((999+14), i, yellow);
    }

    for(int i='0'; i<'Z'; i++)///for(int i=32; i<80; i++)
    {
        int ypos = i-'0';
        sctest.drawCharff(1000, (16*ypos),(i), black);/// (16*ypos),(0x21+i), black);
    }

    return 0;
}
