/* vim: set ts=4 sw=4 expandtab: */
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "lib/handler.h"

#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }
#define recover(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); }

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * thread(void *arg) {
    int client_fd = *((int *)arg), err;
    struct Operation operation;
    struct OperationResult result;
    char buffer[OPERATION_SIZE];
    char response[sizeof(struct OperationResult)];

    while (1) {
      memset(&operation, 0, OPERATION_SIZE);
      memset(&buffer, 0, OPERATION_SIZE);
      memset(&response, 0, sizeof(struct OperationResult));
      memset(&result, 0, sizeof(struct OperationResult));

      int read = recv(client_fd, buffer, OPERATION_SIZE, 0);

      if (!read) break; // done reading
      if (read < 0) on_error("Client read failed\n");

      memcpy(&operation, buffer, sizeof(struct Operation));

      pthread_mutex_lock(&lock);
      txt_handle(operation, &result);
      pthread_mutex_unlock(&lock);

      switch (result.status)
      {
          case TXT_OPERATION_SUCCESS:
          case TXT_ERROR_INSUFFICIENT_FUNDS:
          case TXT_ERROR_INVALID_AMOUNT:
              sprintf(response, "%20s%04d%010.2f\n", result.account.number, result.status, result.account.balance);
              break;
          case TXT_ERROR_UNKNOWN_ACCOUNT:
          case TXT_ERROR_INVALID_ACCOUNT_NUMBER:
          case TXT_ERROR_INVALID_OPERATION:
          case TXT_ERROR_INVALID_TIMESTAMP:
          default:
              recover("Operation failed due to an error: %d\n", result.status);
      }

      err = send(client_fd, response, strlen(response), 0);
      if (err < 0) on_error("Client write failed\n");
    }

    return 0;
}

void vacuum_threads(int i, pthread_t tid[]) {
    if (i >= MAX_THREADS) {
        i = 0;
        while (i < MAX_THREADS) {
            pthread_join(tid[i++], NULL);
        }
    }
}

void listener(int server_fd) {
    int client_fd;
    struct sockaddr_in client;

    pthread_t tid[MAX_THREADS];

    int i = 0;
    while (1) {
        socklen_t client_len = sizeof(client);
        client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

        if (client_fd < 0) on_error("Could not establish new connection\n");

        if (pthread_create(&tid[i], NULL, thread, &client_fd) != 0) on_error("Failed to create thread\n");

        if (i > MAX_THREADS) {
            vacuum_threads(i, tid);
            i = 0;
        }
    }
}

// main 8080
int main (int argc, char *argv[]) {
    if (argc < 2) on_error("Usage: %s [port]\n", argv[0]);

    int init = txt_init();
    if (init < 0) on_error("Could not init transaction service\n");

    int port = atoi(argv[1]);

    int server_fd, err;
    struct sockaddr_in server;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) on_error("Could not create socket\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0) on_error("Could not bind socket\n");

    err = listen(server_fd, 128);
    if (err < 0) on_error("Could not listen on socket\n");

    printf("Server is listening on %d\n", port);

    listener(server_fd);

    return 0;
}
