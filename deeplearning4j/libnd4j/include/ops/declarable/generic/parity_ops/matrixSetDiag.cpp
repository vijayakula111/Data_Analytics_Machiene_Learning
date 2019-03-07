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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 07.12.2017
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_matrix_set_diag)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/matrixSetDiag.h>

namespace nd4j {
namespace ops  {

CONFIGURABLE_OP_IMPL(matrix_set_diag, 2, 1, false, 0, 0) {
    auto input    = INPUT_VARIABLE(0);
    auto diagonal = INPUT_VARIABLE(1);

    auto output   = OUTPUT_VARIABLE(0);

    REQUIRE_TRUE(diagonal->rankOf() == input->rankOf()-1, 0, "MATRIX_SET_DIAG op: rank of diagonal array must be smaller by one compared to rank of input array, but got %i and %i correspondingly !", diagonal->rankOf(), input->rankOf());

    for(int i = 0;  i < diagonal->rankOf() - 1; ++i)
        REQUIRE_TRUE(diagonal->sizeAt(i) == input->sizeAt(i), 0, "MATRIX_SET_DIAG op: the shapes of diagonal and input arrays must be equal till last diagonal dimension but one, however got diagonal=%s and input=%s instead !", ShapeUtils::shapeAsString(diagonal).c_str(), ShapeUtils::shapeAsString(input).c_str());

    REQUIRE_TRUE(diagonal->sizeAt(-1) == (int)nd4j::math::nd4j_min<Nd4jLong>(input->sizeAt(-1), input->sizeAt(-2)),
        0, "MATRIX_SET_DIAG op: the value of last dimension of diagonal array must be equal to min(input_last_shape=%i, input_last_but_one_shape=%i), but got %i instead !", input->sizeAt(-1), input->sizeAt(-2), diagonal->sizeAt(-1));

    helpers::matrixSetDiag(input, diagonal, output);

    return Status::OK();
}

DECLARE_SYN(MatrixSetDiag, matrix_set_diag);

    DECLARE_TYPES(matrix_set_diag) {
        getOpDescriptor()
                ->setAllowedInputTypes(nd4j::DataType::ANY)
                ->setSameMode(true);
    }

}
}

#endif