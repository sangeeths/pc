#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <pthread.h>

#include "pc-cmd.h"
#include "q.h"

queue *q = NULL;
pthread_mutex_t q_mutex;

void * consume(void *id)
{
    int *tid = (int *) id;
    int q_data;
    int deq_ret_val;

    while (1) {
        pthread_mutex_lock(&q_mutex);

        deq_ret_val = dequeue(q, &q_data);

        if (deq_ret_val == QEMPTY)
            printf("%3d : queue empty  :  ", *tid);

        if (deq_ret_val == Q_OP_SUCCESS)
            printf("%3d : consumed %4d :  ", *tid, q_data);

        display_q(q);

        pthread_mutex_unlock(&q_mutex);
        sleep(2);
    }

    return;
}


//void produce(void *id, queue *q, pthread_mutex_t *q_mutex)
void * produce(void *id)
{
    int *tid = (int *) id;
    int enq_ret_val;
    int item = rand() % 99 + 1;

    while (1) {
        pthread_mutex_lock(&q_mutex);

        enq_ret_val = enqueue(q, item);

        if (enq_ret_val == QFULL)
            printf("%3d : queue full   :  ", *tid);

        if (enq_ret_val == Q_OP_SUCCESS)
            printf("%3d : produced %4d :  ", *tid, item);

        display_q(q);

        pthread_mutex_unlock(&q_mutex);
        sleep(2);
    }

    return;
}

int main(int argc, char *argv[])
{
    int t_id_i = 0;
    int i;
    /* command line arguments - parsed */
    pc_cmd *pc = (pc_cmd *) malloc(sizeof(pc_cmd));

    /* global queue */
    int qreturn = 0;
    int dq_data; 
    
    pthread_mutex_init(&q_mutex, NULL);

    if (parse_cmdline_args(&pc, argc, argv) == PARSE_FAILURE) {
        usage();
        if (pc) free(pc);
        return 0;
    }

    /* set the debug mode to the global debug flag */
    g_debug = pc->dbg;

    /* create queue */
    if (create_q(&q, pc->qs) != Q_OP_SUCCESS) {
        printf("ERROR: unable to create queue \n");
        if (pc) free(pc);
        return 0;
    }

    /* initialize mutex */
	pthread_mutex_init(&q_mutex, NULL);	

    if ((t_id = (int *) malloc (sizeof(int) * 
                (pc->nc + pc->np))) == NULL) {
        printf("ERROR: unable to create ids \n");
        if (pc) free(pc);
        delete_q(&q);
        return 0;
    }

	for (i=0 ; i < (pc->np + pc->nc) ; i++) 
        t_id[i] = i;

    /* create producer thread ids */
    if ((p_id = (pthread_t *) malloc(sizeof(pthread_t) * 
                 pc->np)) == NULL) {
        printf("ERROR: unable to create producer id\n");
        if (pc) free(pc);
        delete_q(&q);
        return 0;
    }


    /* create consumer thread ids */
    if ((c_id = (pthread_t *) malloc(sizeof(pthread_t) * 
                 pc->nc)) == NULL) {
        printf("ERROR: unable to create consumer id\n");
        if (pc) free(pc);
        delete_q(&q);
        return 0;
    }

    /* create producer threads */
	for (i=0 ; i<pc->np ; i++) {
        printf("Creating producer thread #%d \n", t_id[t_id_i]);
        if (pthread_create(&p_id[i], NULL, produce, &t_id[t_id_i])) {
			printf("unable to create producer thread! \n");
			exit(-1);
		} 
        t_id_i++; 
	}

    /* create consumer threads */
	for (i=0 ; i < pc->nc ; i++) {
        printf("Creating consumer thread #%d \n", t_id[t_id_i]);
        if (pthread_create(&c_id[i], NULL, consume, &t_id[t_id_i])) {
			printf("unable to create consumer thread! \n");
			exit(-1);
		} 
        t_id_i++;
	}

	for (i=0 ; i < pc->np ; i++) 
		pthread_join(p_id[i], NULL);
	for (i=0 ; i < pc->nc ; i++) 
		pthread_join(c_id[i], NULL);
	

    /* delete queue */
    delete_q(&q);

    /* destroy mutex */
	pthread_mutex_destroy(&q_mutex);

    /* exit all threads */
	pthread_exit(NULL);	

    return 0;
}

