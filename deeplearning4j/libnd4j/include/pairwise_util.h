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
// Created by agibsonccc on 2/15/16.
//

#ifndef NATIVEOPERATIONS_PAIRWISE_UTIL_H
#define NATIVEOPERATIONS_PAIRWISE_UTIL_H
#ifdef __CUDACC__
#include <cuda.h>
#include <cuda_runtime.h>
#endif
#ifndef _OPENMP
#define omp_get_thread_num() 0
#define omp_get_num_threads() 1
#define omp_get_max_threads() 1
#define omp_set_num_threads(threads)
#endif

#include <templatemath.h>
#include <functional>
#include <pointercast.h>
#include <op_boilerplate.h>
#include <dll.h>
#include <nd4jmemset.h>
#ifdef _OPENMP
#include <omp.h>
#endif
//Loops adapted from:
//https://github.com/numpy/numpy/blob/009b17a85a22707e63ac9ea1896413992bbf9ce5/numpy/core/src/private/lowlevel_strided_loops.h#L401-L401

/*
namespace shape {

    Nd4jLong length(const Nd4jLong *shapeInfo);
    Nd4jLong elementWiseStride(const Nd4jLong *shapeInfo);
    char order(const Nd4jLong *shapeInfo);
    bool isStrideSimple(const Nd4jLong* shapeInfo);
    Nd4jLong getIndexOffset(Nd4jLong index, const Nd4jLong *shapeInfo, Nd4jLong arrLen);
}

 */
/************************************************************
 * A struct used by CreateSortedStridePerm, new in 1.7.
 ************************************************************/

typedef struct {
    int perm, stride;
} StridePermutation;


/**
 * Credit to:
 * http://alienryderflex.com/quicksort/
 *
 * The non recursive implementation is important for being able to run on cuda as well
 *  as host.
 *
 *  In practice the work loads intended for
 *  this won't be so hard
 *  that it matters.
 *
 *  We can work on optimizations later that leverage openmp,
 *  the gpu etc
 */
#ifdef __CUDACC__
__host__ __device__
#endif
void quickSort(StridePermutation *arr, int elements);


/* Start raw iteration */
#define ND4J_RAW_ITER_START(idim, ndim, coord, shape) \
        memset((coord), 0, (ndim) * sizeof(coord[0])); \
        do {

/* Increment to the next n-dimensional coordinate for one raw array */
#define ND4J_RAW_ITER_ONE_NEXT(idim, ndim, coord, shape, data, strides) \
            for ((idim) = 0; (idim) < (ndim); (idim)++) { \
                if (++(coord)[idim] == (shape)[idim]) { \
                    (coord)[idim] = 0; \
                    (data) -= ((shape)[idim] - 1) * (strides)[idim]; \
                } \
                else { \
                    (data) += (strides)[idim]; \
                    break; \
                } \
            } \
        } while ((idim) < (ndim))

#define ND4J_RAW_ITER_ONE_NEXTF(idim, ndim, coord, shape, data, strides) \
            for ((idim) = ndim - 1; (idim) >= (0); (idim)--) { \
                if (++(coord)[idim] == (shape)[idim]) { \
                    (coord)[idim] = 0; \
                    (data) -= ((shape)[idim] - 1) * (strides)[idim]; \
                } \
                else { \
                    (data) += (strides)[idim]; \
                    break; \
                } \
            } \
        } while ((idim) >= (0))


/* Increment to the next n-dimensional coordinate for two raw arrays */
#define ND4J_RAW_ITER_TWO_NEXT(idim, ndim, coord, shape, \
                              dataA, stridesA, dataB, stridesB) \
            for ((idim) = 0; (idim) < (ndim); (idim)++) { \
                if (++(coord)[idim] == (shape)[idim]) { \
                    (coord)[idim] = 0; \
                    (dataA) -= ((shape)[idim] - 1) * (stridesA)[idim]; \
                    (dataB) -= ((shape)[idim] - 1) * (stridesB)[idim]; \
                } \
                else { \
                    (dataA) += (stridesA)[idim]; \
                    (dataB) += (stridesB)[idim]; \
                    break; \
                } \
            } \
        } while ((idim) < (ndim))

/* Increment to the next n-dimensional coordinate for three raw arrays */
#define ND4J_RAW_ITER_THREE_NEXT(idim, ndim, coord, shape, \
                              dataA, stridesA, \
                              dataB, stridesB, \
                              dataC, stridesC) \
            for ((idim) = 0; (idim) < (ndim); (idim)++) { \
                if (++(coord)[idim] == (shape)[idim]) { \
                    (coord)[idim] = 0; \
                    (dataA) -= ((shape)[idim] - 1) * (stridesA)[idim]; \
                    (dataB) -= ((shape)[idim] - 1) * (stridesB)[idim]; \
                    (dataC) -= ((shape)[idim] - 1) * (stridesC)[idim]; \
                } \
                else { \
                    (dataA) += (stridesA)[idim]; \
                    (dataB) += (stridesB)[idim]; \
                    (dataC) += (stridesC)[idim]; \
                    break; \
                } \
            } \
        } while ((idim) < (ndim))

/* Increment to the next n-dimensional coordinate for four raw arrays */
#define ND4J_RAW_ITER_FOUR_NEXT(idim, ndim, coord, shape, \
                              dataA, stridesA, \
                              dataB, stridesB, \
                              dataC, stridesC, \
                              dataD, stridesD) \
            for ((idim) = 0; (idim) < (ndim); (idim)++) { \
                if (++(coord)[idim] == (shape)[idim]) { \
                    (coord)[idim] = 0; \
                    (dataA) -= ((shape)[idim] - 1) * (stridesA)[idim]; \
                    (dataB) -= ((shape)[idim] - 1) * (stridesB)[idim]; \
                    (dataC) -= ((shape)[idim] - 1) * (stridesC)[idim]; \
                    (dataD) -= ((shape)[idim] - 1) * (stridesD)[idim]; \
                } \
                else { \
                    (dataA) += (stridesA)[idim]; \
                    (dataB) += (stridesB)[idim]; \
                    (dataC) += (stridesC)[idim]; \
                    (dataD) += (stridesD)[idim]; \
                    break; \
                } \
            } \
        } while ((idim) < (ndim))






/*NUMPY_API
 *
 * This function populates the first ndim elements
 * of strideperm with sorted descending by their absolute values.
 * For example, the stride array (4, -2, 12) becomes
 * [(2, 12), (0, 4), (1, -2)].
 */
#ifdef __CUDACC__
__host__ __device__
#endif
inline void  SortStrideArray(int ndim, int strides[],
                             StridePermutation *out_strideperm) {

    /* Set up the strideperm values */
    for (int i = 0; i < ndim; i++) {
        out_strideperm[i].perm = i;
        out_strideperm[i].stride = strides[i];
    }

    /* Sort them */
    quickSort(out_strideperm,ndim);

}

/*
 * Prepares shape and strides for a simple raw array iteration.
 * This sorts the strides into FORTRAN order, reverses any negative
 * strides, then coalesces axes where possible. The results are
 * filled in the output parameters.
 *
 * This is intended for simple, lightweight iteration over arrays
 * where no buffering of any kind is needed, and the array may
 * not be stored as a PyArrayObject.
 *
 * The arrays shape, outShape, strides, and outStrides must all
 * point to different data.
 *
 * Returns 0 on success, -1 on failure.
 */
template <typename T>

#ifdef __CUDACC__
__host__ __device__
#endif
inline int PrepareOneRawArrayIter(int ndim, Nd4jLong shape[],
                                  T data[], Nd4jLong strides[],
                                  int *out_ndim, Nd4jLong outShape[],
                                  T **out_data, Nd4jLong *outStrides) {

    for (int i = 0; i < ndim; i++) {
        outShape[i] = shape[i];
        outStrides[i] = strides[i];
    }



#if 0
    /* DEBUG */
    {
        printf("raw iter ndim %d\n", ndim);
        printf("shape: ");
        for (int i = 0; i < ndim; ++i) {
            printf("%d ", outShape[i]);
        }
        printf("\n");
        printf("strides: ");
        for (int i = 0; i < ndim; ++i) {
            printf("%d ", outStrides[i]);
        }
        printf("\n");
    }
#endif

    *out_data = data;
    *out_ndim = ndim;
    return 0;
}


class BlockInformation {
public:
    Nd4jLong items;
    int threads;
    Nd4jLong chunks;
    Nd4jLong modulo;
    Nd4jLong remainder;
    
    BlockInformation(Nd4jLong length, int threshold) {

    threads = length / threshold;
    threads = nd4j::math::nd4j_max<int>(1, threads);
    threads = nd4j::math::nd4j_min<int>(threads, omp_get_max_threads());
    
    items = length / threads;
    remainder = length % threads;
    if(items < 1)
        items = 1;
    chunks = length / items;
    modulo = length % items;

    //one left over chunk
    if(modulo > 0)
        chunks++;
    }
};


class CudaBlockInformation {

};


/**
 * Credit to:
 * http://alienryderflex.com/quicksort/
 *
 * The non recursive implementation is important for being able to run on cuda as well
 *  as host.
 *
 *  In practice the work loads intended for
 *  this won't be so hard
 *  that it matters.
 *
 *  We can work on optimizations later that leverage openmp,
 *  the gpu etc
 */
#ifdef __CUDACC__
__host__ __device__
#endif
inline void quickSort(StridePermutation *arr, int elements) {
#define  MAX_LEVELS  300

    int  beg[MAX_LEVELS], end[MAX_LEVELS], i= 0, L, R, swap ;
    StridePermutation piv;
    beg[0] = 0;
    end[0] = elements;
    while (i >= 0) {
        L = beg[i];
        R=  end[i] - 1;
        if (L < R) {
            piv = arr[L];
            while (L < R) {
                while (arr[R].stride >= piv.stride && L < R)
                    R--;
                if (L < R)
                    arr[L++] = arr[R];
                while (arr[L].stride <= piv.stride && L < R)
                    L++;
                if (L<R)
                    arr[R--] = arr[L];
            }

            arr[L] = piv;
            beg[i + 1]= L + 1;
            end[i + 1]= end[i];
            end[i++] = L;
            if (end[i] - beg[i] > end[i - 1] - beg[i - 1]) {
                swap = beg[i];
                beg[i]= beg[i - 1];
                beg[i - 1] = swap;
                swap = end[i];
                end[i] = end[i - 1];
                end[i - 1] = swap;
            }
        }
        else {
            i--;
        }
    }
}

/**
 * The same as PrepareOneRawArrayIter, but for two
 * operands instead of one. Any broadcasting of the two operands
 * should have already been done before calling this function,
 * as the ndim and shape is only specified once for both operands.
 *
 * Only the strides of the first operand are used to reorder
 * the dimensions, no attempt to consider all the strides together
 * is made, as is done in the NpyIter object.
 *
 * You can use this together with NPY_RAW_ITER_START and
 * NPY_RAW_ITER_TWO_NEXT to handle the looping boilerplate of everything
 * but the innermost loop (which is for idim == 0).
 *
 * Returns 0 on success, -1 on failure.
 */
template <typename X, typename Y>
int _CUDA_HD PrepareTwoRawArrayIter(int ndim, Nd4jLong *shape,
                           X *dataA, Nd4jLong *stridesA,
                           Y *dataB, Nd4jLong *stridesB,
                           int *out_ndim, Nd4jLong *outShape,
                           X **out_dataA, Nd4jLong *outStridesA,
                           Y **out_dataB, Nd4jLong *outStridesB) {
    int i;

/* Sort the axes based on the destination strides */
    for (i = 0; i < ndim; ++i) {
        outShape[i] = shape[i];
        outStridesA[i] = stridesA[i];
        outStridesB[i] = stridesB[i];
    }

/* Reverse any negative strides of operand A */
    for (i = 0; i < ndim; i++) {
        Nd4jLong stride_entryA = outStridesA[i];
        Nd4jLong stride_entryB = outStridesB[i];
        Nd4jLong shape_entry = outShape[i];

        if (stride_entryA < 0) {
            dataA += stride_entryA * (shape_entry - 1);
            dataB += stride_entryB * (shape_entry - 1);
            outStridesA[i] = -stride_entryA;
            outStridesB[i] = -stride_entryB;
        }
/* Detect 0-size arrays here */
        if (shape_entry == 0) {
            *out_ndim = 1;
            *out_dataA = dataA;
            *out_dataB = dataB;
            outShape[0] = 0;
            outStridesA[0] = 0;
            outStridesB[0] = 0;
            return 0;
        }
    }


    *out_dataA = dataA;
    *out_dataB = dataB;
    *out_ndim = ndim;


#if 0
    /* DEBUG */
    {
        printf("raw iter ndim %d\n", ndim);
        printf("shape: ");
        for (i = 0; i < ndim; ++i) {
            printf("%d ", (int)outShape[i]);
        }
        printf("\n");
        printf("strides a: ");
        for (i = 0; i < ndim; ++i) {
            printf("%d ", (int)outStridesA[i]);
        }

        printf("\n");
        printf("strides b: ");

        for (i = 0; i < ndim; ++i) {
            printf("%d ", (int)outStridesB[i]);
        }

        printf("\n");

    }
#endif

    return 0;
}

/**
 * The same as PrepareOneRawArrayIter, but for three
 * operands instead of one. Any broadcasting of the three operands
 * should have already been done before calling this function,
 * as the ndim and shape is only specified once for all operands.
 *
 * Only the strides of the first operand are used to reorder
 * the dimensions, no attempt to consider all the strides together
 * is made, as is done in the NpyIter object.
 *
 * You can use this together with NPY_RAW_ITER_START and
 * NPY_RAW_ITER_THREE_NEXT to handle the looping boilerplate of everything
 * but the innermost loop (which is for idim == 0).
 *
 * Returns 0 on success, -1 on failure.
 */
template <typename X, typename Y, typename Z>
#ifdef __CUDACC__
__host__ __device__
#endif
int  PrepareThreeRawArrayIter(int ndim, Nd4jLong shape[],
                              X *dataA, Nd4jLong *stridesA,
                              Y *dataB, Nd4jLong *stridesB,
                              Z *dataC, Nd4jLong *stridesC,
                              int &out_ndim, Nd4jLong *outShape,
                              X **out_dataA, Nd4jLong outStridesA[],
                              Y **out_dataB, Nd4jLong outStridesB[],
                              Z **out_dataC, Nd4jLong outStridesC[])
{

    /* Special case 0 and 1 dimensions */
    if (ndim == 0) {
        out_ndim = 1;
        *out_dataA = dataA;
        *out_dataB = dataB;
        *out_dataC = dataC;
        outShape[0] = 1;
        outStridesA[0] = 0;
        outStridesB[0] = 0;
        outStridesC[0] = 0;
        return 0;
    }
    else if (ndim == 1) {
        auto stride_entryA = stridesA[0];
        auto stride_entryB = stridesB[0];
        auto stride_entryC = stridesC[0];
        auto shape_entry = shape[0];
        out_ndim = 1;
        outShape[0] = shape[0];
        /* Always make a positive stride for the first operand */
        if (stride_entryA >= 0) {
            *out_dataA = dataA;
            *out_dataB = dataB;
            *out_dataC = dataC;
            outStridesA[0] = stride_entryA;
            outStridesB[0] = stride_entryB;
            outStridesC[0] = stride_entryC;
        }
        else {
            *out_dataA = dataA + stride_entryA * (shape_entry - 1);
            *out_dataB = dataB + stride_entryB * (shape_entry - 1);
            *out_dataC = dataC + stride_entryC * (shape_entry - 1);
            outStridesA[0] = -stride_entryA;
            outStridesB[0] = -stride_entryB;
            outStridesC[0] = -stride_entryC;
        }
        return 0;
    }

    for (int i = 0; i < ndim; ++i) {
        outShape[i] = shape[i];
        outStridesA[i] = stridesA[i];
        outStridesB[i] = stridesB[i];
        outStridesC[i] = stridesC[i];
    }

    /* Reverse any negative strides of operand A */
    for (int i = 0; i < ndim; ++i) {
        auto stride_entryA = outStridesA[i];
        auto stride_entryB = outStridesB[i];
        auto stride_entryC = outStridesC[i];
        auto shape_entry = outShape[i];

        if (stride_entryA < 0) {
            dataA += stride_entryA * (shape_entry - 1);
            dataB += stride_entryB * (shape_entry - 1);
            dataC += stride_entryC * (shape_entry - 1);
            outStridesA[i] = -stride_entryA;
            outStridesB[i] = -stride_entryB;
            outStridesC[i] = -stride_entryC;
        }
        /* Detect 0-size arrays here */
        if (shape_entry == 0) {
            out_ndim = 1;
            *out_dataA = dataA;
            *out_dataB = dataB;
            *out_dataC = dataC;
            outShape[0] = 0;
            outStridesA[0] = 0;
            outStridesB[0] = 0;
            outStridesC[0] = 0;
            return 0;
        }
    }


    *out_dataA = dataA;
    *out_dataB = dataB;
    *out_dataC = dataC;
    out_ndim = ndim;
    return 0;
}


////////////////////////////////////////////////////////////////////////
/*
template <typename T>
_CUDA_HD void loop2ArrsSame(const T* x, const Nd4jLong* xShapeInfo, T* z, const Nd4jLong* zShapeInfo, T* extraParams, const std::function<T(T, T*)>& func) {

    const auto len = shape::length(xShapeInfo);
    const auto xEws = shape::elementWiseStride(xShapeInfo);
    const auto zEws = shape::elementWiseStride(zShapeInfo);
    const auto xOrder = shape::order(xShapeInfo);
    const auto zOrder = shape::order(zShapeInfo);
    const bool xSimpe = shape::isStrideSimple(xShapeInfo);
    const bool zSimpe = shape::isStrideSimple(zShapeInfo);
    
    if(xEws >= 1 && zEws >= 1 && xOrder == zOrder) {

        Nd4jLong elementsPerThread = len / ELEMENT_THRESHOLD;
        int numThreads = nd4j::math::nd4j_max<Nd4jLong>(1, elementsPerThread);
        numThreads = nd4j::math::nd4j_min<int>(numThreads, omp_get_max_threads());
        const Nd4jLong span = (len / numThreads) + 8;

        if (xEws == 1 && zEws == 1) {
#pragma omp parallel num_threads(numThreads) if (numThreads>1) proc_bind(close) default(shared)
            {
                int tid = omp_get_thread_num();
                Nd4jLong start = span * tid;
                Nd4jLong end = span * (tid + 1);
                if (end > len)
                    end = len;
#pragma omp simd
                for (Nd4jLong i = start; i < end; i++) 
                    z[i] = func(x[i], extraParams);                        
            }
        } 
        else if(xEws == 1) {
#pragma omp parallel num_threads(numThreads) if (numThreads>1) proc_bind(close) default(shared)
            {
                int tid = omp_get_thread_num();
                Nd4jLong start = span * tid;
                Nd4jLong end = span * (tid + 1);
                if (end > len)
                    end = len;
#pragma omp simd
                for (Nd4jLong i = start; i < end; i++) 
                    z[i*zEws] = func(x[i], extraParams);                    
            }
        }
        else if(zEws == 1) {
#pragma omp parallel num_threads(numThreads) if (numThreads>1) proc_bind(close) default(shared)
            {
                int tid = omp_get_thread_num();
                Nd4jLong start = span * tid;
                Nd4jLong end = span * (tid + 1);
                if (end > len)
                    end = len;
#pragma omp simd
                for (Nd4jLong i = start; i < end; i++) 
                    z[i] = func(x[i*xEws], extraParams);                    
            }
        }
        else {
#pragma omp parallel num_threads(numThreads) if (numThreads>1) proc_bind(close) default(shared)
            {
                int tid = omp_get_thread_num();
                Nd4jLong start = span * tid;
                Nd4jLong end = span * (tid + 1);
                if (end > len)
                    end = len;
#pragma omp simd
                for (Nd4jLong i = start; i < end; i++) 
                    z[i*zEws] = func(x[i*xEws], extraParams);                    
            }
        }
    }
    else {

        if(xSimpe) {
            if(xEws == 1) {
#pragma omp parallel for schedule(guided)
                for(Nd4jLong i = 0; i < len; ++i)
                    z[shape::getIndexOffset(i, zShapeInfo, len)] = func(x[i], extraParams);                                   
            }
            else {
#pragma omp parallel for schedule(guided)
                for(Nd4jLong i = 0; i < len; ++i)
                    z[shape::getIndexOffset(i, zShapeInfo, len)] = func(x[i*xEws], extraParams);
            }
        }
        else if(zSimpe) {
            
            if(zEws == 1) {
#pragma omp parallel for schedule(guided)
                for(Nd4jLong i = 0; i < len; ++i)                    
                    z[i] = func(x[shape::getIndexOffset(i, xShapeInfo, len)], extraParams);         
            }
            else {
#pragma omp parallel for schedule(guided)
                for(Nd4jLong i = 0; i < len; ++i)
                    z[i*zEws] = func(x[shape::getIndexOffset(i, xShapeInfo, len)], extraParams);
            }
        }
        else {
#pragma omp parallel for schedule(guided)
            for(Nd4jLong i = 0; i < len; ++i)                 
                z[shape::getIndexOffset(i, zShapeInfo, len)] = func(x[shape::getIndexOffset(i, xShapeInfo, len)], extraParams);
        }                                         
    }
}
 */


#endif //NATIVEOPERATIONS_PAIRWISE_UTIL_H
