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
// Created by GS <sgazeos@gmail.com> 31.01.2018
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_zero_fraction)

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(zero_fraction, 1, 1, false, 0, 0) {
            auto input = INPUT_VARIABLE(0);
            auto output = OUTPUT_VARIABLE(0);

            REQUIRE_TRUE(output->isScalar(), 0, "Rank output should be scalar");
            int numZeros = 0;
//            for (int e = 0; e < input->lengthOf(); e++)
//                if ((*input)(e) == T(0))
//                    numZeros++;
            auto countZero = input->reduceNumber(reduce::CountZero);
            //nd4j_printf("Zero count is %f for %i elements.", countZero.e<double>(0), input->lengthOf());
            //countZero /= double(input->lengthOf());
            output->p<double>(0, countZero.e<Nd4jLong>(0) / double(input->lengthOf())); //printIndexedBuffer("Zero count");

            return Status::OK();
        }
        DECLARE_SHAPE_FN(zero_fraction) {
            return SHAPELIST(ShapeBuilders::createScalarShapeInfo(nd4j::DataType::DOUBLE, block.workspace()));
        }

        DECLARE_TYPES(zero_fraction) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }
    }
}

#endif