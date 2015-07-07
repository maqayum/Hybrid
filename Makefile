SRC=
MAINFILE=main
CXX=mpiicpc #mpiicpc #mpic++.openmpi
CXXFLAGS=-O2 -fopenmp -mt_mpi #-g
EXEC=main
OBJ= $(SRC:.cpp=.o)
HEA= $(SRC:.cpp=.h)

all: $(EXEC)

main : $(OBJ) $(MAINFILE).o
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS)

$(MAINFILE).o : $(MAINFILE).cpp $(HEA)
	$(CXX) -c $< $(CXXFLAGS)


.PHONY: clean mrproper
clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)	  
