#include "common.h"
#include "../common/io.h"
#include <arinc653/buffer.h>
#include <arinc653/partition.h>
#include <arinc653/time.h>
#include <arinc653/queueing.h>
#include <arinc653/sampling.h>

#define PORT_SIZE 16
#define SECOND 1000000000LL
#define PROCESS_PD 0.2*SECOND
#define PROCESS_DL 0.2*SECOND
#define PROCESS_DLT SOFT
//#define SAMPLING_PD 0.02 * SECOND
#define SAMPLING_PD 200000000LL
#define msg aircraft_dynamics_outs_t

typedef
enum {
    NO_OUT        =  0,    /*  no printf     */
    VERBOSE       =  1,    /*  all possible out : for debug    */
    CSV   	 =  2    /*  printf in CSV mode  */
} DISPLAY;

#define NO_OUT 0
#define VERBOSE 1
#define CSV 2

#define MODE CSV

// encapsulation of the message to be sent
typedef struct {
    unsigned x; // date
    float data; // useful info
    unsigned y; // ?? from original sourcecode
    RETURN_CODE_TYPE ret; // status of the message
}
__attribute__((packed)) aircraft_dynamics_outs_t; // this attribute optimize the space used by the structure
