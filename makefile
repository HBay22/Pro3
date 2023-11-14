EXEC_capture = cppaudiocapture
EXEC_play = cppaudioplay
EXEC_play_record = cpprecordplay
EXEC_fft = fft_program  # Name of the FFTW-based program

CLIB = -I./lib/portaudio/include ./lib/portaudio/lib/.libs/libportaudio.a -lrt -lasound -ljack -pthread

# Add the FFTW-related flags and libraries
CLIB += $(FFTW_LIBS)

# Define the FFTW_LIBS variable to specify FFTW library and flags
FFTW_LIBS = -lfftw3 -lm

$(EXEC_capture): main.cpp
	g++ -o $@ $^ $(CLIB)

$(EXEC_play): play.cpp
	g++ -o $@ $^ $(CLIB)

$(EXEC_play_record): main1.cpp
	g++ -o $@ $^ $(CLIB)

# Define the compilation target for the FFTW-based program
$(EXEC_fft): fft.cpp
	g++ -o $@ $^ $(CLIB) $(FFTW_LIBS)  # Add the FFTW_LIBS variable here

install-deps:
	mkdir -p lib

	curl https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz | tar -zx -C lib
	cd lib/portaudio && ./configure && $(MAKE) -j

.PHONY: install-deps

uninstall-deps:
	cd lib/portaudio && $(MAKE) uninstall
	rm -rf lib/portaudio

.PHONY: uninstall-deps

clean:
	rm -f $(EXEC_capture) $(EXEC_play) $(EXEC_play_record) $(EXEC_fft)  # Add the FFTW program to the clean target

.PHONY: clean
