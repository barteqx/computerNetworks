#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <cstring>

#define BUFSIZE 1000000

class HttpResponse {

public:
  std::string method, contentLength, contentType, contentPath,
      location, http, hostname, connection, workingPath, errorCode, content;

  bool error;

  int length;

  HttpResponse (std::string& request, std::string& workingPath);

  std::string getExtension();
  
  void getType();
  void getError(int errCode);  
  std::string getResponse();

private:
  FILE *file;
  bool checkPath();


};

struct InvalidRequest {};