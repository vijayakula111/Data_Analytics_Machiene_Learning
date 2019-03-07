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
// Created by raver119 on 13.10.2017.
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_lt_scalar)

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        BOOLEAN_OP_IMPL(lt_scalar, 2, true) {
            auto x = INPUT_VARIABLE(0);
            auto y = INPUT_VARIABLE(1);

            nd4j_printf("Comparing [%f] to [%f]\n", x->e<float>(0), y->e<float>(0));
            if (x->e<float>(0) < y->e<float>(0))
                return ND4J_STATUS_TRUE;
            else
                return ND4J_STATUS_FALSE;
        }
        //DECLARE_SYN(Less, lt_scalar);
        //DECLARE_SYN(less, lt_scalar);

        DECLARE_TYPES(lt_scalar) {
            getOpDescriptor()
                    ->setAllowedInputTypes(0, DataType::ANY)
                    ->setAllowedInputTypes(1, DataType::ANY)
                    ->setAllowedOutputTypes(0, DataType::BOOL);
        }
    }
}

#endif