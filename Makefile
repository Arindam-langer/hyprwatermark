CXX = g++
CXXFLAGS = -std=c++23 -shared -fPIC -Wall -Wextra -O3 -flto
PKG_CONFIG = pkg-config
PKG_DEPS = hyprland hyprgraphics
PKG_CFLAGS = $(shell $(PKG_CONFIG) --cflags $(PKG_DEPS))
PKG_LIBS = $(shell $(PKG_CONFIG) --libs $(PKG_DEPS))

SRC = src/main.cpp src/WatermarkDecoration.cpp src/Config.cpp src/TextureManager.cpp
OUT = hyprwatermark.so

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(PKG_CFLAGS) -o $@ $^ $(PKG_LIBS)

clean:
	rm -f $(OUT)
