#include <stdio.h>

#include "pc-cmd.h"
#include "pc.h"

/*
This function prints the command reference for 
the producer-consumer problem

The following are the mandatory parameters:
1. Number of producers (-p)
2. Number of consumers (-c)
3. Queue size (-q)

The following are the optional parameters:
1. Rotate circular queue (-r)
2. Print debug messages (-d)

*/
void usage() 
{
    printf("\n");
    printf("Usage:\n");
    printf("\n");
    printf("pc -p <no-of-producers> ");
    printf("-c <no-of-consumers> ");
    printf("-q <queue-size> -r -d\n");
    printf("\n");
    printf("-p   #  Number of producers (%d-%d)\n", 
                    PRODUCER_MIN, PRODUCER_MAX);
    printf("-c   #  Number of comsumers (%d-%d)\n", 
                    CONSUMER_MIN, CONSUMER_MAX);
    printf("-q   #  Queue size (%d-%d)\n", 
                    QSIZE_MIN, QSIZE_MAX);
    printf("-r      Rotate circular queue\n");
    printf("-d      Print debug messages\n");
    printf("\n");
    printf("# -> mandatory parameters\n");
    printf("\n");
    printf("Example:\n");
    printf("\n");
    printf("pc -p 5 -c 5 -q 50 -r\n");
    printf("\n");

    return;
}


/*
Prints the contents of the given pc_cmd structure.
This is used only for debugging purpose. 
*/
void print_cmdline_args(pc_cmd *pc)
{
    if (!pc)
        return;

    printf("Number of producers   : %d \n", pc->np);;
    printf("Number of consumers   : %d \n", pc->nc);;
    printf("Queue size            : %d \n", pc->qs);;
    printf("Rotate circular queue : %s \n", 
          (pc->cq)?"True":"False");
    printf("Debug mode            : %s \n", 
          (pc->dbg)?"True":"False");

    return;
}


/*
Check whether the given value is within the 
given range (min and max inclusive)

Return 1 if in range, else 0
*/
int value_in_range(int val, int min, int max) 
{
    return (val >= min && val <= max) ? 1 : 0;
}


/*
A simple/primitive string to integer 
conversion function. This is used to 
convert the incoming (char *) integer 
argv[] parameters into int.
*/
int cmd_atoi(char *a)
{
    int val = 0;
    while (*a) {
        val = (val * 10) + *a-'0';
        a++;
    }
    return val;
}


/*
Parse the command line arguments and fill the 
incoming pc_cmd structure. Ensure the mandatory
parameters are provided and are within the 
desired limits. In case of successful parsing, 
the incoming pc_cmd structure will be fill and 
PARSE_SUCCESS will be returned, else if failed
parsing, then PARSE_FAILURE will be returned and 
so the pc_cmd structure will be in an insane
state.

The 'done' variables are used as flag to avoid
duplicate command line arguments for the same 
functionality (f.e. entering -p twice, etc).
*/
int parse_cmdline_args(pc_cmd **cmd, int argc, 
                       char *argv[])
{
    int n = 0;
    int val = 0;
    pc_cmd *pc = *cmd;
    pc_cmd done = {0};

    for(n=1; n<argc; n++) {
        switch((int)argv[n][0]) {
        case '-':   
            switch((int)argv[n][1]) {
            case'p':
                if(done.np) return PARSE_FAILURE;
                if (n+1 >= argc) return PARSE_FAILURE;
                val = cmd_atoi(argv[n+1]); n++;
                if(!value_in_range(val, 
                    PRODUCER_MIN, PRODUCER_MAX))
                    return PARSE_FAILURE;
                pc->np = val;
                done.np = 1;
                break;
            case'c':
                if(done.nc) return PARSE_FAILURE;
                if (n+1 >= argc) return PARSE_FAILURE;
                val = cmd_atoi(argv[n+1]); n++;
                if(!value_in_range(val, 
                    CONSUMER_MIN, CONSUMER_MAX))
                    return PARSE_FAILURE;
                pc->nc = val;
                done.nc = 1;
                break;
            case'q':
                if(done.qs) return PARSE_FAILURE;
                if (n+1 >= argc) return PARSE_FAILURE;
                val = cmd_atoi(argv[n+1]); n++;
                if(!value_in_range(val, 
                    QSIZE_MIN, QSIZE_MAX))
                    return PARSE_FAILURE;
                pc->qs = val;
                done.qs = 1;
                break;
            case'r':
                if(done.cq)
                    return PARSE_FAILURE;
                pc->cq = done.cq = 1;
                break;
            case'd':
                if(done.dbg)
                    return PARSE_FAILURE;
                pc->dbg = done.dbg = 1;
                break;
            default:
                return PARSE_FAILURE;
            }
            break;
        default:
            return PARSE_FAILURE;
            break;
        }
    }

    /* check for the mandatory parameters */
    if (pc->np == 0 || pc->nc == 0 || pc->qs == 0) 
        return PARSE_FAILURE;

    return PARSE_SUCCESS;
}


/*  _END_   */

