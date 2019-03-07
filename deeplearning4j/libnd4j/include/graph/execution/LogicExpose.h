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
// Created by raver119 on 12.11.2017.
//

#ifndef LIBND4J_LOGICEXPOSE_H
#define LIBND4J_LOGICEXPOSE_H

#include <pointercast.h>
#include <graph/Node.h>
#include <graph/Graph.h>

namespace nd4j {
    namespace graph {
        class LogicExpose {
        public:
            static Nd4jStatus processNode(Graph* graph, Node* node);
        };
    }
}



#endif //LIBND4J_LOGICEXPOSE_H
