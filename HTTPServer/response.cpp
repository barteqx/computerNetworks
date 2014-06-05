#include "response.h"

HttpResponse::HttpResponse (std::string& request, std::string& workingPath)
  : workingPath(workingPath) {

  char *tmp, *req = strdup(request.c_str());
  method = strtok(req, "\r\n");
  location = strtok(NULL, "\r\n");
  http = strtok(NULL, "\r\n");
  hostname = strtok(NULL, "\r\n");

  do {
    tmp = strtok(NULL, " \r\n");
    if(tmp == NULL) break;
  } while (tmp != "Connection:");

  tmp = strtok(NULL, "\r\n")
  connection = (tmp == NULL) ? "close" : tmp;

  std::cout << "Request received and parsed.";
}

std::string& HttpResponse::getExtension () {
  std::string ext;
  int i;
  for (i = 0; i < location.size(); i++) {
    if (location[i] == '.') {
      i++;
      break;
    }
  for (int j = 0; j + i < location.size; j++) 
    ext << location[j + i];
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

bool HttpResponse::checkPatch () {
 contentPath = workingPath + location;
 file = fopen(contentPath, "rb");
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

std::string& HttpResponse::getResponse () {
  bool correctPath = checkPatch();
  std::string response;

  if (is_dir(contentPath))
    getError(301);

  else if (!correctPath)
    getError(404);

  else if (method != "GET")
    getError(501);

  if (!error) {
    errorCode = "200 OK";
    content = "";
    fseek (file , 0 , SEEK_END);
    int filesize = ftell(file);
    rewind(file);

    int c = fread(content, 1, filesize, file);
    if(c != filesize) printf("File size doesn't match!");
    contentLength = filesize;

    fclose(file);
  }

  response = std::string(http) + " " + errorCode;
  if (location != "")
    response = std::string(response) + "\nLocation: " + location;

  response += "\nConnection: " + connection;
  response += "\nContent-Length: " + itoa(contentLength);
  response += "\nContent-Type: " + contentType;
  response += "\r\n\r\n";

  response += content;

  return response; 
}