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
#if NOT_EXCLUDED(OP_get_seed)

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(get_seed, -2, 1, false, 0, 0) {
            REQUIRE_TRUE(block.getRNG() != nullptr, 0, "RNG should be defined in Graph");
            auto rng = block.getRNG();
            auto z = OUTPUT_VARIABLE(0);

            z->p(Nd4jLong(0), rng->getSeed());

            return Status::OK();
        }

        DECLARE_SHAPE_FN(get_seed) {
            auto newshape = ShapeBuilders::createScalarShapeInfo(DataType::INT64, block.workspace());

            return SHAPELIST(newshape);
        }

        DECLARE_TYPES(get_seed) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes(DataType::INT64);
        }
    }
}

#endif