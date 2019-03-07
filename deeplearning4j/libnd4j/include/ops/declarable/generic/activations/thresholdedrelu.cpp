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
// @author Yurii Shyrma, created on 24.07.2018
//


#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_thresholdedrelu)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/legacy_helpers.h>
#include <ops/declarable/helpers/activations.h>
namespace nd4j {
namespace ops  {


////////////////////////////////////////////////////////////////////////
CONFIGURABLE_OP_IMPL(thresholdedrelu, 1, 1, true, 0, 0) {
    auto input  = INPUT_VARIABLE(0);
    auto output = OUTPUT_VARIABLE(0);

    auto scalar = block.numT() > 0 ? block.getTArguments()->at(0) : 0.0;

    helpers::thresholdRelu(*input, scalar, *output);

    return Status::OK();
}

    DECLARE_TYPES(thresholdedrelu) {
            getOpDescriptor()
                    ->setAllowedInputTypes(0, DataType::ANY)
                    ->setSameMode(true);
    }

////////////////////////////////////////////////////////////////////////
CONFIGURABLE_OP_IMPL(thresholdedrelu_bp, 2, 1, true, 0, 0) {
    auto input = INPUT_VARIABLE(0);
    auto dLdO  = INPUT_VARIABLE(1);    
    
    auto dLdI = OUTPUT_VARIABLE(0);
    auto threshold = block.numT() > 0 ? block.getTArguments()->at(0) : 0.0;

    helpers::thresholdReluDerivative(input, threshold, dLdO, dLdI);

    return Status::OK();
}

        DECLARE_TYPES(thresholdedrelu_bp) {
            getOpDescriptor()
                    ->setAllowedInputTypes(0, DataType::ANY)
                    ->setAllowedInputTypes(1, {DataType::FLOAT32, DataType ::DOUBLE, DataType::HALF})
                    ->setAllowedOutputTypes(0, {DataType::FLOAT32, DataType ::DOUBLE, DataType::HALF});
        }


}
}

#endif