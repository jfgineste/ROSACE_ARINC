//specifies the scheduler used in each partition. This declaration is an array, each value n corresponds to the scheduler used for partition n.
#define POK_CONFIG_PARTITIONS_SCHEDULER {POK_SCHED_RMS,POK_SCHED_RMS,POK_SCHED_RMS} 

//The POK CONFIG SCHEDULING NBSLOTS specifies the number of time frames allocated for partitions execution.
#define POK_CONFIG_SCHEDULING_SLOTS {2,2,2} 

//specified the allocation of each slot. In other words, which partition is scheduling at which slot. The declaration is an array and the value n specifies which partition uses the slot n.
#define POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION {0,1,2,3} 

//specifies the number of time frames allocated for partitions execution.
#define POK_CONFIG_SCHEDULING_NBSLOTS 4 

//specifies the major frame, the time when interpartitions ports are flushed. It corresponds to the end of a scheduling cycle.
#define POK_CONFIG_SCHEDULING_MAJOR_FRAME 2000 
