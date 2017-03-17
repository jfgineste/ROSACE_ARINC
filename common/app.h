#include "common.h"
#include "../common/io.h"
#include <arinc653/buffer.h>
#include <arinc653/partition.h>
#include <arinc653/time.h>
#include <arinc653/queueing.h>
#include <arinc653/sampling.h>

#define PORT_SIZE 16
#define SECOND 1000000000LL

// encapsulation of the message to be sent
typedef struct {
  unsigned x; // date
  float data; // useful info
  unsigned y; // ?? from original sourcecode
  RETURN_CODE_TYPE ret; // status of the message
}
__attribute__((packed)) msg; // this attribute optimize the space used by the structure
