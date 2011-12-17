#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/stat.h>

#include "q.h"


/*
Check whether a queue is empty or not

RETURN:
    if empty, returns QEMPTY
    else returns QNOTEMPTY

ERROR:
    QNOTAVAILABLE   If the incoming queue
                    is not allocated  
*/
int empty_q(queue *q) 
{
    if (!q) {
        PRINT_INFO("queue not available");
        return QNOTAVAILABLE;
    }
    if (q->rear == QNULL && q->front == QNULL) 
        return QEMPTY;
    return QNOTEMPTY;
}


/*
Check whether a queue is full or not

RETURN:
    if full, returns QFULL
    else returns QNOTFULL

ERROR:
    QNOTAVAILABLE   If the incoming queue
                    is not allocated  
*/
int full_q(queue *q)
{
    if (!q) {
        PRINT_INFO("queue not available");
        return QNOTAVAILABLE;
    }
    if ((q->rear + 1) % q->size == q->front)
        return QFULL;
    return QNOTFULL;
}


/*
Display the content of the queue in the 
following order:

    rear ->  . . . data . . . -> front

ERROR:
    QNOTAVAILABLE   If the incoming queue
                    is not allocated  
    
    Q_OP_SUCCESS    If the elements in the 
                    queue are displayed
*/
int display_q(queue *q)
{
    int i;

    if (!q) {
        PRINT_INFO("queue not available");
        return QNOTAVAILABLE;
    }

    /* empty queue */
    if (q->rear == QNULL && q->front == QNULL) {
        printf("\n");
        return QEMPTY;
    }

    /* one element queue */
    if (q->rear == q->front) {
        printf("%3d \n", q->data[q->front]);
    }

    /* non-circular queue */
    if (q->rear > q->front) {
        for (i=q->rear; i >= q->front; i--)
            printf("%3d ", q->data[i]);
        printf("\n");
    }

    /*
    circular queue - print the following:
      -> rear to start-of-the-queue
      -> end-of-the-queue to front
    */
    if (q->rear < q->front) {
        for (i=q->rear ; i>= 0 ; i--)
            printf("%3d ", q->data[i]);
        for (i=q->size-1 ; i>= q->front ; i--)
            printf("%3d ", q->data[i]);
        printf("\n");
    }

    return Q_OP_SUCCESS;
} 


/*
Create a queue of a requested size

INPUT:
    1. Pointer to a pointer to a queue
    2. Size of the new queue

RETURN:
    Q_OP_FAILURE    Incoming ptr to ptr to queue
                    is not NULL
    MEMERR          Unable to allocate memory
    Q_OP_SUCCESS    Queue successfully created
*/
int create_q(queue **q, int size)
{
    queue *nq = NULL;
    int *p = NULL;
    int shm_q, shm_qdata; 

    if (*q) {
        PRINT_INFO("queue address not null");
        return Q_OP_FAILURE;
    }

    /* create a shared memory for the queue */
    shm_q = shmget (IPC_PRIVATE, sizeof(queue), 
                     IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 
    nq = (queue *) shmat (shm_q, 0, 0); 

    /* create a shared memory for the queue data */
    shm_qdata = shmget (IPC_PRIVATE, sizeof(int) * size, 
                        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 
    nq->data = (int *) shmat (shm_qdata, 0, 0); 

    nq->front = nq->rear = QNULL;
    nq->size = size;

    PRINT_INFO("queue created successfully");

    *q = nq;
    return Q_OP_SUCCESS;
}


/*
Delete a queue

INPUT:
    1. Pointer to a pointer to a queue

RETURN:
    QNOTAVAILABLE   Incoming ptr to ptr to 
                    queue is NULL
    Q_OP_SUCCESS    Queue delete successfully 
*/
int delete_q(queue **q)
{
    queue *oq = NULL;

    if (*q == NULL) {
        PRINT_INFO("queue not available");
        return QNOTAVAILABLE;
    }
    
    oq = *q;
    *q = NULL;

    /* detach the shared memory */
    shmdt(oq->data);
    shmdt(oq);
    
    PRINT_INFO("queue deleted successfully");

    return Q_OP_SUCCESS;
}


/*
Add an element to the rear of the queue

INPUT:
    1. Pointer to a queue
    2. element to be added

RETURN:
    QNOTAVAILABLE   Queue not available
    Q_OP_SUCCESS    Element successfully added
                    to the rear of the queue
    QFULL           Element not added because
                    queue is full
*/
int enqueue(queue *q, int data)
{
    char msg[100];

    if (!q) {
        PRINT_INFO("queue not available");
        return QNOTAVAILABLE;
    }

    /* full queue */
    if (full_q(q) == QFULL)
        return QFULL;

    /* empty queue - adding first element */
    if (q->rear == QNULL && q->front == QNULL) {
        q->rear++;
        q->front++;
        q->data[q->rear] = data;
        sprintf(msg, "%d enqueued successfully", data);
        PRINT_INFO(msg);
        return Q_OP_SUCCESS;
    }

    /* add to the rear of the queue */
    q->rear += 1;
    q->rear %= q->size;
    q->data[q->rear] = data;

    sprintf(msg, "%d enqueued successfully", data);
    PRINT_INFO(msg);
    
    //display_q(q);

    return Q_OP_SUCCESS;
}


/*
Remove an element from the front of the queue

INPUT:
    1. Pointer to the queue
    2. Pointer to store (and return) the element

RETURN:
    QNOTAVAILABLE   Queue is not available or 
                    invalid data pointer 
    QEMPTY          Empty queue - can't dequeue
    Q_OP_SUCCESS    Element successfully dequeued 
                    from the front of the queue
*/
int dequeue(queue *q, int *data)
{
    char msg[100];

    if (!q || !data) {
        PRINT_INFO("incoming queue or data is invalid");
        return QNOTAVAILABLE;
    }

    /* empty queue */
    if (empty_q(q) == QEMPTY)
        return QEMPTY;

    /* only one element queue.. so extract the value 
       and initialize front and rear to QNULL
    */
    if (q->front == q->rear) {
        *data = q->data[q->rear];
        q->rear = q->front = QNULL;
        sprintf(msg, "%d dequeued successfully", *data);
        PRINT_INFO(msg);
        return Q_OP_SUCCESS;
    }
    
    /* remove the element from the front */
    *data = q->data[q->front];
    q->front += 1;
    q->front %= q->size;

    sprintf(msg, "%d dequeued successfully", *data);
    PRINT_INFO(msg);

    //display_q(q);

    return Q_OP_SUCCESS;
}


/*  _END_   */

