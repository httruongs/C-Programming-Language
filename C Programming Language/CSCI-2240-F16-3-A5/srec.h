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

#ifndef SREC_H
#define SREC_H

#include <malloc.h>
#include <string.h>

typedef struct student {
    char lname[10], initial, fname[10];
    unsigned long SID;
    float GPA;
} SREC;

typedef struct listnode {
    SREC data;
    struct listnode *next;
} listNode;

typedef struct linkedlist {
    listNode *first;
    int size;
} linkedList;

void SRECcpy(SREC *dest, SREC *src)
{
    strncpy(dest->lname, src->lname, 10);
    strncpy(dest->fname, src->fname, 10);
    dest->initial = src->initial;
    dest->SID = src->SID;
    dest->GPA = src->GPA;
}

void addNode(linkedList *lst, SREC *src, int (*compare)(SREC *, SREC *))
{
    listNode *prev = NULL;
    listNode *current = lst->first;
    if(current == NULL) {
        /* Empty list, add to beginning */
        lst->first = malloc(sizeof(listNode));
        SRECcpy(&(lst->first->data), src);
        lst->first->next = NULL;
        lst->size = 1;
        return;
    }
    while(current != NULL && compare(&(current->data), src) < 0) {
        prev = current;
        current = current->next;
    }
    if(current == NULL) {
        /* Add to the end of the list */
        prev->next = malloc(sizeof(listNode));
        SRECcpy(&(prev->next->data), src);
        prev->next->next = NULL;
    }
    else {
        if(prev == NULL) {
            /* Add before first element */
            current = lst->first;
            lst->first = malloc(sizeof(listNode));
            SRECcpy(&(lst->first->data), src);
            lst->first->next = current;
        }
        else {
            /* Add somewhere in the middle */
            prev->next = malloc(sizeof(listNode));
            SRECcpy(&(prev->next->data), src);
            prev->next->next = current;
        }
    }
    lst->size++;
}

int removeNode(linkedList *lst, unsigned long SID)
{
    listNode *prev = NULL;
    listNode *current = lst->first;
    listNode *tmp = NULL;
    while(current != NULL && (current->data).SID != SID) {
        prev = current;
        current = current->next;
    }
    if(current != NULL) {
        if(current == lst->first) {
            /* Remove from beggining of list */
            tmp = lst->first;
            lst->first = lst->first->next;
            /* Deallocate removed node */
            free(tmp);
        }
        else {
            /* Remove from the middle of list */
            prev->next = current->next;
            /* Deallocate */
            free(current);
        }
        /* Node to remove found */
        lst->size--;
        return 0;
    }
    /* Node to remove not found */
    return -1;
}

int cmpByFName(SREC *first, SREC *second)
{
    return strncmp(first->fname, second->fname, 9);
}

int cmpByLName(SREC *first, SREC *second)
{
    return strncmp(first->lname, second->lname, 9);
}

int cmpBySID(SREC *first, SREC *second)
{
    if(first->SID > second->SID) {
        return 1;
    }
    else if(first->SID == second->SID) {
        return 0;
    }
    else {
        return -1;
    }
}

int cmpByGPA(SREC *first, SREC *second)
{
    /* Descending order */
    if(first->GPA > second->GPA) {
        return -1;
    }
    else if(first->GPA == second->GPA) {
        return 0;
    }
    else {
        return 1;
    }
}

#endif
