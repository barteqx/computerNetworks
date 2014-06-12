#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#define BUFSIZE 1000000

class HttpResponse {

public:
  std::string method, contentType, contentPath,
      location, http, hostname, connection, workingPath, errorCode, content;

  bool error;

  int length, contentLength, fd;

  FILE *file;

  unsigned char *buffer;
  unsigned char *b;
  unsigned char *r;

  int l;

  HttpResponse (std::string& request, std::string& workingPath);

  std::string getExtension();
  
  void getType();
  void getError(int errCode);  
  void getResponse();
  void fileclose();

  ~HttpResponse() {
    free(buffer);
  }

private:
  bool checkPath();


};

struct InvalidRequest {};