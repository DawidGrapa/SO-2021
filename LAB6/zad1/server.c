#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/ipc.h>

#include "common.h"

int CLIENT_NO = 0;
int SERVER_QUEUE;
client clients[MAX_CLIENTS];

int get_available_client(){
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i].id==-1){
            return i;
        }
    }
    return -1;
}

void init(msg *message){
    int client_queue = atoi(message->message);
    int available = get_available_client();
    if(available!=-1){
        clients[available].id = available;
        clients[available].queue_id = client_queue;
    }
    msg res;
    res.type=INIT;
    sprintf(res.message,"%d", available);
    msgsnd(client_queue, &res, MSG_LEN, 0);
}

void list(msg *message){
    int client_id = message->sender_id;

    msg res;
    res.type = LIST;
    res.sender_id=-1;
    res.message[0] = '\0';
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i].id != -1 && clients[i].connected_client_id != -1){
            sprintf(res.message + strlen(res.message), "%d%s ",clients[i].id, " is active.\n");
        }
        else if(clients[i].id != -1 && clients[i].connected_client_id == -1){
            sprintf(res.message + strlen(res.message), "%d%s ",clients[i].id," is ready to connect.\n");
        }
    }
    msgsnd(clients[client_id].queue_id, &res, MSG_LEN, 0);
}

void connect(msg *message){
    int client_id = message->sender_id;
    int another_client_id = atoi(message->message);

    if(clients[client_id].connected_client_id == -1 && clients[another_client_id].connected_client_id == -1){
        int client_queue = clients[client_id].queue_id;
        int another_client_queue = clients[another_client_id].queue_id;
        clients[client_id].connected_client_id = another_client_id;
        clients[another_client_id].connected_client_id = client_id;
        msg res1;
        msg res2;
        res1.sender_id = -1;
        res2.sender_id = -1;
        res1.type = CONNECT;
        res2.type = CONNECT;

        sprintf(res1.message, "%d", client_queue);
        sprintf(res2.message, "%d", another_client_queue);
        msgsnd(client_queue, &res2, MSG_LEN, 0);
        msgsnd(another_client_queue, &res1, MSG_LEN, 0);
    }
}

void disconnect(msg *message){
    int client_id = message->sender_id;
    if(clients[client_id].connected_client_id != -1){
        int another_client = clients[client_id].connected_client_id;
        clients[client_id].connected_client_id = -1;
        clients[another_client].connected_client_id = -1;

        msg res;
        res.type = DISCONNECT;
        res.sender_id = -1;
        sprintf(res.message,"%s", "DISCONNECTED");
        msgsnd(clients[client_id].queue_id, &res, MSG_LEN, 0);
        msgsnd(clients[another_client].queue_id, &res, MSG_LEN, 0);
    }
}

void stop(msg *message){
    int client_id = message->sender_id;
    if(clients[client_id].connected_client_id != -1){
        int another_client = clients[client_id].connected_client_id;
        clients[client_id].connected_client_id = -1;
        clients[another_client].connected_client_id = -1;

        msg res;
        res.type = DISCONNECT;
        res.sender_id = -1;
        sprintf(res.message,"%s", "DISCONNECTED");
        clients[client_id].id = -1;
        msgsnd(clients[another_client].queue_id, &res, MSG_LEN, 0);
    }
    else if(clients[client_id].connected_client_id == -1){
        puts("A");
        clients[client_id].id = -1;
    }    
}

void handler(int signal){
    msg res;
    res.sender_id = -1;
    res.type = STOP_SERVER;
    sprintf(res.message, "%s", "SERVER stopped.");
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i].id != -1){
            msgsnd(clients[i].queue_id, &res, MSG_LEN, 0);
        }
    }
    msgctl(SERVER_QUEUE, IPC_RMID, NULL);
    exit(0);
}

int main(){
    char *HOME = getpwuid(getuid())->pw_dir;

    for(int i=0;i<MAX_CLIENTS;i++){
        clients[i].id=-1;
        clients[i].connected_client_id=-1;
    }
    key_t server_key = ftok(HOME, SERVER_ID);
    SERVER_QUEUE = msgget(server_key, IPC_CREAT | 0666);
    signal(SIGINT, handler);

    while(TRUE){
        msg message;
        msgrcv(SERVER_QUEUE, &message, MSG_LEN,  0,0);  
        printf("%ld\n", message.type);
        switch(message.type){
            case INIT:
                init(&message);
                break;
            case LIST:
                list(&message);
                break;
            case DISCONNECT:
                disconnect(&message);
                break;
            case CONNECT:
                connect(&message);
                break;
            case STOP:
                stop(&message);
                break;
            default:
                break;
        }
    }
    return 0;
}