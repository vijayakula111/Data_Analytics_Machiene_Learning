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
//  @author raver119@gmail.com
//

#include <op_boilerplate.h>
#include <types/float16.h>
#include <ops/declarable/helpers/batched_gemm.h>
#include <helpers/BlasHelper.h>


namespace nd4j {
    namespace ops {
        namespace helpers {
        

            template <typename T>
            void __bgemm(std::vector<NDArray*>& vA, std::vector<NDArray*>& vB, std::vector<NDArray*>& vC, NDArray* alphas, NDArray* betas, int transA, int transB, int M, int N, int K, int ldA, int ldB, int ldC) {
                int batchSize = vA.size();
                if (BlasHelper::getInstance()->hasBatchedGEMM<T>()) {
                    auto arr = vA.at(0);
                    CBLAS_TRANSPOSE *tA, *tB;
                    int *tM, *tN, *tK, *tldA, *tldB, *tldC, *tsize;
                    // mkl requires mnk etc as arrays, cuda doesn't
                    ALLOCATE(tA, arr->getWorkspace(), batchSize, CBLAS_TRANSPOSE);
                    ALLOCATE(tB, arr->getWorkspace(), batchSize, CBLAS_TRANSPOSE);
                    ALLOCATE(tM, arr->getWorkspace(), batchSize, int);
                    ALLOCATE(tN, arr->getWorkspace(), batchSize, int);
                    ALLOCATE(tK, arr->getWorkspace(), batchSize, int);
                    ALLOCATE(tldA, arr->getWorkspace(), batchSize, int);
                    ALLOCATE(tldB, arr->getWorkspace(), batchSize, int);
                    ALLOCATE(tldC, arr->getWorkspace(), batchSize, int);
                    ALLOCATE(tsize, arr->getWorkspace(), batchSize, int);

                    shape::fill(tA, (CBLAS_TRANSPOSE) transA, batchSize);
                    shape::fill(tB, (CBLAS_TRANSPOSE) transB, batchSize);

                    shape::fill(tM, M, batchSize);
                    shape::fill(tN, N, batchSize);
                    shape::fill(tK, K, batchSize);
                    shape::fill(tldA, ldA, batchSize);
                    shape::fill(tldB, ldB, batchSize);
                    shape::fill(tldC, ldC, batchSize);
                    shape::fill(tsize, 1, batchSize);

                    std::vector<T*> buffersA(batchSize);
                    std::vector<T*> buffersB(batchSize);
                    std::vector<T*> buffersC(batchSize);

                    for (int e = 0; e < batchSize; e++) {
                        buffersA[e] = reinterpret_cast<T *>(vA[e]->buffer());
                        buffersB[e] = reinterpret_cast<T *>(vB[e]->buffer());
                        buffersC[e] = reinterpret_cast<T *>(vC[e]->buffer());
                    }

                    if (std::is_same<T, double>::value) {
                        BlasHelper::getInstance()->dgemmBatched()(CblasColMajor, tA, tB, tM, tN, tK, (double *) alphas->buffer(), (double **) buffersA.data(), tldA, (double **) buffersB.data(), tldB, (double *) betas->buffer(),(double **)  buffersC.data(), tldC, vA.size(), tsize);
                    } else if (std::is_same<T, float >::value) {
                        BlasHelper::getInstance()->sgemmBatched()(CblasColMajor, tA, tB, tM, tN, tK, (float *) alphas->buffer(), (float **) buffersA.data(), tldA, (float **) buffersB.data(), tldB, (float *) betas->buffer(), (float **) buffersC.data(), tldC, vA.size(), tsize);
                    }

                    // release temporary arrays
                    RELEASE(tA, arr->getWorkspace());
                    RELEASE(tB, arr->getWorkspace());
                    RELEASE(tM, arr->getWorkspace());
                    RELEASE(tN, arr->getWorkspace());
                    RELEASE(tK, arr->getWorkspace());
                    RELEASE(tldA, arr->getWorkspace());
                    RELEASE(tldB, arr->getWorkspace());
                    RELEASE(tldC, arr->getWorkspace());
                    RELEASE(tsize, arr->getWorkspace());
                } else {
                    CBLAS_TRANSPOSE tA = (CBLAS_TRANSPOSE) transA;
                    CBLAS_TRANSPOSE tB = (CBLAS_TRANSPOSE) transB;

#pragma omp parallel for
                    for (int p = 0; p < vA.size(); ++p) {
                        auto A = reinterpret_cast<T*>(vA.at(p)->buffer());
                        auto B = reinterpret_cast<T*>(vB.at(p)->buffer());
                        auto C = reinterpret_cast<T*>(vC.at(p)->buffer());
                        auto alpha = alphas->e<T>(p);
                        auto beta = betas->e<T>(p);
                        for (int m = 0; m < M; ++m) {
                            for (int n = 0; n < N; ++n) {
                                T c_mnp = 0;

                                #pragma omp simd
                                for (int k = 0; k < K; ++k)
                                    c_mnp += A[tA == CblasNoTrans ? (m + k * ldA) : (m * ldA + k)] * B[tB == CblasNoTrans ? (k + n * ldB) : (k * ldB + n)];

                                C[m + n * ldC] = alpha * c_mnp + beta * C[m + n * ldC];
                            } 
                        } 
                    }
                }
            };


            void _bgemm(std::vector<NDArray*>& vA, std::vector<NDArray*>& vB, std::vector<NDArray*>& vC, NDArray* alphas, NDArray* betas, int transA, int transB, int M, int N, int K, int ldA, int ldB, int ldC) {
                auto xType = vA.at(0)->dataType();

                BUILD_SINGLE_SELECTOR(xType, __bgemm, (vA, vB, vC, alphas, betas, transA, transB, M, N, K, ldA, ldB, ldC), FLOAT_TYPES);
            }

            BUILD_SINGLE_TEMPLATE(template void __bgemm, (std::vector<NDArray*>& vA, std::vector<NDArray*>& vB, std::vector<NDArray*>& vC, NDArray* alphas, NDArray* betas, int transA, int transB, int M, int N, int K, int ldA, int ldB, int ldC), FLOAT_TYPES);
        }
    }
}