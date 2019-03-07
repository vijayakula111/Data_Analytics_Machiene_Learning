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
// Created by raver119 on 20.10.2017.
//

#ifndef LIBND4J_LOGICCONDITIONAL_H
#define LIBND4J_LOGICCONDITIONAL_H

#include <pointercast.h>
#include <Node.h>
#include <Graph.h>

namespace nd4j {
    namespace graph {
        /**
         * This class is responsible for execution logic of Conditional logical abstraction
         *
         * TL/DR: Class takes 2 ops/scopes with the same number of inputs/outputs and condtion.
         * Condition is evaluated, and based on its result - one of ops/scopes is executed.
         * Results of this execution will be copied to Conditional node, and every other op
         * in the graph will be sure that it's Conditional own result, both alternative nodes will
         * stay in disguise.
         *
         * @tparam T
         */
        class LogicConditional {
        public:
            static Nd4jStatus processNode(Graph* graph, Node* node);
        };
    }
}


#endif //LIBND4J_LOGICCONDITIONAL_H
