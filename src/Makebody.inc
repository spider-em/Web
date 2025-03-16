# 
#  SOURCE: Makebody.inc.send
#
#  PURPOSE: MAKEFILE body FOR: GNU-Linux WEB  
# 
#  AUTHOR: ArDean Leith -- 13 MARCH 1997
 
../bin/$(PGM) :  $(LIB)
	@echo "         linking now --"
	cc   $(LF) $(LIB) -u main $(OTHERLIBS) -o ../bin/$(PGM)

$(LIB) :  $(ELEMENTS)
 

