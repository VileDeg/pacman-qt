PROJ_MAKEFILE=Makefile
EXEC=pacman

.PHONY: all run doxygen pack clean

all: 
	$(MAKE) -C src -f $(PROJ_MAKEFILE) 


run: $(EXEC)
	./$<

doxygen:
	doxygen Doxyfile

pack:
	zip -r $(EXEC).zip src/ doc/ images/ README.txt

#rm -rf $(EXEC) $(EXEC).zip
clean:
	$(MAKE) -C src -f $(PROJ_MAKEFILE) clean
	