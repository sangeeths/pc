
#define PARSE_SUCCESS 0
#define PARSE_FAILURE 1

/*
np  = # of producers
nc  = # of consumers
qs  = queue size
dbg = debug or not
rt  = rotate circular queue or not
*/

typedef struct _pc_cmd {
    unsigned int np;    
    unsigned int nc;
    unsigned int qs;
    unsigned int dbg;
    unsigned int cq;
} pc_cmd;


void usage();
void print_cmdline_args(pc_cmd *pc);
int value_in_range(int val, int min, int max);
int cmd_atoi(char *a);
int parse_cmdline_args(pc_cmd **pc, int argc, char *argv[]);


