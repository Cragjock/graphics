#ifndef MY_CURL_H
#define MY_CURL_H


struct mywx
{
    const char* mycity;
    const char* mysensor;
    float temp;
    float humid;
    float pressure;
    float alt;
    const char* datetime;
    float latitude;
    float longitude;
};

int mycurl(struct mywx &testforcurl);

#endif
