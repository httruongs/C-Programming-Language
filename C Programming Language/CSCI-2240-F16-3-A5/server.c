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
#define DB_FILENAME "serverDB"

void readFromFile(FILE *db, linkedList *fname, linkedList *lname, linkedList *SID, linkedList *GPA)
{
    SREC *newRecord = malloc(sizeof(SREC));
    while(fread(newRecord, sizeof(SREC), 1, db)) {
        addNode(fname, newRecord, cmpByFName);
        addNode(lname, newRecord, cmpByLName);
        addNode(SID, newRecord, cmpBySID);
        addNode(GPA, newRecord, cmpByGPA);
    }
    free(newRecord);
}

void saveToFile(FILE *db, listNode *source)
{
    while(source != NULL) {
        fwrite(&(source->data), sizeof(SREC), 1, db);
        source = source->next;
    }
}

int main()
{
    /* Data structures to keep student records */
    linkedList byLName;
    linkedList byFName;
    linkedList bySID;
    linkedList byGPA;
    FILE *databaseFile;
    /* Socket connection variables */
    int listenFD, connectFD;
    int recvStatus;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    /* Variables for handling client requests*/
    int noBytes = 0;
    int commandCode;
    int srecCode;
    char statusOK[] = "OK";
    char statusBAD[] = "BAD";
    listNode *toSend;
    SREC *helperSREC;
    unsigned long toRemove;

    /* Initialize structures */
    memset(&byLName, 0, sizeof(byLName));
    memset(&byFName, 0, sizeof(byFName));
    memset(&bySID, 0, sizeof(bySID));
    memset(&byGPA, 0, sizeof(byGPA));
    /* Read database info from file if present */
    databaseFile = fopen(DB_FILENAME, "r");
    if(databaseFile != NULL) {
        readFromFile(databaseFile, &byFName, &byLName, &bySID, &byGPA);
        fclose(databaseFile);
    }
    /* Set up connection */
    listenFD = socket(AF_INET, SOCK_STREAM, 0);
    if(listenFD == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NUM);

    if(bind(listenFD, (struct sockaddr *)&server, sizeof(server)) == -1) {
        close(listenFD);
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    if(listen(listenFD, 1) == -1) {
        close(listenFD);
        perror("Listen error");
        exit(EXIT_FAILURE);
    }
    connectFD = accept(listenFD, (struct sockaddr *)&client, &clientSize);
    if(connectFD == -1) {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }
    while(1) {
        /* Main server loop */
        recvStatus = recv(connectFD, &commandCode, sizeof(commandCode), 0);
        if(recvStatus == -1) {
            close(connectFD);
            perror("Recv error");
            exit(EXIT_FAILURE);
        }
        if(recvStatus == 0) {
            fprintf(stderr, "Connection closed prematurely by client\n");
            close(connectFD);
            exit(EXIT_FAILURE);
        }
        switch(commandCode) {
            case 0:
                /* GET */
                recv(connectFD, &srecCode, sizeof(srecCode), 0);
                switch(srecCode) {
                    case 0:
                        toSend = byLName.first;
                        break;
                    case 1:
                        toSend = byFName.first;
                        break;
                    case 2:
                        toSend = bySID.first;
                        break;
                    case 3:
                        toSend = byGPA.first;
                        break;
                    case -1:
                        send(connectFD, &noBytes, sizeof(noBytes), 0);
                        break;
                }
                if(srecCode != -1) {
                    send(connectFD, &(byLName.size), sizeof(byLName.size), 0);
                    while(toSend != NULL) {
                        send(connectFD, &(toSend->data), sizeof(toSend->data), 0);
                        toSend = toSend->next;
                    }
                }
                break;
            case 1:
                /* PUT */
                helperSREC = malloc(sizeof(SREC));
                recv(connectFD, helperSREC, sizeof(SREC), 0);
                if(helperSREC->initial != 0) {
                    addNode(&byFName, helperSREC, cmpByFName);
                    addNode(&byLName, helperSREC, cmpByLName);
                    addNode(&bySID, helperSREC, cmpBySID);
                    addNode(&byGPA, helperSREC, cmpByGPA);
                }
                free(helperSREC);
                break;
            case 2:
                /* DELETE */
                /* Assuming the given SID is a valid unsigned long*/
                recv(connectFD, &toRemove, sizeof(toRemove), 0);
                if(!removeNode(&byFName, toRemove) && !removeNode(&byLName, toRemove) && !removeNode(&bySID, toRemove) && !removeNode(&byGPA, toRemove)) {
                    send(connectFD, statusOK, sizeof(statusOK), 0);
                }
                else {
                    send(connectFD, statusBAD, sizeof(statusBAD), 0);
                }
                break;
            case 3:
                /* STOP */
                databaseFile = fopen(DB_FILENAME, "w");
                if(databaseFile == NULL) {
                    fprintf(stderr, "Couldn't open the database file\n");
                    send(connectFD, statusBAD, sizeof(statusBAD), 0);
                    exit(EXIT_FAILURE);
                }
                saveToFile(databaseFile, byLName.first);
                send(connectFD, statusOK, sizeof(statusOK), 0);
                fclose(databaseFile);
                close(connectFD);
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Unknown command received\n");
                break;
        }
    }
    return 0;
}
