/*
 * LQ.cpp
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

/* Block signals (default storage) */
B_LQ_T LQ_B;

/* External inputs (root inport signals with default storage) */
ExtU_LQ_T LQ_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_LQ_T LQ_Y;

/* Real-time model */
RT_MODEL_LQ_T LQ_M_ = RT_MODEL_LQ_T();
RT_MODEL_LQ_T *const LQ_M = &LQ_M_;

/* Model step function */
void LQ_step(void)
{
  real_T tmp[2];
  int32_T i;
  int32_T i_0;
  for (i = 0; i < 2; i++) {
    /* Gain: '<S1>/Gain Ly' incorporates:
     *  Inport: '<Root>/Feedback'
     */
    tmp[i] = 0.0;
    for (i_0 = 0; i_0 < 5; i_0++) {
      tmp[i] += LQ_P.Ly[(i_0 << 1) + i] * LQ_U.Feedback[i_0];
    }

    /* End of Gain: '<S1>/Gain Ly' */

    /* Sum: '<S1>/Sum' incorporates:
     *  Gain: '<S1>/Gain Lr'
     *  Inport: '<Root>/Rolling force ref.'
     *  Inport: '<Root>/Thickness ref.'
     */
    LQ_B.Controlinput[i] = (LQ_P.Lr[i + 2] * LQ_U.Refrollingforce + LQ_P.Lr[i] *
      LQ_U.Refthickness) - tmp[i];
  }

  /* Outport: '<Root>/xv' */
  LQ_Y.xv = LQ_B.Controlinput[0];

  /* Outport: '<Root>/fx' */
  LQ_Y.fx = LQ_B.Controlinput[1];
}

/* Model initialize function */
void LQ_initialize(void)
{
  /* Registration code */

  /* block I/O */
  (void) std::memset((static_cast<void *>(&LQ_B)), 0,
                     sizeof(B_LQ_T));

  /* external inputs */
  (void)std::memset(&LQ_U, 0, sizeof(ExtU_LQ_T));

  /* external outputs */
  (void) std::memset(static_cast<void *>(&LQ_Y), 0,
                     sizeof(ExtY_LQ_T));
}

/* Model terminate function */
void LQ_terminate(void)
{
  /* (no terminate code required) */
}
