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
// Created by Yurii Shyrma on 18.12.2017
//


#include <ops/declarable/helpers/householder.h>
#include <ops/declarable/helpers/biDiagonalUp.h>
#include <NDArrayFactory.h>


namespace nd4j {
namespace ops {
namespace helpers {


//////////////////////////////////////////////////////////////////////////
BiDiagonalUp::BiDiagonalUp(const NDArray& matrix): _HHmatrix(nd4j::NDArrayFactory::create(matrix.ordering(), {matrix.sizeAt(0), matrix.sizeAt(1)}, matrix.dataType(), matrix.getWorkspace())),
                                                         _HHbidiag(nd4j::NDArrayFactory::create(matrix.ordering(), {matrix.sizeAt(1), matrix.sizeAt(1)}, matrix.dataType(), matrix.getWorkspace())) {

	// input validation
	if(matrix.rankOf() != 2 || matrix.isScalar())
		throw std::runtime_error("ops::helpers::biDiagonalizeUp constructor: input array must be 2D matrix !");

	_HHmatrix.assign(&matrix);
	_HHbidiag.assign(0.);
	
	evalData();

}

	template <typename T>
	void BiDiagonalUp::_evalData() {

		const auto rows = _HHmatrix.sizeAt(0);
		const auto cols = _HHmatrix.sizeAt(1);

		if(rows < cols)
			throw std::runtime_error("ops::helpers::BiDiagonalizeUp::evalData method: this procedure is applicable only for input matrix with rows >= cols !");

		NDArray* bottomRightCorner(nullptr), *column(nullptr), *row(nullptr);
		T coeff, normX;

		T _x, _y;

		for(int i = 0; i < cols-1; ++i ) {

			// evaluate Householder matrix nullifying columns
			column = _HHmatrix.subarray({{i,   rows}, {i, i+1}});

            _x = _HHmatrix.e<T>(i,i);
            _y = _HHbidiag.e<T>(i,i);

			Householder<T>::evalHHmatrixDataI(*column, _x, _y);

            _HHmatrix.p<T>(i, i, _x);
            _HHbidiag.p<T>(i, i, _y);

			// multiply corresponding matrix block on householder matrix from the left: P * bottomRightCorner
			bottomRightCorner =  _HHmatrix.subarray({{i, rows}, {i+1, cols}});	// {i, cols}
			Householder<T>::mulLeft(*bottomRightCorner, _HHmatrix({i+1,rows, i,i+1}, true), _HHmatrix.e<T>(i,i));

			delete bottomRightCorner;
			delete column;

			if(i == cols-2)
				continue; 										// do not apply right multiplying at last iteration

			// evaluate Householder matrix nullifying rows
			row  = _HHmatrix.subarray({{i, i+1}, {i+1, cols}});


            _x = _HHmatrix.e<T>(i,i+1);
            _y = _HHbidiag.e<T>(i,i+1);

			Householder<T>::evalHHmatrixDataI(*row, _x, _y);

            _HHmatrix.p<T>(i, i+1, _x);
            _HHbidiag.p<T>(i, i+1, _y);

			// multiply corresponding matrix block on householder matrix from the right: bottomRightCorner * P
			bottomRightCorner = _HHmatrix.subarray({{i+1, rows}, {i+1, cols}});  // {i, rows}

			Householder<T>::mulRight(*bottomRightCorner, _HHmatrix({i,i+1, i+2,cols}, true), _HHmatrix.e<T>(i,i+1));

			delete bottomRightCorner;
			delete row;
		}

		row  = _HHmatrix.subarray({{cols-2, cols-1}, {cols-1, cols}});

		_x = _HHmatrix.e<T>(cols-2,cols-1);
		_y = _HHbidiag.e<T>(cols-2,cols-1);

		Householder<T>::evalHHmatrixDataI(*row, _x, _y);

        _HHmatrix.p<T>(cols-2,cols-1, _x);
        _HHbidiag.p<T>(cols-2,cols-1, _y);

		delete row;

		column = _HHmatrix.subarray({{cols-1, rows}, {cols-1, cols}});

		_x = _HHmatrix.e<T>(cols-1,cols-1);
		_y = _HHbidiag.e<T>(cols-1,cols-1);

		Householder<T>::evalHHmatrixDataI(*column, _x, _y);

		_HHmatrix.p<T>(cols-1, cols-1, _x);
        _HHbidiag.p<T>(cols-1, cols-1, _y);

		delete column;
	}

//////////////////////////////////////////////////////////////////////////
void BiDiagonalUp::evalData() {
	auto xType = _HHmatrix.dataType();

	BUILD_SINGLE_SELECTOR(xType, _evalData, ();, FLOAT_TYPES);
}


//////////////////////////////////////////////////////////////////////////
template <typename T>
HHsequence BiDiagonalUp::makeHHsequence_(const char type) const {

	if(type == 'u') {

    	const int diagSize = _HHbidiag.sizeAt(0);
    	auto colOfCoeffs = NDArrayFactory::create(_HHmatrix.ordering(),  {diagSize, 1}, _HHmatrix.dataType(), _HHmatrix.getWorkspace());

	    for(int i = 0; i < diagSize; ++i)
	        colOfCoeffs.p(i, _HHmatrix.e<T>(i,i));

    	return HHsequence(_HHmatrix, colOfCoeffs, type);
    }
    else {

    	const int diagUpSize = _HHbidiag.sizeAt(0) - 1;
		NDArray colOfCoeffs = NDArrayFactory::create(_HHmatrix.ordering(), {diagUpSize, 1}, _HHmatrix.dataType(), _HHmatrix.getWorkspace());

    	for(int i = 0; i < diagUpSize; ++i)
        	colOfCoeffs.p(i, _HHmatrix.e<T>(i,i+1));

    	HHsequence result(_HHmatrix, colOfCoeffs, type);
    	result._diagSize = diagUpSize;
    	result._shift  = 1;

    	return result;
    }
}

	HHsequence BiDiagonalUp::makeHHsequence(const char type) const {
		auto xType = _HHmatrix.dataType();

		BUILD_SINGLE_SELECTOR(xType, return makeHHsequence_, (type);, FLOAT_TYPES);
	}



BUILD_SINGLE_TEMPLATE(template void BiDiagonalUp::_evalData, (), FLOAT_TYPES);
BUILD_SINGLE_TEMPLATE(template HHsequence BiDiagonalUp::makeHHsequence_, (const char type) const, FLOAT_TYPES);

}
}
}