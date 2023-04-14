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
	zip -r $(LOGIN).zip $(SRC_DIR)/* $(DOC_DIR)/ sprites/* examples/* saves/ \
	Makefile $(PROJ_FILE) README.txt Doxyfile

unpack:
	unzip $(LOGIN).zip -d $(LOGIN)

clean:
	$(MAKE) -C $(SRC_DIR) -f $(PROJ_MAKEFILE) clean
	rm -rf $(DOC_DIR)/*
	rm $(SRC_DIR)/.qmake.stash
	