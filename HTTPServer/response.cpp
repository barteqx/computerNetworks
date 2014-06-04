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

std::string& HttpResponse::getType () {
  std::string ext = getExtension();
  if (ext == "txt") return "text/plain";
  if (ext == "html") return "text/html";
  if (ext == "css") return "text/css";
  if (ext == "jpeg") return "image/jpeg";
  if (ext == "jpg") return "image/jpg";
  if (ext == "png") return "image/png";
  if (ext == "pdf") return "application/pdf";
  else return "application/octet-stream";
}

bool HttpResponse::checkPatch () {
 contentPath = workingPath + location;
 file = fopen(contentPath, "rb");
 if (file == NULL) return false;
 return true;
}

std::string& HttpResponse::getError(int errCode) {
  if (errCode == 301) 
    errorCode = "301 Moved Permanently";

  if (errCode == 404)
    errorCode = "404 Not Found";

  if (errorCode == 501)
    errorCode = "501 Not Implemented yet";

  std::string content = std::string("<html><head><title>") + errorCode + "</title></head><body><h1>" + errorCode + "</h1></body></html>";
  contentLength = content.size();
  return content;
}

void HttpResponse::getResponse (char * buffer) {

}