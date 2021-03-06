#include "common.h"
#include "io.h"

//#include <math.h>

#include <libm.h>
#include <errno.h>

void aircraft_dynamics (float delta_e, float T,  struct aircraft_dynamics_outs_t *outputs) {

    static int debut = 1;

    static float u 	= 0.0;
    static float w 	= 0.0;
    static float q 	= 0.0;
    static float theta 	= 0.0;
    static float h 	= 0.0;
    static float u_dot 	= 0.0;
    static float w_dot 	= 0.0;
    static float q_dot 	= 0.0;
    static float theta_dot = 0.0;
    static float h_dot 	= 0.0;

    static float CD 	= 0.0;
    static float CL 	= 0.0;
    static float Cm 	= 0.0;

    static float Xa 	= 0.0;
    static float Za 	= 0.0;
    static float Ma 	= 0.0;

    static float alpha 	= 0.0;
    static float qbar 	= 0.0;
    static float V 	= 0.0;
    static float rho 	= 0.0;
//    static float Pi	= (float) 3.14159;

    if (debut==1) {

        u     = Va_eq * cosf(theta_eq);
        w     = Va_eq * sinf(theta_eq);
        q     = 0.0;
        theta = theta_eq;
        h     = h_eq;
        debut = 0;
        //printf("[AC_DYN] [DEBUT] Va_eq=%f, theta_eq=%f, cos(theta_eq)=%f, sin(theta_eq)=%f, u=%f, w=%f\n",Va_eq,theta_eq,cos(theta_eq),sin(theta_eq),u,w);
    }
    //printf("[AC_DYN] cos(Pi/3)=%f, sin(Pi/2)=%f, atan(Pi/2)=%f, sqrt(1)=%f, pow(2,3)=%f\n",cos(1.0471975512), sin(1.57079632679), atan(1.57079632679), (float)sqrt(1.0), (float)pow(2.0,2.0));
/*
printf("[AC_DYN] cos(0)=%f cos(Pi/4)=%f cos(Pi/3)=%f cos(Pi/2)=%f cos(Pi)=%f  cos(4*Pi)=%f\n",cosf(0),cosf(Pi/4.0),cosf(Pi/3.0),cosf(Pi/2.0),cosf(Pi),cosf(4.0*Pi));

printf("[AC_DYN] sin(0)=%f sin(Pi/4)=%f sin(Pi/3)=%f sin(Pi/2)=%f sin(Pi)=%f  sin(4*Pi)=%f\n",sinf(0),sinf(Pi/4.0),sinf(Pi/3.0),sinf(Pi/2.0),sinf(Pi),sinf(4.0*Pi));

printf("[AC_DYN] atan(0)=%f atan(Pi/4)=%f atan(Pi/3)=%f atan(Pi/2)=%f atan(Pi)=%f  atan(4*Pi)=%f\n",atanf(0),atanf(Pi/4.0),atanf(Pi/3.0),atanf(Pi/2.0),atanf(Pi),atanf(4.0*Pi));
*/
/*
printf("[AC_DYN] pow(0,0)=%f pow(2,3)=%f pow(3,2)=%f pow(1/4,1/2)=%f\n",powf(0.0,0.0),powf(2.0,3.0),powf(3.0,2.0),powf(1/4,1/2));
printf("[AC_DYN] sqrt(0)=%f sqrt(2)=%f sqrt(4)=%f sqrt(1/4)=%f\n",sqrtf(0.0),sqrtf(2.0),sqrtf(4.0),((1/4)));
*/


    rho   = rho0 * powf(1.0 + T0_h / T0_0 * h,- g0 / (Rs * T0_h) - 1.0);
//	rho   = rho0;


    //printf("[AC_DYN] alpha=%f - w=%f - u=%f - atan(w/u)=%f\n",alpha,w,u,atan(w/u));

    alpha = atanf(w/u);
    /* print_float(alpha, "alpha:"); */
    /* print_float(w, "w:"); */
    /* print_float(u, "u:"); */
    //printf("[AC_DYN] V: u=%f - w=%f\n",u,w);
    V     = sqrtf(u * u + w * w);
//	V     = (u * u + w * w);

    qbar  = 0.5 * rho * V * V;
    CL    = CL_deltae * delta_e + CL_alpha * (alpha - alpha_0);
    /* print_float(CL_deltae,"CL_deltae:"); */
    /* print_float(delta_e,"delta_e:"); */
    /* print_float(CL_alpha,"CL_alpha:"); */
    /* print_float(alpha_0,"alpha_0:"); */
    CD    = CD_0 + CD_deltae * delta_e + CD_alpha * (alpha - alpha_0) * (alpha - alpha_0);
    //printf("[AC_DYN] Cm : Cm_0=%f - Cm_deltae=%f - delta_e=%f - Cm_alpha=%f - alpha=%f - Cm_q=%f - q=%f - cbar=%f - V=%f\n",Cm_0,Cm_deltae,delta_e,Cm_alpha, alpha, Cm, q, cbar, V);
    Cm    = Cm_0 + Cm_deltae * delta_e + Cm_alpha * alpha + 0.5 * Cm_q * q * cbar / V;

    //printf("[AC_DYN] alpha: alpha=%f - cos(alpha)=%f\n",alpha,cos(alpha));

    Xa    = - qbar * S * (CD * cosf(alpha) - CL * sinf(alpha));
    Za    = - qbar * S * (CD * sinf(alpha) + CL * cosf(alpha));
    /* print_float(qbar, "qbar:"); */
    /* print_float(S, "S:"); */
    /* print_float(CD, "CD:"); */
    /* print_float(sin(alpha), "sin(alpha):"); */
    /* print_float(CL, "CL:"); */
    /* print_float(cos(alpha), "cos(alpha):"); */
    /* print_float(CD * sin(alpha), "CD * sin"); */
    /* print_float(CL * cos(alpha), "CL * cos"); */
    /* print_float((CD * sin(alpha) + CL * cos(alpha)), "le plus"); */
    /* print_float(qbar * S,"qbar * S"); */
    /* print_float(Za, "Za:"); */
    //printf("[AC_DYN] Ma: qbar=%f - cbar=%f - S=%f - Cm=%f\n",qbar,cbar,S,Cm);
    Ma    = qbar * cbar * S * Cm;

    // Output
    outputs -> Va = V;
    outputs -> Vz = w * cosf(theta) - u * sinf(theta);
    outputs -> q  = q;
    outputs -> az = g0 * cosf(theta) + Za / masse;
    /* print_float(g0, "g0:"); */
    /* print_float(cos(theta), "cos(theta):"); */
    /* print_float(Za, "Za:"); */
    /* print_float(masse, "masse:"); */

    outputs -> h  = h;
    // State Equation
    //printf("[AC_DYN] u_dot: g0=%f - theta=%f - q=%f - w=%f - Xa=%f - T=%f - masse=%f\n",g0,theta,q,w,Xa,T,masse);
    u_dot     = - g0 * sinf (theta) - q * w + (Xa + T) / masse;
    w_dot     = g0 * cosf (theta) + q * u + Za / masse;
    //printf("[AC_DYN] q_dot Ma=%f - I_y=%f\n",Ma,I_y);
    q_dot     = Ma / I_y;
    theta_dot = q;
    h_dot     = u * sinf(theta) - w * cosf(theta);
    // Update State
    //printf("[AC_DYN] u : dt=%f - u_dot=%f\n",dt,u_dot);
    u     += dt * u_dot;
    w     += dt * w_dot;
    //printf("[AC_DYN] q: dt=%f - q_dot=%f\n",dt,q_dot);
    q     += dt * q_dot;
//printf("[%f,%f],",dt,q_dot);
    theta += dt * theta_dot;
    h     += dt * h_dot;

//	printf("\nVz=%f,theta=%f,u=%f,w=%f,w=%f,w=%f\n",outputs -> Vz,theta,u,w);

    static float Time = 0.0;


    // instant++;
    Time = Time + dt;
//	printf("%f,",Time);
}

float
elevator(float delta_e_c) {

    //note : delta_e_eq = 0.012009615652468 (valeur équilibre)
    static float y  = delta_e_eq;
    static float x1 = delta_e_eq;
    static float x2 = 0.0;
    static float x1_dot = 0.0;
    static float x2_dot = 0.0;
    static float omega = 25.0;
    static float xi = 0.85;

    // Output
    y = x1;
    // State Equation
    x1_dot = x2;
    x2_dot = -omega * omega * x1 - 2.0 * xi * omega * x2 + omega * omega * delta_e_c;
    // Update State
    //note : dt_de    = 0.0005
    x1 += dt_de * x1_dot;
    x2 += dt_de * x2_dot;

    return y;
}


float
engine(float delta_th_c) {

    static float y      = delta_th_eq;
    static float x1     = delta_th_eq;
    static float x1_dot = 0.0;
    static float tau    = 0.75;

    // Output
    y = 26350.0 * x1;
    // State Equation
    x1_dot = -tau * x1 + tau * delta_th_c;
    // Update State
    x1 += dt_dx * x1_dot;

    return y;
}

