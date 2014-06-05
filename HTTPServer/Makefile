server: server.o response.o
	g++ server.o response.o -o server

server.o: server.cpp server.h response.h
	g++ server.cpp -o server.o

response.o: response.cpp response.h
	g++ response.cpp -o response.o