PROJ_FILE=pacman.pro
PROJ_MAKEFILE=Makefile

EXEC=pacman
DOC_DIR=doc
LOGIN=xgonce00

.PHONY: all run doxygen pack clean

all: 
	$(MAKE) -C src -f $(PROJ_MAKEFILE) 


run: $(EXEC)
	./$<

doxygen:
	doxygen Doxyfile

pack:
	zip $(LOGIN).zip src/ doc/ images/ README.txt Doxyfile Makefile $(PROJ_FILE)

clean:
	$(MAKE) -C src -f $(PROJ_MAKEFILE) clean
	rm -rf $(DOC_DIR)/*
	