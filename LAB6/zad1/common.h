#ifndef COMMON_H
#define COMMON_H

#define MAX_CLIENTS 10
#define MSG_LEN 1024
#define SERVER_ID 1

#define INIT 1
#define STOP 2
#define DISCONNECT 3
#define LIST 4
#define CONNECT 5
#define STOP_SERVER 6
#define SEND 7
#define TRUE 1

typedef struct msg
{
    long type;
    int sender_id;
    char message[MSG_LEN];
} msg;

typedef struct client
{
    int id;
    int queue_id;
    int connected_client_id;
} client;


#endif