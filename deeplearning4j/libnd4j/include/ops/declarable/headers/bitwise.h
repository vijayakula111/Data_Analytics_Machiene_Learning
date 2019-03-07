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

#ifndef LIBND4J_HEADERS_BITWISE_H
#define LIBND4J_HEADERS_BITWISE_H

#include <ops/declarable/headers/common.h>

namespace nd4j {
    namespace ops {
        /**
         * This operation toggles individual bits of each element in array
         * 
         * PLEASE NOTE: This operation is possible only on integer datatypes
         * 
         * @tparam T
         */
        #if NOT_EXCLUDED(OP_toggle_bits)
        DECLARE_OP(toggle_bits, -1, -1, true);
        #endif
    }
}

#endif