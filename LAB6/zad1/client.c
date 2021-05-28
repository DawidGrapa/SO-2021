#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/ipc.h>
#include <sys/times.h>
#include <errno.h>

#include "common.h"

int CLIENT_QUEUE;
int SERVER_QUEUE;
int QUEUE;
int CLIENT_ID=-1;

void handler(int signal){
    msg message;
    message.sender_id=CLIENT_ID;
    message.type=STOP;
    msgsnd(SERVER_QUEUE, &message, MSG_LEN, 0);
    msgctl(CLIENT_QUEUE, IPC_RMID, NULL);
    exit(0);
}

int main(){
    char *HOME = getpwuid(getuid())->pw_dir;
    key_t server_key = ftok(HOME, SERVER_ID);
    SERVER_QUEUE = msgget(server_key, 0666);

    key_t client_key = ftok(HOME, getpid());
    CLIENT_QUEUE = msgget(client_key, IPC_CREAT | 0666);
    QUEUE = CLIENT_QUEUE;

    msg init;
    init.type = INIT;
    sprintf(init.message,"%d",CLIENT_QUEUE);
    msgsnd(SERVER_QUEUE, &init, MSG_LEN, 0);

    msg id;
    signal (SIGINT, handler);
    msgrcv(CLIENT_QUEUE, &id, MSG_LEN, INIT, 0);
    CLIENT_ID = atoi(id.message);
    if(CLIENT_ID == -1){
        printf("Too many clients..\n");
        exit(0);
    }
    printf("My ID: %d\n", CLIENT_ID);
    char command[MSG_LEN] = "";
    while(TRUE){
        msg answer;
        if(msgrcv(CLIENT_QUEUE, &answer, MSG_LEN, 0, IPC_NOWAIT)>=0){
            printf("%s\n", answer.message);
            if(answer.type == CONNECT){
                QUEUE = atoi(answer.message);
            }
            else if(answer.type == DISCONNECT){
                QUEUE = CLIENT_QUEUE;
            }
            else if(answer.type == STOP_SERVER){
                handler(SIGINT);
            }
            
        }
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(STDIN_FILENO, &read_set);
        struct timeval time;
        time.tv_sec = 0;
        time.tv_usec = 0;
        select(1, &read_set, NULL, NULL, &time);

        if(FD_ISSET(STDIN_FILENO, &read_set)){
            if(fgets(command, MSG_LEN, stdin) != NULL){
            
            msg message;
            message.sender_id = CLIENT_ID;
            message.type = SEND;
            if (strncmp(command, "LIST", 4) == 0){
                message.type = LIST;
                msgsnd(SERVER_QUEUE, &message, MSG_LEN, 0);
            }
            else if (strncmp(command, "DISCONNECT", 10) == 0){
                message.type = DISCONNECT;
                msgsnd(SERVER_QUEUE,&message,MSG_LEN,0);
            }
            else if (strncmp(command, "CONNECT", 7) == 0){
                if(QUEUE == CLIENT_QUEUE){
                    message.type = CONNECT;
                    sprintf(message.message, "%s", command+8);
                    msgsnd(SERVER_QUEUE, &message, MSG_LEN, 0);
                }
            }
            else if(strncmp(command, "STOP", 4) == 0){
                message.type=STOP;
                msgsnd(SERVER_QUEUE, &message, MSG_LEN, 0);
                msgctl(CLIENT_QUEUE, IPC_RMID, NULL);
                exit(0);
            }
            else{
                sprintf(message.message, "%s", command);
                msgsnd(QUEUE, &message, MSG_LEN, 0);
            }
        }
    }
    }
}