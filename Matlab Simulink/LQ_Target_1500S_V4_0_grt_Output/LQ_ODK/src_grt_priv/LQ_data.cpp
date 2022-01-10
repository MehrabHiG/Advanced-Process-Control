/*
 * LQ_data.cpp
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "LQ".
 *
 * Model version              : 1.23
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C++ source code generated on : Mon Aug 30 17:02:17 2021
 *
 * Target selection: Target_1500S_V4_0_grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "LQ.h"
#include "LQ_private.h"

/* Block parameters (default storage) */
P_LQ_T LQ_P = {
  /* Variable: Lr
   * Referenced by: '<S1>/Gain Lr'
   */
  { 0.013105542817203657, -4.0088556377257333E-14, 1.7937299090149843E-10,
    3.1126729201737215 },

  /* Variable: Ly
   * Referenced by: '<S1>/Gain Ly'
   */
  { 4.12034703813479E-14, -3.9867710683543072E-25, -1.3735283889787964E-14,
    1.3102099081206513E-25, 0.011683637924484208, -4.0014174078211139E-14,
    3.76902619915779E-5, -3.61430408188072E-16, -1.4039675936293829E-8,
    3.112672920173722 }
};
