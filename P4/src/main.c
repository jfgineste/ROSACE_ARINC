#include <string.h>
#include <stdio.h>
#include "../../common/app.h"
#include "../../common/app_code.c"

static SAMPLING_PORT_ID_TYPE RDELTAEC, RDELTATHC;
static SAMPLING_PORT_ID_TYPE WDELTAE;

static void P4_process(void) {

    /************************************************************
     *			P4 INITIALIZATION			*
     ************************************************************/

    RETURN_CODE_TYPE ret_pause;
    // output (encapsulated) messages
    msg m_delta_e;

    // input (encapsulated) messages
    msg m_delta_ec, m_delta_thc;

    //float T;
    float delta_e;
    //struct aircraft_dynamics_outs_t res; //structure pour accueillir les résultats de aircraft_dynamics

    MESSAGE_SIZE_TYPE len; // don't care
    VALIDITY_TYPE validity; // don't care
    unsigned last_m_delta_ec = 0;
    unsigned last_m_delta_thc = 0;

//  int num_instance = 0;
//  int pd = 1000;
    delta_e = 0.012009615652468;

    m_delta_e.x = 1;
    m_delta_e.data = delta_e;
    m_delta_e.y = -1;


    PERIODIC_WAIT(&ret_pause);
    if (ret_pause!=NO_ERROR) {
#if (MODE==VERBOSE)
        printf("\n\n[P4] PERIODIC_WAIT ERROR CODE : %d \n\n",ret_pause);
#endif
    }

    /************************************************************
     *			P4 END INITIALIZATION			*
     ************************************************************/

    /************************************************************
     *			P4 DATA PROCESSING			*
     ************************************************************/

    while (1) {

        /************************************************************
         *	P4 IN (THE ONLY PARTITION WHICH SENDS BEFORE RECEIVING)	*
         ************************************************************/
        //READ DELTAEC
        READ_SAMPLING_MESSAGE(RDELTAEC, (MESSAGE_ADDR_TYPE)&m_delta_ec,&len,&validity,&m_delta_ec.ret);
#if (MODE==VERBOSE)
        if (m_delta_ec.ret == NO_ERROR) {
            //if (num_instance % pd == 0) {
            printf("[P4] RDELTAEC: new message read: {%u, \"%f\", %u}\n", m_delta_ec.x, m_delta_ec.data, m_delta_ec.y);
            //}
        } else {
            printf("[P4] RDELTAEC: Unable to read message: %u\n", m_delta_ec.ret);
        }

        if (m_delta_ec.x < last_m_delta_ec) {
            printf("[P4] RDELTAEC: warning: received message out of order\n");
        } else if (m_delta_ec.x > last_m_delta_ec + 1) {
            printf("[P4] RDELTAEC: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_delta_ec, m_delta_ec.x);
        } else if (m_delta_ec.x == last_m_delta_ec) {
            printf("[P4] RDELTAEC: warning: possible duplicate message (id=%d)\n", m_delta_ec.x);
        }
#endif

        // READ DELTAETHC
        READ_SAMPLING_MESSAGE(RDELTATHC, (MESSAGE_ADDR_TYPE)&m_delta_thc,&len,&validity,&m_delta_thc.ret);
#if (MODE==VERBOSE)
        if (m_delta_thc.ret == NO_ERROR) {
            //if (num_instance % pd == 0) {
            printf("[P4] RDELTATHC: new message read: {%u, \"%f\", %u}\n", m_delta_thc.x, m_delta_thc.data, m_delta_thc.y);
            //}
        } else {
            printf("[P4] RDELTATHC: Unable to read message: %u\n", m_delta_thc.ret);
        }

        if (m_delta_thc.x < last_m_delta_thc) {
            printf("[P4] RDELTATHC: warning: received message out of order\n");
        } else if (m_delta_thc.x > last_m_delta_thc + 1) {
            printf("[P4] RDELTATHC: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_delta_thc, m_delta_thc.x);
        } else if (m_delta_thc.x == last_m_delta_thc) {
            printf("[P4] RDELTATHC: warning: possible duplicate  (id=%d)\n", m_delta_thc.x);
        }
#endif

        last_m_delta_ec = m_delta_ec.x;
        last_m_delta_thc = m_delta_thc.x;


        /************************************************************
         *				P4 END IN		 	*
         ************************************************************/

        m_delta_e.data = elevator(m_delta_ec.data);
#if (MODE==VERBOSE)
        printf("[P4] >>>> %f ; %f <<<<\n",m_delta_ec.data,elevator(m_delta_ec.data));
#endif
        WRITE_SAMPLING_MESSAGE(WDELTAE, (MESSAGE_ADDR_TYPE)&m_delta_e, sizeof(m_delta_e),&m_delta_e.ret);
#if (MODE==VERBOSE)
        printf("[P4] WDELTAE: new message sent: {%u, \"%f\", %u}\n", m_delta_e.x, m_delta_e.data, m_delta_e.y);
#endif
        //pause until next slot
        m_delta_e.x++;
        PERIODIC_WAIT(&ret_pause);
        if (ret_pause!=NO_ERROR) {
#if (MODE==VERBOSE)
            printf("\n\n[P4] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);
#endif
        }

    }
}

/************************************************************			P4 END DATA PROCESSING			*
 ************************************************************/

int P4Main(void) {

    PROCESS_ID_TYPE pid_P4;
    RETURN_CODE_TYPE ret_process, ret_switch_mode;


    PROCESS_ATTRIBUTE_TYPE P4_process_attrs = {
        .PERIOD = PROCESS_PD, //other values are refused
        .TIME_CAPACITY = PROCESS_DL,
        .STACK_SIZE = 8096, // the only accepted stack size!
        .BASE_PRIORITY = MIN_PRIORITY_VALUE,
        .DEADLINE = PROCESS_DLT, //errors with hard
    };

    P4_process_attrs.ENTRY_POINT = P4_process;
    strncpy(P4_process_attrs.NAME, "P4_process", sizeof(PROCESS_NAME_TYPE));

    CREATE_PROCESS(&P4_process_attrs,&pid_P4,&ret_process);
    if (ret_process != NO_ERROR) {
#if (MODE==VERBOSE)
        printf("[P4] couldn't create P4_process: %d\n", (int) ret_process);
#endif
        return 1;
    } else {
#if (MODE==VERBOSE)
        printf("[P4] P4_process  created\n");
#endif
    }

    START(pid_P4,&ret_process);
    if (ret_process != NO_ERROR) {
#if (MODE==VERBOSE)
        printf("[P4] couldn't start process_1_in: %d\n", (int) ret_process);
#endif
        return 1;
    } else {
#if (MODE==VERBOSE)
        printf("[P4] P4_process started (it won't actually run until operating mode becomes NORMAL)\n");
#endif
    }

    RETURN_CODE_TYPE ret;
    CREATE_SAMPLING_PORT("WDELTAE", PORT_SIZE, SOURCE, SAMPLING_PD,&WDELTAE,&ret);

#if (MODE==VERBOSE)
    printf("[P4] Bilan create output ports: DELTAE=%d\n", (int) WDELTAE);
#endif

    CREATE_SAMPLING_PORT("RDELTAEC", PORT_SIZE, DESTINATION, SAMPLING_PD,&RDELTAEC,&ret);
    CREATE_SAMPLING_PORT("RDELTATHC", PORT_SIZE, DESTINATION, SAMPLING_PD,&RDELTATHC,&ret);

#if (MODE==VERBOSE)
    printf("[P4] Bilan create input ports: DELTA_EC=%d DELTA_THC=%d\n", (int) RDELTAEC, (int) RDELTATHC);
#endif

    SET_PARTITION_MODE(NORMAL,&ret_switch_mode);
#if (MODE==VERBOSE)
    printf("[P4] SWITCHED TO NORMAL \n");
#endif
    STOP_SELF();
    return 0;
}

void main(void) {
    P4Main();
    STOP_SELF();
}
