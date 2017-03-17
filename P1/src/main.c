#include <string.h>
#include <stdio.h>
#include "../../common/app.h"
#include "../../common/app_code.c"

static SAMPLING_PORT_ID_TYPE RDELTAEC, RDELTATHC;
static SAMPLING_PORT_ID_TYPE WH, WAZ, WVZ, WQ, WVA;

static void P1_process(void) {

  /************************************************************
   *			P1 INITIALIZATION			*
   ************************************************************/

  RETURN_CODE_TYPE ret_pause;
//  UNLOCK_PREEMPTION(0, &ret_pause);
  // output (encapsulated) messages
  msg m_Va, m_h, m_az, m_Vz, m_q;

  // input (encapsulated) messages
  msg m_delta_ec, m_delta_thc;

  float T, delta_e;
  struct aircraft_dynamics_outs_t res; //structure pour accueillir les résultats de aircraft_dynamics

  MESSAGE_SIZE_TYPE len; // don't care
  VALIDITY_TYPE validity; // don't care
  unsigned last_m_delta_ec = 0;
  unsigned last_m_delta_thc = 0;

  int num_instance = 0;
  int pd = 1000;

  m_h.x = 0;
  m_az.x = 0;
  m_Vz.x = 0;
  m_q.x = 0;
  m_Va.x = 0;
  m_h.y = -1;
  m_az.y = -1;
  m_Vz.y = -1;
  m_q.y = -1;
  m_Va.y = -1;
  T = 41813.92119463;
  delta_e = 0.012009615652468;

  PERIODIC_WAIT(&ret_pause);
  if (ret_pause!=NO_ERROR) {printf("\n\n[P1] PERIODIC_WAIT ERROR CODE : %d \n\n",ret_pause);}

  /************************************************************
   *			P1 END INITIALIZATION			*
   ************************************************************/

  /************************************************************
   *			P1 DATA PROCESSING			*
   ************************************************************/

  while (1) {

    /************************************************************
     *  P1 OUT (THE ONLY PARTITION WHICH RECEIVES AFTER SENDING) *
     ************************************************************/

    aircraft_dynamics(delta_e, T,&res);

    m_h.data = res.h;
    m_az.data = res.az;
    m_Vz.data = res.Vz;
    m_q.data = res.q;
    m_Va.data = res.Va;

    WRITE_SAMPLING_MESSAGE(WH, (MESSAGE_ADDR_TYPE)&m_h, sizeof(m_h),&m_h.ret);
    WRITE_SAMPLING_MESSAGE(WAZ, (MESSAGE_ADDR_TYPE)&m_az, sizeof(m_az),&m_az.ret);
    WRITE_SAMPLING_MESSAGE(WVZ, (MESSAGE_ADDR_TYPE)&m_Vz, sizeof(m_Vz),&m_Vz.ret);
    WRITE_SAMPLING_MESSAGE(WQ, (MESSAGE_ADDR_TYPE)&m_q, sizeof(m_q),&m_q.ret);
    WRITE_SAMPLING_MESSAGE(WVA, (MESSAGE_ADDR_TYPE)&m_Va, sizeof(m_Va),&m_Va.ret);

    if (num_instance % pd == 0) {
      printf("num_instance %d.%04d\n", num_instance / 1000, num_instance - (num_instance / 1000) * 1000);
      printf("[P1] H: new message sent: {%u, \"%f\", %u}\n", m_h.x, m_h.data, m_h.y);
      printf("[P1] AZ: new message sent: {%u, \"%f\", %u}\n", m_az.x, m_az.data, m_az.y);
      printf("[P1] Vz: new message sent: {%u, \"%f\", %u}\n", m_Vz.x, m_Vz.data, m_Vz.y);
      printf("[P1] Q: new message sent: {%u, \"%f\", %u}\n", m_q.x, m_q.data, m_q.y);
      printf("[P1] VA: new message sent: {%u, \"%f\", %u}\n", m_Va.x, m_Va.data, m_Va.y);
    }

    num_instance += 5;
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


	//PERIOD WAIT BETWEEN SEND AND RECEIVE
    PERIODIC_WAIT(&ret_pause);
    if (ret_pause!=NO_ERROR) {printf("\n\n[P1] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);}



    /************************************************************
    *			P1 DATA PROCESSING			*
    ************************************************************/


    /************************************************************
     *	P1 IN (THE ONLY PARTITION WHICH SENDS BEFORE RECEIVING)	*
     ************************************************************/
    READ_SAMPLING_MESSAGE(RDELTAEC, (MESSAGE_ADDR_TYPE)&m_delta_ec,&len,&validity,&m_delta_ec.ret);
    if (m_delta_ec.ret == NO_ERROR) {
      if (num_instance % pd == 0) {printf("[P1] RDELTAEC: new message read: {%u, \"%f\", %u}\n", m_delta_ec.x, m_delta_ec.data, m_delta_ec.y);}
    } else {
      printf("[P1] RDELTAEC: Unable to read message: %u\n", m_delta_ec.ret);
    }

    if (m_delta_ec.x < last_m_delta_ec) {
      printf("[P1] RDELTAEC: warning: received message out of order\n");
    } else if (m_delta_ec.x > last_m_delta_ec + 1) {
      printf("[P1] RDELTAEC: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_delta_ec, m_delta_ec.x);
    } else if (m_delta_ec.x == last_m_delta_ec&&last_m_delta_ec != 0) {
      printf("[P1] RDELTAEC: warning: possible duplicate message (id=%d)\n", m_delta_ec.x);
    }
    last_m_delta_ec = m_delta_ec.x;

    // READ DELTAETHC
    READ_SAMPLING_MESSAGE(RDELTATHC, (MESSAGE_ADDR_TYPE)&m_delta_thc,&len,&validity,&m_delta_thc.ret);
    if (m_delta_thc.ret == NO_ERROR) {
      if (num_instance % pd == 0) {printf("[P1] RDELTATHC: new message read: {%u, \"%f\", %u}\n", m_delta_thc.x, m_delta_thc.data, m_delta_thc.y);}
    } else {
      printf("[P1] RDELTATHC: Unable to read message: %u\n", m_delta_thc.ret);
    }

    if (m_delta_thc.x < last_m_delta_thc) {
      printf("[P1] RDELTATHC: warning: received message out of order\n");
    } else if (m_delta_thc.x > last_m_delta_thc + 1) {
      printf("[P1] RDELTATHC: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_delta_thc, m_delta_thc.x);
    } else if (m_delta_thc.x == last_m_delta_thc&&last_m_delta_thc != 0) {
      printf("[P1] RDELTATHC: warning: possible duplicate  (id=%d)\n", m_delta_thc.x);
    }
    last_m_delta_thc = m_delta_thc.x;

    /************************************************************
     *				P1 END IN		 	*
     ************************************************************/
	//pause until next slot
    PERIODIC_WAIT(&ret_pause);
    if (ret_pause!=NO_ERROR) {printf("\n\n[P1] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);}

  }
}

/************************************************************			P1 END DATA PROCESSING			*
 ************************************************************/

int P1Main(void) {

  PROCESS_ID_TYPE pid_P1;
  RETURN_CODE_TYPE ret_process, ret_switch_mode;


  PROCESS_ATTRIBUTE_TYPE P1_process_attrs = {
    .PERIOD = -1, //other values are refused
    .TIME_CAPACITY = -1,
    .STACK_SIZE = 8096, // the only accepted stack size!
    .BASE_PRIORITY = MIN_PRIORITY_VALUE,
    .DEADLINE = SOFT, //errors with hard
  };

  P1_process_attrs.ENTRY_POINT = P1_process;
  strncpy(P1_process_attrs.NAME, "P1_process", sizeof(PROCESS_NAME_TYPE));

  CREATE_PROCESS(&P1_process_attrs,&pid_P1,&ret_process);
  if (ret_process != NO_ERROR) {
    printf("[P1] couldn't create P1_process: %d\n", (int) ret_process);
    return 1;
  } else {
    //        printf("[P1] P1_process  created\n");
  }

  START(pid_P1,&ret_process);
  if (ret_process != NO_ERROR) {
    printf("[P1] couldn't start process_1_in: %d\n", (int) ret_process);
    return 1;
  } else {
    printf("[P1] P1_process started (it won't actually run until operating mode becomes NORMAL)\n");
  }

  RETURN_CODE_TYPE ret;
  CREATE_SAMPLING_PORT("WH", PORT_SIZE, SOURCE, 0.02 * SECOND,&WH,&ret);
  CREATE_SAMPLING_PORT("WAZ", PORT_SIZE, SOURCE, 0.02 * SECOND,&WAZ,&ret);
  CREATE_SAMPLING_PORT("WVZ", PORT_SIZE, SOURCE, 0.02 * SECOND,&WVZ,&ret);
  CREATE_SAMPLING_PORT("WQ", PORT_SIZE, SOURCE, 0.02 * SECOND,&WQ,&ret);
  CREATE_SAMPLING_PORT("WVA", PORT_SIZE, SOURCE, 0.02 * SECOND,&WVA,&ret);

  printf("[P1] Bilan create output ports: HF=%d AZF=%d RVF=%d QF=%d VA=%d\n", (int) WH, (int) WAZ, (int) WVZ, (int) WQ, (int) WVA);

  CREATE_SAMPLING_PORT("RDELTAEC", PORT_SIZE, DESTINATION, 0.02 * SECOND,&RDELTAEC,&ret);
  CREATE_SAMPLING_PORT("RDELTATHC", PORT_SIZE, DESTINATION, 0.02 * SECOND,&RDELTATHC,&ret);

  printf("[P1] Bilan create input ports: DELTA_EC=%d DELTA_THC=%d\n", (int) RDELTAEC, (int) RDELTATHC);

  SET_PARTITION_MODE(NORMAL,&ret_switch_mode);
  printf("[P1] SWITCHED TO NORMAL \n");
  // STOP_SELF();
  return 0;
}

void main(void) {
  P1Main();
  STOP_SELF();
}
