
#include <pthread.h>


#define PRODUCER_MIN    1
#define PRODUCER_MAX    10

#define CONSUMER_MIN    1
#define CONSUMER_MAX    10

#define QSIZE_MIN       1
#define QSIZE_MAX       100

/* global debug flag */
int g_debug;

#define PRINT_INFO(msg)                 \
if (g_debug) {                          \
    printf("%s : %s : %d : %s \n",      \
            __FILE__, __FUNCTION__,     \
            __LINE__, msg);             \
}

pthread_t *p_id;
pthread_t *c_id;
int *t_id;


