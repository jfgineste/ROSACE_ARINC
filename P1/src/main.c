#include <string.h> 
#include <stdio.h>

#include "../../common/app.h"
#include "../../common/app_code.c"

static SAMPLING_PORT_ID_TYPE RDELTAE, RT;
static SAMPLING_PORT_ID_TYPE WH, WAZ, WVZ, WQ, WVA;

static void P1_process(void) {

    /************************************************************
     *			P1 INITIALIZATION			*
     ************************************************************/

    RETURN_CODE_TYPE ret_pause;

    // output (encapsulated) messages
    msg m_Va, m_h, m_az, m_Vz, m_q;

    // input (encapsulated) messages
    msg m_delta_e, m_T;
    unsigned last_m_T = 0;

    float T;
    struct aircraft_dynamics_outs_t res; //structure pour accueillir les résultats de aircraft_dynamics

    MESSAGE_SIZE_TYPE len; // don't care
    VALIDITY_TYPE validity; // don't care
#if (MODE==VERBOSE)
    unsigned last_m_delta_e = 0;
#endif
    int num_instance = 0;
//    int pd = 1000;
    int pd = 33;


#if (MODE==TIMER)
	clock_t start, finish; 
	double duration; 
#endif

    m_h.x = 1;
    m_az.x = 1;
    m_Vz.x = 1;
    m_q.x = 1;
    m_Va.x = 1;
    m_h.y = -1;
    m_az.y = -1;
    m_Vz.y = -1;
    m_q.y = -1;
    m_Va.y = -1;
    T = 41813.92119463;

    static int initialized=0;
    PERIODIC_WAIT(&ret_pause);

#if (MODE==VERBOSE)
    if (ret_pause!=NO_ERROR) {
        printf("\n\n[P1] PERIODIC_WAIT ERROR CODE : %d \n\n",ret_pause);
    }
#endif

    /************************************************************
     *			P1 END INITIALIZATION			*
     ************************************************************/

    /************************************************************
     *			P1 DATA PROCESSING			*
     ************************************************************/

    while (1) {
#if (MODE==TIMER)
	start = clock(); 
#endif

        /************************************************************
         *  P1 OUT (THE ONLY PARTITION WHICH RECEIVES AFTER SENDING) *
         ************************************************************/
        if (initialized>0) {
            //READ DELTAE
            READ_SAMPLING_MESSAGE(RDELTAE, (MESSAGE_ADDR_TYPE)&m_delta_e,&len,&validity,&m_delta_e.ret);

#if (MODE==VERBOSE)
            if (m_delta_e.ret == NO_ERROR) {
//		if (num_instance % pd == 0) {
                printf("[P1] RDELTAE: new message read: {%u, \"%f\", %u}\n", m_delta_e.x, m_delta_e.data, m_delta_e.y);
//		}
            } else {
                printf("[P1] RDELTAE: Unable to read message: %u\n", m_delta_e.ret);
            }

            if (m_delta_e.x < last_m_delta_e) {
                printf("[P1] RDELTAEC: warning: received message out of order\n");
            } else if (m_delta_e.x > last_m_delta_e + 1) {
                printf("[P1] RDELTAEC: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_delta_e, m_delta_e.x);
            } else if (m_delta_e.x == last_m_delta_e) {
                printf("[P1] RDELTAEC: warning: possible duplicate message (id=%d)\n", m_delta_e.x);
            }
            last_m_delta_e = m_delta_e.x;
#endif

            //READ T
            READ_SAMPLING_MESSAGE(RT, (MESSAGE_ADDR_TYPE)&m_T,&len,&validity,&m_T.ret);

#if (MODE==VERBOSE)
            if (m_T.ret == NO_ERROR) {
//		if (num_instance % pd == 0) {
                printf("[P1] RT: new message read: {%u, \"%f\", %u}\n", m_T.x, m_T.data, m_T.y);
//		}
            } else {
                printf("[P1] RT: Unable to read message: %u\n", m_T.ret);
            }

            if (m_T.x < last_m_T) {
                printf("[P1] RTC: warning: received message out of order\n");
            } else if (m_T.x > last_m_T + 1) {
                printf("[P1] RTC: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_T, m_T.x);
            } else if (m_T.x == last_m_T) {
                printf("[P1] RTC: warning: possible duplicate message (id=%d)\n", m_T.x);
            }
            last_m_T = m_T.x;
#endif
        }
        initialized=1;
        aircraft_dynamics(m_delta_e.data, m_T.data,&res);

        m_h.data = res.h;
        m_az.data = res.az;
        m_Vz.data = res.Vz;
        m_q.data = res.q;
        m_Va.data = res.Va;

//	printf("\nWh=%f,az=%f,vz=%f,q=%f,va=%f\n",m_h.data, m_az.data, m_Vz.data, m_q.data, m_Va.data);

        WRITE_SAMPLING_MESSAGE(WH, (MESSAGE_ADDR_TYPE)&m_h, sizeof(m_h),&m_h.ret);
        WRITE_SAMPLING_MESSAGE(WAZ, (MESSAGE_ADDR_TYPE)&m_az, sizeof(m_az),&m_az.ret);
        WRITE_SAMPLING_MESSAGE(WVZ, (MESSAGE_ADDR_TYPE)&m_Vz, sizeof(m_Vz),&m_Vz.ret);
        WRITE_SAMPLING_MESSAGE(WQ, (MESSAGE_ADDR_TYPE)&m_q, sizeof(m_q),&m_q.ret);
        WRITE_SAMPLING_MESSAGE(WVA, (MESSAGE_ADDR_TYPE)&m_Va, sizeof(m_Va),&m_Va.ret);




#if MODE == VERBOSE
//    if (num_instance % pd == 0) {
        printf("MODE=%d, NO_OUT=%d, VERBOSE=%d, CSV=%d\n",MODE, NO_OUT, VERBOSE, CSV);
        printf("num_instance %d.%04d\n", num_instance / 1000, num_instance - (num_instance / 1000) * 1000);
        printf("[P1] WH: new message sent: {%u, \"%f\", %u}\n", m_h.x, m_h.data, m_h.y);
        printf("[P1] WAZ: new message sent: {%u, \"%f\", %u}\n", m_az.x, m_az.data, m_az.y);
        printf("[P1] WVZ: new message sent: {%u, \"%f\", %u}\n", m_Vz.x, m_Vz.data, m_Vz.y);
        printf("[P1] WQ: new message sent: {%u, \"%f\", %u}\n", m_q.x,m_q.data, m_q.y);
        printf("[P1] WVA: new message sent: {%u, \"%f\", %u}\n", m_Va.x, m_Va.data, m_Va.y);
//    }
#endif



#if MODE==CSV
    if (num_instance % pd == 0) {
        printf("%d.%03d00000000000,%.15f,%.15f,%.15f,%.15f,%.15f,", num_instance / 1000, num_instance - (num_instance / 1000) * 1000,m_Va.data, m_az.data, m_q.data, m_Vz.data, m_h.data);
//printf("%d.%04d,", num_instance / 1000, num_instance - (num_instance / 1000) * 1000);
    }

#endif
        num_instance += 33; // un cycle=33ms

        // run during 10 minutes
        if (num_instance > (1000 * 60 * 10)) {
            STOP_SELF();
        }
        m_h.x++;
        m_az.x++;
        m_Vz.x++;
        m_q.x++;
        m_Va.x++;


        /************************************************************
         *				P1 END OUT		 	*
         ************************************************************/

#if (MODE==TIMER)
	finish = clock(); 
	duration = (double) (finish - start) / (double)CLOCKS_PER_SEC; 
	printf("%f,",duration);
#endif


        //PERIOD WAIT BETWEEN SEND AND RECEIVE
        PERIODIC_WAIT(&ret_pause);

#if (MODE==VERBOSE)
        if (ret_pause!=NO_ERROR) {
            printf("\n\n[P1] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);
        }
#endif

    }
}


/************************************************************			P1 END DATA PROCESSING			*
 ************************************************************/

int P1Main(void) {

    PROCESS_ID_TYPE pid_P1;
    RETURN_CODE_TYPE ret_process, ret_switch_mode;


    PROCESS_ATTRIBUTE_TYPE P1_process_attrs = {
        .PERIOD = PROCESS_PD, //other values are refused
        .TIME_CAPACITY = PROCESS_DL,
        .STACK_SIZE = 8096, // the only accepted stack size!
        .BASE_PRIORITY = MIN_PRIORITY_VALUE,
        .DEADLINE = PROCESS_DLT, //errors with hard
    };

    P1_process_attrs.ENTRY_POINT = P1_process;
    strncpy(P1_process_attrs.NAME, "P1_process", sizeof(PROCESS_NAME_TYPE));

    CREATE_PROCESS(&P1_process_attrs,&pid_P1,&ret_process);

#if (MODE==VERBOSE)
    if (ret_process != NO_ERROR) {
        printf("[P1] couldn't create P1_process: %d\n", (int) ret_process);
        return 1;
    } else {
        printf("[P1] P1_process  created\n");
    }
#endif
    START(pid_P1,&ret_process);
#if (MODE==VERBOSE)
    if (ret_process != NO_ERROR) {
        printf("[P1] couldn't start process_1_in: %d\n", (int) ret_process);
        return 1;
    } else {
        printf("[P1] P1_process started (it won't actually run until operating mode becomes NORMAL)\n");
    }
#endif

    RETURN_CODE_TYPE ret;
    CREATE_SAMPLING_PORT("WH", PORT_SIZE, SOURCE, SAMPLING_PD,&WH,&ret);
    CREATE_SAMPLING_PORT("WAZ", PORT_SIZE, SOURCE, SAMPLING_PD,&WAZ,&ret);
    CREATE_SAMPLING_PORT("WVZ", PORT_SIZE, SOURCE, SAMPLING_PD,&WVZ,&ret);
    CREATE_SAMPLING_PORT("WQ", PORT_SIZE, SOURCE, SAMPLING_PD,&WQ,&ret);
    CREATE_SAMPLING_PORT("WVA", PORT_SIZE, SOURCE, SAMPLING_PD,&WVA,&ret);


#if (MODE==VERBOSE)
    printf("[P1] Bilan create output ports: HF=%d AZF=%d RVF=%d QF=%d VA=%d\n", (int) WH, (int) WAZ, (int) WVZ, (int) WQ, (int) WVA);
#endif


    CREATE_SAMPLING_PORT("RDELTAE", PORT_SIZE, DESTINATION, SAMPLING_PD,&RDELTAE,&ret);
    CREATE_SAMPLING_PORT("RT", PORT_SIZE, DESTINATION, SAMPLING_PD,&RT,&ret);

#if (MODE==VERBOSE)
    printf("[P1] Bilan create input ports: DELTA_E=%d\n", (int) RDELTAE);
#endif

    SET_PARTITION_MODE(NORMAL,&ret_switch_mode);
#if (MODE==VERBOSE)
    printf("[P1] SWITCHED TO NORMAL \n");
#endif

    STOP_SELF();
    return 0;
}

void main(void) {
#if(MODE==CSV)
    printf("# Vertical Speed control: 0 m/s -> 2.5 m/s and Airspeed: 230 m/s\n# Column headers are:\n# - T: simulation time [s] \n# - Va: airspeed [m/s]\n# - az: normal acceleration [m/s^2] \n# - q: pitch rate [rad/s] \n# - Vz: vertical speed [m/s] \n# - h: altitude [m] \n# - delta_th_c: commanded throttle [-] \n# - delta_e_c: commanded elevator deflection [rad] \n# T, Va, Az, q, Vz, h, delta_th_c, delta_e_c\n");
#endif
    P1Main();
    STOP_SELF();
}
