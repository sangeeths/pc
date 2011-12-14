#include "pc.h"


#define QNULL -1

/*
#define DEBUG_FLAG 1

#define PRINT_INFO(msg) if (DEBUG_FLAG) {   \
    printf("%s : %s : %d : %s \n",          \
            __FILE__, __FUNCTION__,         \
            __LINE__, msg);                 \
    }
*/

/*
char *_q_msg[100] = { 
    "Queue operation success", 
    "Queue operation failure", 
    "Queue full", 
    "Queue empty",
    "Queue not empty",
    "Unable to allocate memory",
    "Queue not available"
};
*/

#define Q_OP_SUCCESS    0x00
#define Q_OP_FAILURE    0x01
#define QFULL           0x02
#define QEMPTY          0x03
#define QNOTEMPTY       0x04
#define MEMERR          0x05
#define QNOTAVAILABLE   0x06

#define QUEUE_SIZE 100

typedef struct _queue {
    int front;
    int rear;
    int data[QUEUE_SIZE];
    //int *data;
    int size;
}queue;


//void disp_q(queue *q);
int empty_q(queue *q);
int display_q(queue *q);
int create_q(queue **q, int size);
int delete_q(queue **q);
int enqueue(queue *q, int data);
int dequeue(queue *q, int *data);

