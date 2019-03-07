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
//  @author raver119@gmail.com, created on 15.12.17.
//  @author Yurii Shyrma (iuriish@yahoo.com)
//

#include <types/types.h>
#include <op_boilerplate.h>
#include <loops/random.h>
#include <OmpLaunchHelper.h>

using namespace randomOps;

namespace functions {
    namespace random {

        template<typename X>
        template<typename OpClass>
        void RandomFunction<X>::execTransform(Nd4jPointer state,
                void *vx,
                Nd4jLong *xShapeInfo,
                void *vy,
                Nd4jLong *yShapeInfo,
                void *vz,
                Nd4jLong *zShapeInfo,
                void *vextraArguments) {

            auto x = reinterpret_cast<X *>(vx);
            auto y = reinterpret_cast<X *>(vy);
            auto z = reinterpret_cast<X *>(vz);
            auto extraArguments = reinterpret_cast<X *>(vextraArguments);

            if (OpClass::requiresSpecial) {
                OpClass::specialOp(state, x, xShapeInfo, y, yShapeInfo, z, zShapeInfo, extraArguments);
                return;
            }

            auto length = shape::length(zShapeInfo);            

//            nd4j::random::RandomBuffer *buffer = reinterpret_cast<nd4j::random::RandomBuffer *> (state);
            nd4j::graph::RandomGenerator* rng = reinterpret_cast<nd4j::graph::RandomGenerator*>(state);
            nd4j::OmpLaunchHelper info(length);

           
            if(shape::haveSameOffsets(xShapeInfo, yShapeInfo) && shape::haveSameOffsets(xShapeInfo, zShapeInfo)) {

                uint xShapeInfoCast[MAX_RANK];
                const bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {                        
                        
                        auto offset = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);
                        z[offset] = OpClass::op(x[offset], y[offset], i, length, rng, extraArguments);
                    }
                }
            }
            else if (shape::haveSameOffsets(xShapeInfo, yShapeInfo)) {

                uint xShapeInfoCast[MAX_RANK];
                uint zShapeInfoCast[MAX_RANK];
                const bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);                
                const bool canCastZ = nd4j::DataTypeUtils::castShapeInfo(zShapeInfo, zShapeInfoCast);

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {                        
                        
                        auto offset  = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);
                        auto zOffset = shape::indexOffset(i + threadOffset, zShapeInfo, zShapeInfoCast, length, canCastZ);
                        z[zOffset] = OpClass::op(x[offset], y[offset], i, length, rng, extraArguments);
                    }
                }
            }
            else if (shape::haveSameOffsets(xShapeInfo, zShapeInfo)) {

                uint xShapeInfoCast[MAX_RANK];
                uint yShapeInfoCast[MAX_RANK];
                const bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);                
                const bool canCastY = nd4j::DataTypeUtils::castShapeInfo(yShapeInfo, yShapeInfoCast);

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {                        
                        
                        auto offset  = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);
                        auto yOffset = shape::indexOffset(i + threadOffset, yShapeInfo, yShapeInfoCast, length, canCastY);
                        z[offset] = OpClass::op(x[offset], y[yOffset], i, length, rng, extraArguments);
                    }
                }
            }
            else if (shape::haveSameOffsets(yShapeInfo, zShapeInfo)) {

                uint xShapeInfoCast[MAX_RANK];
                uint yShapeInfoCast[MAX_RANK];
                const bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);                
                const bool canCastY = nd4j::DataTypeUtils::castShapeInfo(yShapeInfo, yShapeInfoCast);

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {                        
                        
                        auto xOffset = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);
                        auto offset  = shape::indexOffset(i + threadOffset, yShapeInfo, yShapeInfoCast, length, canCastY);
                        z[offset] = OpClass::op(x[xOffset], y[offset], i, length, rng, extraArguments);
                    }
                }
            }
            else {

                uint xShapeInfoCast[MAX_RANK];
                uint yShapeInfoCast[MAX_RANK];
                uint zShapeInfoCast[MAX_RANK];
                const bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);                
                const bool canCastY = nd4j::DataTypeUtils::castShapeInfo(yShapeInfo, yShapeInfoCast);
                const bool canCastZ = nd4j::DataTypeUtils::castShapeInfo(zShapeInfo, zShapeInfoCast);

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {                        
                        
                        auto xOffset = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);
                        auto yOffset = shape::indexOffset(i + threadOffset, yShapeInfo, yShapeInfoCast, length, canCastY);
                        auto zOffset = shape::indexOffset(i + threadOffset, zShapeInfo, zShapeInfoCast, length, canCastZ);
                        z[zOffset] = OpClass::op(x[xOffset], y[yOffset], i, length, rng, extraArguments);
                    }
                }
            }

            // update rng state
            rng->rewindH(length);
        };



        template<typename X>
        template<typename OpClass>
        void RandomFunction<X>::execTransform(Nd4jPointer state,
                void *vx,
                Nd4jLong *xShapeInfo,
                void *vz,
                Nd4jLong *zShapeInfo,
                void *vextraArguments) {
            auto x = reinterpret_cast<X *>(vx);
            auto z = reinterpret_cast<X *>(vz);
            auto extraArguments = reinterpret_cast<X *>(vextraArguments);

            auto length = shape::length(zShapeInfo);

            uint xShapeInfoCast[MAX_RANK];
            const bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);
            
            //nd4j::random::RandomBuffer *buffer = reinterpret_cast<nd4j::random::RandomBuffer *> (state);
            nd4j::graph::RandomGenerator* rng = reinterpret_cast<nd4j::graph::RandomGenerator*>(state);
            nd4j::OmpLaunchHelper info(length);
            
            if(shape::haveSameOffsets(xShapeInfo, zShapeInfo)) {

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {
                        auto offset = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);                        
                        z[offset] = OpClass::op(x[offset], i, length, rng, extraArguments);
                    }
                }
            }
            else {

                uint zShapeInfoCast[MAX_RANK];
                const bool canCastZ = nd4j::DataTypeUtils::castShapeInfo(zShapeInfo, zShapeInfoCast);

                #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {                
                    auto threadNum = omp_get_thread_num();
                    Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                    #pragma omp simd
                    for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {
                        auto xOffset = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, length, canCastX);
                        auto zOffset = shape::indexOffset(i + threadOffset, zShapeInfo, zShapeInfoCast, length, canCastZ);
                        z[zOffset] = OpClass::op(x[xOffset], i, length, rng, extraArguments);
                    }
                }
            }
            // update rng state
            rng->rewindH(length);
        }


        template<typename X>
        template<typename OpClass>
        void RandomFunction<X>::execTransform(Nd4jPointer state, void *vz, Nd4jLong  *zShapeInfo, void *vextraArguments) {

            auto z = reinterpret_cast<X *>(vz);
            auto extraArguments = reinterpret_cast<X *>(vextraArguments);

            auto length = shape::length(zShapeInfo);
          
            //nd4j::random::RandomBuffer *buffer = reinterpret_cast<nd4j::random::RandomBuffer *> (state);
            nd4j::graph::RandomGenerator* rng = reinterpret_cast<nd4j::graph::RandomGenerator*>(state);
            nd4j::OmpLaunchHelper info(length);

            uint zShapeInfoCast[MAX_RANK];
            const bool canCastZ = nd4j::DataTypeUtils::castShapeInfo(zShapeInfo, zShapeInfoCast);
            
            #pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
            {                
                auto threadNum = omp_get_thread_num();
                Nd4jLong threadOffset = info.getThreadOffset(threadNum);        
                     
                #pragma omp simd
                for (Nd4jLong i = 0; i < info.getItersPerThread(threadNum); i++)  {                        
                    auto offset = shape::indexOffset(i + threadOffset, zShapeInfo, zShapeInfoCast, length, canCastZ);
                    z[offset] = OpClass::op(i+threadOffset, length, rng, extraArguments);
                }
            }
            
            // update rng state
            rng->rewindH(length);
        }

        template<typename X>
        void RandomFunction<X>::execTransform(int opNum, Nd4jPointer state, void *x, Nd4jLong *xShapeInfo, void *z, Nd4jLong *zShapeInfo, void *extraArguments) {
            DISPATCH_BY_OPNUM_T(execTransform, PARAMS(state, x, xShapeInfo, z, zShapeInfo, extraArguments), RANDOM_OPS)
        }

        template<typename X>
        void RandomFunction<X>::execTransform(int opNum, Nd4jPointer state, void *x, Nd4jLong *xShapeInfo, void *y, Nd4jLong *yShapeInfo, void *z, Nd4jLong *zShapeInfo, void *extraArguments) {
            DISPATCH_BY_OPNUM_T(execTransform, PARAMS(state, x, xShapeInfo, y, yShapeInfo, z, zShapeInfo, extraArguments), RANDOM_OPS)
        }

        template<typename X>
        void RandomFunction<X>::execTransform(int opNum, Nd4jPointer state, void *z, Nd4jLong *zShapeInfo, void *extraArguments) {
            DISPATCH_BY_OPNUM_T(execTransform, PARAMS(state, z, zShapeInfo, extraArguments), RANDOM_OPS)
        }

        // FIXME: eventually we might want to get rid of that
#ifndef __CLION_IDE__
/*
        BUILD_CALL_1(template void RandomFunction<float>::execTransform, float, (Nd4jPointer state, float *x, Nd4jLong *xShapeInfo, float *y, Nd4jLong *yShapeInfo, float *z, Nd4jLong *zShapeInfo, float *extraArguments), RANDOM_OPS)
        BUILD_CALL_1(template void RandomFunction<float16>::execTransform, float16, (Nd4jPointer state, float16 *x, Nd4jLong *xShapeInfo, float16 *y, Nd4jLong *yShapeInfo, float16 *z, Nd4jLong *zShapeInfo, float16 *extraArguments), RANDOM_OPS)
        BUILD_CALL_1(template void RandomFunction<double>::execTransform, double, (Nd4jPointer state, double *x, Nd4jLong *xShapeInfo, double *y, Nd4jLong *yShapeInfo, double *z, Nd4jLong *zShapeInfo, double *extraArguments), RANDOM_OPS)

        BUILD_CALL_1(template void RandomFunction<float>::execTransform, float, (Nd4jPointer state, float *x, Nd4jLong *xShapeInfo, float *z, Nd4jLong *zShapeInfo, float *extraArguments), RANDOM_OPS)
        BUILD_CALL_1(template void RandomFunction<float16>::execTransform, float16, (Nd4jPointer state, float16 *x, Nd4jLong *xShapeInfo, float16 *z, Nd4jLong *zShapeInfo, float16 *extraArguments), RANDOM_OPS)
        BUILD_CALL_1(template void RandomFunction<double>::execTransform, double, (Nd4jPointer state, double *x, Nd4jLong *xShapeInfo, double *z, Nd4jLong *zShapeInfo, double *extraArguments), RANDOM_OPS)

        BUILD_CALL_1(template void RandomFunction<float>::execTransform, float, (Nd4jPointer state, float *z, Nd4jLong *zShapeInfo, float *extraArguments), RANDOM_OPS)
        BUILD_CALL_1(template void RandomFunction<float16>::execTransform, float16, (Nd4jPointer state, float16 *z, Nd4jLong *zShapeInfo, float16 *extraArguments), RANDOM_OPS)
        BUILD_CALL_1(template void RandomFunction<double>::execTransform, double, (Nd4jPointer state, double *z, Nd4jLong *zShapeInfo, double *extraArguments), RANDOM_OPS)
*/
#endif


        BUILD_SINGLE_TEMPLATE(template class ND4J_EXPORT RandomFunction, , FLOAT_TYPES);
    }
}