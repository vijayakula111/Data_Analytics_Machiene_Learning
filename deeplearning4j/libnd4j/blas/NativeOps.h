/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// Created by agibsonccc on 2/21/16.
//

#ifndef NATIVEOPERATIONS_NATIVEOPS_H
#define NATIVEOPERATIONS_NATIVEOPS_H


#ifndef thread_local
# if __STDC_VERSION__ >= 201112 && !defined __STDC_NO_THREADS__
#  define thread_local _Thread_local
# elif defined _WIN32 && ( \
       defined _MSC_VER || \
       defined __ICL || \
       defined __DMC__ || \
       defined __BORLANDC__ )
#  define thread_local __declspec(thread)
/* note that ICC (linux) and Clang are covered by __GNUC__ */
# elif defined __GNUC__ || \
       defined __SUNPRO_C || \
       defined __xlC__
#  define thread_local __thread
# else
#  error "Cannot define thread_local"
# endif
#endif

#include <pointercast.h>
#include <types/float16.h>
#include <cnpy.h>

//DO NOT REMOVE: THIS IS AN EDITOR SEMANTICS THING FOR CLION
//IT DEFINES THE EXPORT MACRO FOR THE EDITOR AND THEN
//RE ADDS THE DEFINITION VIA dll.h
#ifdef  _WIN32
#define ND4J_EXPORT __declspec(dllexport)
#else
#define ND4J_EXPORT
#endif
#include <dll.h>
#include <helpers/BlasHelper.h>

/*
int tad_threshold = 1;
int element_threshold = 32;

bool debug = false;
bool verbose = false;
*/

#include <array/ShapeList.h>
#include <graph/VariablesSet.h>
#include <graph/GraphState.h>
#include <graph/execution/LogicExecutor.h>
#include <graph/ResultWrapper.h>

class ND4J_EXPORT NativeOps {

public:

    /**
     *
     * @param num
     */
    void setElementThreshold(int num);

    /**
     *
     * @param num
     */
    void setTADThreshold(int num);

    /**
       *
       * @param opNum
       * @param x
       * @param xShapeInfo
       * @param extraParams
       */
    void execIndexReduceScalar(Nd4jPointer *extraPointers,
                                         int opNum,
                                         void *hX, Nd4jLong *hXShapeInfo,
                                         void *dX, Nd4jLong *dXShapeInfo,
                                         void *extraParams,
                                         void *hZ, Nd4jLong *hZShapeInfo,
                                         void *dZ, Nd4jLong *dZShapeInfo);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParams
     * @param result
     * @param resultShapeInfoBuffer
     * @param dimension
     * @param dimensionLength
     */
    void   execIndexReduce(Nd4jPointer *extraPointers,
            int opNum,
            void *hX, Nd4jLong *hXShapeInfo,
            void *dX, Nd4jLong *dXShapeInfo,
            void *extraParams,
            void *hZ, Nd4jLong *hZShapeInfo,
            void *dZ, Nd4jLong *dZShapeInfo,
            void *hDimension, Nd4jLong *hDimensionShape,
            void *dDimension, Nd4jLong *dDimensionShape);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param y
     * @param yShapeInfo
     * @param result
     * @param resultShapeInfo
     * @param dimension
     * @param dimensionLength
     */
    void   execBroadcast(
            Nd4jPointer *extraPointers,
            int opNum,
            void *hX, Nd4jLong *hXShapeInfo,
            void *dX, Nd4jLong *dXShapeInfo,
            void *hY, Nd4jLong *hYShapeInfo,
            void *dY, Nd4jLong *dYShapeInfo,
            void *hZ, Nd4jLong *hZShapeInfo,
            void *dZ, Nd4jLong *dZShapeInfo,
            void *hDimension, Nd4jLong *hDimensionShape,
            void *dDimension, Nd4jLong *dDimensionShape);


    void   execBroadcastBool(
            Nd4jPointer *extraPointers,
            int opNum,
            void *hX, Nd4jLong *hXShapeInfo,
            void *dX, Nd4jLong *dXShapeInfo,
            void *hY, Nd4jLong *hYShapeInfo,
            void *dY, Nd4jLong *dYShapeInfo,
            void *hZ, Nd4jLong *hZShapeInfo,
            void *dZ, Nd4jLong *dZShapeInfo,
            void *hDimension, Nd4jLong *hDimensionShape,
            void *dDimension, Nd4jLong *dDimensionShape);

    /**
     *
     * @param opNum
     * @param dx
     * @param xShapeInfo
     * @param y
     * @param yShapeInfo
     * @param result
     * @param resultShapeInfo
     * @param extraParams
     * @param n
     */
    void execPairwiseTransform(
            Nd4jPointer *extraPointers,
            int opNum,
            void *hX, Nd4jLong *hXShapeInfo,
            void *dX, Nd4jLong *dXShapeInfo,
            void *hY, Nd4jLong *hYShapeInfo,
            void *dY, Nd4jLong *dYShapeInfo,
            void *hZ, Nd4jLong *hZShapeInfo,
            void *dZ, Nd4jLong *dZShapeInfo,
            void *extraParams);

    void execPairwiseTransformBool(
            Nd4jPointer *extraPointers,
            int opNum,
            void *hX, Nd4jLong *hXShapeInfo,
            void *dX, Nd4jLong *dXShapeInfo,
            void *hY, Nd4jLong *hYShapeInfo,
            void *dY, Nd4jLong *dYShapeInfo,
            void *hZ, Nd4jLong *hZShapeInfo,
            void *dZ, Nd4jLong *dZShapeInfo,
            void *extraParams);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParams
     * @param result
     * @param resultShapeInfo
     */
    void  execReduceFloat(Nd4jPointer *extraPointers,
                            int opNum,
                            void *hX, Nd4jLong *hXShapeInfo,
                            void *dX, Nd4jLong *dXShapeInfo,
                            void *extraParams,
                            void *hZ, Nd4jLong *hZShapeInfo,
                            void *dZ, Nd4jLong *dZShapeInfo);

    void  execReduceSame(Nd4jPointer *extraPointers,
                          int opNum,
                          void *hX, Nd4jLong *hXShapeInfo,
                          void *dX, Nd4jLong *dXShapeInfo,
                          void *extraParams,
                          void *hZ, Nd4jLong *hZShapeInfo,
                          void *dZ, Nd4jLong *dZShapeInfo);

    void  execReduceBool(Nd4jPointer *extraPointers,
                          int opNum,
                          void *hX, Nd4jLong *hXShapeInfo,
                          void *dX, Nd4jLong *dXShapeInfo,
                          void *extraParams,
                          void *hZ, Nd4jLong *hZShapeInfo,
                          void *dZ, Nd4jLong *dZShapeInfo);


    void  execReduceLong(Nd4jPointer *extraPointers,
                          int opNum,
                          void *hX, Nd4jLong *hXShapeInfo,
                          void *dX, Nd4jLong *dXShapeInfo,
                          void *extraParams,
                          void *hZ, Nd4jLong *hZShapeInfo,
                          void *dZ, Nd4jLong *dZShapeInfo);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParams
     * @param result
     * @param resultShapeInfo
     */
    void   execReduceFloat(Nd4jPointer *extraPointers,
                            int opNum,
                            void *hX, Nd4jLong *hXShapeInfo,
                            void *dX, Nd4jLong *dXShapeInfo,
                            void *extraParams,
                            void *hZ, Nd4jLong *hZShapeInfo,
                            void *dZ, Nd4jLong *dZShapeInfo,
                           void *hDimension, Nd4jLong *hDimensionShape,
                           void *dDimension, Nd4jLong *dDimensionShape);


    void   execReduceSame(Nd4jPointer *extraPointers,
                      int opNum,
                      void *hX, Nd4jLong *hXShapeInfo,
                      void *dX, Nd4jLong *dXShapeInfo,
                      void *extraParams,
                      void *hZ, Nd4jLong *hZShapeInfo,
                      void *dZ, Nd4jLong *dZShapeInfo,
                          void *hDimension, Nd4jLong *hDimensionShape,
                          void *dDimension, Nd4jLong *dDimensionShape);


    void   execReduceBool(Nd4jPointer *extraPointers,
                      int opNum,
                      void *hX, Nd4jLong *hXShapeInfo,
                      void *dX, Nd4jLong *dXShapeInfo,
                      void *extraParams,
                      void *hZ, Nd4jLong *hZShapeInfo,
                      void *dZ, Nd4jLong *dZShapeInfo,
                          void *hDimension, Nd4jLong *hDimensionShape,
                          void *dDimension, Nd4jLong *dDimensionShape);


    void   execReduceLong(Nd4jPointer *extraPointers,
                      int opNum,
                      void *hX, Nd4jLong *hXShapeInfo,
                      void *dX, Nd4jLong *dXShapeInfo,
                      void *extraParams,
                      void *hZ, Nd4jLong *hZShapeInfo,
                      void *dZ, Nd4jLong *dZShapeInfo,
                          void *hDimension, Nd4jLong *hDimensionShape,
                          void *dDimension, Nd4jLong *dDimensionShape);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParamsVals
     * @param y
     * @param yShapeInfo
     * @param result
     * @param resultShapeInfo
     */
    void   execReduce3(Nd4jPointer *extraPointers,
                            int opNum,
                            void *hX, Nd4jLong *hXShapeInfo,
                            void *dX, Nd4jLong *dXShapeInfo,
                            void *extraParamsVals,
                            void *hY, Nd4jLong *hYShapeInfo,
                            void *dY, Nd4jLong *dYShapeInfo,
                            void *hZ, Nd4jLong *hZShapeInfo,
                            void *dZ, Nd4jLong *dZShapeInfo,
                            Nd4jLong *tadOnlyShapeInfo, Nd4jLong *tadOffsets,
                            Nd4jLong *yTadOnlyShapeInfo, Nd4jLong *yTadOffsets);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParamsVals
     * @param y
     * @param yShapeInfo
     */
    void execReduce3Scalar(Nd4jPointer *extraPointers,
                            int opNum,
                            void *hX, Nd4jLong *hXShapeInfo,
                            void *dX, Nd4jLong *dXShapeInfo,
                            void *extraParamsVals,
                            void *hY, Nd4jLong *hYShapeInfo,
                            void *dY, Nd4jLong *dYShapeInfo,
                            void *hZ, Nd4jLong *hZShapeInfo,
                            void *dZ, Nd4jLong *dZShapeInfo);
    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParamsVals
     * @param y
     * @param yShapeInfo
     * @param result
     * @param resultShapeInfoBuffer
     * @param dimension
     * @param dimensionLength
     */
    void execReduce3(Nd4jPointer *extraPointers,
                            int opNum,
                            void *hX, Nd4jLong *hXShapeInfo,
                            void *dX, Nd4jLong *dXShapeInfo,
                            void *extraParamsVals,
                            void *hY, Nd4jLong *hYShapeInfo,
                            void *dY, Nd4jLong *dYShapeInfo,
                            void *hZ, Nd4jLong *hZShapeInfo,
                            void *dZ, Nd4jLong *dZShapeInfo,
                            void *hDimension, Nd4jLong *hDimensionShape,
                            void *dDimension, Nd4jLong *dDimensionShape,
                            Nd4jLong *tadOnlyShapeInfo, Nd4jLong *tadOffsets,
                            Nd4jLong *yTadOnlyShapeInfo, Nd4jLong *yTadOffsets);


    void execReduce3All(Nd4jPointer *extraPointers,
                            int opNum,
                            void *hX, Nd4jLong *hXShapeInfo,
                            void *dX, Nd4jLong *dXShapeInfo,
                            void *extraParamsVals,
                            void *hY, Nd4jLong *hYShapeInfo,
                            void *dY, Nd4jLong *dYShapeInfo,
                            void *hZ, Nd4jLong *hZShapeInfo,
                            void *dZ, Nd4jLong *dZShapeInfo,
                            void *hDimension, Nd4jLong *hDimensionShape,
                            void *dDimension, Nd4jLong *dDimensionShape,
                            Nd4jLong *xTadShapeInfo, Nd4jLong *xOffsets,
                            Nd4jLong *yTadShapeInfo, Nd4jLong *yOffsets);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param result
     * @param resultShapeInfo
     * @param scalar
     * @param extraParams
     * @param n
     */
    void execScalar(Nd4jPointer *extraPointers,
                          int opNum,
                          void *hX, Nd4jLong *hXShapeInfo,
                          void *dX, Nd4jLong *dXShapeInfo,
                          void *hZ, Nd4jLong *hZShapeInfo,
                          void *dZ, Nd4jLong *dZShapeInfo,
                          void *hScalar, Nd4jLong *hSscalarShapeInfo,
                          void *dScalar, Nd4jLong *dSscalarShapeInfo,
                          void *extraParams);

    void execScalarBool(Nd4jPointer *extraPointers,
                    int opNum,
                    void *hX, Nd4jLong *hXShapeInfo,
                    void *dX, Nd4jLong *dXShapeInfo,
                    void *hZ, Nd4jLong *hZShapeInfo,
                    void *dZ, Nd4jLong *dZShapeInfo,
                    void *hScalar, Nd4jLong *hSscalarShapeInfo,
                    void *dScalar, Nd4jLong *dSscalarShapeInfo,
                    void *extraParams);

    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParams
     */
    void execSummaryStatsScalar(Nd4jPointer *extraPointers,
                                          int opNum,
                                          void *hX, Nd4jLong *hXShapeInfo,
                                          void *dX, Nd4jLong *dXShapeInfo,
                                          void *extraParams,
                                          void *hZ, Nd4jLong *hZShapeInfo,
                                          void *dZ, Nd4jLong *dZShapeInfo,
                                          bool biasCorrected);
    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParams
     * @param result
     * @param resultShapeInfo
     */
    void execSummaryStats(Nd4jPointer *extraPointers,
                                  int opNum,
                                  void *hX, Nd4jLong *hXShapeInfo,
                                  void *dX, Nd4jLong *dXShapeInfo,
                                  void *extraParams,
                                  void *hZ, Nd4jLong *hZShapeInfo,
                                  void *dZ, Nd4jLong *dZShapeInfo,
                                  bool biasCorrected);
    /**
     *
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param extraParams
     * @param result
     * @param resultShapeInfoBuffer
     * @param dimension
     * @param dimensionLength
     */
    void execSummaryStats(Nd4jPointer *extraPointers,
                                  int opNum,
                                  void *hX, Nd4jLong *hXShapeInfo,
                                  void *dX, Nd4jLong *dXShapeInfo,
                                  void *extraParams,
                                  void *hZ, Nd4jLong *hZShapeInfo,
                                  void *dZ, Nd4jLong *dZShapeInfo,
                                  void *hDimension, Nd4jLong *hDimensionShape,
                                  void *dDimension, Nd4jLong *dDimensionShape,
                                  bool biasCorrected,
                                  Nd4jLong *tadShapeInfo, Nd4jLong *tadOffsets);

    /**
     *
     * @param opNum
     * @param dx
     * @param xShapeInfo
     * @param result
     * @param resultShapeInfo
     * @param extraParams
     * @param n
     */
    void execTransformFloat(Nd4jPointer *extraPointers,
                              int opNum,
                              void *hX, Nd4jLong *hXShapeInfo,
                              void *dX, Nd4jLong *dXShapeInfo,
                              void *hZ, Nd4jLong *hZShapeInfo,
                              void *dZ, Nd4jLong *dZShapeInfo,
                              void *extraParams);

    void execTransformSame(Nd4jPointer *extraPointers,
                      int opNum,
                      void *hX, Nd4jLong *hXShapeInfo,
                      void *dX, Nd4jLong *dXShapeInfo,
                      void *hZ, Nd4jLong *hZShapeInfo,
                      void *dZ, Nd4jLong *dZShapeInfo,
                      void *extraParams);

    void execTransformBool(Nd4jPointer *extraPointers,
                      int opNum,
                      void *hX, Nd4jLong *hXShapeInfo,
                      void *dX, Nd4jLong *dXShapeInfo,
                      void *hZ, Nd4jLong *hZShapeInfo,
                      void *dZ, Nd4jLong *dZShapeInfo,
                      void *extraParams);

    void execTransformAny(Nd4jPointer *extraPointers,
                           int opNum,
                           void *hX, Nd4jLong *hXShapeInfo,
                           void *dX, Nd4jLong *dXShapeInfo,
                           void *hZ, Nd4jLong *hZShapeInfo,
                           void *dZ, Nd4jLong *dZShapeInfo,
                           void *extraParams);

    void execTransformStrict(Nd4jPointer *extraPointers,
                          int opNum,
                          void *hX, Nd4jLong *hXShapeInfo,
                          void *dX, Nd4jLong *dXShapeInfo,
                          void *hZ, Nd4jLong *hZShapeInfo,
                          void *dZ, Nd4jLong *dZShapeInfo,
                          void *extraParams);

    /**
     *
     * @param extraPointers
     * @param opNum
     * @param x
     * @param xShapeInfo
     * @param z
     * @param zShapeInfo
     * @param scalars
     * @param extraParams
     * @param dimension
     * @param dimensionLength
     */
    void execScalar(Nd4jPointer *extraPointers,
                          int opNum,
                          void *hX, Nd4jLong *hXShapeInfo,
                          void *dX, Nd4jLong *dXShapeInfo,
                          void *hZ, Nd4jLong *hZShapeInfo,
                          void *dZ, Nd4jLong *dZShapeInfo,
                          void *hScalars, Nd4jLong *hScalarShapeInfo,
                          void *dScalars, Nd4jLong *dScalarShapeInfo,
                          void *extraParams,
                          void *hDimension, Nd4jLong *hDimensionShape,
                          void *dDimension, Nd4jLong *dDimensionShape,
                          Nd4jLong *tadShapeInfo, Nd4jLong *tadOffsets,
                          Nd4jLong *tadShapeInfoZ, Nd4jLong *tadOffsetsZ);

    void execScalarBool(Nd4jPointer *extraPointers,
                    int opNum,
                    void *hX, Nd4jLong *hXShapeInfo,
                    void *dX, Nd4jLong *dXShapeInfo,
                    void *hZ, Nd4jLong *hZShapeInfo,
                    void *dZ, Nd4jLong *dZShapeInfo,
                    void *hScalars, Nd4jLong *hScalarShapeInfo,
                    void *dScalars, Nd4jLong *dScalarShapeInfo,
                    void *extraParams,
                    void *hDimension, Nd4jLong *hDimensionShape,
                    void *dDimension, Nd4jLong *dDimensionShape,
                    Nd4jLong *tadShapeInfo, Nd4jLong *tadOffsets,
                    Nd4jLong *tadShapeInfoZ, Nd4jLong *tadOffsetsZ);


/**
* Append an input array
* to the end of a flat array
* in a particular order
* @param offset the offset of the array to start at
* @param order the order
* @param result the result array
* @param resultShapeInfo the shape info for te array
* @param input the input for the array
* @param inputShapeInfo the shape information for that array
*/
    void flatten(
            Nd4jPointer *extraPointers,
            int offset,
            char order,
            void *result, Nd4jLong *resultShapeInfo,
            void *dresult, Nd4jLong *dresultShapeInfo,
            void *input, Nd4jLong *inputShapeInfo,
            void *dinput, Nd4jLong *dinputShapeInfo);

    void concat(
            Nd4jPointer *extraPointers,
            int dimension,
            int numArrays,
            Nd4jPointer *data, Nd4jPointer *inputShapeInfo,
            Nd4jPointer *ddata, Nd4jPointer *dinputShapeInfo,
            void *result, Nd4jLong *resultShapeInfo,
            void *dresult, Nd4jLong *dresultShapeInfo,
            Nd4jPointer *tadPointers, Nd4jPointer *offsetPointers);


    void specialConcat (
            Nd4jPointer *extraPointers,
            int dimension,
            int numArrays,
            Nd4jPointer *data,
            Nd4jPointer *inputShapeInfo,
            void *result,
            Nd4jLong *resultShapeInfo,
            Nd4jPointer *tadPointers,
            Nd4jPointer *offsetPointers);

    /**
     * This method implementation exists only for cuda.
     * The other backends should have dummy method for JNI compatibility reasons.
     */
    void initializeDevicesAndFunctions();

    void initializeFunctions(Nd4jPointer *functions);

    /**
     * This method acquires memory chunk of requested size on host side
     *
     * @param pointer pointer that'll be used for allocation
     * @param memorySize memory size, in bytes
     * @param flags optional parameter
     */
    Nd4jPointer mallocHost(Nd4jLong memorySize, int flags);

    /**
     * This method acquires memory chunk of requested size on specified device
     *
     * @param pointer pointer that'll be used for allocation
     * @param memorySize memory size, in bytes
     * @param ptrToDeviceId pointer to deviceId. For cuda that's just and int, for OpenCL that's pointer to device_id, etc
     * @param flags optional parameter
     */
    Nd4jPointer mallocDevice(Nd4jLong memorySize, Nd4jPointer ptrToDeviceId, int flags);

    /**
     * This method releases previously allocated host memory space
     *
     * @param pointer pointer that'll be freed
     */
    int freeHost(Nd4jPointer pointer);

    /**
     * This method releases previously allocated memory space on device
     *
     * @param pointer pointer that'll be freed
     * @param ptrToDeviceId pointer to deviceId.
     */
    int freeDevice(Nd4jPointer pointer, Nd4jPointer ptrToDeviceId);

    /**
     *
     * @return
     */
    int ompGetMaxThreads();

    /**
     *
     * @return
     */
    int ompGetNumThreads();

    /**
     *
     * @param threads
     */
    void setOmpNumThreads(int threads);

    /**
     *
     * @param threads
     */
    void setOmpMinThreads(int threads);




    /**
     *
     * @return
     */
    Nd4jPointer createContext();

    /**
     *
     * @return
     */
    Nd4jPointer createStream();

    /**
     *
     * @return
     */
    Nd4jPointer createEvent();

    /**
     *
     * @param event
     * @param stream
     * @return
     */
    int registerEvent(Nd4jPointer event, Nd4jPointer stream);

    /**
     *
     * @param event
     * @return
     */
    int destroyEvent(Nd4jPointer event);

    /**
     *
     * @param ptrToDeviceId
     * @return
     */
    int setDevice(Nd4jPointer ptrToDeviceId);

    /**
     *
     * @return
     */
    int getDevice();

    /**
     *
     * @param stream
     * @return
     */
    int streamSynchronize(Nd4jPointer stream);

    /**
     *
     * @param event
     * @return
     */
    int eventSynchronize(Nd4jPointer event);

    /**
     *
     * @param ptrToDeviceId
     * @return
     */
    Nd4jLong getDeviceFreeMemory(Nd4jPointer ptrToDeviceId);

    /**
     *
     * @param ptrToDeviceId
     * @return
     */
    Nd4jLong getDeviceTotalMemory(Nd4jPointer ptrToDeviceId);

    /**
     *
     * @param ptrToDeviceId
     * @return
     */
    int getDeviceMajor(Nd4jPointer ptrToDeviceId);

    /**
     *
     * @param ptrToDeviceId
     * @return
     */
    int getDeviceMinor(Nd4jPointer ptrToDeviceId);

    /**
     *
     * @param ptrToDeviceId
     * @return
     */
    const char * getDeviceName(Nd4jPointer ptrToDeviceId);

    /**
     *
     * @param dst
     * @param src
     * @param size
     * @param flags
     * @param reserved
     * @return
     */
    int memcpy(Nd4jPointer dst,
               Nd4jPointer src,
               Nd4jLong size,
               int flags,
               Nd4jPointer reserved);

    /**
     *
     * @param dst
     * @param src
     * @param size
     * @param flags
     * @param reserved
     * @return
     */
    int memcpyAsync(Nd4jPointer dst,
                    Nd4jPointer src,
                    Nd4jLong size,
                    int flags,
                    Nd4jPointer reserved);

    /**
     *
     * @param dst
     * @param value
     * @param size
     * @param flags
     * @param reserved
     * @return
     */
    int memset(Nd4jPointer dst,
               int value,
               Nd4jLong size,
               int flags,
               Nd4jPointer reserved);

    /**
     *
     * @param dst
     * @param value
     * @param size
     * @param flags
     * @param reserved
     * @return
     */
    int memsetAsync(Nd4jPointer dst,
                    int value,
                    Nd4jLong size,
                    int flags,
                    Nd4jPointer reserved);

    /**
     *
     * @param dst
     * @param src
     * @param size
     * @param flags
     * @param reserved
     * @return
     */
    int memcpyConstantAsync(Nd4jLong dst,
                            Nd4jPointer src,
                            Nd4jLong size,
                            int flags,
                            Nd4jPointer reserved);

    /**
     *
     * @return
     */
    Nd4jPointer getConstantSpace();

    /**
     *
     * @return
     */
    int getAvailableDevices();

    /**
     *
     * @param reallyEnable
     */
    void enableDebugMode(bool reallyEnable);

    /**
     *
     * @param reallyEnable
     */
    void enableVerboseMode(bool reallyEnable);

    /**
     *
     * @param gridSize
     */
    void setGridLimit(int gridSize);

    /**
     *
     * @param xShapeInfo
     * @param dimension
     * @param dimensionLength
     * @param targetBuffer
     * @param offsetsBuffer
     */
    void tadOnlyShapeInfo(Nd4jLong *xShapeInfo,
                          int *dimension,
                          int dimensionLength,
                          Nd4jLong *targetBuffer,
                          Nd4jLong *offsetsBuffer);

    /*
     * PullRow special op
     */

    /**
     *
     * @param extraPointers
     * @param x
     * @param xShapeInfo
     * @param z
     * @param zShapeInfo
     * @param n
     * @param indexes
     * @param tadShapeInfo
     * @param tadOffsets
     * @param zTadShapeInfo
     * @param zTadOffsets
     */
    void pullRows(Nd4jPointer *extraPointers,
                        void *x, Nd4jLong *xShapeInfo,
                        void *dx, Nd4jLong *dxShapeInfo,
                        void *z, Nd4jLong *zShapeInfo,
                        void *dz, Nd4jLong *dzShapeInfo,
                        Nd4jLong n,
                        Nd4jLong *indexes,
                        Nd4jLong *tadShapeInfo,
                        Nd4jLong *tadOffsets,
                        Nd4jLong *zTadShapeInfo,
                        Nd4jLong *zTadOffsets);

    /**
     *
     * @param extras
     * @param dx
     * @param dz
     * @param n
     * @param length
     * @param propagate
     */
    void average(Nd4jPointer *extras,
                       Nd4jPointer *x, Nd4jLong *xShapeInfo,
                       Nd4jPointer *dx, Nd4jLong *dxShapeInfo,
                       void *z, Nd4jLong *zShapeInfo,
                       void *dz, Nd4jLong *dzShapeInfo,
                       int n,
                       Nd4jLong length,
                       bool propagate);


    void accumulate(Nd4jPointer *extras,
                       Nd4jPointer *x, Nd4jLong *xShapeInfo,
                       Nd4jPointer *dx, Nd4jLong *dxShapeInfo,
                       void *z, Nd4jLong *zShapeInfo,
                       void *dz, Nd4jLong *dzShapeInfo,
                       int n,
                       Nd4jLong length);


    /**
     * P2P enabler
     */
    /**
     *
     * @param enable
     */
    void enableP2P(bool enable);

    /**
     *
     */
    void checkP2P();

    /**
     *
     * @return
     */
    bool isP2PAvailable();

    /**
     * Shuffle methods
     */

    /**
     *
     * @param extras
     * @param dx
     * @param xShapeInfo
     * @param dz
     * @param zShapeInfo
     * @param N
     * @param shuffleMap
     * @param tadShapeInfo
     * @param tadOffsets
     */
    void shuffle(Nd4jPointer *extras,
                       Nd4jPointer *x, Nd4jPointer *xShapeInfo,
                       Nd4jPointer *dx, Nd4jPointer *dxShapeInfo,
                       Nd4jPointer *z, Nd4jPointer *zShapeInfo,
                       Nd4jPointer *dz, Nd4jPointer *dzShapeInfo,
                       int N,
                       int *shuffleMap,
                       Nd4jPointer *tadShapeInfo,
                       Nd4jPointer *tadOffsets);


    /**
     * Type Conversions
     */

    /**
     *
     * @param extras
     * @param srcType
     * @param x
     * @param N
     * @param dstType
     * @param z
     */
    void convertTypes(Nd4jPointer *extras, int srcType, Nd4jPointer x, Nd4jLong N, int dstType, Nd4jPointer z);


    /**
     *
     * @return
     */
    bool isExperimentalEnabled();

    /**
     * Aggregate
     */

    /**
     *
     * @param extraPointers
     * @param opNum
     * @param arguments
     * @param numArguments
     * @param shapeArguments
     * @param numShapeArguments
     * @param indexArguments
     * @param numIndexArguments
     * @param intArrays
     * @param numIntArrays
     * @param realArguments
     * @param numRealArguments
     */
    void execAggregate(Nd4jPointer *extraPointers,
                             int opNum,
                             void **arguments,
                             int numArguments,
                             Nd4jLong **shapeArguments,
                             int numShapeArguments,
                             int *indexArguments,
                             int numIndexArguments,
                             int **intArrays,
                             int numIntArrays,
                             void *realArguments,
                             int numRealArguments,
                             nd4j::DataType dtype);


    template <typename T>
    void _batchExecutor(Nd4jPointer *extraPointers,
                                   int numAggregates,
                                   int opNum,
                                   int maxArgs,
                                   int maxShapes,
                                   int maxIntArrays,
                                   int maxIntArraySize,
                                   int maxIdx,
                                   int maxReals,
                                   void *ptrToArguments,
                                   nd4j::DataType dtype);

    void execAggregateBatch(Nd4jPointer *extraPointers,
                                  int numAggregates,
                                  int opNum,
                                  int maxArgs,
                                  int maxShapes,
                                  int maxIntArrays,
                                  int maxIntArraySize,
                                  int maxIdx,
                                  int maxReals,
                                  void *ptrToArguments,
                                  nd4j::DataType dtype);

    /**
     * Random operations
     */

    /**
     *
     * @param extraPointers
     * @param opNum
     * @param state
     * @param z
     * @param zShapeBuffer
     * @param extraArguments
     */
    void execRandom(Nd4jPointer *extraPointers,
                          int opNum,
                          Nd4jPointer state,
                          void *hZ, Nd4jLong *hZShapeBuffer,
                          void *dZ, Nd4jLong *dZShapeBuffer,
                          void *extraArguments);

    /**
     *
     * @param extraPointers
     * @param opNum
     * @param state
     * @param x
     * @param xShapeBuffer
     * @param y
     * @param yShapeBuffer
     * @param z
     * @param zShapeBuffer
     * @param extraArguments
     */
    void execRandom(Nd4jPointer *extraPointers,
                          int opNum,
                          Nd4jPointer state,
                          void *hX, Nd4jLong *hXShapeBuffer,
                          void *dX, Nd4jLong *dXShapeBuffer,
                          void *hY, Nd4jLong *hYShapeBuffer,
                          void *dY, Nd4jLong *dYShapeBuffer,
                          void *hZ, Nd4jLong *hZShapeBuffer,
                          void *dZ, Nd4jLong *dZShapeBuffer,
                          void *extraArguments);

    /**
     *
     * @param extraPointers
     * @param opNum
     * @param state
     * @param x
     * @param xShapeBuffer
     * @param z
     * @param zShapeBuffer
     * @param extraArguments
     */
    void execRandom(Nd4jPointer *extraPointers,
                          int opNum,
                          Nd4jPointer state,
                          void *hX, Nd4jLong *hXShapeBuffer,
                          void *dX, Nd4jLong *dXShapeBuffer,
                          void *hZ, Nd4jLong *hZShapeBuffer,
                          void *dZ, Nd4jLong *dZShapeBuffer,
                          void *extraArguments);


    /**
     *
     * @param extraPointers
     * @param seed
     * @param bufferSize
     * @param ptrToBuffer
     * @return
     */
    Nd4jPointer initRandom(Nd4jPointer *extraPointers,
                           long seed,
                           long bufferSize,
                           Nd4jPointer ptrToBuffer);

    /**
     *
     * @param extraPointers
     * @param seed
     * @param ptrRandom
     */
    void refreshBuffer(Nd4jPointer *extraPointers,
                       long seed,
                       Nd4jPointer ptrRandom);

    /**
     *
     * @param extraPointers
     * @param seed
     * @param ptrRandom
     */
    void reSeedBuffer(Nd4jPointer *extraPointers,
                      long seed,
                      Nd4jPointer ptrRandom);

    /**
     *
     * @param ptrRandom
     */
    void destroyRandom(Nd4jPointer ptrRandom);

    /**
     * Grid operations
     */




    /**
     *
     * @param extras
     * @param opTypeA
     * @param opNumA
     * @param opTypeB
     * @param opNumB
     * @param N
     * @param dx
     * @param xShapeInfo
     * @param dy
     * @param yShapeInfo
     * @param dz
     * @param zShapeInfo
     * @param extraA
     * @param extraB
     * @param scalarA
     * @param scalarB
     */
     /*
    void execMetaPredicateShape(Nd4jPointer *extras,
                                      const int opTypeA,
                                      const int opNumA,
                                      const int opTypeB,
                                      const int opNumB,
                                      Nd4jLong N,
                                      void *hX, Nd4jLong *hXShapeBuffer,
                                      void *dX, Nd4jLong *dXShapeBuffer,
                                      void *hY, Nd4jLong *hYShapeBuffer,
                                      void *dY, Nd4jLong *dYShapeBuffer,
                                      void *hZ, Nd4jLong *hZShapeBuffer,
                                      void *dZ, Nd4jLong *dZShapeBuffer,
                                      void *extraA,
                                      void *extraB,
                                      double scalarA,
                                      double scalarB);

*/
/**
 *
 * @param data
 * @param shapeBuffer
 * @param wordSize
 * @param headerSize
 * @return
 */
    Nd4jPointer numpyHeaderForNd4j(Nd4jPointer data,Nd4jPointer shapeBuffer,Nd4jLong wordSize,Nd4jLong *headerSize) {
        Nd4jLong *shapeBufferCast = reinterpret_cast<Nd4jLong *>(shapeBuffer);
        int  rank = shape::rank(shapeBufferCast);
        Nd4jLong *shape = shape::shapeOf(shapeBufferCast);
        unsigned int *npShape = new unsigned int[rank];
        for(int i = 0; i < rank; i++) {
            npShape[i] = shape[i];
        }

        Nd4jLong length = shape::prodLong(shape,rank);
        auto npHeader = cnpy::createNpyHeader(data,npShape,rank,wordSize);
        char *ret = new char[npHeader.size() + 1];
        int count = 0;
        for(int i = 0; i < npHeader.size(); i++) {
            if (npHeader[i] != '\0') {
                ret[count] = npHeader[i];
                count++;
            }
            else {
                nd4j_debug("Found null terminated at %d. Skipping\n",i);
            }
        }

        ret[count] = '\0';
        count++;
        *headerSize = count;
        return reinterpret_cast<Nd4jPointer>(ret);

    }

/**
   * Load numpy from a header
    * based on the cnpy parse from header method.
   * @param data the header data to parse
   * @return a pointer to a numpy cnpy:NpyArray struct
   */
    Nd4jPointer loadNpyFromHeader(Nd4jPointer data) {
        char *header = reinterpret_cast<char *>(data);

        cnpy::NpyArray arr = cnpy::loadNpyFromHeader(header);
        cnpy::NpyArray *ret = new cnpy::NpyArray();
        int totalLengthOfShape = 1;
        for(int i = 0; i < arr.shape.size(); i++) {
            totalLengthOfShape *= arr.shape[i];
        }

        ret->data = arr.data;
        ret->wordSize = arr.wordSize;
        ret->shape = arr.shape;
        return reinterpret_cast<Nd4jPointer>(ret);
    }


/**
   * Create a numpy array from an nd4j
   * array
   * @param data a pointer to the data
   * @param shapeBuffer  the shapebuffer for the nd4j array
   * @param wordSize  the word size (4 for float, 8 for doubles)
   * @return a pointer to a numpy array
   */
    Nd4jPointer numpyFromNd4j(Nd4jPointer data,Nd4jPointer shapeBuffer,Nd4jLong wordSize) {
        Nd4jLong *shapeBufferCast = reinterpret_cast<Nd4jLong *>(shapeBuffer);
        int  rank = shape::rank(shapeBufferCast);
        Nd4jLong *shape = shape::shapeOf(shapeBufferCast);
        unsigned int *npShape = new unsigned int[rank];
        for(int i = 0; i < rank; i++) {
            npShape[i] = shape[i];
        }

        Nd4jLong length = shape::prodLong(shape,rank);
        auto npHeader = cnpy::createNpyHeader(data,npShape,rank,wordSize);
        char *dataChar = reinterpret_cast<char *>(data);
        char *npHeaderData = npHeader.data();
        char *ret = new char[(wordSize * length) +  npHeader.size()];
        char *cursorStart = ret;
        std::memcpy(reinterpret_cast<void *>(ret), reinterpret_cast<void *>(npHeaderData), npHeader.size() * sizeof(Nd4jLong));
        //move to next
        cursorStart += npHeader.size();
        std::memcpy(reinterpret_cast<void *>(ret), reinterpret_cast<void *>(dataChar), length * wordSize * sizeof(Nd4jLong));
        Nd4jPointer  rettPointer = reinterpret_cast<Nd4jPointer>(ret);
        return rettPointer;
    }


/**
 *
 * @param npyArray
 * @return
 */
    Nd4jPointer shapeBufferForNumpy(Nd4jPointer npyArray) {
        cnpy::NpyArray arr = cnpy::loadNpyFromPointer(reinterpret_cast<char *>(npyArray));
        auto shape = new unsigned int[arr.shape.size()];
        for(unsigned int i = 0; i < arr.shape.size(); i++) {
            shape[i] = arr.shape[i];
        }

        auto shapeBuffer = shape::shapeBufferOfNpy(arr.shape.size(), shape, arr.fortranOrder);
        delete[] shape;
        return reinterpret_cast<Nd4jPointer>(shapeBuffer);
    }


/**
* Get the shape buffer from a
* numpy array.
* **Warning** this allocates memory
* @param npyArray
* @return
*/
    Nd4jPointer shapeBufferForNumpyHeader(Nd4jPointer npyArray) {
        cnpy::NpyArray arr = cnpy::loadNpyFromHeader(reinterpret_cast<char *>(npyArray));
        auto shape = new unsigned int[arr.shape.size()];
        for(unsigned int i = 0; i < arr.shape.size(); i++) {
            shape[i] = arr.shape[i];
        }

        auto shapeBuffer = shape::shapeBufferOfNpy(arr.shape.size(), shape, arr.fortranOrder);
        delete[] shape;
        return reinterpret_cast<Nd4jPointer>(shapeBuffer);
    }



/**
 *
 * @param npyArray
 * @return
 */
    Nd4jPointer dataPointForNumpyHeader(Nd4jPointer npyArray) {
        cnpy::NpyArray arr = cnpy::loadNpyFromHeader(reinterpret_cast<char *>(npyArray));
        unsigned  char *dataToPrint = reinterpret_cast<unsigned  char *>(arr.data);
        return dataToPrint;
    }

/**
 *
 * @param npyArray
 * @return
 */
    Nd4jPointer dataPointForNumpyStruct(Nd4jPointer npyArrayStruct) {
        cnpy::NpyArray *arrPointer = reinterpret_cast<cnpy::NpyArray *>(npyArrayStruct);
        unsigned  char *dataToPrint = reinterpret_cast<unsigned  char *>(arrPointer->data);
        return reinterpret_cast<Nd4jPointer>(dataToPrint);
    }

/**
 *
 * @param npyArray
 * @param fromFile
 * @return
 */
    Nd4jPointer dataPointForNumpy(Nd4jPointer npyArray) {
        char *npyArrayBuffer = reinterpret_cast<  char *>(npyArray);
        cnpy::NpyArray arr = cnpy::loadNpyFromPointer(npyArrayBuffer);
        return dataPointForNumpyStruct(reinterpret_cast<Nd4jPointer>(&arr));
    }

/**
 * Load a numpy array from a file
 * and return it as an Nd4jPointer
 * @param path
 * @return
 */
    Nd4jPointer numpyFromFile(std::string path) {
        char *numpyBuffer = cnpy::loadFile(path.data());
        return reinterpret_cast<Nd4jPointer >(numpyBuffer);
    }


    ////// NPZ //////

    void* mapFromNpzFile(std::string path){
        cnpy::npz_t* mapPtr = new cnpy::npz_t();
        cnpy::npz_t map = cnpy::npzLoad(path);
        mapPtr->insert(map.begin(), map.end());
        return reinterpret_cast<void*>(mapPtr);
    }


    int getNumNpyArraysInMap(void *map){
        cnpy::npz_t* arrays = reinterpret_cast<cnpy::npz_t*>(map);
        int n = arrays->size();
        return n;
    }

    const char* getNpyArrayNameFromMap(void *map, int index){
        cnpy::npz_t* arrays = reinterpret_cast<cnpy::npz_t*>(map);
        cnpy::npz_t::iterator it = arrays->begin();
        cnpy::npz_t::iterator end = arrays->end();
        int cnt = 0;
        for(; it != end; ++it, ++cnt){
            if (cnt == index){
                // FIXME: @fariz, this is a leak!
                return const_cast<const char *>(strdup(it->first.c_str()));
            }
        }
        throw std::runtime_error("No array at index.");
    }

    void* getNpyArrayFromMap(void *map, int index){
        cnpy::npz_t* arrays = reinterpret_cast<cnpy::npz_t*>(map);
        cnpy::npz_t::iterator it = arrays->begin();
        cnpy::npz_t::iterator end = arrays->end();
        cnpy::NpyArray *arr = new cnpy::NpyArray();
        int cnt = 0;
        for(; it != end; ++it, ++cnt){
            if (cnt == index){
                *arr = it->second;
                return arr;
            }
        }
        throw std::runtime_error("No array at index.");
    }

    void* getNpyArrayData(void *npArray){
        cnpy::NpyArray* npyArray2 = reinterpret_cast<cnpy::NpyArray*>(npArray);
        return reinterpret_cast<void*>(npyArray2->data);
    }

    int getNpyArrayRank(void *npArray){
        cnpy::NpyArray* arr = reinterpret_cast<cnpy::NpyArray*>(npArray);
        int rank = arr->shape.size();
        return rank;
    }

    Nd4jLong* getNpyArrayShape(void *npArray){
        cnpy::NpyArray* arr = reinterpret_cast<cnpy::NpyArray*>(npArray);
        int ndim = arr->shape.size();
        Nd4jLong* shape = new Nd4jLong[ndim];
        for (int i=0; i<ndim; i++){
            shape[i] = arr->shape.at(i);
        }
        return shape;
    }

    char getNpyArrayOrder(void *npArray){
        cnpy::NpyArray* arr = reinterpret_cast<cnpy::NpyArray*>(npArray);
        return (arr->fortranOrder)?'f':'c';
    }

    int getNpyArrayElemSize(void *npArray){
        cnpy::NpyArray* arr = reinterpret_cast<cnpy::NpyArray*>(npArray);
        return arr->wordSize;
    }

    void deleteNPArrayStruct(void *npArray){
        cnpy::NpyArray* arr = reinterpret_cast<cnpy::NpyArray*>(npArray);
        delete arr;
    }

    void deleteNPArrayMap(void *map){
        cnpy::npz_t* arrays = reinterpret_cast<cnpy::npz_t*>(map);
        delete arrays;
    }
    //////

/**
  * Get the element size for a numpy array
  * @param npyArray  the numpy array's address
  * to get the length for
  * @return
  */
    int elementSizeForNpyArray(Nd4jPointer npyArray) {
        cnpy::NpyArray arr = cnpy::loadNpyFromPointer(reinterpret_cast<char *>(npyArray));
        cnpy::NpyArray *arrPointer = &arr;
        int size = arrPointer->wordSize;
        // arrPointer->destruct();
        return size;
    }


/**
* Get the element size for a numpy array
* @param npyArray  the numpy array's address
* to get the length for
* @return
*/
    int elementSizeForNpyArrayHeader(Nd4jPointer npyArray) {
        cnpy::NpyArray arr = cnpy::loadNpyFromHeader(reinterpret_cast<char *>(npyArray));
        cnpy::NpyArray *arrPointer = &arr;
        int size = arrPointer->wordSize;
        return size;
    }


    void releaseNumpy(Nd4jPointer npyArray) {
        free(reinterpret_cast<void *>(npyArray));
    }


    /**
     * Return the length of a shape buffer
     * based on the pointer
     * @param buffer  the buffer pointer to check
     * @return
     */
    int lengthForShapeBufferPointer(Nd4jPointer buffer);


      /**
   * The pointer to get the address for
   *
   * @param address the address to get the pointer
   * @return the pointer for the given address
   */

    Nd4jPointer pointerForAddress(Nd4jLong address);

    /**
     * This method takes single N-dimensional tensor, and copies its TADs to target arrays
     *
     * @param x
     * @param xShapeInfo
     * @param targets
     * @param zShapeInfo
     * @return
     */
    void tear(Nd4jPointer *extraPointers,
            void *x, Nd4jLong *xShapeInfo,
            void *dx, Nd4jLong *dxShapeInfo,
            Nd4jPointer *targets, Nd4jLong *zShapeInfo,
            Nd4jLong *tadShapeInfo,
            Nd4jLong *tadOffsets);

    Nd4jLong encodeBitmap(Nd4jPointer *extraPointers, void *dx, Nd4jLong *xShapeInfo, Nd4jLong N, int *dz, float threshold);
    void decodeBitmap(Nd4jPointer *extraPointers, void *dx, Nd4jLong N, void *dz, Nd4jLong *zShapeInfo);


    void encodeThresholdP1(Nd4jPointer *extraPointers, void *dx, Nd4jLong *xShapeInfo, Nd4jLong N, int *dz, float threshold);
    void encodeThresholdP2Int(Nd4jPointer *extraPointers, int *dx, Nd4jLong N, int *dz);
    void encodeThresholdP3(Nd4jPointer *extraPointers, void *dx, Nd4jLong *xShapeInfo, int *offsets, Nd4jLong N, int *dz);


    void decodeThreshold(Nd4jPointer *extraPointers, void *dx, Nd4jLong N, void *dz, Nd4jLong *zShapeInfo);


    void sort(Nd4jPointer *extraPointers,
            void *x, Nd4jLong *xShapeInfo,
            void *dx, Nd4jLong *dxShapeInfo,
            bool descending);

    void sortTad(Nd4jPointer *extraPointers,
            void *x, Nd4jLong *xShapeInfo,
            void *dx, Nd4jLong *dxShapeInfo,
            int *dimension,
            int dimensionLength,
            Nd4jLong *tadShapeInfo,
            Nd4jLong *tadOffsets,
            bool descending);


    // special sort impl for sorting out COO indices and values
    void sortCooIndices(Nd4jPointer *extraPointers, Nd4jLong *indices, void *values, Nd4jLong length, int rank);


    Nd4jLong* mmapFile(Nd4jPointer *extraPointers, const char *fileName, Nd4jLong length);

    void munmapFile(Nd4jPointer *extraPointers, Nd4jLong* ptrMap, Nd4jLong length);


    // flatbuffers execution
    nd4j::graph::ResultWrapper* executeFlatGraph(Nd4jPointer *extraPointers, Nd4jPointer flatBufferPointer);


    const char* getAllCustomOps();

    const char* getAllOperations();

    // customOp executioner
    int execCustomOp(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputBuffers, Nd4jPointer* inputShapes, int numInputs, Nd4jPointer* outputBuffers, Nd4jPointer* outputShapes, int numOutputs, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs, bool* bArgs, int numBArgs, bool isInplace);
    int execCustomOp(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer opContext);

    nd4j::ShapeList* calculateOutputShapes(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputShapes, int numInputShapes, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs);
    nd4j::ShapeList* calculateOutputShapes(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputBuffers, Nd4jPointer* inputShapes, int numInputShapes, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs, bool *bArgs, int numBArgs);

    void deleteShapeList(Nd4jPointer shapeList);

    int registerGraph(Nd4jPointer *extraPointers, Nd4jLong graphId, Nd4jPointer flatBufferPointer);

    nd4j::graph::VariablesSet *executeStoredGraph(Nd4jPointer *extraPointers, Nd4jLong graphId, Nd4jPointer *inputBuffers, Nd4jPointer *inputShapes, int* inputIndices, int numInputs);

    int unregisterGraph(Nd4jPointer *extraPointers, Nd4jLong graphId);

    void deleteIntArray(Nd4jPointer pointer);
    void deleteLongArray(Nd4jPointer pointer);
    void deletePointerArray(Nd4jPointer pointer);

    void deleteVariablesSet(Nd4jPointer pointer);

    // GraphState creation
    Nd4jPointer getGraphState(Nd4jLong id);

    void deleteGraphState(Nd4jPointer state);

    void deleteResultWrapper(Nd4jPointer ptr);

    int estimateThreshold(Nd4jPointer *extraPointers, Nd4jPointer x, Nd4jLong *xShapeInfo, int N, float threshold);

    // this method executes op that requires scope to be present: if/while/cond/whatever
    Nd4jStatus execCustomOpWithScope(Nd4jPointer *extraPointers, Nd4jPointer state, Nd4jLong opHash, Nd4jLong *scopes, int numScopes, Nd4jPointer *inputBuffers, Nd4jPointer *inputShapes, int numInputs, Nd4jPointer *outputBuffers, Nd4jPointer *outputShapes, int numOutputs);

    //void fillUtf8String(Nd4jPointer *extraPointers, const char **string, int numStrings, Nd4jPointer buffer);
    Nd4jPointer createUtf8String(Nd4jPointer *extraPointers, const char *string, int length);
    void deleteUtf8String(Nd4jPointer *extraPointers, Nd4jPointer ptr);

    void scatterUpdate(Nd4jPointer *extraPointers, int opCode, int numOfSubArrs,
                      void* hX, Nd4jLong* hXShapeInfo, Nd4jLong* hXOffsets,
                      void* dX, Nd4jLong* dXShapeInfo, Nd4jLong* dXOffsets,
                      void* hY, Nd4jLong* hYShapeInfo, Nd4jLong* hYOffsets,
                      void* dY, Nd4jLong* dYShapeInfo, Nd4jLong* dYOffsets,
                      int* hIindexes, int* dIindexes);

};






#endif //NATIVEOPERATIONS_NATIVEOPS_H
