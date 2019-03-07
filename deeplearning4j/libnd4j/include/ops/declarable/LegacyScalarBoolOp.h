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
// Created by raver119 on 16.10.2017.
//

#ifndef LIBND4J_LEGACYSCALAR_BOOL_OP_H
#define LIBND4J_LEGACYSCALAR_BOOL_OP_H

#include <ops/declarable/LegacyOp.h>

namespace nd4j {
    namespace ops {
        /**
        *   This class provides wrapper for scalar transform operations, i.e. a + b = c, where either a or b is scalar primitive and other operand is NDArray
        */
        class ND4J_EXPORT LegacyScalarBoolOp : public LegacyOp {
        protected:
            Nd4jStatus validateAndExecute(Context& block);

        public:
            LegacyScalarBoolOp();
            LegacyScalarBoolOp(int opNum);
            LegacyScalarBoolOp(int opNum, NDArray &scalar);

            ShapeList* calculateOutputShape(ShapeList* inputShape, nd4j::graph::Context& block);
            virtual LegacyOp* clone();
        };
    }
}


#endif //LIBND4J_LEGACYSCALAROP_H
