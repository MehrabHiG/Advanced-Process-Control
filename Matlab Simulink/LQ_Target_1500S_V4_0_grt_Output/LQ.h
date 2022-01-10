/*
 * LQ.h
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

#ifndef RTW_HEADER_LQ_h_
#define RTW_HEADER_LQ_h_
#include <cstring>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "LQ_types.h"

/* Shared type includes */
#include "multiword_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Controlinput[2];              /* '<S1>/Sum' */
} B_LQ_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Feedback[5];                  /* '<Root>/Feedback' */
  real_T Refthickness;                 /* '<Root>/Thickness ref.' */
  real_T Refrollingforce;              /* '<Root>/Rolling force ref.' */
} ExtU_LQ_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T xv;                           /* '<Root>/xv' */
  real_T fx;                           /* '<Root>/fx' */
} ExtY_LQ_T;

/* Parameters (default storage) */
struct P_LQ_T_ {
  real_T Lr[4];                        /* Variable: Lr
                                        * Referenced by: '<S1>/Gain Lr'
                                        */
  real_T Ly[10];                       /* Variable: Ly
                                        * Referenced by: '<S1>/Gain Ly'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_LQ_T {
  const char_T *errorStatus;
};

/* Block parameters (default storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern P_LQ_T LQ_P;

#ifdef __cplusplus

}
#endif

/* Block signals (default storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern B_LQ_T LQ_B;

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  /* External inputs (root inport signals with default storage) */
  extern ExtU_LQ_T LQ_U;

  /* External outputs (root outports fed by signals with default storage) */
  extern ExtY_LQ_T LQ_Y;

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  /* Model entry point functions */
  extern void LQ_initialize(void);
  extern void LQ_step(void);
  extern void LQ_terminate(void);

#ifdef __cplusplus

}
#endif

/* Real-time Model object */
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_LQ_T *const LQ_M;

#ifdef __cplusplus

}
#endif

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('RollingMill_SIM_Discrete/LQ Controller 1')    - opens subsystem RollingMill_SIM_Discrete/LQ Controller 1
 * hilite_system('RollingMill_SIM_Discrete/LQ Controller 1/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'RollingMill_SIM_Discrete'
 * '<S1>'   : 'RollingMill_SIM_Discrete/LQ Controller 1'
 */
#endif                                 /* RTW_HEADER_LQ_h_ */
