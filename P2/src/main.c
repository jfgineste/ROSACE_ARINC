#include <string.h>
#include <stdio.h>
#include "../../common/app.h"
#include "../../common/app2_code.c"

static SAMPLING_PORT_ID_TYPE RH, RAZ, RVZ, RQ, RVA;
static SAMPLING_PORT_ID_TYPE WHF, WAZF, WVZF, WQF, WVAF;

static void P2_process(void) {

  /**************************************************************
   *			P2 INITIALIZATION			*
   **************************************************************/
  // input (encapsulated) messages
  static msg m_Va, m_h, m_az, m_Vz, m_q;

  // output (encapsulated) messages
  msg m_Va_f, m_h_f, m_az_f, m_Vz_f, m_q_f;
  
  static RETURN_CODE_TYPE ret_pause;

  int num_instance = 0;
  int pd = 2;

  MESSAGE_SIZE_TYPE len; // don't care
  VALIDITY_TYPE Validity; // don't care

  m_h_f.x = 0;
  m_az_f.x = 0;
  m_Vz_f.x = 0;
  m_q_f.x = 0;
  m_Va_f.x = 0;
  m_h_f.y = -1;
  m_az_f.y = -1;
  m_Vz_f.y = -1;
  m_q_f.y = -1;
  m_Va_f.y = -1;

  unsigned last_m_h = 0;
  unsigned last_m_az = 0;
  unsigned last_m_Vz = 0;
  unsigned last_m_q = 0;
  unsigned last_m_Va = 0;

  PERIODIC_WAIT(&ret_pause);
    if (ret_pause!=NO_ERROR) {printf("\n\n[P2] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);}
  /**************************************************************
   *			P2 END INITIALIZATION			*
   **************************************************************/

  /***************************************************************
   *			P2 DATA PROCESSING			 *
   **************************************************************/

  while (1) {

    if (num_instance == 0) {

      /******************************************************************
       * 				P2 IN				*
       *****************************************************************/

      // READ HF
      READ_SAMPLING_MESSAGE(RH, (MESSAGE_ADDR_TYPE)  &m_h,  &len,  &Validity,  &m_h.ret);
      if (m_h.ret == NO_ERROR) {
        printf("[P2] RH: new message read: {%u, \"%f\", %u}\n", m_h.x, m_h.data, m_h.y);
      } else {
        printf("[P2] RH: Unable to read message: %u\n", m_h.ret);
      }

      if (m_h.x < last_m_h) {
        printf("[P2] RH: warning: received message out of order\n");
      } else if (m_h.x > last_m_h + 1) {
        printf("[P2] RH: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_h, m_h.x);
      } else if (m_h.x == last_m_h  &&last_m_h != 0) {
        printf("[P2] RH: warning: possible duplicate message (id=%d)\n", m_h.x );
      }
      last_m_h = m_h.x;

      // READ azf
      READ_SAMPLING_MESSAGE(RAZ, (MESSAGE_ADDR_TYPE)  &m_az,  &len,  &Validity,  &m_az.ret);
      if (m_az.ret == NO_ERROR) {
        printf("[P2] RAZ: new message read: {%u, \"%f\", %u}\n", m_az.x, m_az.data, m_az.y);
      } else {
        printf("[P2] RAZ: Unable to read message: %u\n", m_az.ret);
      }

      if (m_az.x < last_m_az) {
        printf("[P2] RAZ: warning: received message out of order\n");
      } else if (m_az.x > last_m_az + 1) {
        printf("[P2] RAZ: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_az, m_az.x);
      } else if (m_az.x == last_m_az  &&last_m_az != 0) {
        printf("[P2] RAZ: warning: possible duplicate (id=%d)\n", m_az.x);
      }
      last_m_az = m_az.x;

      // READ VZF
      READ_SAMPLING_MESSAGE(RVZ, (MESSAGE_ADDR_TYPE)  &m_Vz,  &len,  &Validity,  &m_Vz.ret);
      if (m_Vz.ret == NO_ERROR) {
        printf("[P2] RVZ: new message read: {%u, \"%f\", %u}\n", m_Vz.x, m_Vz.data, m_Vz.y);
      } else {
        printf("[P2] RVZ: Unable to read message: %u\n", m_Vz.ret);
      }

      if (m_Vz.x < last_m_Vz) {
        printf("[P2] RVZ: warning: received message out of order\n");
      } else if (m_Vz.x > last_m_Vz + 1) {
        printf("[P2] RVZ: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_Vz, m_Vz.x);
      } else if (m_Vz.x == last_m_Vz  &&last_m_Vz != 0) {
        printf("[P2] RVZ: warning: possible duplicate message (id=%d)\n", m_Vz.x);
      }
      last_m_Vz = m_Vz.x;

      // READ Q
      READ_SAMPLING_MESSAGE(RQ, (MESSAGE_ADDR_TYPE)  &m_q,  &len,  &Validity,  &m_q.ret);
      if (m_q.ret == NO_ERROR) {
        printf("[P2] RQ: new message read: {%u, \"%f\", %u}\n", m_q.x, m_q.data, m_q.y);
      } else {
        printf("[P2] RQ: Unable to read message: %u\n", m_q.ret);
      }

      if (m_q.x < last_m_q) {
        printf("[P2] RQ: warning: received message out of order\n");
      } else if (m_q.x > last_m_q + 1) {
        printf("[P2] RQ: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_q, m_q.x);
      } else if (m_q.x == last_m_q  &&last_m_q != 0) {
        printf("[P2] RQ: warning: possible duplicate message (id=%d)\n", m_q.x);
      }
      last_m_q = m_q.x;

      // READ VA
      READ_SAMPLING_MESSAGE(RVA, (MESSAGE_ADDR_TYPE)  &m_Va,  &len,  &Validity,  &m_Va.ret);
      if (m_Va.ret == NO_ERROR) {
        printf("[P2] RVA: new message read: {%u, \"%f\", %u}\n", m_Va.x, m_Va.data, m_Va.y);
      } else {
        printf("[P2] Va_f: Unable to read message: %u\n", m_Va.ret);
      }

      if (m_Va.x < last_m_Va) {
        printf("[P2] RVA: warning: received message out of order\n");
      } else if (m_Va.x > last_m_Va + 1) {
        printf("[P2] RVA: warning: possible message loss (jumped from id=%d to id=%d)\n", last_m_Va, m_Va.x);
      } else if (m_Va.x == last_m_Va  &&last_m_Va != 0) {
        printf("[P2] RVA: warning: possible duplicate message (id=%d)\n", m_Va.x);
      }
      last_m_Va = m_Va.x;

      /************************************************************
       * 				P2 END IN			*
       ************************************************************/

      /************************************************************
       * 				P2 OUT				*
       ************************************************************/
      m_h_f.data = h_filter(m_h.data);
      WRITE_SAMPLING_MESSAGE(WHF, (MESSAGE_ADDR_TYPE)  &m_h_f, sizeof(m_h_f),  &m_h_f.ret);
      m_az_f.data = az_filter(m_az.data);
      WRITE_SAMPLING_MESSAGE(WAZF, (MESSAGE_ADDR_TYPE)  &m_az_f, sizeof(m_az_f),  &m_az_f.ret);
      m_Vz_f.data = Vz_filter(m_Vz.data);
      WRITE_SAMPLING_MESSAGE(WVZF, (MESSAGE_ADDR_TYPE)  &m_Vz_f, sizeof(m_Vz_f),  &m_Vz_f.ret);
      m_q_f.data = q_filter(m_q.data);
      WRITE_SAMPLING_MESSAGE(WQF, (MESSAGE_ADDR_TYPE)  &m_q_f, sizeof(m_q_f),  &m_q_f.ret);
      m_Va_f.data = Va_filter(m_Va.data);
      WRITE_SAMPLING_MESSAGE(WVAF, (MESSAGE_ADDR_TYPE)  &m_Va_f, sizeof(m_Va_f),  &m_Va_f.ret);;

      if (m_h_f.ret != NO_ERROR) {
        printf("[P2] HF: error during writing on sampling: %u\n", m_h_f.ret);
      } else {
        printf("[P2] HF: new message sent: {%u, \"%f\", %u}\n", m_h_f.x, m_h_f.data, m_h_f.y);
      }
      if (m_az_f.ret != NO_ERROR) {
        printf("[P2] AZF: error during writing on sampling: %u\n", m_az_f.ret);
      } else {
        printf("[P2] AZF: new message sent: {%u, \"%f\", %u}\n", m_az_f.x, m_az_f.data, m_az_f.y);
      }
      if (m_Vz_f.ret != NO_ERROR) {
        printf("[P2] VZF: error during writing on sampling: %u\n", m_Vz_f.ret);
      } else {
        printf("[P2] VZF: new message sent: {%u, \"%f\", %u}\n", m_Vz_f.x, m_Vz_f.data, m_Vz_f.y);
      }
      if (m_q_f.ret != NO_ERROR) {
        printf("[P2] QF: error during writing on sampling: %u\n", m_q_f.ret);
      } else {
        printf("[P2] QF: new message sent: {%u, \"%f\", %u}\n", m_q_f.x, m_q_f.data, m_q_f.y);
      }
      if (m_Va.ret != NO_ERROR) {
        printf("[P2] VAF: error during writing on sampling: %u\n", m_Va.ret);
      } else {
        printf("[P2] VAF: new message sent: {%u, \"%f\", %u}\n", m_Va_f.x, m_Va_f.data, m_Va_f.y);
      }

      //printf("[P2] Bilan du write : %d %d %d %d %d [message_size=%d]\n",(int) m_h_f.ret,(int) m_az.ret,(int) m_Vz.ret,(int) m_q.ret,(int) m_Va.ret,(int) sizeof(m_h_f));
      m_h_f.x++;
      m_az_f.x++;
      m_Vz_f.x++;
      m_q_f.x++;
      m_Va_f.x++;

      /************************************************************
       * 				P2 END OUT			*
       ************************************************************/

    }
    PERIODIC_WAIT(&ret_pause);
    if (ret_pause!=NO_ERROR) {printf("\n\n[P2] PERIODIC_WAIT ERROR CODE : %d (1=NO_ACTION;2=NOT_AVAILABLE;3=INVALID_PARAM;4=INVALID_CONFIG;5=INVALID_MODE;6=TIMED_OUT)\n\n",ret_pause);}
    num_instance = (num_instance + 1) % pd;

  }

}

/************************************************************
 *			P2 END DATA PROCESSING			*
 ************************************************************/

int P2Main(void) {

  PROCESS_ID_TYPE pid_P2;
  RETURN_CODE_TYPE ret_process, ret_switch_mode;

  PROCESS_ATTRIBUTE_TYPE P2_process_attrs = {
    .PERIOD = INFINITE_TIME_VALUE,
    .TIME_CAPACITY = INFINITE_TIME_VALUE,
    .STACK_SIZE = 8096, // the only accepted stack size!
    .BASE_PRIORITY = MIN_PRIORITY_VALUE,
    .DEADLINE = SOFT,
  };

  P2_process_attrs.ENTRY_POINT = P2_process;
  strncpy(P2_process_attrs.NAME, "P2_process", sizeof(PROCESS_NAME_TYPE));

  CREATE_PROCESS(  &P2_process_attrs,  &pid_P2,  &ret_process);
  if (ret_process != NO_ERROR) {
    printf("[P2] couldn't create P2_process: %d\n", (int) ret_process);
    return 1;
  } else {
    printf("[P2] P2_process  created\n");
  }

  START(pid_P2,  &ret_process);
  if (ret_process != NO_ERROR) {
    printf("[P2] couldn't start P2_process: %d\n", (int) ret_process);
    return 1;
  } else {
    printf("[P2] P2_process started (it won't actually run until operating mode becomes NORMAL)\n");
  }

  RETURN_CODE_TYPE ret;
  CREATE_SAMPLING_PORT("WHF", PORT_SIZE, SOURCE, 1 * SECOND,  &WHF,  &ret);
  CREATE_SAMPLING_PORT("WAZF", PORT_SIZE, SOURCE, 1 * SECOND,  &WAZF,  &ret);
  CREATE_SAMPLING_PORT("WVZF", PORT_SIZE, SOURCE, 1 * SECOND,  &WVZF,  &ret);
  CREATE_SAMPLING_PORT("WQF", PORT_SIZE, SOURCE, 1 * SECOND,  &WQF,  &ret);
  CREATE_SAMPLING_PORT("WVAF", PORT_SIZE, SOURCE, 1 * SECOND,  &WVAF,  &ret);

  printf("[P2] Bilan create output ports: HF=%d AZF=%d RVF=%d QF=%d VAF=%d\n", (int) WHF, (int) WAZF, (int) WVZF, (int) WQF, (int) WVAF);

  CREATE_SAMPLING_PORT("RH", PORT_SIZE, DESTINATION, 1 * SECOND,  &RH,  &ret);
  CREATE_SAMPLING_PORT("RAZ", PORT_SIZE, DESTINATION, 1 * SECOND,  &RAZ,  &ret);
  CREATE_SAMPLING_PORT("RVZ", PORT_SIZE, DESTINATION, 1 * SECOND,  &RVZ,  &ret);
  CREATE_SAMPLING_PORT("RQ", PORT_SIZE, DESTINATION, 1 * SECOND,  &RQ,  &ret);
  CREATE_SAMPLING_PORT("RVA", PORT_SIZE, DESTINATION, 1 * SECOND,  &RVA,  &ret);

  printf("[P2] Bilan create input ports: H=%d AZ=%d RV=%d Q=%d VA=%d\n", (int) RH, (int) RAZ, (int) RVZ, (int) RQ, (int) RVA);

  SET_PARTITION_MODE(NORMAL,  &ret_switch_mode);
  printf("[P2] SWITCHED TO NORMAL \n");
  return 0;
}

void main(void) {
  P2Main();
  STOP_SELF();
}
