#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_types.h"
#include "write_callback.h"

struct location get_location() {
  int _exit = 0;
  char asked_url[] = "https://ipinfo.io/json";
  struct location ret = {0};

  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Error: Failed to initialize curl\n");
    exit(1);
  }

  curl_easy_setopt(curl, CURLOPT_URL, asked_url);
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

  cJSON *loc = cJSON_GetObjectItemCaseSensitive(json, "loc");
  if (cJSON_IsString(loc) && (loc->valuestring != NULL)) {
    char *location = strdup(loc->valuestring);
    ret.latitude = strtod(strtok(location, ","), NULL);
    ret.longitude = strtod(strtok(NULL, ","), NULL);
  }

clear:
  curl_easy_cleanup(curl);
  cJSON_Delete(json);
  if (_exit == 1) {
    exit(1);
  }

  return ret;
}
