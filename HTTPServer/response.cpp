#include "response.h"

HttpResponse::HttpResponse (std::string& request, std::string& workingPath)
  : workingPath(workingPath) {

  char *tmp, *req = const_cast<char*>(request.c_str());
  method = strtok(req, "\r\n");
  location = strtok(NULL, "\r\n");
  http = strtok(NULL, "\r\n");
  hostname = strtok(NULL, "\r\n");

  do {
    tmp = strtok(NULL, " \r\n");
    if(tmp == NULL) break;
  } while (strcmp(tmp, "Connection:") == 0);

  tmp = strtok(NULL, "\r\n");
  if (tmp == NULL) {
    connection = "close";
  } else connection = tmp;

  std::cout << "Request received and parsed.";
}

std::string HttpResponse::getExtension () {
  std::string ext;
  unsigned int i;
  for (i = 0; i < location.size(); i++) {
    if (location[i] == '.') {
      i++;
      break;
    }
  for (int j = 0; j + i < location.size(); j++) 
    ext += location[j + i];
  }
  return ext;
}

void HttpResponse::getType () {
  std::string ext = getExtension();
  if (ext == "txt") contentType = "text/plain";
  if (ext == "html") contentType = "text/html";
  if (ext == "css") contentType = "text/css";
  if (ext == "jpeg") contentType = "image/jpeg";
  if (ext == "jpg") contentType = "image/jpg";
  if (ext == "png") contentType = "image/png";
  if (ext == "pdf") contentType = "application/pdf";
  else contentType = "application/octet-stream";
}

bool HttpResponse::checkPath () {
 contentPath = workingPath + location;
 file = fopen(contentPath.c_str(), "rb");
 if (file == NULL) return false;
 return true;
}

void HttpResponse::getError(int errCode) {

  error = true;

  if (errCode == 301) 
    errorCode = "301 Moved Permanently";

  if (errCode == 404)
    errorCode = "404 Not Found";

  else
    errorCode = "501 Not Implemented";

  content = std::string("<html><head><title>") + errorCode + "</title></head><body><h1>" + errorCode + "</h1></body></html>";
  contentLength = content.size();
}

std::string HttpResponse::getResponse () {
  bool correctPath = checkPath();
  std::string response;

  DIR *dir = opendir(contentPath.c_str());

  if (dir) {
    getError(301);
    closedir(dir);
  }

  else if (!correctPath)
    getError(404);

  else if (method != "GET")
    getError(501);

  if (!error) {
    getType();
    errorCode = "200 OK";
    content = "";
    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);
    rewind(file);

    char cont[] = "";

    int c = fread(cont, 1, filesize, file);
    content = cont;
    if(c != filesize) std::cout << "File size doesn't match!";
    contentLength = filesize;

    fclose(file);
  }

  response = std::string(http) + " " + errorCode;
  if (location != "")
    response = std::string(response) + "\nLocation: " + location;

  response += "\nConnection: " + connection;
  response += std::string("\nContent-Length: ") + contentLength;
  response += "\nContent-Type: " + contentType;
  response += "\r\n\r\n";

  response += content;

  length = response.size();

  return response; 
}