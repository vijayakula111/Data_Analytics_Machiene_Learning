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
//#include <ops/declarable/helpers/reduce_product.h>
#include <ops/declarable/helpers/legacy_helpers.h>

namespace nd4j {
namespace ops {
namespace helpers {

    template <typename T>
    static void reduceNorm2BP_scalar_(NDArray *input, NDArray *epsilon, NDArray *tempNorm, NDArray *output) {
        T eps = epsilon->e<T>(0);
        T n2 = tempNorm->e<T>(0);
        auto norm2Backprop = LAMBDA_T(_x, eps, n2) {
            return eps * _x / n2;
        };
        input->applyLambda<T>(norm2Backprop, output);
    }
    BUILD_SINGLE_TEMPLATE(template void reduceNorm2BP_scalar_, (NDArray *input, NDArray *epsilon, NDArray *tempNorm, NDArray *output), FLOAT_TYPES);


    void reduceNorm2BP_scalar(NDArray *input, NDArray *epsilon, NDArray *tempNorm, NDArray *output) {
        auto xType = epsilon->dataType();

        BUILD_SINGLE_SELECTOR(xType, reduceNorm2BP_scalar_, (input, epsilon, tempNorm, output), FLOAT_TYPES);
    }

    void reduceNorm1BP(NDArray* input, NDArray* epsilon, NDArray* tempNorm, NDArray* output, std::vector<int> const& axes, bool keepDims) {

        if (epsilon->isScalar()) {
#pragma omp parallel for
            for (Nd4jLong e = 0; e < input->lengthOf(); ++e)
                if (input->e<float>(e) > 0.f)
                    output->p(e, epsilon->e<double>(0));
                else
                    output->p(e, -epsilon->e<double>(0));
            return;
        }

        std::vector<int> dimensions; //(input->rankOf() - axes.size());
        for (Nd4jLong e = 0; e < input->rankOf(); e++) {
            if (std::find(axes.begin(), axes.end(), e) == axes.end()) {
                dimensions.emplace_back(e);
            }
        }
        std::unique_ptr<ResultSet> outList(output->allTensorsAlongDimension(dimensions));
        std::unique_ptr<ResultSet> inList(input->allTensorsAlongDimension(dimensions));
        for (int e = 0; e < outList->size(); ++e) {
            //inList->at(e)->applyPairwiseTransform(pairwise::ReduceNorm1E, epsilon, outList->at(e), nullptr);
            helpers::reduceNorm1(inList->at(e), epsilon, outList->at(e));
        }
    }

    void reduceNorm2BP(NDArray* input, NDArray* epsilon, NDArray* tempNorm, NDArray* output, std::vector<int> const& axes, bool keepDims) {

        std::vector<int> dimensions; //(input->rankOf() - axes.size());
        for (Nd4jLong e = 0; e < input->rankOf(); e++) {
            if (std::find(axes.begin(), axes.end(), e) == axes.end()) {
                dimensions.emplace_back(e);
            }
        }
        std::unique_ptr<ResultSet> outList(output->allTensorsAlongDimension(dimensions));
        std::unique_ptr<ResultSet> inList(input->allTensorsAlongDimension(dimensions));
        for (int e = 0; e < outList->size(); ++e) {
            epsilon->applyPairwiseTransform(pairwise::Multiply, inList->at(e), outList->at(e), nullptr);
            outList->at(e)->applyPairwiseTransform(pairwise::Divide, tempNorm, outList->at(e), nullptr);
        }
    }

    void reduceSquareNormBP(NDArray* input, NDArray* epsilon, NDArray* tempNorm, NDArray* output, std::vector<int> const& axes, bool keepDims) {

        std::vector<int> dimensions; //(input->rankOf() - axes.size());
        for (Nd4jLong e = 0; e < input->rankOf(); e++) {
            if (std::find(axes.begin(), axes.end(), e) == axes.end()) {
                dimensions.emplace_back(e);
            }
        }
        std::unique_ptr<ResultSet> outList(output->allTensorsAlongDimension(dimensions));
        std::unique_ptr<ResultSet> inList(input->allTensorsAlongDimension(dimensions));
        for (int e = 0; e < outList->size(); ++e) {
            auto outputS = outList->at(e);
            auto inputS = inList->at(e);
            outputS->assign(2.f);
            *outputS *= *epsilon;
            *outputS *= *inputS;
        }
    }
}
}
}