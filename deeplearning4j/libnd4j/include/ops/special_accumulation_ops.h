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
// @author raver119@gmail.com
//

#ifndef LIBND4J_SPECIAL_ACCUMULATION_OPS_H
#define LIBND4J_SPECIAL_ACCUMULATION_OPS_H

#include <templatemath.h>
#include <helpers/TAD.h>
//#include <ops/ops.h>
//#include <loops/reduce.h>

namespace simdOps {

    template<typename T, typename Z>
    class LogSumExp {
    public:
        static const bool requiresSpecialAccumulation = true;

        constexpr static functions::ReduceType reduceType = functions::ReduceType::SUM;

        op_def static T startingValue(T *input) {
            return (T) 0.0f;
        }

        op_def static Z merge(T old, T opOutput, Z *extraParams) {
            return opOutput + old;
        }

        op_def static T update(T old, T opOutput, Z *extraParams) {
            return opOutput + old;
        }

        op_def static Z op(T d1, T d2) {
            return nd4j::math::nd4j_exp<T, Z>(d1 - d2);
        }

        op_def static Z op(T d1, Z* extraParams) {
            return nd4j::math::nd4j_exp<Z, Z>(static_cast<Z>(d1) - extraParams[0]);
        }

        op_def static Z postProcess(T reduction, Nd4jLong n, Z *extraParams) {
            return extraParams[0] + nd4j::math::nd4j_log<T, Z>(reduction);
        }

#ifdef __CUDACC__
        __device__ static inline void aggregatePartials(Z *sPartials, int tid, int numItems, Z *extraParams) {
            // start the shared memory loop on the next power of 2 less
            // than the block size.  If block size is not a power of 2,
            // accumulate the intermediate sums in the remainder range.
            int floorPow2 = numItems;

            if (floorPow2 & (floorPow2 - 1)) {
                while (floorPow2 & (floorPow2 - 1)) {
                    floorPow2 &= floorPow2 - 1;
                }
                if (tid >= floorPow2) {
                    sPartials[tid - floorPow2] = update(sPartials[tid - floorPow2], sPartials[tid], extraParams);
                }

                __syncthreads();
            }


            for (int activeThreads = floorPow2 >> 1; activeThreads; activeThreads >>= 1) {
                if (tid < activeThreads && tid + activeThreads < numItems) {
                    sPartials[tid] = update(sPartials[tid], sPartials[tid + activeThreads], extraParams);
                }
                __syncthreads();
            }
        }

        static inline __device__ void execSpecialCuda(
				T *dx,
				Nd4jLong *xShapeInfo,
				Z *extraParams,
				Z *result,
				Nd4jLong *resultShapeInfo,
				int *dimension,
				int dimensionLength,
				Z *reductionBuffer,
				Nd4jLong *tadOnlyShapeInfo,
				Nd4jLong *tadOffsets) {

				// we assume that RESULT already holds max values

				//shared memory space for storing intermediate results
				__shared__ Z *sPartials;

				//                __shared__ shape::TAD *tad;
				__shared__ Nd4jLong tadLength;
				__shared__ Nd4jLong numTads;

				if (threadIdx.x == 0) {
				    extern __shared__ unsigned char shmem[];
				    sPartials = (Z *) shmem;
					tadLength = shape::tadLength(xShapeInfo, dimension, dimensionLength);
					numTads = shape::length(xShapeInfo) / tadLength;					
				}
				__syncthreads();				

				for (int r = blockIdx.x; r < numTads; r += gridDim.x) {
					auto tadOffsetForBlock = tadOffsets[r];

					sPartials[threadIdx.x] = startingValue(dx + tadOffsetForBlock);

					for (int i = threadIdx.x; i < tadLength; i += blockDim.x) {						
						auto xOffset = tadOffsetForBlock + shape::getIndexOffset(i, tadOnlyShapeInfo, tadLength);
						sPartials[threadIdx.x] = update(sPartials[threadIdx.x], op(dx[xOffset], result[r]), extraParams);
					}
					__syncthreads();

					// aggregate. do NOT reduce for elements > tadLength
					aggregatePartials(sPartials, threadIdx.x, nd4j::math::nd4j_min<int>(blockDim.x, tadLength), &result[r]);

					__syncthreads();
					if (threadIdx.x == 0)
						result[r] = postProcess(sPartials[threadIdx.x], tadLength, &result[r]);
				}
			}
#endif

        static void execSpecial(T *x,
                         Nd4jLong *xShapeInfo,
                         Z *extraParams,
                         Z *result,
                         Nd4jLong *resultShapeInfoBuffer,
                         int *dimension,
                         int dimensionLength,
                         Nd4jLong *tadShapeInfo,
                         Nd4jLong *tadOffset) {
            Nd4jLong resultLength = shape::length(resultShapeInfoBuffer);

            auto tadOnlyShapeInfo = tadShapeInfo;
            auto tadOffsets = tadOffset;
            shape::TAD *tad = nullptr;

            if (tadOnlyShapeInfo == nullptr || tadOffsets == nullptr) {
                tad = new shape::TAD();
                tad->init(xShapeInfo, dimension, dimensionLength);
                tad->createTadOnlyShapeInfo();
                tad->createOffsets();

                if (tad->dimensionLength < 1) {
                    delete tad;
                    return;
                }

                tadOnlyShapeInfo = tad->tadOnlyShapeInfo;
                tadOffsets = tad->tadOffsets;
            }


            const Nd4jLong tadLength = shape::tadLength(xShapeInfo, dimension, dimensionLength);
            auto numTads = shape::length(xShapeInfo) / tadLength;
            auto tadEWS = shape::elementWiseStride(tadOnlyShapeInfo);

            int tadsPerThread = resultLength / TAD_THRESHOLD;
            int num_threads = nd4j::math::nd4j_max<int>(1, tadsPerThread);
            num_threads = nd4j::math::nd4j_min<int>(num_threads, omp_get_max_threads());

            if (tadEWS > 0 && (numTads == 1 || shape::isVector(tadOnlyShapeInfo) || shape::isScalar(tadOnlyShapeInfo))) {

#pragma omp parallel for schedule(guided) num_threads(num_threads) if (num_threads > 1) proc_bind(close) default(shared)
                for (int i = 0; i < resultLength; i++) {

                    T *iter = x + tadOffsets[i];
                    T start = startingValue(iter);
                    if (tadEWS == 1) {
                        for (int j = 0; j < tadLength; j++) {
                            start = update(start, op(iter[j], result[i]), extraParams);

                        }
                    }
                    else {
                        for (int j = 0; j < tadLength; j++) {
                            start = update(start, op(iter[j * tadEWS], result[i]), extraParams);
                        }
                    }
                    result[i] = postProcess(start, tadLength, &result[i]);
                }
            }
            else {

#pragma omp  parallel for schedule(guided) num_threads(num_threads) if (num_threads > 1) proc_bind(close) default(shared)
                for (int i = 0; i < resultLength; i++) {

                    auto offset = tadOffsets[i];
                    T start = startingValue(x + offset);

                    for (int j = 0; j < tadLength; j++) {                        
                        auto xOffset = offset + shape::getIndexOffset(j, tadOnlyShapeInfo, tadLength);
                        start = update(start, op(x[xOffset], result[i]), extraParams);
                    }

                    result[i] = postProcess(start, tadLength, &result[i]);;
                }
            }

            if (tad != nullptr)
                delete tad;

        }
    };
}

#endif //LIBND4J_SPECIAL_ACCUMULATION_OPS_H
