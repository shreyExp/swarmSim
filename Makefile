main: main.cpp
	g++ -o main main.cpp `pkg-config --cflags --libs armadillo jsoncpp opencv4` 2> errors
