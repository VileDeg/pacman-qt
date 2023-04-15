PROJ_FILE=pacman.pro
PROJ_MAKEFILE=Makefile

EXEC=pacman
SRC_DIR=src
DOC_DIR=doc
LOGIN=xgonce00

.PHONY: all run doxygen pack unpack clean clean_pack

all: 
	$(MAKE) -C $(SRC_DIR) -f $(PROJ_MAKEFILE) 

run: $(EXEC)
	export LIBGL_ALWAYS_INDIRECT=1 && \
	./$<

doxygen:
	doxygen Doxyfile

pack:
	rm -f $(LOGIN).zip
	zip -r $(LOGIN).zip $(SRC_DIR) sprites examples
	zip $(LOGIN).zip $(PROJ_FILE) $(DOC_DIR) saves Makefile README.txt Doxyfile

unpack:
	unzip -o $(LOGIN).zip -d $(LOGIN)

clean:
	$(MAKE) -C $(SRC_DIR) -f $(PROJ_MAKEFILE) clean
	rm -f $(EXEC)
	rm -rf $(DOC_DIR)/*
	rm -f $(SRC_DIR)/.qmake.stash

clean_pack:
	rm -f $(LOGIN).zip
	rm -rf $(LOGIN)

