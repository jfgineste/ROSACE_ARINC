#include <string.h>
#include <stdio.h>
#include "../../common/app.h"
#include "../../common/app3_code.c"

static SAMPLING_PORT_ID_TYPE RHF, RAZF, RVZF, RQF, RVAF;
static SAMPLING_PORT_ID_TYPE WDELTAEC, WDELTATHC;

static void P3_process(void) {

    /**************************************************************
     *			P3 INITIALIZATION			*
     **************************************************************/
    RETURN_CODE_TYPE ret_pause;

    int num_instance_operatoire = 0;
    int num_instance_totale = 0;

    int pd_operatoire = 1;
//    int pd_totale = 20; // 1000ms / 50ms
    int pd_totale = 1; // 1000ms / 50ms
    MESSAGE_SIZE_TYPE len; // don't care
    VALIDITY_TYPE validity; // don't care

    // output (encapsulated) messages
    msg m_delta_ec, m_delta_thc;

    // input (encapsulated) messages
    msg m_Va_f, m_h_f, m_az_f, m_Vz_f, m_q_f;

    m_delta_ec.x = 1;
    m_delta_thc.x = 1;
    m_delta_ec.y = -1;
    m_delta_thc.y = -1;
#if (MODE == VERBOSE)
    unsigned last_m_h_f;
    unsigned last_m_az_f;
    unsigned last_m_Vz_f;
    unsigned last_m_q_f;
    unsigned last_m_Va_f;
#endif

#if (MODE==TIMER)
	clock_t start, finish; 
	double duration; 
#endif


    PERIODIC_WAIT(&ret_pause);
#if (MODE==VERBOSE)
    if (ret_pause!=NO_ERROR) {
        printf("\n\n[P3] PERIODIC_WAIT ERROR CODE : %d(1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);
    }
#endif

    /**************************************************************
     *			P3 END INITIALIZATION			*
     **************************************************************/

    /***************************************************************
     *			P3 DATA PROCESSING			 *
     **************************************************************/

    while (1) {
#if (MODE==TIMER)
	start = clock(); 
#endif

    if (num_instance_operatoire == 0) {

        /******************************************************************
         * 				P3 IN				*
         *****************************************************************/

//	printf("\nhf=%f,azf=%f,vzf=%f,qf=%f,vaf=%f\n",m_h_f.data, m_az_f.data, m_Vz_f.data, m_q_f.data, m_Va_f.data);
        READ_SAMPLING_MESSAGE(RHF, (MESSAGE_ADDR_TYPE)&m_h_f,&len,&validity,&m_h_f.ret);
#if (MODE==VERBOSE)
        if (m_h_f.ret == NO_ERROR) {
            printf("[P3] RHF: new message read: {%u, \"%f\", %u}\n", m_h_f.x, m_h_f.data, m_h_f.y);
        } else {
            printf("[P3] RHF: Unable to read message: %u\n", m_h_f.ret);
        }

        if (m_h_f.x < last_m_h_f) {
            printf("[P3] RH: warning: received message out of order\n");
        } else if (m_h_f.x > last_m_h_f + 1) {
            printf("[P3] RHF: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_h_f, m_h_f.x);
        } else if (m_h_f.x == last_m_h_f) {
            printf("[P3] RHF: warning: possible duplicate message (id=%d)\n", m_h_f.x);
        }
#endif

        // READ azf
        READ_SAMPLING_MESSAGE(RAZF, (MESSAGE_ADDR_TYPE)&m_az_f,&len,&validity,&m_az_f.ret);
#if (MODE==VERBOSE)
        if (m_az_f.ret == NO_ERROR) {
            printf("[P3] RAZF: new message read: {%u, \"%f\", %u}\n", m_az_f.x, m_az_f.data, m_az_f.y);
        } else {
            printf("[P3] RAZF: Unable to read message: %u\n", m_az_f.ret);
        }

        if (m_az_f.x < last_m_az_f) {
            printf("[P3] RAZF: warning: received message out of order\n");
        } else if (m_az_f.x > last_m_az_f + 1) {
            printf("[P3] RAZF: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_az_f, m_az_f.x);
        } else if (m_az_f.x == last_m_az_f) {
            printf("[P3] RAZF: warning: possible duplicate message (id=%d)\n", m_az_f.x);
        }
#endif

        // READ VZF
        READ_SAMPLING_MESSAGE(RVZF, (MESSAGE_ADDR_TYPE)&m_Vz_f,&len,&validity,&m_Vz_f.ret);
#if (MODE==VERBOSE)
        if (m_Vz_f.ret == NO_ERROR) {
            printf("[P3] RVZF: new message read: {%u, \"%f\", %u}\n", m_Vz_f.x, m_Vz_f.data, m_Vz_f.y);
        } else {
            printf("[P3] RVZF: Unable to read message: %u\n", m_Vz_f.ret);
        }

        if (m_Vz_f.x < last_m_Vz_f) {
            printf("[P3] RVZF: warning: received message out of order\n");
        } else if (m_Vz_f.x > last_m_Vz_f + 1) {
            printf("[P3] RVZF: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_Vz_f, m_Vz_f.x);
        } else if (m_Vz_f.x == last_m_Vz_f) {
            printf("[P3] RVZF: warning: possible duplicate message (id=%d)\n", m_Vz_f.x);
        }
#endif

        // READ QF
        READ_SAMPLING_MESSAGE(RQF, (MESSAGE_ADDR_TYPE)&m_q_f,&len,&validity,&m_q_f.ret);
#if (MODE==VERBOSE)
        if (m_q_f.ret == NO_ERROR) {
            printf("[P3] RQF: new message read: {%u, \"%f\", %u}\n", m_q_f.x, m_q_f.data, m_q_f.y);
        } else {
            printf("[P3] RQF: Unable to read message: %u\n", m_q_f.ret);
        }

        if (m_q_f.x < last_m_q_f) {
            printf("[P3] RQF: warning: received message out of order\n");
        } else if (m_q_f.x > last_m_q_f + 1) {
            printf("[P3] RQF: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_q_f, m_q_f.x);
        } else if (m_q_f.x == last_m_q_f) {
            printf("[P3] RQF: warning: possible duplicate message (id=%d)\n", m_q_f.x);
        }
#endif

        // READ VAF
        READ_SAMPLING_MESSAGE(RVAF, (MESSAGE_ADDR_TYPE)&m_Va_f,&len,&validity,&m_Va_f.ret);
#if (MODE==VERBOSE)
        if (m_Va_f.ret == NO_ERROR) {
            printf("[P3] RVAF: new message read: {%u, \"%f\", %u}\n", m_Va_f.x, m_Va_f.data, m_Va_f.y);
        } else {
            printf("[P3] RVAF: Unable to read message: %u\n", m_Va_f.ret);
        }

        if (m_Va_f.x < last_m_Va_f) {
            printf("[P3] RVAF: warning: received message out of order\n");
        } else if (m_Va_f.x > last_m_Va_f + 1) {
            printf("[P3] RVAF: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_Va_f, m_Va_f.x);
        } else if (m_Va_f.x == last_m_Va_f) {
            printf("[P3] RVAF: warning: possible duplicate message (id=%d)\n", m_Va_f.x);
        }
#endif

#if (MODE==VERBOSE)
        last_m_h_f = m_h_f.x;
        last_m_az_f = m_az_f.x;
        last_m_Vz_f = m_Vz_f.x;
        last_m_q_f = m_q_f.x;
        last_m_Va_f = m_Va_f.x;
#endif

        /******************************************************************
         * 				P3 END IN			*
         *****************************************************************/

        /******************************************************************
         * 				P3 OUT				*
         ******************************************************************/
        Vz_c = altitude_hold(m_h_f.data, h_c);
        m_delta_ec.data = Vz_control(m_Vz_f.data, Vz_c, m_q_f.data, m_az_f.data);
        WRITE_SAMPLING_MESSAGE(WDELTAEC, (MESSAGE_ADDR_TYPE)&m_delta_ec, sizeof(m_delta_ec),&m_delta_ec.ret);
        m_delta_thc.data = Va_control(m_Va_f.data, m_Vz_f.data, m_q_f.data, Va_c);
        WRITE_SAMPLING_MESSAGE(WDELTATHC, (MESSAGE_ADDR_TYPE)&m_delta_thc, sizeof(m_delta_thc),&m_delta_thc.ret);

#if (MODE==VERBOSE)
        printf("[P3] WDELTAEC: new message sent: {%u, \"%f\", %u}\n", m_delta_ec.x, m_delta_ec.data, m_delta_ec.y);
        printf("[P3] WDELTATHC: new message sent: {%u, \"%f\", %u}\n", m_delta_thc.x, m_delta_thc.data, m_delta_thc.y);

        if (m_delta_ec.ret != NO_ERROR) {
            printf("[P3] WDELTAEC: error during writing on sampling: %d\n", m_delta_ec.ret);
        }
        if (m_delta_thc.ret != NO_ERROR) {
            printf("[P3] WDELTATHC: error during writing on sampling: %u\n", m_delta_thc.ret);
        }
#endif

        m_delta_ec.x++;
        m_delta_thc.x++;
}

        /************************************************************
         * 				P3 END OUT			*
         ************************************************************/

//    }
#if (MODE==TIMER)
	finish = clock(); 
	duration = (double) (finish - start) / (double)CLOCKS_PER_SEC; 
	printf("%f,",duration);
#endif

#if (MODE==CSV)
        if (num_instance_totale == 0) {
            printf("%.15f,%.15f\n", m_delta_thc.data, m_delta_ec.data);
        }
#endif

        PERIODIC_WAIT(&ret_pause);

#if (MODE==VERBOSE)
        if (ret_pause!=NO_ERROR) {
            printf("\n\n[P3] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);
        }
#endif
        num_instance_operatoire = (num_instance_operatoire + 1) % pd_operatoire;
        num_instance_totale = (num_instance_totale + 1) % pd_totale;

    }

}

/************************************************************
 *			P3 END DATA PROCESSING			*
 ************************************************************/

int P3Main(void) {

    PROCESS_ID_TYPE pid_P3;
    RETURN_CODE_TYPE ret_process, ret_switch_mode;

    PROCESS_ATTRIBUTE_TYPE P3_process_attrs = {
        .PERIOD = PROCESS_PD, //other values are refused
        .TIME_CAPACITY = PROCESS_DL,
        .STACK_SIZE = 8096, // the only accepted stack size!
        .BASE_PRIORITY = MIN_PRIORITY_VALUE,
        .DEADLINE = PROCESS_DLT, //errors with hard
    };

    P3_process_attrs.ENTRY_POINT = P3_process;
    strncpy(P3_process_attrs.NAME, "P3_process", sizeof(PROCESS_NAME_TYPE));

    CREATE_PROCESS(&P3_process_attrs,&pid_P3,&ret_process);
#if (MODE==VERBOSE)
    if (ret_process != NO_ERROR) {
        printf("[P3] couldn't create P3_process: %d\n", (int) ret_process);
        return 1;
    } else {
        printf("[P3] P3_process  created\n");
    }
#endif

    START(pid_P3,&ret_process);
    if (ret_process != NO_ERROR) {
#if (MODE==VERBOSE)
        printf("[P3] couldn't start P3_process: %d\n", (int) ret_process);
#endif
        return 1;
    } else {
#if (MODE==VERBOSE)
        printf("[P3] P3_process started (it won't actually run until operating mode becomes NORMAL)\n");
#endif
    }

    RETURN_CODE_TYPE ret;
    CREATE_SAMPLING_PORT("WDELTAEC", PORT_SIZE, SOURCE, SAMPLING_PD,&WDELTAEC,&ret);
    CREATE_SAMPLING_PORT("WDELTATHC", PORT_SIZE, SOURCE, SAMPLING_PD,&WDELTATHC,&ret);

#if (MODE==VERBOSE)
    printf("[P3] Bilan create output ports: DELTA_EC=%d DELTA_THC=%d\n", (int) WDELTAEC, (int) WDELTATHC);
#endif

    CREATE_SAMPLING_PORT("RHF", PORT_SIZE, DESTINATION, SAMPLING_PD,&RHF,&ret);
    CREATE_SAMPLING_PORT("RAZF", PORT_SIZE, DESTINATION, SAMPLING_PD,&RAZF,&ret);
    CREATE_SAMPLING_PORT("RVZF", PORT_SIZE, DESTINATION, SAMPLING_PD,&RVZF,&ret);
    CREATE_SAMPLING_PORT("RQF", PORT_SIZE, DESTINATION, SAMPLING_PD,&RQF,&ret);
    CREATE_SAMPLING_PORT("RVAF", PORT_SIZE, DESTINATION, SAMPLING_PD,&RVAF,&ret);

#if (MODE==VERBOSE)
    printf("[P3] Bilan create input ports: HF=%d AZF=%d RVF=%d QF=%d VA=%d\n", (int) RHF, (int) RAZF, (int) RVZF, (int) RQF, (int) RVAF);
#endif


    SET_PARTITION_MODE(NORMAL,&ret_switch_mode);
#if (MODE==VERBOSE)
    printf("[P3] SWITCHED TO NORMAL \n");
#endif
    return 0;
}

void main(void) {
    P3Main();
    STOP_SELF();
}
