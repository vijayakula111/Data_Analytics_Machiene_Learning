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
#if NOT_EXCLUDED(OP_shape)

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(shape_of, 1, 1, false, 0, 0) {
            auto x = INPUT_VARIABLE(0);
            auto z = OUTPUT_VARIABLE(0);

            for (int e = 0; e < x->rankOf(); e++)
                z->p(e, x->sizeAt(e));

            STORE_RESULT(z);

            return Status::OK();
        };
        DECLARE_SYN(shape, shape_of);

        DECLARE_SHAPE_FN(shape_of) {
            auto inShape = inputShape->at(0);

            // LONG by default
            auto dtype = DataType::INT64;
            if (block.numI() > 0)
                dtype = DataTypeUtils::fromInt(INT_ARG(0));

            return SHAPELIST(ShapeBuilders::createVectorShapeInfo(dtype, shape::rank(inShape), block.workspace()));
        };

        DECLARE_TYPES(shape_of) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_INTS});
        }
    }
}

#endif