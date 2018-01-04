/* Name : Huong Truong
 * Class : CSCI 2240-003
 * Program # : 5
 * Due Date : Dec 9 2016
 *
 * Honor Pledge: On my honor as a student of the University
 * of Nebraska at Omaha, I have neither given nor received
 * unauthorized help on this homework assignment.
 *
 * NAME: Huong Truong
 * EMAIL: httruong@unomaha.edu
 * NUID: 52233854
 * Colleagues: None
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "srec.h"

#define PORT_NUM 20690

void prettyPrint(SREC *record)
{
    if(record == NULL) {
        printf("| %-5s | %-10s | %-10s | %-1s | %-4s |\n", "SID", "Fname", "Lname", "M", "GPA");
        printf("+-------+------------+------------+---+------+\n");
    }
    else {
        printf("| %05lu | %-10s | %-10s | %-1c | %-.2f |\n", record->SID, record->fname, record->lname, record->initial, record->GPA);
    }
}

int parseBuffer(char *buffer, char **command, char **argument)
{

    if(!strcmp(buffer, "stop\n")) {
        *command = buffer;
        (*command)[4] = '\0';
        return 0;
    }
    *command = strtok(buffer, " ");
    if(*command == NULL) {
        return 1;
    }
    *argument = strtok(NULL, " ");
    if(*argument == NULL) {
        return 1;
    }
    /* Get rid of trailing newline */
    (*argument)[strlen(*argument) - 1] = '\0';
    return 0;
}

int getCmdCode(char *command)
{
    if(!strcmp(command, "get")) {
        return 0;
    }
    else if(!strcmp(command, "put")) {
        return 1;
    }
    else if(!strcmp(command, "delete")) {
        return 2;
    }
    else if(!strcmp(command, "stop")) {
        return 3;
    }
    else {
        return -1;
    }
}

int parseSRECName(char *argument)
{
    if(!strcmp(argument, "lname")) {
        return 0;
    }
    else if(!strcmp(argument, "fname")) {
        return 1;
    }
    else if(!strcmp(argument, "SID")) {
        return 2;
    }
    else if(!strcmp(argument, "GPA")) {
        return 3;
    }
    else {
        return -1;
    }
}

SREC *getSREC(char *argument)
{
    SREC *newSREC = malloc(sizeof(SREC));
    char *buf;

    buf = strtok(argument, ",");
    if(buf == NULL) {
        free(newSREC);
        return NULL;
    }
    strncpy(newSREC->lname, buf, 10);
    newSREC->lname[9] = '\0';

    buf = strtok(NULL, ",");
    if(buf == NULL) {
        free(newSREC);
        return NULL;
    }
    strncpy(newSREC->fname, buf, 10);
    newSREC->fname[9] = '\0';

    buf = strtok(NULL, ",");
    if(buf == NULL) {
        free(newSREC);
        return NULL;
    }
    newSREC->initial = buf[0];

    buf = strtok(NULL, ",");
    if(buf == NULL) {
        free(newSREC);
        return NULL;
    }
    /* Assuming SID is always valid */
    newSREC->SID = strtoul(buf, NULL, 0);

    buf = strtok(NULL, "\n");
    if(buf == NULL) {
        free(newSREC);
        return NULL;
    }
    /* Assuming GPA is always valid */
    newSREC->GPA = strtod(buf, NULL);

    return newSREC;
}

int main()
{
    /* Socket connection variables */
    int connectFD;
    struct sockaddr_in server;
    socklen_t serverSize = sizeof(server);
    /* Server return variables */
    SREC recordReceived;
    int cnt;
    int numRecords;
    char status[4];
    /* Client request variables */
    int sendStatus;
    char inputBuf[120];
    char *command;
    char *argument;
    int cmdCode;
    int srecCode;
    SREC *newRecord;
    unsigned long toRemove;

    /* Set up connection */
    connectFD = socket(AF_INET, SOCK_STREAM, 0);
    if(connectFD == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* localhost */
    server.sin_port = htons(PORT_NUM);

    if(connect(connectFD, (struct sockaddr *)&server, serverSize) == -1) {
        close(connectFD);
        perror("Connect error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        /* Main client loop */
        printf("Enter next command> ");
        if(fgets(inputBuf, sizeof(inputBuf), stdin) == NULL) {
            printf("\nError getting the command, please try again.\n");
            continue;
        }
        if(parseBuffer(inputBuf, &command, &argument)) {
            printf("\nWrong command format, please try again.\n");
            continue;
        }
        cmdCode = getCmdCode(command);
        sendStatus = send(connectFD, &cmdCode, sizeof(cmdCode), 0);
        if(sendStatus == -1) {
            close(connectFD);
            perror("Send error");
            exit(EXIT_FAILURE);
        }
        switch(cmdCode) {
            case 0:
                /* GET */
                srecCode = parseSRECName(argument);
                send(connectFD, &srecCode, sizeof(srecCode), 0);
                recv(connectFD, &numRecords, sizeof(numRecords), 0);
                if(srecCode == -1) {
                    printf("Incorrect argument to get, please try again.\n");
                    break;
                }
                printf("\n");
                prettyPrint(NULL);
                cnt = 0;
                while(cnt < numRecords) {
                    recv(connectFD, &recordReceived, sizeof(SREC), 0);
                    prettyPrint(&recordReceived);
                    cnt++;
                }
                if(cnt > 0) {
                    printf("+-------+------------+------------+---+------+\n");
                }
                printf("\n");
                break;
            case 1:
                /* PUT */
                newRecord = getSREC(argument);
                if(newRecord == NULL) {
                    printf("Wrong SREC format input. Please try again.\n");
                    newRecord = malloc(sizeof(SREC));
                    memset(newRecord, 0, sizeof(SREC));
                }
                send(connectFD, newRecord, sizeof(SREC), 0);
                free(newRecord);
                break;
            case 2:
                /* DELETE */
                toRemove = strtoul(argument, NULL, 10);
                send(connectFD, &toRemove, sizeof(toRemove), 0);
                recv(connectFD, status, sizeof(status), 0);
                if(!strcmp(status, "BAD")) {
                    printf("Server returned an error status - no student with given SID.\n");
                }
                break;
            case 3:
                /* STOP */
                recv(connectFD, status, sizeof(status), 0);
                if(!strcmp(status, "BAD")) {
                    printf("Server returned an error status - database not saved.\n");
                }
                close(connectFD);
                exit(EXIT_SUCCESS);
                break;
            case -1:
                printf("Unknown command. Please try again.\n");
                break;
        }
    }
    return 0;
}
