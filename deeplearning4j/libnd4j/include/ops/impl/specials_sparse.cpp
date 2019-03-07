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

#include <ops/specials_sparse.h>
#include <dll.h>
#include <pointercast.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <types/float16.h>
#include <types/types.h>

namespace nd4j {
    namespace sparse {

        template <typename T>
        void SparseUtils<T>::printIndex(Nd4jLong *indices, int rank, int x) {
            printf(" [");
            for (int e = 0; e < rank; e++) {
                if (e > 0)
                    printf(", ");

                printf("%lld", (long long) indices[x * rank + e]);
            }
            printf("] ");
        }

        template <typename T>
        bool SparseUtils<T>::ltIndices(Nd4jLong *indices, int rank, Nd4jLong x, Nd4jLong y) {
            for (int e = 0; e < rank; e++) {
                Nd4jLong idxX = indices[x * rank + e];
                Nd4jLong idxY = indices[y * rank + e];
                // we're comparing indices one by one, starting from outer dimension
                if (idxX < idxY) {
                    return true;
                } else if (idxX == idxY) {
                    // do nothing, continue to next dimension
                } else
                    return false;
            }

            return false;
        }

        template <typename T>
        bool SparseUtils<T>::gtIndices(Nd4jLong *indices, int rank, Nd4jLong x, Nd4jLong y) {
            for (int e = 0; e < rank; e++) {
                // we're comparing indices one by one, starting from outer dimension
                Nd4jLong idxX = indices[x * rank + e];
                Nd4jLong idxY = indices[y * rank + e];
                if ( idxX > idxY) {
                    return true;
                } else if (idxX == idxY) {
                    // do nothing, continue to next dimension
                } else
                    return false;
            }
            return false;
        }

        template <typename T>
        void SparseUtils<T>::swapEverything(Nd4jLong *indices, T *array, int rank, Nd4jLong x, Nd4jLong y) {
            // swap indices
            for (int e = 0; e < rank; e++) {
                Nd4jLong tmp = indices[x * rank + e];
                indices[x * rank + e] = indices[y * rank + e];
                indices[y * rank + e] = tmp;
            }

            // swap values
            T tmp = array[x];
            array[x] = array[y];
            array[y] = tmp;
        }

        template <typename T>
        Nd4jLong SparseUtils<T>::coo_quickSort_findPivot(Nd4jLong *indices, T *array, Nd4jLong left, Nd4jLong right,
                                                         int rank) {
            Nd4jLong  mid = (left + right) / 2;

            // ensure left < mid
            if (ltIndices(indices, rank, mid, left)) {  // ensure lo < mid
               swapEverything(indices, array, rank, mid, left);
            }

            // ensure left < right
            if (ltIndices(indices, rank, right, left)) {
                swapEverything(indices, array, rank, right, left);
            }

            // ensure mid < right
            if (ltIndices(indices, rank, right, mid)) {
                swapEverything(indices, array, rank, right, mid);
            }

            // mid is the median of the 3, and is the optimal pivot point
            return mid;
    }

        template<typename T>
        void SparseUtils<T>::coo_quickSort_parallel_internal(Nd4jLong *indices, T* array, Nd4jLong left, Nd4jLong right, int cutoff, int rank) {
            Nd4jLong span = right - left;  // elements to be partitioned - 1

            if (span == 1){
                // only 2 elements to partition. swap if needed and return directly without further sorting.
                if (ltIndices(indices, rank, right, left)){
                    swapEverything(indices, array, rank, left, right);
                }
                return;
            }


            // find optimal pivot and sort left < right < right
            Nd4jLong pvt = coo_quickSort_findPivot(indices, array, left, right, rank);

            if (span == 2){
                // only 3 elements to partition. findPivot has already sorted them. no further sorting is needed.
                return;
            }

            // index that is greater than pivot - leftmost element is already partitioned because of findPivot.
            Nd4jLong i = left + 1;

            // index that is smaller than pivot - rightmost element is already partitioned because of findPivot.
            Nd4jLong j = right - 1;


            {
                // flag that indicates that pivot index lies between i and j and *could* be swapped.
                bool checkPivot = true;
                /* PARTITION PART */
                while (i <= j) {
                    while (ltIndices(indices, rank, i, pvt))
                        i++;

                    while (gtIndices(indices, rank, j, pvt))
                        j--;


                    if (i <= j) {
                        if(i != j) { // swap can be fairly expensive. don't swap i -> i
                            swapEverything(indices, array, rank, i, j);
                        }

                        // only check pivot if it hasn't already been swapped.
                        if (checkPivot) {
                            // check if we moved the pivot, if so, change pivot index accordingly
                            if (pvt == j) {
                                pvt = i;
                                checkPivot = false;
                            } else if (pvt == i) {
                                pvt = j;
                                checkPivot = false;
                            }
                        }

                        i++;
                        j--;
                    }
                }

            }

            if ( (span < cutoff) ){
                if (left < j){ coo_quickSort_parallel_internal(indices, array, left, j, cutoff, rank); }
                if (i < right){ coo_quickSort_parallel_internal(indices, array, i, right, cutoff, rank); }

            }else{
#pragma omp task
                { coo_quickSort_parallel_internal(indices, array, left, j, cutoff, rank); }
#pragma omp task
                { coo_quickSort_parallel_internal(indices, array, i, right, cutoff, rank); }
            }

        }

        template <typename T>
        void SparseUtils<T>::coo_quickSort_parallel(Nd4jLong *indices, T* array, Nd4jLong lenArray, int numThreads, int rank){

            int cutoff = 1000;

#pragma omp parallel num_threads(numThreads) if (numThreads>1)
            {
#pragma omp single nowait
                {
                    coo_quickSort_parallel_internal(indices, array, 0, lenArray-1, cutoff, rank);
                }
            }

        }

        template <typename T>
        void SparseUtils<T>::sortCooIndicesGeneric(Nd4jLong *indices, T *values, Nd4jLong length, int rank) {
#ifdef _OPENMP
            coo_quickSort_parallel(indices, values, length, omp_get_max_threads(), rank);
#else
            coo_quickSort_parallel(indices, values, length, 1, rank);
#endif
        }

        BUILD_SINGLE_TEMPLATE(template class ND4J_EXPORT SparseUtils, , LIBND4J_TYPES);
    }
}
