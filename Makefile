FILES = main.cpp ./src/Tokenizer.cpp ./src/Parser.cpp  ./src/Generator.cpp -I ./lib/include

all:
	g++ -std=c++17 $(FILES) -o main