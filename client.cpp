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
#include <thread>

#include "main.cpp"

using namespace std;

// max queue size
#define MAX_QUEUE_SIZE 3000000

// read / write interface names, port number
#define read_iface "lo"
#define write_iface "lo"
#define read_ip_addr "127.0.0.1"
#define read_port 1000
#define write_port 1002

// reader process pops from this queue, writer pushes to this queue
queue<char *> read_write_queue;

// lock for manipulating queue
mutex rw_lock;

void writer() {
    char *buff;
    while(1) {
        while(read_write_queue.empty()) {
            cout << "queue empty, writer sleeping for 1s..." << "\n";
            sleep(1);
        }

        rw_lock.lock();
        buff = read_write_queue.front();
        read_write_queue.pop();
        rw_lock.unlock();

        send(n_sd, buff, strlen(buff), 0);
        free(buff);
    }
}

void reader() {
    char *buff;
    while(1) {
        buff = new char[BUFF_SIZE];
        recv(r_sd, buff, sizeof(buff), 0);

        if(read_write_queue.size() >= MAX_QUEUE_SIZE) {
            // cout << "queue full, msg dropped" << "\n";
            free(buff);
            continue;
        }

        rw_lock.lock();
        read_write_queue.push(buff);
        rw_lock.unlock();
    }
}

int main(int argc, char * argv[]) {
    // assuming server will first start listening to connections
    init_reader(read_port, read_ip_addr, read_iface);
    init_writer(write_port, write_iface);

    cout << "reader, writer intialized, starting communication" << "\n";

    thread reader_thread(reader);
    thread writer_thread(writer);

    writer_thread.join();
    reader_thread.join();

    return 0;
}