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

#include <ops/declarable/helpers/top_k.h>
#include <MmulHelper.h>
#include <NDArrayFactory.h>
#include <Status.h>

namespace nd4j {
namespace ops {
namespace helpers {

    template <typename T> 
    static void _swapRows(NDArray* matrix, int theFirst, int theSecond) {

        if (theFirst != theSecond)
            for (int i = 0; i < matrix->columns(); i++) {
                T _e0 = matrix->e<T>(theFirst, i);
                T _e1 = matrix->e<T>(theSecond, i);

                matrix->p<T>(theFirst, i, _e1);
                matrix->p<T>(theSecond, i, _e0);
            }
    }
    BUILD_SINGLE_TEMPLATE(template void _swapRows, (NDArray* matrix, int theFirst, int theSecond), FLOAT_TYPES);

    void swapRows(NDArray* matrix, int theFirst, int theSecond) {
        BUILD_SINGLE_SELECTOR(matrix->dataType(), _swapRows, (matrix, theFirst, theSecond), FLOAT_TYPES);
    }

    template <typename T>
    static void _invertLowerMatrix(NDArray* inputMatrix, NDArray* invertedMatrix) {
        int n = inputMatrix->rows();
        invertedMatrix->assign(0.f);
#pragma omp parallel for if(n > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int i = 0; i < n; i++)
            invertedMatrix->p(i, i, 1.0f);

        if (inputMatrix->isIdentityMatrix()) return;

#pragma omp parallel for if(n > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int i = 1; i < n; i++)
            invertedMatrix->p(i, i - 1,  -inputMatrix->e<T>(i, i - 1));

#pragma omp parallel for if(n > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int i = 2; i < n; i++) {
            for (int j = i - 2; j > -1; --j) 
                for (int k = 0; k < i; k++) 
                    invertedMatrix->p(i, j, invertedMatrix->e<T>(i, j) - (invertedMatrix->e<T>(k, j) * inputMatrix->e<T>(i, k)));
        }
    }

    BUILD_SINGLE_TEMPLATE(template void _invertLowerMatrix, (NDArray* inputMatrix, NDArray* invertedMatrix);, FLOAT_TYPES);

    void invertLowerMatrix(NDArray* inputMatrix, NDArray* invertedMatrix) {
        BUILD_SINGLE_SELECTOR(inputMatrix->dataType(), _invertLowerMatrix, (inputMatrix, invertedMatrix), FLOAT_TYPES);
    }

    template <typename T>
    static void _invertUpperMatrix(NDArray* inputMatrix, NDArray* invertedMatrix) {
        int n = inputMatrix->rows();
        invertedMatrix->setIdentity();

        if (inputMatrix->isIdentityMatrix()) { // the inverse for I is I
            return;
        }

#pragma omp parallel for if(n > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int i = 0; i < n; i++)
            invertedMatrix->p(i, i, invertedMatrix->e<T>(i, i) / inputMatrix->e<T>(i, i));

#pragma omp parallel for if(n > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int i = 0; i < n - 1; i++)
            invertedMatrix->p(i, i + 1, invertedMatrix->e<T>(i, i+1) - (inputMatrix->e<T>(i, i + 1) * invertedMatrix->e<T>(i + 1, i + 1) / inputMatrix->e<T>(i, i)));

#pragma omp parallel for if(n > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int i = n - 2; i > - 1; i--) {
            for (int j = i + 2; j < n; j++) 
                for (int k = i; k < n; k++) 
                    invertedMatrix->p(i, j, invertedMatrix->e<T>(i, j) - ((invertedMatrix->e<T>(k, j) * inputMatrix->e<T>(i, k) / inputMatrix->e<T>(i, i))));
        }
    }

    BUILD_SINGLE_TEMPLATE(template void _invertUpperMatrix, (NDArray* inputMatrix, NDArray* invertedMatrix);, FLOAT_TYPES);

    void invertUpperMatrix(NDArray* inputMatrix, NDArray* invertedMatrix) {
        BUILD_SINGLE_SELECTOR(inputMatrix->dataType(), _invertUpperMatrix, (inputMatrix, invertedMatrix), FLOAT_TYPES);
    }


    template <typename T>
    static NDArray _lup(NDArray* input, NDArray* compound, NDArray* permutation) {

        const int rowNum = input->rows();
        const int columnNum = input->columns();

        NDArray determinant = NDArrayFactory::create<T>(1.f);
        std::unique_ptr<NDArray> compoundMatrix(input->dup()); // copy
        std::unique_ptr<NDArray> permutationMatrix(input->dupUninitialized()); //put identity
        permutationMatrix->setIdentity();

        T pivotValue; // = T(0.0);
        int pivot; // = -1;
        int swapCount = 0;
//#pragma omp parallel for if(rowNum > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for(int i = 0; i < rowNum; i++ ) {
            pivotValue = T(0.0);
            pivot = -1;

            for(int rowCounter = i; rowCounter < rowNum; rowCounter++ ) {
                if(nd4j::math::nd4j_abs(compoundMatrix->e<T>(rowCounter, i)) > pivotValue ) {
                    pivotValue = nd4j::math::nd4j_abs(compoundMatrix->e<T>(rowCounter, i));
                    pivot = rowCounter;
                }
            }

            if( pivotValue != T(0.0) ) {
                swapRows(compoundMatrix.get(), pivot, i);
                swapRows(permutationMatrix.get(), pivot, i);
                if (pivot != i)
                    swapCount++;

                for( int j = i + 1; j < rowNum; j++ ) {
                    compoundMatrix->p(j, i, compoundMatrix->e<T>(j, i) / compoundMatrix->e<T>(i, i));
                    for( int k = i + 1; k < rowNum; k++ ) {
                        T arg = compoundMatrix->e<T>(j, i) * compoundMatrix->e<T>(i, k);
                        compoundMatrix->p(j, k, compoundMatrix->e<T>(j, k) - arg);
                    }
                }
            }
        }
        // nd4j_printf("Pivot: %i, Pivot value: %f.\n", pivot, pivotValue);
//#pragma omp parallel for
// if(rowNum > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int e = 0; e < rowNum; e++) {
            // nd4j_printf("Compound matrix diag %i %f.\n", e, (*compoundMatrix)(e, e));
            determinant *= compoundMatrix->e<T>(e, e);
        }
        if (swapCount % 2) determinant = -determinant;
        if (compound != nullptr)
            *compound = *compoundMatrix;
        if (permutation != nullptr)
            *permutation = *permutationMatrix;
        return determinant;
    }

    BUILD_SINGLE_TEMPLATE(template NDArray _lup, (NDArray* input, NDArray* output, NDArray* permutation), FLOAT_TYPES);



    template <typename T>
    static int _determinant(NDArray* input, NDArray* output) {

        Nd4jLong n = input->sizeAt(-1);
        Nd4jLong n2 = n * n;

        auto matrix = NDArrayFactory::create(input->ordering(), {n, n}, input->dataType(), input->getWorkspace()); //, block.getWorkspace());
//#pragma omp parallel for if(output->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int e = 0; e < output->lengthOf(); e++) {
            for (int k = e * n2, row = 0; k < (e + 1) * n2; ++k, ++row) {
                matrix.p(row, input->e<T>(k));
            }

            output->p(e, _lup<T>(&matrix, (NDArray*)nullptr, (NDArray*)nullptr));
        }

        return Status::OK();
    }

    BUILD_SINGLE_TEMPLATE(template int _determinant, (NDArray* input, NDArray* output), FLOAT_TYPES);

    int determinant(NDArray* input, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), return _determinant, (input, output), FLOAT_TYPES);
    }

template <typename T>
    int log_abs_determinant_(NDArray* input, NDArray* output) {

        Nd4jLong n = input->sizeAt(-1);
        Nd4jLong n2 = n * n;

        NDArray matrix = NDArrayFactory::create(input->ordering(), {n, n}, input->dataType(), input->getWorkspace()); //, block.getWorkspace());
//#pragma omp parallel for if(output->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (int e = 0; e < output->lengthOf(); e++) {
            for (int k = e * n2, row = 0; k < (e + 1) * n2; ++k, ++row) {
                matrix.p(row, input->e<T>(k));
            }
	    NDArray det = _lup<T>(&matrix, (NDArray*)nullptr, (NDArray*)nullptr);
	    if (det.e<T>(0) != 0.f)
             	output->p(e, nd4j::math::nd4j_log<T,T>(nd4j::math::nd4j_abs(det.t<T>(0))));
        }

        return ND4J_STATUS_OK;
    }

    BUILD_SINGLE_TEMPLATE(template int log_abs_determinant_, (NDArray* input, NDArray* output), FLOAT_TYPES);

    int log_abs_determinant(NDArray* input, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), return log_abs_determinant_, (input, output), FLOAT_TYPES);
    }

    template <typename T>
    static int _inverse(NDArray* input, NDArray* output) {

        auto n = input->sizeAt(-1);
        auto n2 = n * n;
        auto totalCount = output->lengthOf() / n2;
        
        output->assign(0.f); // fill up output tensor with zeros
        auto matrix = NDArrayFactory::create('c', {n, n}, DataTypeUtils::fromT<T>(), input->getWorkspace()); //, block.getWorkspace());
        auto compound = NDArrayFactory::create('c', {n, n}, DataTypeUtils::fromT<T>(), input->getWorkspace()); //, block.getWorkspace());
        auto permutation = NDArrayFactory::create('c', {n, n}, DataTypeUtils::fromT<T>(), input->getWorkspace());
        auto lowerMatrix = NDArrayFactory::create('c', {n, n}, DataTypeUtils::fromT<T>(), input->getWorkspace());
        auto upperMatrix = NDArrayFactory::create('c', {n, n}, DataTypeUtils::fromT<T>(), input->getWorkspace());

        for (int e = 0; e < totalCount; e++) {
            if (e)
                matrix.assign(0.f);

            for (int k = e * n2, row = 0; k < (e + 1) * n2; k++) {
                matrix.p(row++, input->e<T>(k));
            }
            T det = _lup<T>(&matrix, &compound, &permutation).template e<T>(0);

            // FIXME: and how this is going to work on float16?
            if (nd4j::math::nd4j_abs<T>(det) < T(0.0000001)) {
                nd4j_printf("matrix_inverse: The matrix %i has no inverse due determinant is %lf. Quiting...\n", e, det);
                matrix.printIndexedBuffer("Wrong matrix");
                return ND4J_STATUS_VALIDATION;
            }
            lowerMatrix.setIdentity(); // set up U to identity matrix
            for (int k = 1; k < n; k++) {  // and then put all values under main diagonal on to it
                for (int j = 0; j < k; j++)
                    lowerMatrix.p(k, j, compound.template e<T>(k, j));
            }
            upperMatrix.setIdentity(); // set up U to identity matrix
            for (int k = 0; k < n; k++) {  // and then put all values under main diagonal on to it
                for (int j = k; j < n; j++)
                    upperMatrix.p(k, j, compound.template e<T>(k, j));
            }
            invertUpperMatrix(&upperMatrix, &matrix);

            invertLowerMatrix(&lowerMatrix, &upperMatrix);

            nd4j::MmulHelper::mmul(&matrix, &upperMatrix, &compound, 1.0, 0.0);
            nd4j::MmulHelper::mmul(&compound, &permutation, &matrix, 1.0, 0.0);
            for (int k = e * n2, row = 0; k < (e + 1) * n2; k++) {
                output->p(k, matrix.template e<T>(row++));
            }
        }

        return Status::OK();
    }

    int inverse(NDArray* input, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), return _inverse, (input, output), FLOAT_TYPES);
    }

    template <typename T>
    static bool checkCholeskyInput_(NDArray const* input) {
        //std::unique_ptr<NDArray> matrix(NDArrayFactory::create_('c', {n, n}, input->dataType())); //, block.getWorkspace());
        std::unique_ptr<ResultSet> lastMatrixList(input->allTensorsAlongDimension({input->rankOf() - 2, input->rankOf()-1}));
        for (size_t i = 0; i < lastMatrixList->size(); i++) {
            auto thisMatrix = lastMatrixList->at(i);
            // check for symmetric
            for (Nd4jLong r = 0; r < thisMatrix->rows(); r++)
                for (Nd4jLong c = 0; c < thisMatrix->columns(); c++)
                    if (nd4j::math::nd4j_abs(thisMatrix->e<T>(r, c) - lastMatrixList->at(i)->e<T>(c,r)) > T(1.e-6f)) return false;

            NDArray output = NDArrayFactory::create<T>(0.);
            if (ND4J_STATUS_OK != determinant(thisMatrix, &output)) return false;
            if (output.e<T>(0) <= T(0)) return 0;
            NDArray reversedMatrix(*thisMatrix);
            if (ND4J_STATUS_OK != inverse(thisMatrix, &reversedMatrix)) return false;
            if (ND4J_STATUS_OK != determinant(&reversedMatrix, &output)) return false;
            if (output.e<T>(0) <= T(0)) return 0;

        }


        return true;
    }
    BUILD_SINGLE_TEMPLATE(template bool checkCholeskyInput_, (NDArray const* input), FLOAT_TYPES);

    bool checkCholeskyInput(NDArray const* input) {
        BUILD_SINGLE_SELECTOR(input->dataType(), return checkCholeskyInput_, (input), FLOAT_TYPES);
    }

    template <typename T>
    int cholesky_(NDArray* input, NDArray* output, bool inplace) {

        auto n = input->sizeAt(-1);
        auto n2 = n * n;
        auto totalCount = output->lengthOf() / n2;
        if (!inplace)
             output->assign(0.f); // fill up output tensor with zeros only inplace=false

        std::unique_ptr<NDArray> matrix(NDArrayFactory::create_('c', {n, n}, input->dataType())); //, block.getWorkspace());
        std::unique_ptr<NDArray> lowerMatrix(NDArrayFactory::create_('c',{n, n}, input->dataType()));

        for (int e = 0; e < totalCount; e++) {

            // fill up matrix
            for (int k = e * n2, l = 0; k < (e + 1) * n2; k++) {
                matrix->p(l++, input->e<T>(k));
            }
            //if (e) // from the second loop need to zero matrix
            lowerMatrix->assign(0.f);

            for (Nd4jLong col = 0; col < n; col++) {
                for (Nd4jLong row = 0; row < col; row++) {
                    T rowSum = 0;
                    for (Nd4jLong k = 0; k < row; ++k)
                        rowSum += (lowerMatrix->e<T>(col, k) * lowerMatrix->e<T>(row, k));
                    lowerMatrix->p(col, row, (matrix->e<T>(row, col) - rowSum) / lowerMatrix->e<double>(row, row));
                }
                T diagonalSum = 0;
                for (Nd4jLong k = 0; k < col;  ++k)
                    diagonalSum += lowerMatrix->e<T>(col, k) * lowerMatrix->e<T>(col, k);
                lowerMatrix->p(col, col, nd4j::math::nd4j_sqrt<T, T>(matrix->e<T>(col, col) - diagonalSum));
                //nd4j_printf("%i: ", col);
                //lowerMatrix->printIndexedBuffer("Lower matrix");
            }
            for (int k = e * n2, l = 0; k < (e + 1) * n2; k++) {
                output->p(k, lowerMatrix->e<T>(l++));
            }
        }

        return ND4J_STATUS_OK;
    }

    int cholesky(NDArray* input, NDArray* output, bool inplace) {
        BUILD_SINGLE_SELECTOR(input->dataType(), return cholesky_, (input, output, inplace), FLOAT_TYPES);
    }    
    BUILD_SINGLE_TEMPLATE(template int cholesky_, (NDArray* input, NDArray* output, bool inplace), FLOAT_TYPES);
    BUILD_SINGLE_TEMPLATE(template int _inverse, (NDArray* input, NDArray* output), FLOAT_TYPES);

}
}
}
