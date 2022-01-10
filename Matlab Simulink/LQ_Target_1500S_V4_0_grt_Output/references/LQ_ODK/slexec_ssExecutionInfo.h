#ifndef SLEXEC_EXECUTIONINFO_H
#define SLEXEC_EXECUTIONINFO_H
/**
 * @file slexec_executioninfo.h
 *
 * This header contains execution related parameters
 * which are used in rapid accelerator target.
 *
 */

/* Copyright 2019-2020 The MathWorks, Inc. */

#include "slexec_parallel.h"
#include <setjmp.h>

/* Guard against early inclusions */
#ifndef _SIMSTRUCT
#error "This file should be included from simstruc.h"
#endif

/* Information required to initialize/run Execution Engine */
typedef struct ssExecutionInfo_tag {
    /* Root Simstruct */
    SimStruct* simstruct_;

    struct SimulationOptions {
        const char* parameterFileName_; /* simulation option parameter file name */
        int parameterArrayIndex_;       /* simulation options array index */
        /* time out for executable (is an int as of now) */
        int timeLimit_;
        int enableSLExecSSBridgeFeatureValue_;
        const char* inportFileName_;
        int* matFileFormat_;
        const char* simMetadataFilePath_;
        const char* channelName_; /* channel name for communication with sim service */
        int isDrivenBySimulationService_; /* if true, then wait for simulation service commands */
    } simulationOptions_;

    struct ModelMethods {
        /* model start */
        void (*start)(void);
        /* parameter update function */
        void (*outputsParameterSampleTime)(int);
        /* model terminate */
        void (*terminate)(void);
    } modelMethods_;

    struct RuntimeCallbacks {
        /* external mode call backs */
        ssBridgeExtModeCallbacks_T* externalModeCallbacks_;
        /* logging function */
        void (*loggingFunction)(SimStruct*);
        void (*setupMMIStateLog)(SimStruct*);
        const char* (*startDataLoggingWithStartTime)(
            RTWLogInfo*, const real_T, const real_T, const real_T, const char_T**);
        const char* (*rapidReadInportsAndAperiodicHitTimes)(SimStruct*, const char*, int*);
        const char* (*rapidCheckRemappings)(void);
        void (*sdiBindObserversAndStartStreamingEngine)(const char*);
        double (*nextAperiodicPartitionHitTime)(SimStruct*, double);

    } runtimeCallbacks_;

    struct RootIODataMethods {
        void* (*getRootInput)(int /*idx*/);  /* idxth root input buffer */
        void* (*getRootOutput)(int /*idx*/); /* idxth root output buffer */
        int (*getNumRootInputs)(void);
        int (*getNumRootOutputs)(void);
        int (*getRootInputSize)(int);
        int (*getRootOutputSize)(int);
        int (*getRootInputDatatypeSSId)(int idx);
        uint_T const* (*getRootInputDimArray)(int idx);
        int (*getRootInputNumDims)(int idx);
        int (*getRootOutputDatatypeSSId)(int idx);
        uint_T const* (*getRootOutputDimArray)(int idx);
        int (*getRootOutputNumDims)(int idx);
		int (*getRootOutputDataIsComplex)(int idx);
		int (*getRootInputDataIsComplex)(int idx);
        int (*getRootInputPortNumber)(int idx);
        int (*getRootOutputPortNumber)(int idx);
    } rootIODataMethods_;

    struct RuntimeFlags {
        boolean_T* parameterPacketReceived_;
        boolean_T startPacketReceived_;
        const char* sFcnInfoFileName;
    } runtimeFlags_;

    struct RuntimeObjects {
        jmp_buf* longJumpBuffer_;
        void* iSigstreamManager_;
    } runtimeObjects_;

    struct ParallelExecution {
        boolean_T enabled_;
        int_T simulatorType_;
        ParallelExecutionOptions options_;
    } parallelExecution_;
    
    struct DeployedExecution {
        boolean_T runningDeployed_;
    } deployedExecution_;

    struct OperatingPoint {
        boolean_T loadInitialState_;
        void* opModelData_;
        /* This should probably not live here, but the executionOptions_->operatingPoint rename happend in a merge, so here we are. */
        void* aperiodicPartitionHitTimes_;
    } operatingPoint_;
} ssExecutionInfo;

/* Get ExecutionInfo from simstruct */
#define ssGetExecutionInfo(S) ((ssExecutionInfo*)((S)->mdlInfo->executionInfo))

/* Set ExecutionInfo on simstruct */
#define ssSetExecutionInfo(S, exInfo) (S)->mdlInfo->executionInfo = (void*)(exInfo); \
(exInfo)->simstruct_ = (S)

/* This macro is used to get the status whether InitialState is set from outside */
#define ssGetGlobalInitialStatesAvailable(S) ssGetExecutionInfo(S)->operatingPoint_.loadInitialState_

/* This macro is used to get the aperiodic partition hit times */
#define ssGetExecOptionAperiodicPartitionHitTimes(S) ssGetExecutionInfo(S)->operatingPoint_.aperiodicPartitionHitTimes_

#endif
