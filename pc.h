
#include <sys/shm.h>

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

/* shared memory variables */
/*
#define SHM_KEY 1409198229

int shm_id;
char *data;
*/

/*
int segment_id; 
char* shared_memory; 
struct shmid_ds shmbuffer; 
int segment_size; 
const int shared_segment_size = 0x6400; 
*/

