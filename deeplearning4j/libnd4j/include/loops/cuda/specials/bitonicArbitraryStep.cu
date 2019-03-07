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
// @author Yurii Shyrma, created on 28.11.2018
//

#include <ops/specials_cuda.h>


//////////////////////////////////////////////////////////////////////////
template<typename T>
__device__
void bitonicArbitraryStepKernel(void *vx, Nd4jLong *xShapeInfo, int window, int length,  int reverse, bool descending) {

    auto x = static_cast<T*>(vx);

    int tid = threadIdx.x + blockDim.x * blockIdx.x;
    int half = window>>1;

    __shared__ T *shmem;
    if (threadIdx.x == 0) {
        extern __shared__ unsigned char shrd[];
        shmem = (T *) shrd;
    }
    __syncthreads();

    //for (int i = 0; i < length; i+= window)
    /*
        if window == 4;
        iterations will be: 0; 4; 8; 12; 16; 20
        if gridDim = 3;
        on first iteration we'll have: 0; 4; 8;
        on second iteration we'll have: 0 + (3 * 4) = 12;  4 + (3 * 4) = 16; 8 + (3 * 4) = 20
    */
    int firstPosition;
    int firstStep;
    int secondPosition;
    int secondStep;

    int WARP_SIZE = 32;
    int numWarps = (gridDim.x * blockDim.x) / 32;
    int warpId = tid / WARP_SIZE;
    int warpIdx = tid % WARP_SIZE;

    if (half >= 128) {
        firstPosition = blockIdx.x * window;
        firstStep = gridDim.x * window;

        secondPosition = threadIdx.x;
        secondStep = blockDim.x;
    } else if (half >= 32) {
        firstPosition = warpId * window;
        firstStep = numWarps * window;

        secondPosition = warpIdx;
        secondStep = WARP_SIZE;
    } else {
        firstPosition = tid * window;
        firstStep = blockDim.x * gridDim.x * window;

        secondPosition = 0;
        secondStep = 1;
    }


    for (int i = firstPosition; i < length; i += firstStep) {
        for (int j = secondPosition; j < half; j += secondStep) {
            int it = (reverse) ? i + j + half : i + window - j - 1;
            int ij = i+j;
            if (it < length && ij < length ) {
                int posIT = getDevicePosition(xShapeInfo,it);
                int posIJ = getDevicePosition(xShapeInfo, ij);

                shmem[threadIdx.x] = x[posIJ];
                shmem[threadIdx.x + blockDim.x] = x[posIT];

                if(!descending == (shmem[threadIdx.x] > shmem[threadIdx.x + blockDim.x])) {
                    x[posIJ] = shmem[threadIdx.x + blockDim.x];
                    x[posIT] = shmem[threadIdx.x];
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
__global__ void execBitonicArbitraryStepKernel(void *vx, Nd4jLong *xShapeInfo, int window, int length,  int reverse, bool descending) {

    bitonicArbitraryStepKernel<T>(vx, xShapeInfo, window, length, reverse, descending);
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
__host__ void bitonicArbitraryStepGeneric(dim3 &launchDims, cudaStream_t *stream, void *vx, Nd4jLong *xShapeInfo, int window, int length,  int reverse, bool descending) {

    execBitonicArbitraryStepKernel<T><<<launchDims.x, launchDims.y, launchDims.z, *stream>>>(vx, xShapeInfo, window, length, reverse, descending);
}
BUILD_SINGLE_TEMPLATE(template void ND4J_EXPORT bitonicArbitraryStepGeneric, (dim3 &launchDims, cudaStream_t *stream, void *vx, Nd4jLong *xShapeInfo, int window, int length,  int reverse, bool descending), LIBND4J_TYPES);
