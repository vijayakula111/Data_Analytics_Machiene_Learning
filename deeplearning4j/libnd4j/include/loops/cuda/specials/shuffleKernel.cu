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
// @author Yurii Shyrma, created on 15.11.2018
//

#include <loops/special_kernels.h>

namespace nd4j {

////////////////////////////////////////////////////////////////////////
    template<typename T>
    __device__ void shuffleKernel(void **vdX, Nd4jLong **xShapeInfo,
                                  void **vdZ,
                                  int N,
                                  int *shuffleMap,
                                  Nd4jLong **tadOnlyShapeInfo, Nd4jLong **tadOffsets) {

        // we assume that shuffle map for each X contains pair TAD Y

        auto dX = reinterpret_cast<T **>(vdX);
        auto dZ = reinterpret_cast<T **>(vdZ);

        __shared__ int tadLength;
        __shared__ int tadEWS;
        __shared__ int numTads;

        for (int f = 0; f < N; f++) {

            T *x = (T *) dX[f];
            T *z = (T *) dZ[f];

            __syncthreads();

            if (threadIdx.x == 0) {
                tadLength = shape::length(tadOnlyShapeInfo[f]);
                tadEWS = shape::elementWiseStride(tadOnlyShapeInfo[f]);
                numTads = shape::length(xShapeInfo[f]) / tadLength;
            }
            __syncthreads();

            // we roll over the pairs of TADs, thus limit is numTads / 2
            for (Nd4jLong r = blockIdx.x; r < numTads; r += blockDim.x) {

                if (shuffleMap[r] < 0)
                    continue;

                Nd4jLong oldOffset = tadOffsets[f][r];
                Nd4jLong newOffset = tadOffsets[f][shuffleMap[r]];

                T *rX = x + oldOffset;
                T *rY = x + newOffset;

                T *zX = z + oldOffset;
                T *zY = z + newOffset;

                // so we're going to change TAD[oldOffset] with TAD[newOffset]
                if (tadEWS == 1) {

                    for (Nd4jLong i = threadIdx.x; i < tadLength; i += blockDim.x) {

                        T oldX = rX[i];
                        rX[i] = rY[i];
                        zY[i] = oldX;
                    }

                } else {

                    for (Nd4jLong i = threadIdx.x; i < tadLength; i += blockDim.x) {

                        auto xOffset = shape::getIndexOffset(i, tadOnlyShapeInfo[f], tadLength);
                        auto yOffset = newOffset + xOffset;
                        xOffset += oldOffset;

                        T oldX = x[xOffset];
                        z[xOffset] = x[yOffset];
                        z[yOffset] = oldX;
                    }
                }
            }
        }
    }

////////////////////////////////////////////////////////////////////////
    template<typename T>
    __global__ void execShuffleKernel(void **vdX, Nd4jLong **xShapeInfo,
                                      void **vdZ,
                                      int N,
                                      int *shuffleMap,
                                      Nd4jLong **tadOnlyShapeInfo, Nd4jLong **tadOffsets) {

        shuffleKernel<T>(vdX, xShapeInfo, vdZ, N, shuffleMap, tadOnlyShapeInfo, tadOffsets);
    }

////////////////////////////////////////////////////////////////////////
    template<typename T>
    __host__ void shuffleKernelGeneric(dim3 &launchDims, cudaStream_t *stream,
                                       void **vdX, Nd4jLong **xShapeInfo,
                                       void **vdZ,
                                       int N,
                                       int *shuffleMap,
                                       Nd4jLong **tadOnlyShapeInfo, Nd4jLong **tadOffsets) {

        execShuffleKernel<T><<<launchDims.x, launchDims.y, launchDims.z, *stream>>>(vdX, xShapeInfo, vdZ, N, shuffleMap, tadOnlyShapeInfo, tadOffsets);
    }

    BUILD_SINGLE_TEMPLATE(template void ND4J_EXPORT shuffleKernelGeneric, (dim3 & launchDims, cudaStream_t * stream, void * *vdX, Nd4jLong * *xShapeInfo, void **vdZ, int N, int * shuffleMap, Nd4jLong * *tadOnlyShapeInfo, Nd4jLong * *tadOffsets), LIBND4J_TYPES);
}