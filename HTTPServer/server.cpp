#include "server.h"

void HttpServer::runServer() {

  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (sockd == -1) {
    perror("Socket creation error!");
    exit(1);
  }

  client.sin_family = AF_INET;
  client.sin_addr.s_addr = INADDR_ANY;
  client.sin_port = htons(port);

  socketListener = bind(sockd, (struct sockaddr*)&client, sizeof(client));

  if(st == -1) {
    perror("socket bind error");
    exit(1);
  }

  while (true) {

    socketConnection = accept(socketDescriptor, NULL, NULL);
    if (socketConnection == -1) {
      perror("connection error");
      continue;
    }

    int e = recv(sockc, request, BUFSIZE, 0);
    if (e == -1) {
        perror("recv error");
        continue;
    }

    HttpResponse resp(request, path);

    response = resp.getResponse();
    e = send(socketConnection, response, resp.length, 0);

    response = "";

    if (resp.connection == "close")
      break;

    t.tv_sec = 1;

    fd_set = descriptors;
    FD_ZERO(&descriptors);
    FD_SET(socketConnection, &descriptors);

    if ((e = select(socketConnection+1, &descriptors, NULL, NULL, &t)) < 0) {
      perror("Socket selection error!");
      break;
    }
  }

  if (close(socketConnection) < 0)
    perror("Socket closing error");
}