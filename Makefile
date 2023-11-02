EXEC_CAPTURE = audiocapture
EXEC_PLAY = audioplay
EXEC_TEST = audiotest
EXEC_RECORD = record

#Includes header files of portaudio | -lrt.. and so on enables audio
CLIB = -I./lib/portaudio/include ./lib/portaudio/lib/.libs/libportaudio.a -lrt -lasound -ljack -pthread

$(EXEC_CAPTURE): main.cpp
	g++ -o $@ $^ $(CLIB)

$(EXEC_PLAY): playsound.cpp
	g++ -o $@ $^ $(CLIB)

$(EXEC_RECORD): record.cpp
	g++ -o $@ $^ $(CLIB)

$(EXEC_TEST): playsoundtest.cpp
	g++ -o $@ $^ $(CLIB)

	
install-deps:
	mkdir -p lib
# curl downloads a file from the internet.
# Makes a lib folder, creating a portaudio folder inside the lib folder by extracting the file from the link.
# For full explanation watch https://youtu.be/jpsJCji71Ec?feature=shared&t=425
	curl https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz | tar -zx -C lib
	cd lib/portaudio && ./configure && $(MAKE) -j
# PHONY is used to declare files that are not intended to produce output files.
.PHONY: install-deps

# Make uninstall-deps to uninstall
uninstall-deps:
	cd lib/portaudio && $(MAKE) uninstall
	rm -rf lib/portaudio
.PHONY: uninstall-deps

clean: 
	rm -f $(EXEC)
.PHONY: clean