#ifndef SLSA_SIM_ENGINE_H
#define SLSA_SIM_ENGINE_H

/**
 * @file slsa_sim_engine.h
 *
 * This file declares functions which provide
 * access to new executionengine.
**/ 

/* Copyright 2011-2020 The MathWorks, Inc. */

#include "libmwslsa_sim.h"

/* run simulation */
SLSA_SIM_PUBLISHED_C void ssRunSimulation(void* ss);

SLSA_SIM_PUBLISHED_C void ssWriteSimMetadata(
    void* ss,
    const char* metadataFilePath
    );

/* Create OperatingPoint ModelData */
SLSA_SIM_PUBLISHED_C void allocOPModelData(void* executionInfo);

/* Delete OperatingPoint ModelData */
SLSA_SIM_PUBLISHED_C void freeOPModelData(void* executionInfo);

/* Cache the pointers */
SLSA_SIM_PUBLISHED_C void ssCachePointer(void* ss, void** ptr);

/* Cache the raw memory */
SLSA_SIM_PUBLISHED_C void ssSaveRawMemory(
        void* ss,
        const char* key,
        void* ptr,
        unsigned int sizeInBytes
        );

/* Set the different WORK sizes in bytes */
SLSA_SIM_PUBLISHED_C void ssSetWorkSizeInBytes(
        void*  ss,
        unsigned int sizeInBytes,
        const char*  type
        );
#endif /* SLSA_SIM_ENGINE_H */
