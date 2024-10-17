#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_types.h"
#include "write_callback.h"

struct weather get_weather(struct location ldata, char *API_KEY) {
  int _exit = 0;
  char template_str[] =
      "https://api.openweathermap.org/data"
      "/2.5/weather?lat=%lg&lon=%lg&appid=%s&units=metric";
  char result_str[400];
  struct weather returned = {0};
  sprintf(result_str, template_str, ldata.latitude, ldata.longitude, API_KEY);

  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Error: Failed to initialize curl\n");
    exit(1);
  }

  curl_easy_setopt(curl, CURLOPT_URL, result_str);
  cJSON *json = NULL;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Error: curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    _exit = 1;
    goto clear;
  }

  if (!json) {
    fprintf(stderr, "Error: No JSON data received\n");
    _exit = 1;
    goto clear;
  }

  cJSON *weather_array = cJSON_GetObjectItem(json, "weather");
  if (!weather_array || !cJSON_IsArray(weather_array) ||
      cJSON_GetArraySize(weather_array) == 0) {
    fprintf(stderr, "Error: 'weather' array not found or empty\n");
    _exit = 1;
    goto clear;
  }

  cJSON *weather_fi = cJSON_GetArrayItem(weather_array, 0);
  cJSON *weather_str = cJSON_GetObjectItem(weather_fi, "main");
  if (weather_str) {
    strcpy(returned.weather, weather_str->valuestring);
  } else {
    fprintf(stderr, "Error: 'weather' field not found\n");
    _exit = 1;
    goto clear;
  }

  cJSON *main_data = cJSON_GetObjectItem(json, "main");
  if (!main_data) {
    fprintf(stderr, "Error: 'main' object not found\n");
    _exit = 1;
    goto clear;
  }

  cJSON *temp = cJSON_GetObjectItem(main_data, "temp");
  if (!temp || !cJSON_IsNumber(temp)) {
    fprintf(stderr, "Error: 'temp' field not found or NaN\n");
    _exit = 1;
    goto clear;
  }

  returned.temp = temp->valuedouble;

clear:
  curl_easy_cleanup(curl);
  cJSON_Delete(json);
  if (_exit == 1) {
    exit(1);
  }

  return returned;
}
