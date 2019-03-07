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
// @author raver119@gmail.com, created on 29/10/17.
// @author Yurii Shyrma (iuriish@yahoo.com)
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_ismax)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/ismax.h>

namespace nd4j {
namespace ops  {

CONFIGURABLE_OP_IMPL(ismax, 1, 1, true, 0, -1) {
                
    auto x = INPUT_VARIABLE(0);
    auto z = OUTPUT_VARIABLE(0);
    auto dimensions = *(block.getIArguments());			// argI
    helpers::ismax(x, z, dimensions);

    return Status::OK();
}
DECLARE_SYN(IsMax, ismax);

    DECLARE_TYPES(ismax) {
        getOpDescriptor()
                ->setAllowedInputTypes(0, DataType::ANY)
                ->setAllowedOutputTypes(0, DataType::BOOL);

    }

}
}

#endif