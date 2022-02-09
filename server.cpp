/**
 * @file server.cpp
 * @author Harsh Kumar (hadron43@yahoo.com)
 * @brief
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
#include <thread>

#include "main.cpp"

using namespace std;

// read / write interface names, port number
#define read_iface "lo"
#define write_iface "lo"
#define read_ip_addr "127.0.0.1"
#define read_port 1002
#define write_port 1000

void writer() {
    char buff[BUFF_SIZE];
    while(1) {
        for(int i = 0; i < sizeof(buff); ++i)   buff[i] = random() % 255;
        send(n_sd, buff, strlen(buff), 0);
    }
}

void reader() {
    char buff[BUFF_SIZE];
    while(1) {
        recv(r_sd, buff, sizeof(buff), 0);
    }
}

int main(int argc, char * argv[]) {
    init_writer(write_port, write_iface);
    cout << "sleep..." << "\n";
    sleep(3);
    init_reader(read_port, read_ip_addr, read_iface);

    cout << "reader, writer intialized, starting communication" << "\n";

    thread writer_thread(writer);
    thread reader_thread(reader);

    writer_thread.join();
    reader_thread.join();

    return 0;
}