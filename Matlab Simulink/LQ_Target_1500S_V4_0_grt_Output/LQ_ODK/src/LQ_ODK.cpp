/*
 * This file was generated using "SIMATIC Target 1500S for Simulink"
 * Target Language Compiler provided by Siemens AG.
 *
 * Simulink model name          : LQ
 * Model version                : 1.23
 *
 * File name                    : LQ_ODK.cpp
 * File creation date           : 2021-08-30 17:02:21
 *
 * Target 1500S version         : V4.0
 * Simulink system target file  : Target_1500S_V4_0_grt.tlc
 *
 * ODK 1500S version            : V2.5
 *
 * MATLAB version               : 9.9 (R2020b) 29-Jul-2020
 * MATLAB Coder version         : 5.1 (R2020b) 29-Jul-2020
 * Simulink version             : 10.2 (R2020b) 29-Jul-2020
 * Simulink Coder version       : 9.4 (R2020b) 29-Jul-2020
 *
 */

#include <stddef.h>
#include "ODK_Functions.h"
#include "ODK_CpuReadWriteData.h"
#include "rtwtypes.h"
#include "LQ.h"
#include "LQ_private.h"
#include "Target_Params.h"
#include <vector>

boolean_T initialized = false;
boolean_T terminated = false;
RWLock *rw_lock = NULL;
void modelTerminate()
{
  if (terminated) {
    return;
  }

  terminated = true;
  initialized = false;
  LQ_terminate();
  if (rw_lock != NULL)
    delete(rw_lock);
}

void modelInitialize()
{
  if (initialized) {
    return;
  }

  LQ_initialize();
  initialized = true;
  terminated = false;
}

/*
 * OnLoad() is invoked after the application binary was loaded.
 * @return ODK_SUCCESS       notify, that no error occurs
 *                            - OnRun() will be invoked automatically
 *         any other value   notify, that an error occurs (user defined)
 *                            - OnUnload() will be invoked automatically
 *                            - ODK application will be unloaded automatically
 */
EXPORT_API ODK_RESULT OnLoad(void)
{
  modelInitialize();
  return ODK_SUCCESS;
}

/*
 * OnUnload() is invoked before the application binary is unloaded or when
 *            ODK-Host terminates.
 * @return ODK_SUCCESS       notify, that no error occurs
 *         any other value   notify, that an error occurs (user defined)
 *                            - ODK application will be unloaded anyway
 */
EXPORT_API ODK_RESULT OnUnload(void)
{
  // place your code here
  modelTerminate();
  return ODK_SUCCESS;
}

/*
 * OnRun() is invoked when the CPU transitions to the RUN state and
 *         after OnLoad().
 * @return Does not affect the load operation or state transition.
 */
EXPORT_API ODK_RESULT OnRun(void)
{
  // place your code here
  modelInitialize();
  return ODK_SUCCESS;
}

/*
 * OnStop() is invoked when the CPU transitions to the STOP/SHUTDOWN state
 *          or before unloading the ODK application.
 * In case of a CPU transition RUN->STOP this function will be canceled,
 * when execution overruns a reaction time of more than ~20ms.
 * @return Does not affect the load operation or state transition.
 */
EXPORT_API ODK_RESULT OnStop(void)
{
  modelTerminate();
  return ODK_SUCCESS;
}

/*
 * main() is defined for windows debugging, only.
 * Therefore all automatically invoked functions (OnLoad, OnRun, OnStop, OnUnload)
 * have to be called manually.
 */
#ifdef _DEBUG

int main (int argc, char* argv[])
{
  ODK_RESULT ret = ODK_SUCCESS;
  ret = OnLoad();

  // error handling
  ret = OnRun();

  // error handling

  // place your test code here
  ret = OnStop();

  // error handling
  ret = OnUnload();

  // error handling
  return ret;
}

#endif                                 // _DEBUG

_ODK_FUNCTION_ONESTEP {
  for (int it1 = 0; it1 < 5; it1++) {
    LQ_U.Feedback[it1] = Feedback[it1];
  }

  LQ_U.Refthickness = Refthickness;
  LQ_U.Refrollingforce = Refrollingforce;
  if (rw_lock != NULL)
    rw_lock->RdLock();
  LQ_step();
  if (rw_lock != NULL)
    rw_lock->RdUnlock();
  xv = LQ_Y.xv;
  fx = LQ_Y.fx;
  for (int it1 = 0; it1 < 2; it1++) {
    internalSignals.Controlinput[it1] = LQ_B.Controlinput[it1];
  }

  for (int it1 = 0; it1 < 5; it1++) {
    internalSignals.Feedback[it1] = LQ_U.Feedback[it1];
  }

  internalSignals.LQ_Refthickness = LQ_U.Refthickness;
  internalSignals.LQ_Refrollingforce = LQ_U.Refrollingforce;
  return ODK_SUCCESS;
}
  _ODK_FUNCTION_READWRITEPARAMETERS
{
  if (rw_lock == NULL)
    rw_lock = new RWLock();
  if (!write) {
    rw_lock->RdLock();
    for (int it1 = 0; it1 < 4; it1++) {
      inOutParams.Lr[it1] = LQ_P.Lr[it1];
    }

    for (int it1 = 0; it1 < 10; it1++) {
      inOutParams.Ly[it1] = LQ_P.Ly[it1];
    }

    rw_lock->RdUnlock();
    return ODK_SUCCESS;
  }

  // Check which members will be updated
  std::vector<UpdatedParam> v;
  rw_lock->RdLock();
  for (int it1 = 0; it1 < 4; it1++) {
    if (LQ_P.Lr[it1] != inOutParams.Lr[it1])
      v.push_back( UpdatedParam(&(LQ_P.Lr[it1]),inOutParams.Lr[it1]) );
  }

  for (int it1 = 0; it1 < 10; it1++) {
    if (LQ_P.Ly[it1] != inOutParams.Ly[it1])
      v.push_back( UpdatedParam(&(LQ_P.Ly[it1]),inOutParams.Ly[it1]) );
  }

  rw_lock->RdUnlock();

  // Write only those to be updated
  if (v.size() > 0) {
    rw_lock->WrLock();
    size_t s = v.size();
    for (size_t i=0;i<s;i++) {
      v[i].AssignValue();
    }

    rw_lock->WrUnlock();
  }

  v.clear();
  return ODK_SUCCESS;
}
