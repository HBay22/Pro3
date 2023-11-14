.PHONY: all clean

CC:= g++-11

CFLAGS = -std=c++11 -Wno-deprecated-enum-enum-conversion -O2
# CFLAGS += `pkg-config --cflags portaudio` 
# LDFLAGS = `pkg-config --libs portaudio`
	# Additional include directories for PortAudio
CFLAGS += -I./lib/portaudio/include 
	# Libraries for PortAudio, rt, asound, jack, and pthread
LDFLAGS += ./lib/portaudio/lib/.libs/libportaudio.a -lrt -lasound -ljack -pthread
LDFLAGS += -lfftw3 #include fftw3




SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

all: main

clean:
	$(RM) $(OBJECTS) $(DEPENDS) main
#	$(RM) $(OBJECTS) $(DEPENDS) main audio_data.csv

-include $(DEPENDS)

# Make object files
%.o: %.cpp Makefile
	$(CC) $(WARNING) $(CFLAGS) -MMD -MP -c $< -o $@

# Linking the executable from the object files
main: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@
	./main

install-deps:
	mkdir -p lib
	
	curl https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz | tar -zx -C lib
	cd lib/portaudio && ./configure && $(MAKE) -j
.PHONY: install-deps

uninstall-deps:
	cd lib/portaudio && $(MAKE) uninstall
	rm -rf lib/portaudio
.PHONY: uninstall-deps

# CLIB = -I./lib/portaudio/include ./lib/portaudio/lib/.libs/libportaudio.a -lrt -lasound -ljack -pthread
