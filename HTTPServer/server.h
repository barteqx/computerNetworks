#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "response.h"

class HttpServer {

private:
  int port. socketDescriptor, socketConnection, socketListener;
  std::string path, request, response;

public:
  HttpServer (int port, std::string& path)
    : port(port), path(path) {};

  void runServer();
};