/**
 * @file main.cpp
 * @author Harsh Kumar (hadron43@yahoo.com)
 * @brief Main contents of l2-forward
 * @version 0.1
 * @date 2022-02-09
 *
 */
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

// read / write buffer size
#define BUFF_SIZE 1024

void check_error(int return_val, char *str) {
    if(return_val < 0) {
        perror(str);
        exit(return_val);
    }
}

int r_sd;
struct sockaddr_in serv_addr;

void init_reader(int port, char *addr, char *interface) {
    int tmp, opt = 1;

    // Initialize myaddr
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(port);
    r_sd = socket(AF_INET, SOCK_STREAM, 0);
    check_error(r_sd, "socket");

    tmp = inet_pton(AF_INET, addr, &serv_addr.sin_addr);
    check_error(tmp, "inet_pton");

    tmp = setsockopt(r_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    check_error(tmp, "setsockopt");

    // sets the interface for the socket
    tmp = setsockopt(r_sd, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));
    check_error(tmp, "setsockopt");

    cout << "reader waiting for connection..." << "\n";
    tmp = connect(r_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    check_error(tmp, "connect");
    cout << "reader connection successful" << "\n";
}

int w_sd, n_sd;
struct sockaddr_in myaddr, client_addr;

void init_writer(int port, char *interface) {
    int opt = 1, tmp;
    // Initialize myaddr
    myaddr.sin_family = PF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    w_sd = socket(AF_INET, SOCK_STREAM, 0);

    tmp = setsockopt(w_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    check_error(tmp, "setsockopt");

    // sets the interface for the socket
    tmp = setsockopt(w_sd, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));
    check_error(tmp, "setsockopt");

    // Bind to an address
    tmp = bind(w_sd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    check_error(tmp, "bind");

    cout << "writer listening for connection..." << "\n";
    tmp = listen(w_sd, 0);
    check_error(tmp, "listen");

    n_sd = accept(w_sd, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr);
    check_error(tmp, "accept");
}
