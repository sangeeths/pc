#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "q.h"

/*
void disp_q(queue *q)
{
    int i;
    for (i=0; i<q->size; i++)
        printf("[%d]%d - ", i, q->data[i]);
    printf("\n");
    return;
}
*/



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
    if (q->rear == QNULL && q->front == QNULL) 
        return QEMPTY;

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

    if (*q) {
        PRINT_INFO("queue address not null");
        return Q_OP_FAILURE;
    }

    /* Create a queue */
    if ((nq = (queue *) malloc(sizeof(queue))) == NULL) {
        PRINT_INFO("unable to create a queue\n"); 
        return MEMERR;
    }

    /* Create elements of the queue */
    /*
    if ((p = (int *) malloc(sizeof(int) * size)) == NULL) {
        PRINT_INFO("unable to allocate memory for queue\n"); 
        free(nq);
        return MEMERR;
    }

    memset(p, 0, sizeof(int) * size);    
    */

    //nq->data = p;
    nq->front = nq->rear = QNULL;
    //nq->size = size;
    nq->size = QUEUE_SIZE;

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

    //free(oq->data);
    free(oq);
    oq = NULL;
    
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

    /* empty queue - adding first element */
    if (q->rear == QNULL && q->front == QNULL) {
        q->rear++;
        q->front++;
        q->data[q->rear] = data;
        sprintf(msg, "%d enqueued successfully", data);
        PRINT_INFO(msg);
        return Q_OP_SUCCESS;
    }

    /* if queue full, then return */
    if((q->rear + 1)% q->size == q->front) {
        sprintf(msg, "queue full - can't add %d", data);
        PRINT_INFO(msg);
        return QFULL;
    }

    /* add to the rear of the queue */
    q->rear += 1;
    q->rear %= q->size;
    q->data[q->rear] = data;

    sprintf(msg, "%d enqueued successfully", data);
    PRINT_INFO(msg);
    
    display_q(q);

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
    if (q->front == QNULL && q->rear == QNULL) {
        PRINT_INFO("queue empty - can't dequeue");
        return QEMPTY;
    }
     
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

    display_q(q);

    return Q_OP_SUCCESS;
}


/*  _END_   */

