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
//  xw_plus_b op. Created by GS <george@skymind.io> 31.01.2018
//
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_xw_plus_b)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/matmul.h>
#include <MmulHelper.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(xw_plus_b, 3, 1, false, 0, 0) {
            auto x = INPUT_VARIABLE(0);
            auto y = INPUT_VARIABLE(1);
            auto b = INPUT_VARIABLE(2);
            auto z = OUTPUT_VARIABLE(0);

            REQUIRE_TRUE(x->rankOf() <= 2 && y->rankOf() <= 2 && z->rankOf() <= 2, 0, "xw_plus_b: Input and Output NDArrays should have rank less or equal to 2");
            REQUIRE_TRUE(b->isVector() && b->lengthOf() == z->sizeAt(-1), 0, "xw_plus_b: Input vector should have proper dimension 1x%i. "
                "But %i != %i.", z->sizeAt(-1), b->lengthOf(), z->sizeAt(-1));
            // multiply x to y
            MmulHelper::mmul(x, y, z, 1.0, 0.0);

            // adding b vector
            z->addiRowVector(b);

            return Status::OK();
        }

        DECLARE_SHAPE_FN(xw_plus_b) {
            auto outputShape = ShapeUtils::matrixProductShape(inputShape->at(0), inputShape->at(1), false, false,
                    ArrayOptions::dataType(inputShape->at(0)), block.getWorkspace());
            
            return SHAPELIST(outputShape);
        }

        DECLARE_TYPES(xw_plus_b) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }
    }
}

#endif