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

#define __STDC_CONSTANT_MACROS

#include "../NativeOps.h"
#include "../NativeOpExcutioner.h"
#include "../NDArray.h"
#include "../GraphExecutioner.h"
#include <graph/GraphHolder.h>
#include <templatemath.h>
#include <types/float8.h>
#include <loops/type_conversions.h>
#include <loops/aggregates.h>
#include <helpers/helper_ptrmap.h>
#include <helpers/logger.h>
#include <pointercast.h>
#include <pairwise_util.h>
#include <types/types.h>
#include <ops/declarable/helpers/transforms.h>


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#else
#include <io.h>
#include <helpers/mman.h>
#endif
#include <sys/types.h>

#include <ops/declarable/CustomOperations.h>
#include <errno.h>


char *name;
bool nameSet = false;


#ifdef __ND4J_EXPERIMENTAL__
bool experimentalSupport = true;
#else
bool experimentalSupport = false;
#endif

#include <ops/specials.h>
#include "../Environment.h"
#include <TAD.h>
#include <ops/declarable/OpRegistrator.h>
#include <graph/Context.h>
#include <graph/ResultWrapper.h>

using namespace nd4j;

void NativeOps::setElementThreshold(int num) {
    if (num > 0)
        nd4j::Environment::getInstance()->setElementwiseThreshold(num);
}

void NativeOps::setTADThreshold(int num) {
    if (num > 0)
        nd4j::Environment::getInstance()->setTadThreshold(num);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 */
void NativeOps::execIndexReduceScalar(Nd4jPointer *extraPointers,
                                                int opNum,
                                                void *hX, Nd4jLong *hXShapeInfo,
                                                void *dX, Nd4jLong *dXShapeInfo,
                                                void *extraParams,
                                                void *hZ, Nd4jLong *hZShapeInfo,
                                                void *dZ, Nd4jLong *dZShapeInfo) {

    NativeOpExcutioner::execIndexReduceScalar(opNum, hX, hXShapeInfo, extraParams, hZ, hZShapeInfo);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 * @param hZ
 * @param hZShapeInfo
 * @param dimension
 * @param dimensionLength
 */
void  NativeOps::execIndexReduce(Nd4jPointer *extraPointers,int opNum,
                                        void *hX, Nd4jLong *hXShapeInfo,
                                        void *dX, Nd4jLong *dXShapeInfo,
                                        void *extraParams,
                                        void *hZ, Nd4jLong *hZShapeInfo,
                                        void *dZ, Nd4jLong *dZShapeInfo,
                                        void *hDimension, Nd4jLong *hDimensionShape,
                                        void *dDimension, Nd4jLong *dDimensionShape) {

    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    Nd4jLong *hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    Nd4jLong *hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);
    auto hz = reinterpret_cast<Nd4jLong*>(hZ);

    NativeOpExcutioner::execIndexReduce(opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hz,
            hZShapeInfo,
            dimension,
            dimensionLength,
            hTADShapeInfo,
            hTADOffsets);
}


/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param hY
 * @param hYShapeInfo
 * @param hZ
 * @param hZShapeInfo
 * @param dimension
 * @param dimensionLength
 */
void NativeOps::execBroadcast(Nd4jPointer *extraPointers,
                                      int opNum,
                                      void *hX, Nd4jLong *hXShapeInfo,
                                      void *dX, Nd4jLong *dXShapeInfo,
                                      void *hY, Nd4jLong *hYShapeInfo,
                                      void *dY, Nd4jLong *dYShapeInfo,
                                      void *hZ, Nd4jLong *hZShapeInfo,
                                      void *dZ, Nd4jLong *dZShapeInfo,
                                      void *hDimension, Nd4jLong *hDimensionShape,
                                      void *dDimension, Nd4jLong *dDimensionShape) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    Nd4jLong *hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    Nd4jLong *hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);
    Nd4jLong *hTADShapeInfoZ = reinterpret_cast<Nd4jLong *>(extraPointers[2]);
    Nd4jLong *hTADOffsetsZ = reinterpret_cast<Nd4jLong *>(extraPointers[3]);

    NativeOpExcutioner::execBroadcast(
            opNum,
            hX,
            hXShapeInfo,
            hY,
            hYShapeInfo,
            hZ, hZShapeInfo,
            dimension,
            dimensionLength, hTADShapeInfo, hTADOffsets, hTADShapeInfoZ, hTADOffsetsZ);
}

void NativeOps::execBroadcastBool(Nd4jPointer *extraPointers,
                              int opNum,
                              void *hX, Nd4jLong *hXShapeInfo,
                              void *dX, Nd4jLong *dXShapeInfo,
                              void *hY, Nd4jLong *hYShapeInfo,
                              void *dY, Nd4jLong *dYShapeInfo,
                              void *hZ, Nd4jLong *hZShapeInfo,
                              void *dZ, Nd4jLong *dZShapeInfo,
                                  void *hDimension, Nd4jLong *hDimensionShape,
                                  void *dDimension, Nd4jLong *dDimensionShape) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    Nd4jLong *hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    Nd4jLong *hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);
    Nd4jLong *hTADShapeInfoZ = reinterpret_cast<Nd4jLong *>(extraPointers[2]);
    Nd4jLong *hTADOffsetsZ = reinterpret_cast<Nd4jLong *>(extraPointers[3]);

    NativeOpExcutioner::execBroadcastBool(
            opNum,
            hX,
            hXShapeInfo,
            hY,
            hYShapeInfo,
            hZ, hZShapeInfo,
            dimension,
            dimensionLength, hTADShapeInfo, hTADOffsets, hTADShapeInfoZ, hTADOffsetsZ);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param hY
 * @param hYShapeInfo
 * @param hZ
 * @param hZShapeInfo
 * @param extraParams
 * @param n
 */
void NativeOps::execPairwiseTransform(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hY, Nd4jLong *hYShapeInfo,
        void *dY, Nd4jLong *dYShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {
    NativeOpExcutioner::execPairwiseTransform(
            opNum,
            hX,
            hXShapeInfo,
            hY,
            hYShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams);
}

void NativeOps::execPairwiseTransformBool(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hY, Nd4jLong *hYShapeInfo,
        void *dY, Nd4jLong *dYShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {
    NativeOpExcutioner::execPairwiseBoolTransform(
            opNum,
            hX,
            hXShapeInfo,
            hY,
            hYShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 * @param hZ
 * @param hZShapeInfo
 */
void NativeOps::execReduceFloat(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *extraParams,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo) {

    NativeOpExcutioner::execReduceFloatScalar(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo);

}

void NativeOps::execReduceSame(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *extraParams,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo) {

    NativeOpExcutioner::execReduceSameScalar(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo);

}

void NativeOps::execReduceBool(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *extraParams,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo) {

    NativeOpExcutioner::execReduceBoolScalar(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo);

}

void NativeOps::execReduceLong(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *extraParams,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo) {

    NativeOpExcutioner::execReduceLongScalar(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo);

}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 * @param hZ
 * @param hZShapeInfo
 */
void NativeOps::execReduceFloat(Nd4jPointer *extraPointers,
                                   int opNum,
                                   void *hX, Nd4jLong *hXShapeInfo,
                                   void *dX, Nd4jLong *dXShapeInfo,
                                   void *extraParams,
                                   void *hZ, Nd4jLong *hZShapeInfo,
                                   void *dZ, Nd4jLong *dZShapeInfo,
                                void *hDimension, Nd4jLong *hDimensionShape,
                                void *dDimension, Nd4jLong *dDimensionShape) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    auto hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    auto hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);
    
    NativeOpExcutioner::execReduceFloat(opNum,
                                           hX,
                                           hXShapeInfo,
                                           extraParams,
                                           hZ,
                                           hZShapeInfo,
                                           dimension,
                                           dimensionLength,
                                           hTADShapeInfo,
                                           hTADOffsets);
}

void NativeOps::execReduceBool(Nd4jPointer *extraPointers,
                                int opNum,
                                void *hX, Nd4jLong *hXShapeInfo,
                                void *dX, Nd4jLong *dXShapeInfo,
                                void *extraParams,
                                void *hZ, Nd4jLong *hZShapeInfo,
                                void *dZ, Nd4jLong *dZShapeInfo,
                               void *hDimension, Nd4jLong *hDimensionShape,
                               void *dDimension, Nd4jLong *dDimensionShape) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    auto hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    auto hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);

    NativeOpExcutioner::execReduceBool(opNum,
                                        hX,
                                        hXShapeInfo,
                                        extraParams,
                                        hZ,
                                        hZShapeInfo,
                                        dimension,
                                        dimensionLength,
                                        hTADShapeInfo,
                                        hTADOffsets);
}

void NativeOps::execReduceSame(Nd4jPointer *extraPointers,
                                int opNum,
                                void *hX, Nd4jLong *hXShapeInfo,
                                void *dX, Nd4jLong *dXShapeInfo,
                                void *extraParams,
                                void *hZ, Nd4jLong *hZShapeInfo,
                                void *dZ, Nd4jLong *dZShapeInfo,
                               void *hDimension, Nd4jLong *hDimensionShape,
                               void *dDimension, Nd4jLong *dDimensionShape) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    auto hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    auto hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);

    NativeOpExcutioner::execReduceSame(opNum,
                                        hX,
                                        hXShapeInfo,
                                        extraParams,
                                        hZ,
                                        hZShapeInfo,
                                        dimension,
                                        dimensionLength,
                                        hTADShapeInfo,
                                        hTADOffsets);
}

void NativeOps::execReduceLong(Nd4jPointer *extraPointers,
                                int opNum,
                                void *hX, Nd4jLong *hXShapeInfo,
                                void *dX, Nd4jLong *dXShapeInfo,
                                void *extraParams,
                                void *hZ, Nd4jLong *hZShapeInfo,
                                void *dZ, Nd4jLong *dZShapeInfo,
                               void *hDimension, Nd4jLong *hDimensionShape,
                               void *dDimension, Nd4jLong *dDimensionShape) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    auto hTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
    auto hTADOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);

    NativeOpExcutioner::execReduceLong(opNum,
                                        hX,
                                        hXShapeInfo,
                                        extraParams,
                                        hZ,
                                        hZShapeInfo,
                                        dimension,
                                        dimensionLength,
                                        hTADShapeInfo,
                                        hTADOffsets);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParamsVals
 * @param hY
 * @param hYShapeInfo
 * @param hZ
 * @param hZShapeInfo
 */
void NativeOps::execReduce3(Nd4jPointer *extraPointers,
                                    int opNum,
                                    void *hX, Nd4jLong *hXShapeInfo,
                                    void *dX, Nd4jLong *dXShapeInfo,
                                    void *extraParams,
                                    void *hY, Nd4jLong *hYShapeInfo,
                                    void *dY, Nd4jLong *dYShapeInfo,
                                    void *hZ, Nd4jLong *hZShapeInfo,
                                    void *dZ, Nd4jLong *dZShapeInfo,
                                    Nd4jLong *tadOnlyShapeInfo, Nd4jLong *tadOffsets,
                                    Nd4jLong *yTadOnlyShapeInfo, Nd4jLong *yTadOffsets) {

    NativeOpExcutioner::execReduce3(opNum, hX, hXShapeInfo, extraParams, hY, hYShapeInfo, hZ, hZShapeInfo);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParamsVals
 * @param hY
 * @param hYShapeInfo
 */
void NativeOps::execReduce3Scalar(Nd4jPointer *extraPointers,int opNum,
                                            void *hX, Nd4jLong *hXShapeInfo,
                                            void *dX, Nd4jLong *dXShapeInfo,
                                            void *extraParams,
                                            void *hY, Nd4jLong *hYShapeInfo,
                                            void *dY, Nd4jLong *dYShapeInfo,
                                            void *hZ, Nd4jLong *hZShapeInfo,
                                            void *dZ, Nd4jLong *dZShapeInfo) {

    NativeOpExcutioner::execReduce3Scalar(opNum,hX,hXShapeInfo,extraParams,hY,hYShapeInfo, hZ, hZShapeInfo);
}
/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParamsVals
 * @param hY
 * @param hYShapeInfo
 * @param hZ
 * @param hZShapeInfo
 * @param dimension
 * @param dimensionLength
 */
void NativeOps::execReduce3(Nd4jPointer *extraPointers,
                                    int opNum,
                                    void *hX, Nd4jLong *hXShapeInfo,
                                    void *dX, Nd4jLong *dXShapeInfo,
                                    void *extraParams,
                                    void *hY, Nd4jLong *hYShapeInfo,
                                    void *dY, Nd4jLong *dYShapeInfo,
                                    void *hZ, Nd4jLong *hZShapeInfo,
                                    void *dZ, Nd4jLong *dZShapeInfo,
                                    void *hDimension, Nd4jLong *hDimensionShape,
                                    void *dDimension, Nd4jLong *dDimensionShape,
                                    Nd4jLong *tadOnlyShapeInfo, Nd4jLong *tadOffsets,
                                    Nd4jLong *yTadOnlyShapeInfo, Nd4jLong *yTadOffsets) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    if (extraPointers == nullptr || extraPointers[2] == 0) {
        NativeOpExcutioner::execReduce3(opNum, hX, hXShapeInfo, extraParams, hY, hYShapeInfo, hZ, hZShapeInfo, dimension, dimensionLength);
    } else {
        // going tad-way
        auto tadShapeInfo = reinterpret_cast<Nd4jLong *> (extraPointers[0]);
        auto tadOffsets = reinterpret_cast<Nd4jLong *>(extraPointers[1]);

        NativeOpExcutioner::execReduce3TAD(opNum, hX, hXShapeInfo, extraParams, hY, hYShapeInfo, hZ, hZShapeInfo, dimension, dimensionLength, tadShapeInfo, tadOffsets);
    }

}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param hZ
 * @param hZShapeInfo
 * @param hScalar
 * @param extraParams
 * @param n
 */
void NativeOps::execScalar(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *hScalar, Nd4jLong *hScalarShapeInfo,
        void *dScalar, Nd4jLong *dScalarShapeInfo,
        void *extraParams) {
    NativeOpExcutioner::execScalar(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            hScalar,
            hScalarShapeInfo,
            extraParams);
}

void NativeOps::execScalarBool(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *hScalar, Nd4jLong *hScalarShapeInfo,
        void *dScalar, Nd4jLong *dScalarShapeInfo,
        void *extraParams) {
    NativeOpExcutioner::execScalarBool(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            hScalar,
            hScalarShapeInfo,
            extraParams);
}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 */
void NativeOps::execSummaryStatsScalar(Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *extraParams,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        bool biasCorrected) {
    NativeOpExcutioner::execSummaryStatsScalar(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo,
            biasCorrected);
}
/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 * @param hZ
 * @param hZShapeInfo
 */
void NativeOps::execSummaryStats(Nd4jPointer *extraPointers,
                                         int opNum,
                                         void *hX, Nd4jLong *hXShapeInfo,
                                         void *dX, Nd4jLong *dXShapeInfo,
                                         void *extraParams,
                                         void *hZ, Nd4jLong *hZShapeInfo,
                                         void *dZ, Nd4jLong *dZShapeInfo,
                                         bool biasCorrected) {
    NativeOpExcutioner::execSummaryStats(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo,
            biasCorrected);
}
/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param extraParams
 * @param hZ
 * @param hZShapeInfo
 * @param dimension
 * @param dimensionLength
 */
void NativeOps::execSummaryStats(Nd4jPointer *extraPointers,
                                         int opNum,
                                         void *hX, Nd4jLong *hXShapeInfo,
                                         void *dX, Nd4jLong *dXShapeInfo,
                                         void *extraParams,
                                         void *hZ, Nd4jLong *hZShapeInfo,
                                         void *dZ, Nd4jLong *dZShapeInfo,
                                         void *hDimension, Nd4jLong *hDimensionShape,
                                         void *dDimension, Nd4jLong *dDimensionShape,
                                         bool biasCorrected,
                                         Nd4jLong *tadShapeInfo, Nd4jLong *tadOffsets) {
    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    NativeOpExcutioner::execSummaryStats(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo,
            dimension,
            dimensionLength,
            biasCorrected);

}

/**
 *
 * @param opNum
 * @param hX
 * @param hXShapeInfo
 * @param hZ
 * @param hZShapeInfo
 * @param extraParams
 * @param n
 */
void NativeOps::execTransformFloat(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {
    auto tadShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[0] : nullptr);
    auto tadOffsets = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[1] : nullptr);

    NativeOpExcutioner::execTransformFloat(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams,
            tadShapeInfo,
            tadOffsets);
}

void NativeOps::execTransformSame(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {
    auto tadShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[0] : nullptr);
    auto tadOffsets = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[1] : nullptr);

    NativeOpExcutioner::execTransformSame(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams,
            tadShapeInfo,
            tadOffsets);
}

void NativeOps::execTransformBool(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {
    auto tadShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[0] : nullptr);
    auto tadOffsets = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[1] : nullptr);

    NativeOpExcutioner::execTransformBool(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams,
            tadShapeInfo,
            tadOffsets);
}

void NativeOps::execTransformAny(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {

    NativeOpExcutioner::execTransformAny(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams,
            nullptr,
            nullptr);
}

void NativeOps::execTransformStrict(
        Nd4jPointer *extraPointers,
        int opNum,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *extraParams) {
    auto tadShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[0] : nullptr);
    auto tadOffsets = reinterpret_cast<Nd4jLong *>(extraPointers != nullptr ? extraPointers[1] : nullptr);

    NativeOpExcutioner::execTransformStrict(
            opNum,
            hX,
            hXShapeInfo,
            hZ,
            hZShapeInfo,
            extraParams,
            tadShapeInfo,
            tadOffsets);
}

void NativeOps::execReduce3All(Nd4jPointer *extraPointers,
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
                                     Nd4jLong *xTadShapeInfo,
                                     Nd4jLong *xOffsets,
                                     Nd4jLong *yTadShapeInfo,
                                     Nd4jLong *yOffsets) {

    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));


    NativeOpExcutioner::execReduce3All(opNum, hX, hXShapeInfo, extraParamsVals, hY, hYShapeInfo, hZ, hZShapeInfo, dimension, dimensionLength, xTadShapeInfo, xOffsets, yTadShapeInfo, yOffsets);
}


template <typename T>
void flattenGeneric(Nd4jPointer *extraPointers,
                    int offset,
                    char order,
                    void *vresult,
                    Nd4jLong *hZShapeInfo,
                    void *vinput,
                    Nd4jLong *inputShapeInfo) {

    auto hZ = reinterpret_cast<T *>(vresult);
    auto input = reinterpret_cast<T *>(vinput);

    int numOnes = 0;
    auto shape = shape::shapeOf(inputShapeInfo);
    int wholeRank = shape::rank(inputShapeInfo);
    for(int i = 0; i < wholeRank; i++) {
        if(shape[i] == 1)
            numOnes++;
    }



    //start at the given offset
    hZ += offset;
    char inputOrder = shape::order(inputShapeInfo);
    auto len = shape::length(inputShapeInfo);
    auto resultEleStride = shape::elementWiseStride(hZShapeInfo);
    auto inputEleStride = shape::elementWiseStride(inputShapeInfo);
    Nd4jLong numTads, stride;
    int dimension, dimensionLength;
    int rank = shape::rank(inputShapeInfo);
    auto xStride = shape::stride(inputShapeInfo);
    auto xShape = shape::shapeOf(inputShapeInfo);

    dimensionLength = 1;
    if(order == 'f') {
        dimension = 0;
    }
    else {
        dimension = rank - 1;
    }
    stride  = xStride[dimension];
    // numTads is product of length of all dimensions excluding
    // the one we do the tad on
    numTads = 1;
    for (int i = 0; i < rank; i++) {
        if (i != dimension)
            numTads *= xShape[i];
    }

    if (inputOrder == order) {
        if (resultEleStride == 1 && inputEleStride == 1) {
            memcpy(hZ, input, len* sizeof(T));
        }
        else if (resultEleStride >= 1 && inputEleStride >= 1) {
            if (len < ELEMENT_THRESHOLD) {
#pragma omp simd
                for (int i = 0; i < len; i++) {
                    hZ[i * resultEleStride] = input[i * inputEleStride];
                }
            }
            else {
#pragma omp parallel for simd
                for (int i = 0; i < len; i++) {
                    hZ[i * resultEleStride] = input[i * inputEleStride];
                }
            }
        }
        else {
            int idx = 0;            
            for(int i = 0; i < len; i++)                            
                    hZ[idx++] = input[shape::getIndexOffset(i, inputShapeInfo, len)];
        }
    }
    else {
        int rank = shape::rank(inputShapeInfo);
        auto xShape = shape::shapeOf(inputShapeInfo);
        auto tadShape = xShape[dimension];
        shape::TAD tad;
        tad.init(inputShapeInfo,&dimension,dimensionLength);
        tad.createTadOnlyShapeInfo();
#pragma omp  parallel for schedule(guided) default(shared)
        for(int i = 0; i < numTads; i++) {

            Nd4jLong resultOffset;

            if (order == 'f') {
                // 1. get c ordering coordinates
                auto cIndexCoordinates = new Nd4jLong[rank - 1];
                int divisor = 1;
                for (int dim = rank - 1; dim > 0; dim--) {
                    cIndexCoordinates[dim - 1] = (i / divisor) % xShape[dim];
                    divisor *= xShape[dim];
                }


                // 2. convert to f ordering index
                int fIndex = 0;
                int multiplier = 1;
                for (int dim = 1; dim <= rank - 1; dim++) {
                    fIndex += cIndexCoordinates[dim - 1] * multiplier;
                    multiplier *= xShape[dim];
                }

                resultOffset = fIndex * tadShape;
                delete[] cIndexCoordinates;

            }
            else {
                resultOffset = i *  tadShape;
            }

            auto tadOffset = tad.tadOffset(i);
            for( int j = 0; j < tadShape; j++) {

                // TAD are returned in C ordering always
                hZ[resultOffset + j] = input[tadOffset + j * stride];

            }
        }
    }
}


/**
    * Concatneate multi array of the same shape together
    * along a particular dimension
    */
void NativeOps::concat(
        Nd4jPointer *extraPointers,
        int dimension,
        int numArrays,
        Nd4jPointer *data, Nd4jPointer *inputShapeInfo,
        Nd4jPointer *ddata, Nd4jPointer *dinputShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        Nd4jPointer *tadPointers,
        Nd4jPointer *offsetPointers) {
    
    auto zType = nd4j::ArrayOptions::dataType(hZShapeInfo);

    BUILD_SINGLE_SELECTOR(zType, nd4j::SpecialMethods, ::concatCpuGeneric(dimension, numArrays, data, inputShapeInfo, hZ, hZShapeInfo), LIBND4J_TYPES);
}

/**
    * Concatneate multi array of the same shape together
    * along a particular dimension
    */
void NativeOps::specialConcat(
        Nd4jPointer *extraPointers,
        int dimension,
        int numArrays,
        Nd4jPointer *data,
        Nd4jPointer *inputShapeInfo,
        void *hZ,
        Nd4jLong *hZShapeInfo,
        Nd4jPointer *tadPointers,
        Nd4jPointer *offsetPointers) {

    auto zType = nd4j::ArrayOptions::dataType(hZShapeInfo);

    BUILD_SINGLE_SELECTOR(zType, nd4j::SpecialMethods, ::concatCpuGeneric(dimension, numArrays, data, inputShapeInfo, hZ, hZShapeInfo), LIBND4J_TYPES);
}

/**
* Append an input array
* to the end of a flat array
* in a particular order
* @param offset the offset of the array to start at
* @param order the order
* @param hZ the hZ array
* @param hZShapeInfo the shape info for te array
* @param input the input for the array
* @param inputShapeInfo the shape information for that array
*/
void NativeOps::flatten(
        Nd4jPointer *extraPointers,
        int offset,
        char order,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        void *input, Nd4jLong *inputShapeInfo,
        void *dinput, Nd4jLong *dinputShapeInfo) {
    
    auto xType = nd4j::ArrayOptions::dataType(inputShapeInfo);
    auto zType = nd4j::ArrayOptions::dataType(hZShapeInfo);

    if (xType != zType)
        throw std::runtime_error("NativeOps::flatten requires all operands to have same data type");

    BUILD_SINGLE_SELECTOR(xType, flattenGeneric, (extraPointers, offset, order, hZ, hZShapeInfo, input, inputShapeInfo), LIBND4J_TYPES);
}

/**
 * This is dummy method for JNI compatibility
 * Since we'll use this from java, jni compiler would like to have method no matter what.
 */
void NativeOps::initializeDevicesAndFunctions() {

}

void NativeOps::initializeFunctions(Nd4jPointer *functions) {
    nd4j::BlasHelper::getInstance()->initializeFunctions(functions);
}

/**
       * This method acquires memory chunk of requested size on host side
       *
       * @param pointer pointer that'll be used for allocation
       * @param memorySize memory size, in bytes
       * @param flags optional parameter
       */
Nd4jPointer NativeOps::mallocHost(Nd4jLong memorySize, int flags) {
    Nd4jPointer pointer = (Nd4jPointer) malloc(memorySize);
    if (pointer == 0)
        return 0L;
    return pointer;
}

/**
 * This method acquires memory chunk of requested size on specified device
 *
 * PLEASE NOTE: This method is NOT supported and has NO effect in CPU-based backend.
 *
 * @param pointer pointer that'll be used for allocation
 * @param memorySize memory size, in bytes
 * @param ptrToDeviceId pointer to deviceId. For cuda that's just and int, for OpenCL that's pointer to device_id, etc
 * @param flags optional parameter
 */
Nd4jPointer NativeOps::mallocDevice(Nd4jLong memorySize, Nd4jPointer ptrToDeviceId, int flags) {
    // not supported
    return 0L;
}

/**
 * This method releases previously allocated host memory space
 *
 * @param pointer pointer that'll be freed
 */
int NativeOps::freeHost(Nd4jPointer pointer) {
    free(reinterpret_cast<void *>(pointer));
    return 1L;
}

/**
 * This method releases previously allocated memory space on device
 *
 * PLEASE NOTE: This method is NOT supported and has NO effect in CPU-based backend.
 *
 * @param pointer pointer that'll be freed
 * @param ptrToDeviceId pointer to deviceId.
 */
int NativeOps::freeDevice(Nd4jPointer pointer, Nd4jPointer ptrToDeviceId) {
    // not supported
    return 0L;
}


/**
 * Returns the maximum number open mp threads
 */
int NativeOps::ompGetMaxThreads() {
    return omp_get_max_threads();
}

/**
 * Returns the number open mp threads
 */
int NativeOps::ompGetNumThreads() {
    return omp_get_num_threads();
}

/**
 * Sets the number of openmp threads
 */
void NativeOps::setOmpNumThreads(int threads) {
    omp_set_num_threads(threads);

}

Nd4jPointer NativeOps::createContext() {
    return 0L;
}

Nd4jPointer NativeOps::createStream() {
    return 0L;
}

Nd4jPointer NativeOps::createEvent() {
    return 0L;
}

int NativeOps::getDeviceMajor(Nd4jPointer ptrToDeviceId) {
    return 0;
}

int NativeOps::getDeviceMinor(Nd4jPointer ptrToDeviceId) {
    return 0;
}

int NativeOps::registerEvent(Nd4jPointer event, Nd4jPointer stream) {
    return 0L;
}

int NativeOps::setDevice(Nd4jPointer ptrToDeviceId) {
    return 0L;
}

Nd4jLong NativeOps::getDeviceFreeMemory(Nd4jPointer ptrToDeviceId) {
    return 0L;
}

Nd4jLong NativeOps::getDeviceTotalMemory(Nd4jPointer ptrToDeviceId) {
    return 0L;
}

int NativeOps::memcpy(Nd4jPointer dst, Nd4jPointer src, Nd4jLong size, int flags, Nd4jPointer reserved) {
    return 0L;
}

int NativeOps::memcpyAsync(Nd4jPointer dst, Nd4jPointer src, Nd4jLong size, int flags, Nd4jPointer reserved) {
    return 0L;
}

int NativeOps::memset(Nd4jPointer dst, int value, Nd4jLong size, int flags, Nd4jPointer reserved) {
    return 0L;
}

int NativeOps::memsetAsync(Nd4jPointer dst, int value, Nd4jLong size,  int flags, Nd4jPointer reserved) {
    return 0L;
}

int NativeOps::destroyEvent(Nd4jPointer event) {
    return 0L;
}

int NativeOps::streamSynchronize(Nd4jPointer stream) {
    return 0L;
}

int NativeOps::eventSynchronize(Nd4jPointer event) {
    return 0L;
}

int NativeOps::getAvailableDevices() {
    return 0L;
}

void NativeOps::enableDebugMode(bool reallyEnable) {
    nd4j::Environment::getInstance()->setDebug(reallyEnable);
}

void NativeOps::enableVerboseMode(bool reallyEnable) {
    nd4j::Environment::getInstance()->setVerbose(reallyEnable);
}

void NativeOps::setGridLimit(int gridSize) {
    // no-op
}

void NativeOps::tadOnlyShapeInfo(Nd4jLong *hXShapeInfo, int *dimension, int dimensionLength, Nd4jLong *target, Nd4jLong *offsets) {
    shape::TAD tad;
    tad.init(hXShapeInfo, dimension, dimensionLength);
    //tad->setOutputBuffer(target);
    tad.createTadOnlyShapeInfo();
    tad.createOffsets();


    std::memcpy(reinterpret_cast<void *>(target), tad.tadOnlyShapeInfo, shape::shapeInfoByteLength(tad.tadOnlyShapeInfo));
    std::memcpy(reinterpret_cast<void *>(offsets), tad.tadOffsets, tad.numTads * sizeof(Nd4jLong));
}

int NativeOps::memcpyConstantAsync(Nd4jLong dst, Nd4jPointer src, Nd4jLong size, int flags, Nd4jPointer reserved) {
    // no-op
    return 0L;
}

Nd4jPointer NativeOps::getConstantSpace() {
    // no-op
    return 0L;
}

template<typename T>
void pullRowsGeneric(void *vx,
                     Nd4jLong *hXShapeInfo,
                     void *vz,
                     Nd4jLong *hZShapeInfo,
                     const int n,
                     Nd4jLong *indexes,
                     Nd4jLong *tadShapeInfo,
                     Nd4jLong *tadOffsets,
                     Nd4jLong *zTadShapeInfo,
                     Nd4jLong *zTadOffsets) {
    auto hX = reinterpret_cast<T *>(vx);
    auto hZ = reinterpret_cast<T *>(vz);

    const auto xEWS = shape::elementWiseStride(tadShapeInfo);
    const auto zEWS = shape::elementWiseStride(zTadShapeInfo);
    const auto tadLength = shape::length(tadShapeInfo);

    int elementsPerThread = n / TAD_THRESHOLD;
    int _threads = nd4j::math::nd4j_max<int>(1, elementsPerThread);
    _threads = nd4j::math::nd4j_min<int>(_threads, omp_get_max_threads());

#pragma omp parallel for num_threads(_threads) if (n > 1) schedule(guided) default(shared)
    for (int idx = 0; idx < n; idx++) {
        auto xTadOffsetForBlock = tadOffsets[indexes[idx]];
        auto zTadOffsetForBlock = zTadOffsets[idx];

        auto rX = hX + xTadOffsetForBlock;
        auto rZ = hZ + zTadOffsetForBlock;

        if (xEWS == 1 && zEWS == 1) {

#pragma omp simd
            for (int i = 0; i < tadLength; i++ ) {
                rZ[i] = rX[i];
            }
        } else if (xEWS >= 1 && zEWS >= 1) {

#pragma omp simd
            for (int i = 0; i < tadLength; i++ ) {
                rZ[i * zEWS] = rX[i * xEWS];
            }
        } 
        else {            
            for (int i = 0; i < tadLength; i++) {                
                auto xOffset = xTadOffsetForBlock + shape::getIndexOffset(i, tadShapeInfo, tadLength);
                auto zOffset = zTadOffsetForBlock + shape::getIndexOffset(i, zTadShapeInfo, tadLength);
                hZ[zOffset] = hX[xOffset];
            }
        }
    }
}

void NativeOps::pullRows(Nd4jPointer *extraPointers,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        void *hZ, Nd4jLong *hZShapeInfo,
        void *dZ, Nd4jLong *dZShapeInfo,
        Nd4jLong n,
        Nd4jLong *indexes,
        Nd4jLong *tadShapeInfo,
        Nd4jLong *tadOffsets,
        Nd4jLong *zTadShapeInfo,
        Nd4jLong *zTadOffsets) {
    auto xType = nd4j::ArrayOptions::dataType(hXShapeInfo);

    BUILD_SINGLE_SELECTOR(xType, pullRowsGeneric, (hX, hXShapeInfo, hZ, hZShapeInfo, n, indexes, tadShapeInfo, tadOffsets, zTadShapeInfo, zTadOffsets), LIBND4J_TYPES);
}

template<typename T>
void tearGeneric(void *vx,
        Nd4jLong *hXShapeInfo,
        Nd4jPointer *targets,
        Nd4jLong *hZShapeInfo,
        Nd4jLong *tadShapeInfo,
        Nd4jLong *tadOffsets) {

    auto hX = reinterpret_cast<T *>(vx);

    const auto tadLength = shape::length(tadShapeInfo);
    auto tadEWS = shape::elementWiseStride(tadShapeInfo);
    auto zEWS = shape::elementWiseStride(hZShapeInfo);
    auto numTads = shape::length(hXShapeInfo) / tadLength;

#pragma omp parallel for schedule(guided) default(shared)
    for (Nd4jLong i = 0; i < numTads; i++) {
        auto hZ = reinterpret_cast<T *>(targets[i]);
        auto s = hX + tadOffsets[i];

        if (zEWS == 1 && tadEWS == 1) {
#pragma omp simd
            for (Nd4jLong j = 0; j < tadLength; j++) {
                hZ[j] = s[j];
            }
        } else if (zEWS > 0 && tadEWS > 0) {
#pragma omp simd
            for (Nd4jLong j = 0; j < tadLength; j++) {
                hZ[j * zEWS] = s[j * tadEWS];
            }
        } 
        else {
            
            for (Nd4jLong j = 0; j < tadLength; j++)                 
                hZ[shape::getIndexOffset(j, hZShapeInfo, tadLength)] = s[shape::getIndexOffset(j, tadShapeInfo, tadLength)];            
        }
    }
}

void NativeOps::tear(Nd4jPointer *extraPointers,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        Nd4jPointer *targets,
        Nd4jLong *hZShapeInfo,
        Nd4jLong *tadShapeInfo,
        Nd4jLong *tadOffsets) {
    auto xType = nd4j::ArrayOptions::dataType(hXShapeInfo);

    BUILD_SINGLE_SELECTOR(xType, tearGeneric, (hX, hXShapeInfo, targets, hZShapeInfo, tadShapeInfo, tadOffsets), LIBND4J_TYPES);
}


void NativeOps::average(Nd4jPointer *extras,
        Nd4jPointer *hX, Nd4jLong *hXShapeInfo,
        Nd4jPointer *dX, Nd4jLong *dXShapeInfo,
        void *z, Nd4jLong *hZShapeInfo,
        void *dz, Nd4jLong *dZShapeInfo,
        int n,
        Nd4jLong length,
        bool propagate) {
    auto xType = nd4j::ArrayOptions::dataType(hXShapeInfo);

    BUILD_SINGLE_SELECTOR(xType, nd4j::SpecialMethods, ::averageGeneric(hX, z, hZShapeInfo, n, length, propagate), LIBND4J_TYPES);
}

void NativeOps::accumulate(Nd4jPointer *extras,
        Nd4jPointer *hX, Nd4jLong *hXShapeInfo,
        Nd4jPointer *dX, Nd4jLong *dXShapeInfo,
        void *hz, Nd4jLong *hZShapeInfo,
        void *dz, Nd4jLong *dZShapeInfo,
        int n,
        Nd4jLong length) {

    auto xType = nd4j::ArrayOptions::dataType(hXShapeInfo);

    BUILD_SINGLE_SELECTOR(xType, nd4j::SpecialMethods, ::accumulateGeneric(hX, hz, hZShapeInfo, n, length), LIBND4J_TYPES);
}

void NativeOps::enableP2P(bool enable) {
    // no-op
}



void NativeOps::encodeThresholdP1(Nd4jPointer *extraPointers, void *hX, Nd4jLong *hXShapeInfo, Nd4jLong N, int *dz, float threshold) {
    // TODO: to be implemented
}


void NativeOps::encodeThresholdP2Int(Nd4jPointer *extraPointers, int *hX, Nd4jLong N, int *dz) {
    // TODO: to be implemented
}


void NativeOps::encodeThresholdP3(Nd4jPointer *extraPointers, void *hX, Nd4jLong *hXShapeInfo, int *offsets, Nd4jLong N, int *dz){
    // offsets won't be used here

    // TODO: to be implemented
}

void NativeOps::decodeThreshold(Nd4jPointer *extraPointers, void *hX, Nd4jLong N, void *dz, Nd4jLong *hZShapeInfo){
    // TODO: to be implemented
}

bool NativeOps::isP2PAvailable() {
    // always TRUE for cpu backend
    return true;
}

void NativeOps::checkP2P() {
    // no-op
}

void NativeOps::decodeBitmap(Nd4jPointer *extraPointers, void *hX, Nd4jLong N, void *dz, Nd4jLong *hZShapeInfo) {
    NativeOpExcutioner::decodeBitmap(hX, N, dz, hZShapeInfo);
}

template<typename T>
void shuffleGeneric(void **hX, Nd4jLong **hXShapeInfo, void **dz, Nd4jLong **hZShapeInfo, int N, int *shuffleMap, Nd4jLong **tadOnlyShapeInfo, Nd4jLong **tadOffsets) {

    auto dX = reinterpret_cast<T **>(hX);
    auto dZ = reinterpret_cast<T **>(dz);

#pragma omp parallel for if (N > 1) default(shared)
    for (int f = 0; f < N; f++) {
        auto hX = reinterpret_cast<T *>(dX[f]);
        //auto hZ = reinterpret_cast<T *>(dZ[f]);

        auto tadOffset = reinterpret_cast<Nd4jLong *>(tadOffsets[f]);


        const auto tadLength = shape::length(tadOnlyShapeInfo[f]);
        auto tadEWS = shape::elementWiseStride(tadOnlyShapeInfo[f]);
        auto tadRank = shape::rank(tadOnlyShapeInfo[f]);
        auto numTads = shape::length(hXShapeInfo[f]) / tadLength;

        auto tadShape = shape::shapeOf(tadOnlyShapeInfo[f]);
        auto tadStride = shape::stride(tadOnlyShapeInfo[f]);

        // TODO: omp *probably* has no sense here, since 99% of uses for this method will be inside DataSet. but worth a check

        for (Nd4jLong r = 0; r < numTads; r++) {
            if (shuffleMap[r] < 0)
                continue;

            auto oldOffset = tadOffset[r];
            auto newOffset = tadOffset[shuffleMap[r]];

            auto rX = hX + oldOffset;
            auto rY = hX + newOffset;

            if (tadEWS == 1) {

#pragma omp simd
                for (Nd4jLong i = 0; i < tadLength; i++) {
                    nd4j::math::nd4j_swap<T>(rX[i], rY[i]);
                }

            } 
            else {
                
#pragma omp parallel for schedule(guided) if (N == 1 && tadLength > 512)
                for (Nd4jLong i = 0; i < tadLength; i++) {                    
                    auto offset = shape::getIndexOffset(i, tadOnlyShapeInfo[f], tadLength);                    
                    nd4j::math::nd4j_swap<T>(hX[offset + oldOffset], hX[offset + newOffset]);
                }

            }

        }

    }
}

void NativeOps::shuffle(Nd4jPointer *extras,
                              Nd4jPointer *hX, Nd4jPointer *hXShapeInfo,
                              Nd4jPointer *dX, Nd4jPointer *dXShapeInfo,
                              Nd4jPointer *hz, Nd4jPointer *hZShapeInfo,
                              Nd4jPointer *dz, Nd4jPointer *dZShapeInfo,
                              int N,
                              int *shuffleMap,
                              Nd4jPointer *tadShapeInfo,
                              Nd4jPointer *tadOffsets) {
    auto xShape = reinterpret_cast<Nd4jLong **>(hXShapeInfo);
    auto zShape = reinterpret_cast<Nd4jLong **>(hZShapeInfo);
    auto tadOnlyShapeInfo = reinterpret_cast<Nd4jLong **>(tadShapeInfo);
    auto tadOffset = reinterpret_cast<Nd4jLong **>(tadOffsets);

    auto xType = nd4j::ArrayOptions::dataType(xShape[0]);

    BUILD_SINGLE_SELECTOR(xType, shuffleGeneric, (hX, xShape, hz, zShape, N, shuffleMap, tadOnlyShapeInfo, tadOffset), LIBND4J_TYPES);
}


bool NativeOps::isExperimentalEnabled() {
    return nd4j::Environment::getInstance()->isExperimentalBuild();
}


void NativeOps::setOmpMinThreads(int threads) {
    // TODO: to be implemented
}

/*
void NativeOps::execMetaPredicateShape(Nd4jPointer *extras, 
                                        const int opTypeA, 
                                        const int opNumA, 
                                        const int opTypeB, 
                                        const int opNumB, 
                                        Nd4jLong N, 
                                        void *hX, Nd4jLong *hXShapeInfo,
                                        void *dX, Nd4jLong *dXShapeInfo,
                                        void *hY, Nd4jLong *hYShapeInfo,
                                        void *dY, Nd4jLong *dYShapeInfo,
                                        void *hZ, Nd4jLong *hZShapeInfo,
                                        void *dZ, Nd4jLong *dZShapeInfo,
                                        void *extraA, 
                                        void *extraB, 
                                        double scalarA, 
                                        double scalarB) {    
    // no-op;
}
*/

int NativeOps::getDevice() {
    return 0;
}

void NativeOps::execScalar(Nd4jPointer *extraPointers,
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
                                 Nd4jLong *tadShapeInfoZ, Nd4jLong *tadOffsetsZ) {

    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    NativeOpExcutioner::execScalar(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo,
            hScalars,
            hScalarShapeInfo,
            dimension,
            shape::length(hDimensionShape),
            tadShapeInfo,
            tadOffsets,
            tadShapeInfoZ,
            tadOffsetsZ);
}

void NativeOps::execScalarBool(Nd4jPointer *extraPointers,
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
                           Nd4jLong *tadShapeInfoZ, Nd4jLong *tadOffsetsZ) {

    auto dimension = reinterpret_cast<int *>(hDimension);
    int dimensionLength = static_cast<int>(shape::length(hDimensionShape));

    NativeOpExcutioner::execScalarBool(
            opNum,
            hX,
            hXShapeInfo,
            extraParams,
            hZ,
            hZShapeInfo,
            hScalars,
            hScalarShapeInfo,
            dimension,
            dimensionLength,
            tadShapeInfo,
            tadOffsets,
            tadShapeInfoZ,
            tadOffsetsZ);
}

const char * NativeOps::getDeviceName(Nd4jPointer ptrToDeviceId) {
    if (!nameSet) {
        name = reinterpret_cast<char *>(malloc(256 * sizeof(char)));

        CHECK_ALLOC(name, "Failed to allocate new string buffer");

        std::memset(name, 0, 256 * sizeof(char));
        nameSet = true;

        // TODO: provide proper CPU model name here
        sprintf(name, "x86-compatible CPU");
    }


    return name;
}


void NativeOps::execAggregate(Nd4jPointer *extraPointers,int opNum,
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
                                    nd4j::DataType dtype) {

    BUILD_SINGLE_SELECTOR(dtype, NativeOpExcutioner::execAggregate, (opNum, arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, intArrays, numIntArrays, realArguments, numRealArguments), FLOAT_TYPES);

}

template <typename T>
void NativeOps::_batchExecutor(Nd4jPointer *extraPointers,
                           int numAggregates,
                           int opNum,
                           int maxArgs,
                           int maxShapes,
                           int maxIntArrays,
                           int maxIntArraySize,
                           int maxIdx,
                           int maxReals,
                           void *ptrToArguments,
                           nd4j::DataType dtype) {
    // probably, we don't want too much threads as usually
    int _threads = nd4j::math::nd4j_min<int>(numAggregates, omp_get_max_threads());

    nd4j::PointersHelper<T> helper(ptrToArguments,
                                        numAggregates,
                                        maxArgs,
                                        maxShapes,
                                        maxIntArrays,
                                        maxIntArraySize,
                                        maxIdx,
                                        maxReals);

    // special case here, we prefer spread arrangement here, all threads are detached from each other
#pragma omp parallel for num_threads(_threads) schedule(guided) proc_bind(spread) default(shared)
    for (int i = 0; i < numAggregates; i++) {
        auto intArrays = new int *[maxIntArrays];

        auto arguments = helper.getArguments(i);
        auto shapes = helper.getShapeArguments(i);
        auto idxArg = helper.getIndexArguments(i);
        auto realArg = helper.getRealArguments(i);

        for (int e = 0; e < maxIntArrays; e++) {
            intArrays[e] = helper.getIntArrayArguments(i, e);
        }

        execAggregate(extraPointers,
                      opNum,
                      reinterpret_cast<void **>(arguments),
                      helper.getNumArguments(i),
                      shapes,
                      helper.getNumShapeArguments(i),
                      idxArg,
                      helper.getNumIndexArguments(i),
                      intArrays,
                      helper.getNumIntArrayArguments(i),
                      realArg,
                      helper.getNumRealArguments(i),
                      dtype);

        delete [] intArrays;
    }
}
BUILD_SINGLE_TEMPLATE(template void NativeOps::_batchExecutor, (Nd4jPointer *extraPointers, int numAggregates, int opNum, int maxArgs, int maxShapes, int maxIntArrays, int maxIntArraySize, int maxIdx, int maxReals, void *ptrToArguments, nd4j::DataType dtype), FLOAT_TYPES);

void NativeOps::execAggregateBatch(Nd4jPointer *extraPointers,
                                         int numAggregates,
                                         int opNum,
                                         int maxArgs,
                                         int maxShapes,
                                         int maxIntArrays,
                                         int maxIntArraySize,
                                         int maxIdx,
                                         int maxReals,
                                         void *ptrToArguments,
                                         nd4j::DataType dtype) {
    BUILD_SINGLE_SELECTOR(dtype, _batchExecutor, (extraPointers, numAggregates, opNum, maxArgs, maxShapes, maxIntArrays, maxIntArraySize, maxIdx, maxReals, ptrToArguments, dtype), FLOAT_TYPES);
}


void NativeOps::execRandom(Nd4jPointer *extraPointers,
                                 int opNum,
                                 Nd4jPointer state,
                                 void *hZ, Nd4jLong *hZShapeInfo,
                                 void *dZ, Nd4jLong *dZShapeInfo,
                                 void *extraArguments) {
    NativeOpExcutioner::execRandom(opNum, state, hZ, hZShapeInfo, extraArguments);
}

void NativeOps::execRandom(Nd4jPointer *extraPointers,
                                 int opNum,
                                 Nd4jPointer state,
                                 void *hX, Nd4jLong *hXShapeInfo,
                                 void *dX, Nd4jLong *dXShapeInfo,
                                 void *hY, Nd4jLong *hYShapeInfo,
                                 void *dY, Nd4jLong *dYShapeInfo,
                                 void *hZ, Nd4jLong *hZShapeInfo,
                                 void *dZ, Nd4jLong *dZShapeInfo,
                                 void *extraArguments) {
    NativeOpExcutioner::execRandom(opNum, state, hX, hXShapeInfo, hY, hYShapeInfo, hZ, hZShapeInfo, extraArguments);
}

void NativeOps::execRandom(Nd4jPointer *extraPointers,
                                 int opNum,
                                 Nd4jPointer state,
                                 void *hX, Nd4jLong *hXShapeInfo,
                                 void *dX, Nd4jLong *dXShapeInfo,
                                 void *hZ, Nd4jLong *hZShapeInfo,
                                 void *dZ, Nd4jLong *dZShapeInfo,
                                 void *extraArguments) {
    NativeOpExcutioner::execRandom(opNum, state, hX, hXShapeInfo, hZ, hZShapeInfo, extraArguments);
}

Nd4jPointer NativeOps::initRandom(Nd4jPointer *extraPointers, long seed, long bufferSize, Nd4jPointer ptrToBuffer) {
    auto ptrBuf = reinterpret_cast<long *>(ptrToBuffer);
    auto buffer = new nd4j::random::RandomBuffer(seed, bufferSize, reinterpret_cast<uint64_t *>(ptrBuf));

    nd4j::random::Xoroshiro128 generator(buffer);
    generator.refreshBuffer();

    return (Nd4jPointer) buffer;
}

void NativeOps::refreshBuffer(Nd4jPointer *extraPointers, long seed, Nd4jPointer ptrRandom) {
    auto buffer = reinterpret_cast<nd4j::random::RandomBuffer *> (ptrRandom);

    buffer->setSeed(seed);
    buffer->setOffset(0);
    nd4j::random::Xoroshiro128 generator(buffer);
    generator.refreshBuffer();
}

void NativeOps::reSeedBuffer(Nd4jPointer *extraPointers, long seed, Nd4jPointer ptrRandom) {
    auto buffer = reinterpret_cast<nd4j::random::RandomBuffer *> (ptrRandom);

    buffer->reSeed(seed);
}


void NativeOps::destroyRandom(Nd4jPointer ptrBuffer) {
    auto buffer = reinterpret_cast<nd4j::random::RandomBuffer *>(ptrBuffer);
    delete buffer;
}




/**
    * Return the length of a shape buffer
    * based on the pointer
    * @param buffer  the buffer pointer to check
    * @return
    */
int NativeOps::lengthForShapeBufferPointer(Nd4jPointer buffer) {
    auto shapeBuffer = reinterpret_cast<Nd4jLong *>(buffer);
    return shape::shapeInfoLength(shape::rank(shapeBuffer));
}


/**
  * The pointer to get the address for
  *
  * @param address the address to get the pointer
  * @return the pointer for the given address
  */

Nd4jPointer NativeOps::pointerForAddress(Nd4jLong address) {
    return reinterpret_cast<Nd4jPointer >(address);
}

void NativeOps::sort(Nd4jPointer *extraPointers,
        void *hX, Nd4jLong *hXShapeInfo,
        void *dX, Nd4jLong *dXShapeInfo,
        bool descending) {
    NativeOpExcutioner::execSort(hX, hXShapeInfo, descending);
}

void NativeOps::sortTad(Nd4jPointer *extraPointers,
            void *hX, Nd4jLong *hXShapeInfo,
            void *dX, Nd4jLong *dXShapeInfo,
            int *dimension,
            int dimensionLength,
            Nd4jLong *tadShapeInfo,
            Nd4jLong *tadOffsets,
            bool descending) {
    NativeOpExcutioner::execSort(hX, hXShapeInfo, dimension, dimensionLength, tadShapeInfo, tadOffsets, descending);
}

void NativeOps::sortCooIndices(Nd4jPointer *extraPointers,
        Nd4jLong *indices,
        void *values,
        Nd4jLong length,
        int rank) {
    NativeOpExcutioner::execSortCooIndices(indices, values, length, rank);
}

Nd4jLong NativeOps::encodeBitmap(Nd4jPointer *extraPointers, void *hX, Nd4jLong *hXShapeInfo, Nd4jLong N, int *dz, float threshold) {
    return NativeOpExcutioner::encodeBitmap(hX, hXShapeInfo, N, dz, threshold);
}



Nd4jLong* NativeOps::mmapFile(Nd4jPointer *extraPointers, const char *fileName, Nd4jLong length) {
    auto hZ = new Nd4jLong[2];errno = 0;

#if defined(_WIN32) || defined(_WIN64)
    _mmap(hZ, static_cast<size_t>(length), fileName);
#else
    int fd = open(fileName, O_RDWR, 0);// checking for failed fopen
    if (fd < 0) {
        nd4j_printf("Errno: %i\n", errno);
        throw std::runtime_error("Failed to open file for MMAP");
    }
    void * ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

// check for failed allocation
    if (ptr == MAP_FAILED)
        return nullptr;

    hZ[0] = (Nd4jLong) ptr;
    hZ[1] = fd;

#endif

    return hZ;

}

void NativeOps::munmapFile(Nd4jPointer *extraPointers, Nd4jLong *ptrMap, Nd4jLong length) {
    munmap((Nd4jPointer) ptrMap[0], length);
#if defined(_WIN32) || defined(_WIN64)
    CloseHandle(reinterpret_cast<HANDLE>(ptrMap[1]));
#else
    close((int) ptrMap[1]);
#endif

    delete[] ptrMap;
}

nd4j::graph::ResultWrapper* NativeOps::executeFlatGraph(Nd4jPointer *extraPointers, Nd4jPointer flatBufferPointer) {
    return nd4j::graph::GraphExecutioner::executeFlatBuffer(flatBufferPointer);
}

const char* NativeOps::getAllCustomOps() {
    return nd4j::ops::OpRegistrator::getInstance()->getAllCustomOperations();
}

template <typename T>
FORCEINLINE int estimateThresholdGeneric(Nd4jPointer *extraPointers, Nd4jPointer hX, int N, T threshold) {
    auto buffer = reinterpret_cast<T *>(hX);

    int span = (N / 6) + 8;
    int cnt = 0;

#pragma omp parallel reduction(+:cnt)
    {
        int tid = omp_get_thread_num();
        int start = span * tid;
        int stop = span * (tid + 1);
        if (stop > N)
            stop = N;

#pragma omp simd
        for (int e = start; e < stop; e++) {
            auto v = nd4j::math::nd4j_abs<T>(buffer[e]);
            if (v >= threshold)
                cnt++;
        }
    }

    return cnt;
}


int NativeOps::estimateThreshold(Nd4jPointer *extraPointers, Nd4jPointer hX, Nd4jLong *hXShapeInfo, int N, float threshold) {
    auto xType = ArrayOptions::dataType(hXShapeInfo);
    BUILD_SINGLE_SELECTOR(xType, return estimateThresholdGeneric, (extraPointers, hX, N, threshold), FLOAT_TYPES);
}



void NativeOps::deleteShapeList(Nd4jPointer shapeList) {
    auto list = reinterpret_cast<nd4j::ShapeList*>(shapeList);

    list->destroy();
    delete list;
}

nd4j::ShapeList* _calculateOutputShapes(Nd4jPointer* extraPointers, nd4j::ops::DeclarableOp* op, Nd4jPointer* inputBuffers, Nd4jPointer* inputShapes, int numInputShapes, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs, bool *bArgs, int numBArgs) {
    nd4j::graph::VariableSpace varSpace;
    Context block(2, &varSpace);
    nd4j::ShapeList inShapes;

    for (int e = 0; e < numIArgs; e++)
        block.getIArguments()->push_back(iArgs[e]);

    for (int e = 0; e < numTArgs; e++)
        block.getTArguments()->push_back(tArgs[e]);

    for (int e = 0; e < numBArgs; e++)
        block.getBArguments()->push_back(bArgs[e]);

    for (int e = 0; e < numInputShapes; e++) {
        auto shape_ = reinterpret_cast<Nd4jLong *>(inputShapes[e]);

        // we shouldn't copy buffer if that's empty array
        void *buffer_ = nd4j::ArrayOptions::arrayType(shape_) == ArrayType::EMPTY ? nullptr : inputBuffers[e];

        auto array = new nd4j::NDArray(buffer_, shape_);
        array->triggerAllocationFlag(false, false);

        // block should contain references to proper variable
        varSpace.putVariable(1, e, array);
        block.pickInput(1, e);

        inShapes.push_back(shape_);
    }

    auto status = op->validateDataTypes(block);
    if (status != Status::OK())
        throw std::runtime_error("Data types validation failed");

    auto shapeList = op->calculateOutputShape(&inShapes, block);

    if (varSpace.workspace() != nullptr)
        shapeList->detach();

    return shapeList;
}

nd4j::ShapeList* NativeOps::calculateOutputShapes(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputBuffers, Nd4jPointer* inputShapes, int numInputShapes, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs, bool *bArgs, int numBArgs) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation(hash);

    return _calculateOutputShapes(extraPointers, op, inputBuffers, inputShapes, numInputShapes, tArgs, numTArgs, iArgs, numIArgs, bArgs, numBArgs);
}

nd4j::ShapeList* _calculateOutputShapes(Nd4jPointer* extraPointers, nd4j::ops::DeclarableOp *op, Nd4jPointer* inputShapes, int numInputShapes, double *tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs) {
    Context block(1);
    nd4j::ShapeList inShapes;

    for (int e = 0; e < numIArgs; e++)
        block.getIArguments()->push_back(iArgs[e]);

    for (int e = 0; e < numTArgs; e++)
        block.getTArguments()->push_back(tArgs[e]);

    for (int e = 0; e < numInputShapes; e++)
        inShapes.push_back(reinterpret_cast<Nd4jLong *>(inputShapes[e]));

    auto shapeList = op->calculateOutputShape(&inShapes, block);

    return shapeList;
}

nd4j::ShapeList* NativeOps::calculateOutputShapes(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputShapes, int numInputShapes, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation(hash);

    return _calculateOutputShapes(extraPointers, op, inputShapes, numInputShapes, tArgs, numTArgs, iArgs, numIArgs);
}

int NativeOps::execCustomOp(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer opContext) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation(hash);
    auto context = reinterpret_cast<Context*>(opContext);

    return op->execute(context);
}

Nd4jStatus realExec(nd4j::ops::DeclarableOp* op, Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputBuffers, Nd4jPointer* inputShapes, int numInputs, Nd4jPointer* outputBuffers, Nd4jPointer* outputShapes, int numOutputs, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs, bool* bArgs, int numBArgs, bool isInplace) {
    if (op == nullptr)
        nd4j_printf("Can't find requested operation: [%lld]\n", hash);

    // we're using the same fake nodeId everywhere here

    std::vector<nd4j::NDArray*> inputs(numInputs);
    std::vector<nd4j::NDArray*> outputs(numOutputs);
    std::vector<double> ttArgs(numTArgs);
    std::vector<Nd4jLong> iiArgs(numIArgs);
    std::vector<bool> biArgs(numBArgs);

    // filling block now with inputs
    for (int e = 0; e < numInputs; e++) {
        auto shape = reinterpret_cast<Nd4jLong *>(inputShapes[e]);
        void *buffer = nd4j::ArrayOptions::arrayType(shape) == ArrayType::EMPTY ? nullptr : inputBuffers[e];

        inputs[e] = new nd4j::NDArray(buffer, shape);
    }

    // if not inplace - transferring output arrays

    if (!isInplace)
        for (int e = 0; e < numOutputs; e++) {
            // we want to keep original output shape intact
            auto shape = shape::copyShape(reinterpret_cast<Nd4jLong *>(outputShapes[e]));
            void *buffer = nd4j::ArrayOptions::arrayType(shape) == ArrayType::EMPTY ? nullptr : outputBuffers[e];

            // FIXME: revisit this.
            bool canNullify = true;
            for (int i = 0; i < numInputs; i++) {
                void *ibuffer = nd4j::ArrayOptions::arrayType(shape) == ArrayType::EMPTY ? nullptr : inputBuffers[i];
                if (ibuffer == buffer) {
                    canNullify = false;
                    break;
                }
            }

            if (canNullify)
                memset((uint8_t *) buffer, '\0', shape::length(shape) * DataTypeUtils::sizeOfElement(ArrayOptions::dataType(shape)));

            auto array = new nd4j::NDArray(buffer, shape);
            outputs[e] = array;

            // and we want to release shape copy once we're done
            array->triggerAllocationFlag(false, true);
        }

    for (int e = 0; e < numIArgs; e++)
        iiArgs[e] = iArgs[e];


    for (int e = 0; e < numTArgs; e++)
        ttArgs[e] = tArgs[e];

    for (int e = 0; e < numBArgs; e++)
        biArgs[e] = bArgs[e];

    // hypothetically at this point we have everything filled
    auto hZ = op->execute(inputs, outputs, ttArgs, iiArgs, biArgs, isInplace);
    //auto hZ = op->execute(inputs, ttArgs, iiArgs, isInplace);



    if (!isInplace)
        for (int e = 0; e < numOutputs; e++) {
            //shape::printShapeInfoLinear("JVM output shape", (int *) outputShapes[e]);
            //shape::printShapeInfoLinear("C++ output shape", (int *) outputs[e]->shapeInfo());
            //outputs[e]->printIndexedBuffer("C++ raw output");
            //outputs[e]->printBuffer("C++ indexed output");

            if (outputs[e]->ordering() != shape::order(reinterpret_cast<Nd4jLong *>(outputShapes[e])))
                outputs[e]->streamline(shape::order(reinterpret_cast<Nd4jLong *>(outputShapes[e])));
        }

/*
    if (!isInplace) {
        if (hZ->size() != numOutputs) {
            return ND4J_STATUS_BAD_OUTPUT;
        }

        for (int e = 0; e < numOutputs; e++) {
            auto buffer = (T *) outputBuffers[e];
            auto shape = (int *) outputShapes[e];
            nd4j::NDArray<T> tmp(buffer, shape);

            if (tmp.lengthOf() != hZ->at(e)->lengthOf()) {
                nd4j_printf("Provided output array for [%s] has length of %i, but actual hZ has length of %i\n", op->getOpName()->c_str(), tmp.lengthOf(), hZ->at(e)->lengthOf());
                return ND4J_STATUS_BAD_OUTPUT;
            }

            tmp.assign(hZ->at(e));
        }
    } else {
        // if op is inplace, our ResultSet holds pointers
        hZ->purge();
    }


    delete hZ;

*/

    for (auto v: inputs)
        delete v;

    for (auto v: outputs)
        delete v;

    return hZ;
}


int NativeOps::execCustomOp(Nd4jPointer* extraPointers, Nd4jLong hash, Nd4jPointer* inputBuffers, Nd4jPointer* inputShapes, int numInputs, Nd4jPointer* outputBuffers, Nd4jPointer* outputShapes, int numOutputs, double* tArgs, int numTArgs, Nd4jLong *iArgs, int numIArgs, bool* bArgs, int numBArgs, bool isInplace) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation(hash);
    return realExec(op, extraPointers, hash, inputBuffers, inputShapes, numInputs, outputBuffers, outputShapes, numOutputs, tArgs, numTArgs, iArgs, numIArgs, bArgs, numBArgs, isInplace);
}

int NativeOps::registerGraph(Nd4jPointer *extraPointers, Nd4jLong graphId, Nd4jPointer flatBufferPointer) {
    auto graph = nd4j::graph::GraphExecutioner::importFromFlatPointer(flatBufferPointer);

    nd4j::graph::GraphHolder::getInstance()->registerGraph(graphId, graph);

    return ND4J_STATUS_OK;
}

static VariablesSet* executeStoredGraphT(Nd4jPointer *extraPointers, Nd4jLong graphId, Nd4jPointer *inputBuffers, Nd4jPointer *inputShapes, int* inputIndices, int numInputs) {
    auto graph = nd4j::graph::GraphHolder::getInstance()->cloneGraph(graphId);
    auto varSpace = graph->getVariableSpace();

    std::vector<nd4j::NDArray*> handles;

    for (int e = 0; e < numInputs; e++) {
        auto idx = inputIndices[e];

        // we'll delete this array later, together with cloned VariableSpace
        auto array = new nd4j::NDArray(inputBuffers[e], reinterpret_cast<Nd4jLong *>(inputShapes[e]));
        handles.emplace_back(array);

        if (varSpace->hasVariable(idx)) {
            auto var = varSpace->getVariable(idx);
            if (var->hasNDArray())
                delete var->getNDArray();

            var->setNDArray(array);
        } else
            varSpace->putVariable(idx, array);
    }

    auto hZ = nd4j::graph::GraphExecutioner::execute(graph, varSpace);
    auto varSet = new nd4j::graph::VariablesSet(hZ);

    if (hZ == ND4J_STATUS_OK) {
        // pull back results, and provide them
        auto outputs = graph->fetchOutputs();
        for (int e = 0; e < outputs->size(); e++) {
            // we're only getting variable ID/Index from original grap. values will be taken from cloned workspace
            std::pair<int, int> varId(outputs->at(e)->id(), outputs->at(e)->index());

            auto var = varSpace->getVariable(varId);

            varSet->push_back(var->clone());
        }

        delete outputs;
    }

    delete graph;

    return varSet;
}

nd4j::graph::VariablesSet* NativeOps::executeStoredGraph(Nd4jPointer *extraPointers, Nd4jLong graphId, Nd4jPointer *inputBuffers, Nd4jPointer *inputShapes, int* inputIndices, int numInputs) {
    return nullptr;
}

int NativeOps::unregisterGraph(Nd4jPointer *extraPointers, Nd4jLong graphId) {

    nd4j::graph::GraphHolder::getInstance()->dropGraphAny(graphId);

    return nd4j::Status::OK();
}

void NativeOps::deletePointerArray(Nd4jPointer pointer) {
    auto ptr = reinterpret_cast<Nd4jPointer *>(pointer);
    delete[] ptr;
}

void NativeOps::deleteIntArray(Nd4jPointer pointer) {
    auto ptr = reinterpret_cast<int *>(pointer);
    delete[] ptr;
}

void NativeOps::deleteLongArray(Nd4jPointer pointer) {
    auto ptr = reinterpret_cast<Nd4jLong *>(pointer);
    delete[] ptr;
}

template <typename T>
static void deleteVariablesSetT(Nd4jPointer pointer) {
    auto ptr = reinterpret_cast<nd4j::graph::VariablesSet*>(pointer);
    delete ptr;
}

void NativeOps::deleteVariablesSet(Nd4jPointer pointer) {
    deleteVariablesSetT<double>(pointer);
}

const char* NativeOps::getAllOperations() {
    return nd4j::OpTracker::getInstance()->exportOperations();
}


Nd4jPointer NativeOps::getGraphState(Nd4jLong id) {
    return (Nd4jPointer) new nd4j::graph::GraphState(id);
}

void NativeOps::deleteGraphState(Nd4jPointer state) {
    auto stateP = reinterpret_cast<nd4j::graph::GraphState*>(state);
    delete stateP;
}

Nd4jStatus execCustomOpWithScope_(Nd4jPointer *extraPointers, nd4j::graph::GraphState *state, Nd4jLong opHash, Nd4jLong *scopes, int numScopes, Nd4jPointer *inputBuffers, Nd4jPointer *inputShapes, int numInputs, Nd4jPointer *outputBuffers, Nd4jPointer *outputShapes, int numOutputs) {
    /**
     * That's basically exec, with VariableSpace provided in GraphState:
     * depending on operation (i.e. while of if), different logic executors could be used
     */

    auto graph = state->graph();
    auto varSpace = state->variableSpace();

    // Node is dynamically created, and has nothing beyond it: only inputs and outputs
    // this node has id of 0, and inputs are
    Node node(OpType_LOGIC, opHash, 0);

    // mapping inputs
    for (int e = 0; e < numInputs; e++) {
        auto buffer = inputBuffers[e];
        auto shapeInfo = reinterpret_cast<Nd4jLong *>(inputShapes[e]);

        auto array = new nd4j::NDArray(buffer, shapeInfo, varSpace->workspace());

        // now we just put array to VarSpace
        varSpace->putVariable(0, e, array);
        node.pickInput(0, e);
    }

    // mapping scopes
    for (int e = 0; e < numScopes; e++) {
        // we should check scope existence in GraphState/Graph
        int scopeId = (int) scopes[e];
        if (!state->hasScope(scopeId)) {
            // nd4j_printf("execCustomOpWithScope: referenced scope [%i] doesn't exist\n", scopeId);
            return Status::THROW();
        }
        node.pickInput(scopeId, 0);
    }

    auto hZ = LogicExecutor::processNode(graph, &node);
    if (hZ != Status::OK())
        return hZ;

    // mapping outputs

    for (int e = 0; e < numOutputs; e++) {
        auto buffer = outputBuffers[e];
        auto shapeInfo = reinterpret_cast<Nd4jLong *>(outputShapes[e]);

        NDArray array(buffer, shapeInfo, varSpace->workspace());

        // now we just put array to VarSpace to the same ID
        //varSpace->putVariable(0, e, array);

        auto t = varSpace->getVariable(0, e)->getNDArray();
        array.assign(t);
    }

    // removing input variables
    for (int e = 0; e < numInputs; e++) {
        varSpace->dropVariable(0, e);
    }


    // after some bla-bla-bla we should have Graph and Node for current op
    return Status::OK();
}

Nd4jStatus NativeOps::execCustomOpWithScope(Nd4jPointer *extraPointers, Nd4jPointer state, Nd4jLong opHash, Nd4jLong *scopes, int numScopes, Nd4jPointer *inputBuffers, Nd4jPointer *inputShapes, int numInputs, Nd4jPointer *outputBuffers, Nd4jPointer *outputShapes, int numOutputs) {
    return execCustomOpWithScope_(extraPointers, reinterpret_cast<nd4j::graph::GraphState*>(state), opHash, scopes, numScopes, inputBuffers, inputShapes, numInputs, outputBuffers, outputShapes, numOutputs);
}

void NativeOps::deleteResultWrapper(Nd4jPointer ptr) {
    // just 0 room for compiler s@!t
    auto p = reinterpret_cast<nd4j::graph::ResultWrapper *>(ptr);
    delete p;
}

/*
 * TypeDef:
 *     void convertTypes(Nd4jPointer *extras, int srcType, Nd4jPointer hX, long N, int dstType, Nd4jPointer hZ);
 */
void NativeOps::convertTypes(Nd4jPointer *extras, int srcType, Nd4jPointer hX, Nd4jLong N, int dstType, Nd4jPointer hZ) {
    auto hx = reinterpret_cast<void *>(hX);
    auto hz = reinterpret_cast<void *>(hZ);

    if (srcType == ND4J_FLOAT8) {
        if (dstType == ND4J_FLOAT8) {
            // convertGeneric<double, nd4j::float8>(hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, nd4j::int8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, nd4j::uint8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, nd4j::int16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, nd4j::uint16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {

        } else if (dstType == ND4J_FLOAT32) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            //nd4j::TypeCast::convertGeneric<nd4j::float8, double>(nullptr, hx, N, hz);
        } else {
            //nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_INT8) {
        if (dstType == ND4J_FLOAT8) {
            //nd4j::TypeCast::convertGeneric<nd4j::int8, nd4j::float8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            //convertGeneric<nd4j::int8, nd4j::int8>(hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            nd4j::TypeCast::convertGeneric<int8_t, uint8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertGeneric<int8_t, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            nd4j::TypeCast::convertGeneric<int8_t, int16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
            //nd4j::TypeCast::convertGeneric<int8_t, uint16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {
            // TODO: eventually we might want to add it
        } else if (dstType == ND4J_FLOAT32) {
            nd4j::TypeCast::convertGeneric<int8_t, float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            nd4j::TypeCast::convertGeneric<int8_t, double>(nullptr, hx, N, hz);
        } else {
            nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_UINT8) {
        if (dstType == ND4J_FLOAT8) {
        //    nd4j::TypeCast::convertGeneric<uint8_t, nd4j::float8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            nd4j::TypeCast::convertGeneric<uint8_t, int8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            nd4j::TypeCast::convertGeneric<uint8_t, uint8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertGeneric<uint8_t, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            nd4j::TypeCast::convertGeneric<uint8_t, int16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
     //       nd4j::TypeCast::convertGeneric<uint8_t, uint16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {
            // TODO: still might want to add
        } else if (dstType == ND4J_FLOAT32) {
            nd4j::TypeCast::convertGeneric<uint8_t, float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            nd4j::TypeCast::convertGeneric<uint8_t, double>(nullptr, hx, N, hz);
        } else {
            nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_FLOAT16) {
        if (dstType == ND4J_FLOAT8) {
        //    nd4j::TypeCast::convertGeneric<float16, nd4j::float8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            nd4j::TypeCast::convertGeneric<float16, int8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            nd4j::TypeCast::convertGeneric<float16, uint8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertGeneric<float16, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            nd4j::TypeCast::convertGeneric<float16, int16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
//            nd4j::TypeCast::convertGeneric<float16, uint16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {
            // TODO: .... ^^^
        } else if (dstType == ND4J_FLOAT32) {
            nd4j::TypeCast::convertGeneric<float16, float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            nd4j::TypeCast::convertGeneric<float16, double>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_THRESHOLD) {
            nd4j::TypeCast::convertToThreshold<float16>(nullptr, hx, N, hz);
        } else {
            nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_INT16) {
        if (dstType == ND4J_FLOAT8) {
         //   nd4j::TypeCast::convertGeneric<int16_t, nd4j::float8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            nd4j::TypeCast::convertGeneric<int16_t, int8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            nd4j::TypeCast::convertGeneric<int16_t, uint8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertGeneric<int16_t, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            //nd4j::TypeCast::convertGeneric<int16_t, int16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
//            nd4j::TypeCast::convertGeneric<int16_t, uint16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {
            // TODO...
        } else if (dstType == ND4J_FLOAT32) {
            nd4j::TypeCast::convertGeneric<int16_t, float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            nd4j::TypeCast::convertGeneric<int16_t, double>(nullptr, hx, N, hz);
        } else {
            printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_FLOAT24) {

    } else if (srcType == ND4J_FLOAT32) {
        if (dstType == ND4J_FLOAT8) {
        //    nd4j::TypeCast::convertGeneric<float, nd4j::float8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            nd4j::TypeCast::convertGeneric<float, int8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            nd4j::TypeCast::convertGeneric<float, uint8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertGeneric<float, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            nd4j::TypeCast::convertGeneric<float, int16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
//            nd4j::TypeCast::convertGeneric<float, uint16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {

        } else if (dstType == ND4J_DOUBLE) {
            nd4j::TypeCast::convertGeneric<float, double>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_THRESHOLD) {
            nd4j::TypeCast::convertToThreshold<float>(nullptr, hx, N, hz);
        } else {
            nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_DOUBLE) {
        if (dstType == ND4J_FLOAT8) {
         //   nd4j::TypeCast::convertGeneric<double, nd4j::float8>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT8) {
            nd4j::TypeCast::convertGeneric<double, int8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT8) {
            nd4j::TypeCast::convertGeneric<double, uint8_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertGeneric<double, float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_INT16) {
            nd4j::TypeCast::convertGeneric<double, int16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_UINT16) {
//            nd4j::TypeCast::convertGeneric<double, uint16_t>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT24) {

        } else if (dstType == ND4J_FLOAT32) {
            nd4j::TypeCast::convertGeneric<double, float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            //
        } else if (dstType == ND4J_THRESHOLD) {
            nd4j::TypeCast::convertToThreshold<double>(nullptr, hx, N, hz);
        } else {
            nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else if (srcType == ND4J_THRESHOLD) {
        if (dstType == ND4J_FLOAT16) {
            nd4j::TypeCast::convertFromThreshold<float16>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_FLOAT32) {
            nd4j::TypeCast::convertFromThreshold<float>(nullptr, hx, N, hz);
        } else if (dstType == ND4J_DOUBLE) {
            nd4j::TypeCast::convertFromThreshold<double>(nullptr, hx, N, hz);
        } else {
            nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
        }
    } else {
        nd4j_printf("Unsupported types conversion: [%i] -> [%i]\n", srcType, dstType);
    }
}

/*
void NativeOps::fillUtf8String(Nd4jPointer *extraPointers, const char **strings, int numStrings, Nd4jPointer buffer) {
    auto hZ = reinterpret_cast<nd4j::utf8string**>(buffer);
    for (int e = 0; e < numStrings; e++) {
        hZ[e] = reinterpret_cast<nd4j::utf8string*>(createUtf8String(extraPointers, strings[e]));
    }
}
 */

Nd4jPointer NativeOps::createUtf8String(Nd4jPointer *extraPointers, const char *string, int length) {
    auto u = new nd4j::utf8string(string, length);
    return reinterpret_cast<Nd4jPointer>(u);
}

void NativeOps::deleteUtf8String(Nd4jPointer *extraPointers, Nd4jPointer ptr) {
    delete(reinterpret_cast<nd4j::utf8string*>(ptr));
}


////////////////////////////////////////////////////////////////////////
void NativeOps::scatterUpdate(Nd4jPointer *extraPointers, int opCode, int numOfSubArrs,
                      void* hX, Nd4jLong* hXShapeInfo, Nd4jLong* hXOffsets,
                      void* dX, Nd4jLong* dXShapeInfo, Nd4jLong* dXOffsets,
                      void* hY, Nd4jLong* hYShapeInfo, Nd4jLong* hYOffsets,
                      void* dY, Nd4jLong* dYShapeInfo, Nd4jLong* dYOffsets,
                      int* hIindexes, int* dIindexes) {


    int numThreads = omp_get_max_threads();

    #pragma omp parallel default(shared)
    {
        for (int i = 0; i < numOfSubArrs; ++i) {

            int threadIndex = omp_get_thread_num();
            const auto xIndex = hIindexes[i];
            const bool isOwner = xIndex < numThreads ? threadIndex == xIndex : threadIndex == xIndex % numThreads;

            if (!isOwner)
                continue;

            NDArray inSubArr(reinterpret_cast<int8_t *>(hX) + (hXOffsets[hIindexes[i]] * DataTypeUtils::sizeOf(hXShapeInfo)), hXShapeInfo);
            NDArray updSubArr(reinterpret_cast<int8_t *>(hY) + (hYOffsets[i] * DataTypeUtils::sizeOf(hXShapeInfo)), hYShapeInfo);

            if (inSubArr.lengthOf() != updSubArr.lengthOf()) {
                continue;
            }

            switch (opCode) {
                case 0:
                    inSubArr.applyPairwiseTransform(pairwise::Add, &updSubArr, &inSubArr, nullptr);
                    break;
                case 1:
                    inSubArr.applyPairwiseTransform(pairwise::Subtract, &updSubArr, &inSubArr, nullptr);
                    break;
                case 2:
                    inSubArr.applyPairwiseTransform(pairwise::Multiply, &updSubArr, &inSubArr, nullptr);
                    break;
                case 3:
                    inSubArr.applyPairwiseTransform(pairwise::Divide, &updSubArr, &inSubArr, nullptr);
                    break;
                case 4:
                    inSubArr.applyPairwiseTransform(pairwise::ReverseSubtract, &updSubArr, &inSubArr, nullptr);
                    break;
                case 5:
                    inSubArr.applyPairwiseTransform(pairwise::ReverseDivide, &updSubArr, &inSubArr, nullptr);
                    break;
                case 6:
                    inSubArr.applyPairwiseTransform(pairwise::CopyPws, &updSubArr, &inSubArr, nullptr);
                    break;
                default:
                    continue;
            }
        }
    }
}

BUILD_SINGLE_TEMPLATE(template void flattenGeneric,(Nd4jPointer*, int, char, void*, Nd4jLong*, void*, Nd4jLong*), LIBND4J_TYPES);
BUILD_SINGLE_TEMPLATE(template void pullRowsGeneric, (void *, Nd4jLong*, void*, Nd4jLong*, const int, Nd4jLong*, Nd4jLong*, Nd4jLong*, Nd4jLong*, Nd4jLong*), LIBND4J_TYPES);
BUILD_SINGLE_TEMPLATE(template void tearGeneric, (void *, Nd4jLong*, Nd4jPointer*, Nd4jLong*, Nd4jLong*, Nd4jLong*), LIBND4J_TYPES);
BUILD_SINGLE_TEMPLATE(template void shuffleGeneric, (void**, Nd4jLong**, void**, Nd4jLong**, int, int*, Nd4jLong**, Nd4jLong**), LIBND4J_TYPES);


