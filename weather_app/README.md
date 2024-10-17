## Overview
A simple application that creates ```/opt/weather_app/weather``` file
with current geolocation weather data

## Defaults
Geolocation updates every service restart \
Weather update time is 10 sec, could be changed with
```
make UPDATE_TIME=20
```

> **⚠️ Note** location accuracy is provided only through 
detection of nearby cities in most cases

## Important
	You have to create a /opt/weather_app/api_key file
    with api key from openweathermap
    
    Also make sure bins on your distro in /usr/bin, systemd services
    in /usr/lib/systemd/system otherwise edit Makefile

## Usage example
<img src="uarea.png" title="i3status">

## Dependencies
    clang    >= 17
	libcurl  >= 8.7.1
	cJSON    >= 1.7.18

### Status
    Released

### Install
	sudo make install

### Uninstall
	sudo make uninstall

### Enjoy
	sudo systemctl daemon-reload
	sudo systemctl enable --now weather.service
