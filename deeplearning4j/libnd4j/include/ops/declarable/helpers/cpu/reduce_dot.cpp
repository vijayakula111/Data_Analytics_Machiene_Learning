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
//  @author sgazeos@gmail.com
//

#include <ResultSet.h>
#include <ops/declarable/helpers/reduce_dot.h>

namespace nd4j {
namespace ops {
namespace helpers {

    void reduceDotBP(NDArray* inputX, NDArray* inputY, NDArray* epsilon, NDArray* outputX, NDArray* outputY, std::vector<int> const& axes) {
//                std::unique_ptr<ResultSet<T>> outList(output->allTensorsAlongDimension(dimensions));
        std::vector<int> dimensions; //(input->rankOf() - axes.size());
        for (Nd4jLong e = 0; e < inputX->rankOf(); e++) {
            if (std::find(axes.begin(), axes.end(), e) == axes.end()) {
                   dimensions.emplace_back(e);
            }
        }
        std::unique_ptr<ResultSet> outListX(outputX->allTensorsAlongDimension(dimensions));
        std::unique_ptr<ResultSet> outListY(outputY->allTensorsAlongDimension(dimensions));
        std::unique_ptr<ResultSet> yList(inputY->allTensorsAlongDimension(dimensions));
        std::unique_ptr<ResultSet> xList(inputX->allTensorsAlongDimension(dimensions));
                //output->
#pragma omp parallel for if (outListX->size() > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (Nd4jLong e = 0; e < outListX->size(); ++e) {
            outListX->at(e)->assign(epsilon);
            outListX->at(e)->applyPairwiseTransform(pairwise::Multiply, yList->at(e), outListX->at(e), nullptr);
        }

#pragma omp parallel for if (outListY->size() > Environment::getInstance()->elementwiseThreshold()) schedule(static)
        for (Nd4jLong e = 0; e < outListY->size(); ++e) {
            outListY->at(e)->assign(epsilon);
            outListY->at(e)->applyPairwiseTransform(pairwise::Multiply, xList->at(e), outListY->at(e), nullptr);
        }
    }

}
}
}