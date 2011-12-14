#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include "pc-cmd.h"
#include "q.h"


int main(int argc, char *argv[])
{
    /* command line arguments - parsed */
    pc_cmd *pc = (pc_cmd *) malloc(sizeof(pc_cmd));

    /* global queue */
    queue *q = NULL;
    queue *shared_q = NULL;
    int qreturn = 0;
    int dq_data; 

    if (parse_cmdline_args(&pc, argc, argv) == PARSE_FAILURE) {
        usage();
        goto DEALLOCATE_AND_EXIT;
    }

    /* set the debug mode to the global debug flag */
    g_debug = pc->dbg;
    PRINT_INFO("cmdline args parsed successfully");
    print_cmdline_args(pc); 


    /* create queue */
    if (create_q(&q, pc->qs) != Q_OP_SUCCESS) {
        printf("ERROR: unable to create queue \n");
        goto DEALLOCATE_AND_EXIT;
    }

    PRINT_INFO("queue d created successfully");


    /* Allocate a shared memory segment.  */ 
    shm_id = shmget (IPC_PRIVATE, sizeof(*q),
                     IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 

    shared_q = (queue *) shmat (shm_id, 0, 0); 
     
    shared_q->front = shared_q->rear = QNULL;
    shared_q->size = QUEUE_SIZE;  

    enqueue(shared_q, 10);
    enqueue(shared_q, 20);
    enqueue(shared_q, 30);
    enqueue(shared_q, 40);
    dequeue(shared_q, &dq_data);
    enqueue(shared_q, 50);
    enqueue(shared_q, 60);
    enqueue(shared_q, 70);
    dequeue(shared_q, &dq_data);
    display_q(shared_q);
    //memcpy(shared_q, q, sizeof(*q));
    /*
    printf("shared_q->front     :   %d \n", shared_q->front);
    printf("shared_q->rear      :   %d \n", shared_q->rear);
    printf("shared_q->size      :   %d \n", shared_q->size);
    printf("shared_q->data      :   %p \n", shared_q->data);
    */

DEALLOCATE_AND_EXIT:    
    /* deallocate command line args */
    if (q)  free(q);
    if (pc) free(pc);

    return 0;
}

