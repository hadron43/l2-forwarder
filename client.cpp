/**
 * @file client.cpp
 * @author Harsh Kumar (hadron43@yahoo.com)
 * @brief L2 forwarder opens two simultaneous connections with a computer,
 * and forward the traffic received from one socket to the other one.
 * @version 0.1
 * @date 2022-02-08
 *
 */
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include "main.cpp"

using namespace std;

// max queue size
#define MAX_QUEUE_SIZE 1024

// read / write interface names, port number
#define read_iface "lo"
#define write_iface "lo"
#define read_ip_addr "127.0.0.1"
#define read_port 1000
#define write_port 1002

// reader process pops from this queue, writer pushes to this queue
queue<char *> read_write_queue;

int main(int argc, char * argv[]) {
    // assuming server will first start listening to connections
    init_reader(read_port, read_ip_addr, read_iface);
    init_writer(write_port, write_iface);

    cout << "reader, writer intialized, starting communication" << "\n";

    char buff[1024] = {0};
    recv(r_sd, buff, sizeof(buff), 0);
    cout << "received: " << buff << "\n";

    char *msg = "Hello World!";
    send(n_sd, msg, strlen(msg), 0);
    cout << "sent: " << msg << "\n";
}