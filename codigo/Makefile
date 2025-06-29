CXXFLAGS := -std=c++23 -Wall -Wextra -lSDL2 -lSDL2main

all: $(concepts_folder) $(primitives_files)
	$(CXX) -c ./src/concepts/primitives.cpp $(CXXFLAGS)
	mv primitives.o ./compiled/
	$(CXX) -c ./src/concepts/texture.cpp $(CXXFLAGS)
	mv texture.o ./compiled/
	$(CXX) -c ./src/pr_objects/circle.cpp $(CXXFLAGS)
	mv circle.o ./compiled/
	$(CXX) -c ./src/concepts/physical.cpp $(CXXFLAGS)
	mv physical.o ./compiled/
	$(CXX) ./compiled/* ./headers/pr_objects/circle.hpp ./headers/concepts/physical.hpp ./headers/concepts/texture.hpp ./headers/concepts/primitives.hpp ./pruebas.cpp -o ./fin $(CXXFLAGS)

clean: 
	rm -rf ./compiled/*
