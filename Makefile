PROJ_FILE=pacman.pro
PROJ_MAKEFILE=Makefile

EXEC=pacman
SRC_DIR=src
DOC_DIR=doc
LOGIN=xgonce00

.PHONY: all run doxygen pack unpack clean

all: 
	$(MAKE) -C $(SRC_DIR) -f $(PROJ_MAKEFILE) 

run: $(EXEC)
	./$<

doxygen:
	doxygen Doxyfile

pack:
	rm -f $(LOGIN).zip
	zip -r $(LOGIN).zip $(SRC_DIR) sprites examples
	zip $(LOGIN).zip $(DOC_DIR) saves Makefile README.txt Doxyfile

unpack:
	unzip $(LOGIN).zip -d $(LOGIN)

clean:
	$(MAKE) -C $(SRC_DIR) -f $(PROJ_MAKEFILE) clean
	rm -rf $(DOC_DIR)/*
	rm -f $(SRC_DIR)/.qmake.stash
	rm -f $(LOGIN).zip