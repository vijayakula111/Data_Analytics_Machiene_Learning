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
//  @author  raver119@gmail.com
//

#include <op_boilerplate.h>
#include <types/types.h>
#include <loops/transform_bool.h>
#include <loops/legacy_ops.h>

using namespace simdOps;

namespace functions {
    namespace transform {

        template <typename X, typename Y>
        void TransformBool<X, Y>::exec(
				int opNum,
				void *x,
				Nd4jLong *xShapeInfo,
				void *z,
				Nd4jLong *zShapeInfo,
				void *extraParams,
				Nd4jLong *tadShapeInfo,
				Nd4jLong *tadOffsets) {
                    DISPATCH_BY_OPNUM_TT(exec, PARAMS(x, xShapeInfo, z, zShapeInfo, extraParams, tadShapeInfo, tadOffsets), TRANSFORM_BOOL_OPS);
		}

        template <typename X, typename Z>
        template<typename OpType>
		void _CUDA_H TransformBool<X, Z>::exec(
                    void *vx,
                    Nd4jLong *xShapeInfo,
                    void *vz,
                    Nd4jLong *zShapeInfo,
                    void *vextraParams,
                    Nd4jLong *tadShapeInfo,
                    Nd4jLong *tadOffsets) {

            auto x = reinterpret_cast<X *>(vx);
		    auto z = reinterpret_cast<Z *>(vz);
		       auto extraParams = reinterpret_cast<X *>(vextraParams);

            if(OpType::requiresSpecial) {
                OpType::execSpecial(x, xShapeInfo, z, zShapeInfo, extraParams, tadShapeInfo, tadOffsets);
                return;
            }

            const auto len = shape::length(xShapeInfo);
                        
            nd4j::OmpLaunchHelper info(len);

            if (shape::elementWiseStride(xShapeInfo) == 1 && shape::elementWiseStride(zShapeInfo) == 1 && shape::order(xShapeInfo) == shape::order(zShapeInfo)) {

#pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {
                    auto threadNum = omp_get_thread_num();
                    auto threadOffset = info.getThreadOffset(threadNum);

                    auto tz = z + threadOffset;
                    auto tx = x + threadOffset;

                    #pragma omp simd
                    for (unsigned int i = 0; i < info.getItersPerThread(threadNum); i++)
                        tz[i] = OpType::op(tx[i], extraParams);
                }
            } else if(shape::haveSameOffsets(xShapeInfo, zShapeInfo)) {
                uint xShapeInfoCast[MAX_RANK];
                bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);

#pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {
                    auto threadNum = omp_get_thread_num();
                    auto threadOffset = info.getThreadOffset(threadNum);                        

                    #pragma omp simd
                    for (unsigned int i = 0; i < info.getItersPerThread(threadNum); i++) {
                        auto offset = shape::indexOffset(i + threadOffset, zShapeInfo, xShapeInfoCast, len, canCastX);
                        z[offset] = OpType::op(x[offset], extraParams);
                    }
                }
            }
            else {
                uint xShapeInfoCast[MAX_RANK];
                bool canCastX = nd4j::DataTypeUtils::castShapeInfo(xShapeInfo, xShapeInfoCast);

                uint zShapeInfoCast[MAX_RANK];
                bool canCastZ = nd4j::DataTypeUtils::castShapeInfo(zShapeInfo, zShapeInfoCast);

#pragma omp parallel num_threads(info._numThreads) if (info._numThreads > 1) default(shared)
                {
                    auto threadNum = omp_get_thread_num();
                    auto threadOffset = info.getThreadOffset(threadNum);

                    #pragma omp simd
                    for (unsigned int i = 0; i < info.getItersPerThread(threadNum); i++) {
                        auto xOffset = shape::indexOffset(i + threadOffset, xShapeInfo, xShapeInfoCast, len, canCastX);
                        auto zOffset = shape::indexOffset(i + threadOffset, zShapeInfo, zShapeInfoCast, len, canCastZ);
                        z[zOffset] = OpType::op(x[xOffset], extraParams);
                    }
                }
            }
        }

        BUILD_DOUBLE_TEMPLATE(template class ND4J_EXPORT TransformBool, , LIBND4J_TYPES, BOOL_TYPES);
    }
}