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
#if NOT_EXCLUDED(OP_stop_gradient)

#include <ops/declarable/headers/parity_ops.h>

namespace nd4j {
    namespace ops {
        OP_IMPL(stop_gradient, 1, 1, true) {
            auto x = INPUT_VARIABLE(0);
            auto out = OUTPUT_VARIABLE(0);
            // just for lulz
            x->applyTransform(transform::Identity, out, nullptr);

            return Status::OK();
        }
        DECLARE_SYN(StopGradient, stop_gradient);

        DECLARE_TYPES(stop_gradient) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setSameMode(true);
        }
    }
}

#endif