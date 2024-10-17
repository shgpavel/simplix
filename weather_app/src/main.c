#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "data_types.h"
#include "get_location.h"
#include "get_weather.h"

#define UPDATE_TIME 10

int main() {
  curl_global_init(CURL_GLOBAL_ALL);

  struct location ldata = get_location();

  FILE *api_key = fopen("/opt/weather_app/api_key", "r");
  char API_KEY[100];

  if (!api_key) {
    fprintf(stderr, "Error: Could not open api_key file\n");
    return 1;
  }

  int err = fscanf(api_key, "%99s", API_KEY);
  if (err != 1) {
    fprintf(stderr, "Error: api_key filetype invalid\n");
    return 1;
  }
  err = fclose(api_key);
  if (err != 0) {
    fprintf(stderr, "Error: Could not close api_key file\n");
    return 1;
  }

  while (1) {
    struct weather res = get_weather(ldata, API_KEY);
    FILE *f = fopen("/opt/weather_app/weather", "w");
    if (!f) {
      fprintf(stderr, "Error: Could not open weather output file\n");
      return 1;
    }

    err = fprintf(f, "%lg %s\n", res.temp, res.weather);
    if (err < 2) {
      fprintf(stderr, "Error: For some reason data not written\n");
      return 1;
    }
    err = fclose(f);
    if (err != 0) {
      fprintf(stderr, "Error: Could not close weather file\n");
      return 1;
    }
    sleep(UPDATE_TIME);
  }
  curl_global_cleanup();
  return 0;
}
