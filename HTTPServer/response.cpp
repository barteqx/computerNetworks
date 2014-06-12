#include "response.h"

HttpResponse::HttpResponse (std::string& request, std::string& workingPath)
  : workingPath(workingPath) {

  std::cout << "\n\n___________\nRequest:\n" << request << std::endl;

  char *tmp, *req = const_cast<char*>(request.c_str());
  method = strtok(req, " ");
  location = strtok(NULL, " ");
  http = strtok(NULL, " \r\n");
  tmp = strtok(NULL, " ");
  hostname = strtok(NULL, ":\r\n");
  port = strtok(NULL, "\r\n");

  do {
    tmp = strtok(NULL, " \r\n");
    if(tmp == NULL) break;
  } while (strcmp(tmp, "Connection:") == 1);

  tmp = strtok(NULL, " \r\n");
  if (tmp == NULL) {
    connection = "close";
  } else connection = tmp;


  std::cout << "Request received and parsed." << std::endl;
}

std::string HttpResponse::getExtension () {
  std::string ext;
  unsigned int i;
  for (i = 0; i < location.size(); i++) {
    if (location[i] == '.') {
      i++;
      break;
    }
  }
  for (int j = 0; j + i < location.size(); j++) 
    ext += location[j + i];
  return ext;
}

void HttpResponse::getType () {
  char *ext = const_cast<char*>(getExtension().c_str());
  if (strcmp(ext, "txt") == 0) contentType = "text/plain";
  else if (strcmp(ext, "html") == 0) contentType = "text/html";
  else if (strcmp(ext, "css") == 0) contentType = "text/css";
  else if (strcmp(ext, "jpeg") == 0) contentType = "image/jpeg";
  else if (strcmp(ext, "jpg") == 0) contentType = "image/jpg";
  else if (strcmp(ext, "png") == 0) contentType = "image/png";
  else if (strcmp(ext, "pdf") == 0) contentType = "application/pdf";
  else contentType = "application/octet-stream";
}

bool HttpResponse::checkPath () {
 contentPath = workingPath + "/" + hostname + location;
 std::cout << "File to be sent: " << contentPath << std::endl;
 file = fopen(contentPath.c_str(), "rb");
 if (file == NULL) return false;
 return true;
}

void HttpResponse::getError(int errCode) {

  error = true;
  contentType = "text/html";


  if (errCode == 301)  {
    errorCode = "301 Moved Permanently";
    response << http << " " << errorCode;
    response << std::string("\nLocation: ") << "http://" << hostname << ":" << port << "/index.html" << "\r\n\r\n";
  }

  else if (errCode == 404) {
    errorCode = "404 Not Found";
    content = std::string("<html><head><title>") + errorCode + "</title></head><body><h1>" + errorCode + "</h1></body></html>";
    contentLength = content.size();
    response << http << errorCode;
    response << "\nConnection: " << connection;
    response << "\nContent-Length: " << contentLength;
    response << "\nContent-Type: " << contentType;
    response << "\r\n\r\n";
    b = (unsigned char*)content.c_str();
  }


  else {
    errorCode = "501 Not Implemented";
    content = std::string("<html><head><title>") + errorCode + "</title></head><body><h1>" + errorCode + "</h1></body></html>";
    contentLength = content.size();
    response << http << "" << errorCode;
    response << "\nConnection: " << connection;
    response << "\nContent-Length: " << contentLength;
    response << "\nContent-Type: " << contentType;
    response << "\r\n\r\n";
    b = (unsigned char*)content.c_str();
  }
}

void HttpResponse::getResponse () {
  bool correctPath = checkPath();
  

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
    fseek(file, 0, SEEK_END);
    contentLength = ftell(file);
    rewind(file);
    b = (unsigned char*)malloc(contentLength*sizeof(unsigned char));
    fread(b, sizeof(unsigned char), contentLength, file);
    fclose(file);
  }

  if (location != "" and !error) {
    response << http << " " << errorCode;
    response << "\nLocation: " << location;
    response << "\nConnection: " << connection;
    response << "\nContent-Length: " << contentLength;
    response << "\nContent-Type: " << contentType;
    response << "\r\n\r\n";
  }
  r = (unsigned char*)response.str().c_str();

  l = response.str().size();

  l += contentLength;

  buffer = (unsigned char*)malloc(l*sizeof(unsigned char));
  for (unsigned int i = 0; i < response.str().size(); i++) {
    buffer[i] = r[i];
  }

  for (int i = 0; i < contentLength; i++) {
    buffer[i + response.str().size()] = b[i];
  }

  std::cout <<"___________\nResponse:\n" << buffer << std::endl;
}