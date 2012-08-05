#GPP = g++ -Wall -pedantic
GPP = g++ -Wall -ggdb -pedantic
mainos = measure.o unit.o
mainhpps = unit.hpp unitdefs.hpp measure.hpp measuredefs.hpp

default: cvtunits falltime falldist test

measure.o: measure.cpp $(mainhpps)
	$(GPP) -c $<

unit.o: unit.cpp unit.hpp unitdefs.hpp
	$(GPP) -c $<

cvtunits: cvtunits.o $(mainos)
	$(GPP) -o cvtunits $+

cvtunits.o: cvtunits.cpp $(mainhpps)
	$(GPP) -c $<

falltime: falltime.o $(mainos)
	$(GPP) -o falltime $+

falltime.o: falltime.cpp $(mainhpps)
	$(GPP) -c $<

falldist: falldist.o $(mainos)
	$(GPP) -o falldist $+

falldist.o: falldist.cpp $(mainhpps)
	$(GPP) -c $<

test: test.o $(mainos)
	$(GPP) -o test $+

test.o: test.cpp $(mainhpps)
	$(GPP) -c $<

clean:
	rm cvtunits falldist falltime test *.o

