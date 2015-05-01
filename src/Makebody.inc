# 
#  SOURCE: Makebody.inc
#
#  MAKEFILE body FOR GNU-Linux WEB  
# 
#  AUTHOR: ArDean Leith -- 13 MARCH 1997
#          used ../bin dir   Oct 2005
 
BINLOC = ../bin 

../bin/$(PGM) :  $(LIB)
	@echo "        linking ../bin/$(PGM)"
	cc  $(LF) $(LIB) -u main $(OTHERLIBS) -o ../bin/$(PGM)_tmp
	@chmod ug+w ../bin/$(PGM)_tmp
	@echo "Creating `Nextversion ../bin/$(PGM)`"
	@\cp ../bin/$(PGM)_tmp `Nextversion ../bin/$(PGM)`

$(LIB) :  $(ELEMENTS)
 

