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

#ifndef DEV_TESTS_NLP_H
#define DEV_TESTS_NLP_H
#include <ops/declarable/headers/common.h>

namespace nd4j {
    namespace ops {

        #if NOT_EXCLUDED(OP_skipgram)
        DECLARE_CONFIGURABLE_OP(skipgram, 12, 12, true, 0, 0);
        #endif

        #if NOT_EXCLUDED(OP_cbow)
        DECLARE_CONFIGURABLE_OP(cbow, 14, 14, true, 0, 0);
        #endif
    }
}

#endif //DEV_TESTS_NLP_H
