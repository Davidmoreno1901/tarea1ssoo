# Makefile 

CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall

# nombre
all: carrera

# compilar
carrera: carrera.cpp
	$(CXX) $(CXXFLAGS) -o carrera carrera.cpp

# eliminar
clean:
	rm -f carrerarm -f *.o $(TARGET)
