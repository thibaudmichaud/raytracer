LDFLAGS=-LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system
CXXFLAGS=-std=c++11 -O3 -Wall -Wextra -Wpedantic -ISFML/include -fopenmp
OBJ=src/main.o \
    src/data.o \
    src/parser.o \
    src/light.o \
    src/raytrace.o

HEADERS=src/collision.hh \
        src/color.hh \
        src/data.hh \
        src/parser.hh \
        src/raytrace.hh \
        src/texture.hh \
        src/light.hh \
        src/vector3d.hh

all: rt

rt: $(OBJ) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@

src/cpu/%.o : src/cpu/%.cc
	$(CXX) $(CXXFLAGS) -Isrc/cpu -c $< -o $@

clean:
	$(RM) $(OBJ)
