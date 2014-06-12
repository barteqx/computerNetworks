#include "server.h"

void HttpServer::runServer() {

  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socketDescriptor == -1) {
    perror("Socket creation error!");
    exit(1);
  }

  client.sin_family = AF_INET;
  client.sin_addr.s_addr = INADDR_ANY;
  client.sin_port = htons(port);

  socketListener = bind(socketDescriptor, (struct sockaddr*)&client, sizeof(client));
  listen(socketDescriptor, 5);

  if (socketListener == -1) {
    perror("Socket bind error");
    exit(1);
  }

  while (true) {

    int e;

    socklen_t clilen = sizeof(client);
    socketConnection = accept(socketDescriptor, (struct sockaddr*) &client, &clilen);
    if (socketConnection == -1) {
      perror("Connection error");
      continue;
    }

    
    char r[BUFSIZE+1];

    e = recv(socketConnection, r, BUFSIZE, 0);
    if (e == -1) {
      perror("Recv error");
    }

    request = r;

    HttpResponse resp(request, path);

    resp.getResponse();
    
    e = send(socketConnection, resp.buffer, resp.l, 0);

    if (resp.connection == "close")
      break;

    t.tv_sec = 1;

    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(socketConnection, &descriptors);

    if ((e = select(socketConnection, &descriptors, NULL, NULL, &t)) < 0) {
      printf("Socket selection error");
    }
  }

  if (close(socketConnection) < 0)
    perror("Socket closing error");
}

int main(int argc, char const *argv[]) {

  DIR *dir = opendir(argv[2]);

  if (argc != 3 || !dir) {
    perror("Bad arguments! Usage: server <port> <directory>");
    exit(1);
  }
  closedir(dir);

  std::string path = argv[2];

  HttpServer server(atoi(argv[1]), path);

  server.runServer();

  std::cout << "Quitiing..." << std::endl;
}