
#include <iostream>
#include "ArduinoJson-v6.14.1.h"
#include <curl/curl.h>
#include "mycurl.h"


using namespace std;

extern struct mywx pwx;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    /// cout << "in callback  world!\n" << (char*)contents <<endl;      /// dumps to STDout
    /// cout << "in callback  size!" << size <<endl;
    /// cout << "in callback  nmemb!" << nmemb <<endl;
    const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(8) + 110;
    DynamicJsonDocument doc(capacity);
    DeserializationError err = deserializeJson(doc, (char*)contents);
    if(err)
    {
        cout<<"deserializeJson fails"<<endl;
        return -1;
    }

    const char* city = doc["city"]; // "Yorba Linda"
    ///cout << "in city  " << city <<endl;
    const char* sensor = doc["sensor"]; // "BME280"
    ///cout << "in sensor  " << sensor <<endl;
    float humidity = doc["humidity"]; // 28.92969
    ///cout << "in humidity  " << humidity <<endl;
    float temperature = doc["temperature"]; // 74.102
    ///cout << "in temperature " << temperature <<endl;
    float pressure = doc["pressure"]; // 29.13247
    ///cout << "in pressure  " << pressure <<endl;
    float altitude = doc["altitude"]; // 681.14
    ///cout << "in altitude " << altitude <<endl;
    const char* timestamp = doc["timestamp"]; // "10:46:44 PM "
    ///cout << "in timestamp " << timestamp <<endl;

    float data_0 = doc["data"][0]; // 33.909
    ///cout << "in LAT " << data_0 <<endl;
    float data_1 = doc["data"][1]; // -117.87
    ///cout << "in LONG " << data_1 <<endl;

    mywx* pwx = (mywx*)userp;
    pwx->humid = humidity;
    pwx->temp = temperature;
    pwx->pressure = pressure;
    pwx->alt = altitude;
    pwx->latitude = data_0;
    pwx->longitude = data_1;
    pwx->datetime = timestamp;
    pwx->mycity = city;
    pwx->mysensor = sensor;

    return nmemb;
}


int mycurl(struct mywx &testforcurl)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

   if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.108/wx");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &testforcurl);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    return 0;
}
