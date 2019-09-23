.PHONY: all
all: parser finder

.PHONY: parser
parser:
	g++ -iquote ./ -iquote ./include -o parser examples/main-parser.cpp

.PHONY: finder
finder:
	g++ -iquote ./ -iquote ./include -o finder examples/main-finder.cpp
