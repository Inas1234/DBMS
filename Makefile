FILES = main.cpp ./src/Tokenizer.cpp ./src/Parser.cpp ./src/User.cpp ./src/Generator.cpp ./src/Menu.cpp -I ./lib/include

all:
	g++ -std=c++17 $(FILES) -o mlinql