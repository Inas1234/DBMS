FILES = main.cpp ./src/Tokenizer.cpp ./src/Parser.cpp 

all:
	g++ $(FILES) -o main