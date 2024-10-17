CFLAGS := -Wall -Wextra -O3 -std=c23 ${CFLAGS}
LDFLAGS := -lcurl -lcjson ${LDFLAGS}

CC = clang
LD = clang

SRC_DIR = src
SRC = $(wildcard $(addsuffix /*.c, $(SRC_DIR)))

INSTALL_DIR = /opt/weather_app
SYSTEMD_UNIT_PATH = /usr/lib/systemd/system
BIN_INSTALL_PATH = /usr/bin

UPDATE_TIME ?= 10
CFLAGS += -DUPDATE_TIME=${UPDATE_TIME}

.PHONY: all current_weather install uninstall clean
all: current_weather

current_weather: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o current_weather

install: current_weather
	@mkdir -p $(INSTALL_DIR)
	cp weather.service $(SYSTEMD_UNIT_PATH)
	cp current_weather $(BIN_INSTALL_PATH)

uninstall:
	rm -rf $(INSTALL_DIR)/*
	rm $(SYSTEMD_UNIT_PATH)/weather.service
	rm $(BIN_INSTALL_PATH)/current_weather

clean:
	rm current_weather
