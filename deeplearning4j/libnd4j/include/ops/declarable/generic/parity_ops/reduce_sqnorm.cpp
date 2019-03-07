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
// Created by george@skymind.io on 6/4/2018.
//

#include <ops/declarable/helpers/reduce_norm.h>
#include <ops/declarable/helpers/axis.h>
#include <ops/declarable/CustomOperations.h>

namespace nd4j {
namespace ops {
#if NOT_EXCLUDED(OP_reduce_sqnorm)

    CUSTOM_OP_IMPL(reduce_sqnorm, 1, 1, false, 0, 0) {
        auto input = INPUT_VARIABLE(0);
        auto output = OUTPUT_VARIABLE(0);
        auto axes = *block.getIArguments();
        if (block.width() > 1) {
            auto axesVector = INPUT_VARIABLE(1);
            helpers::adjustAxis(input, axesVector, axes);
        }
//            else if (block.getIArguments()->size())
        bool keepDims = false;
        if (block.getBArguments()->size())
            keepDims = B_ARG(0);
        else if (block.getTArguments()->size())
            keepDims = (bool)T_ARG(0);

        for(const auto& item : axes)
            REQUIRE_TRUE(item > -input->shapeInfo()[0] || item <input->shapeInfo()[0], 0, "REDUCE_MEAN OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , input->rankOf(), input->rankOf(), item);

        input->reduceAlongDimension(reduce::SquaredNorm, output, axes, keepDims);

        return Status::OK();
    }

    DECLARE_SHAPE_FN(reduce_sqnorm) {

        auto axes = *block.getIArguments();
        if (block.width() > 1) {
            auto axesVector = INPUT_VARIABLE(1);
            helpers::adjustAxis(INPUT_VARIABLE(0), axesVector, axes);
        }
//            else if (block.getIArguments()->size())
        bool keepDims = false;
        if (block.getBArguments()->size())
            keepDims = B_ARG(0);
        else if (block.getTArguments()->size())
            keepDims = (bool)T_ARG(0);

        Nd4jLong* outShapeInfo = ShapeUtils::evalReduceShapeInfo(shape::order(inputShape->at(0)), axes, inputShape->at(0), keepDims, false, block.getWorkspace());
        //ArrayOptions::setDataType(outShapeInfo, ArrayOptions::dataType(inputShape->at(0)));

        return SHAPELIST(outShapeInfo);
    }

        DECLARE_TYPES(reduce_sqnorm) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }
#endif 
#if NOT_EXCLUDED(OP_reduce_sqnorm_bp)

    DECLARE_SHAPE_FN(reduce_sqnorm_bp) {    

        Nd4jLong* outShapeInfo;// = ShapeUtils::evalReduceShapeInfo(shape::order(inputShape->at(0)), dimensions, inputShape->at(0), keepDims, false, block.getWorkspace());
        COPY_SHAPE(inputShape->at(0), outShapeInfo);

        return SHAPELIST(outShapeInfo);
    }

        DECLARE_TYPES(reduce_sqnorm_bp) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }

    CUSTOM_OP_IMPL(reduce_sqnorm_bp, 2, 1, false, 0, 0) {

            auto input = INPUT_VARIABLE(0);
            auto epsilon = INPUT_VARIABLE(1);
            auto output = OUTPUT_VARIABLE(0);

            if (epsilon->isScalar()) {
                output->assign(epsilon->e<double>(0) * 2.);
                *output *= *input;
                //output->applyPairwiseTransform(pairwise::Multiply, input, output, nullptr);
            }
            else {
                auto axes = *block.getIArguments();
                if (block.width() > 2) {
                    auto axesVector = INPUT_VARIABLE(2);
                    helpers::adjustAxis(input, axesVector, axes);
                }
//            else if (block.getIArguments()->size())
                bool keepDims = false;
                if (block.getBArguments()->size())
                    keepDims = B_ARG(0);
                else if (block.getTArguments()->size())
                    keepDims = (bool)T_ARG(0);

                helpers::reduceSquareNormBP(input, epsilon, (NDArray*)nullptr, output, axes, keepDims);
            }
            return Status::OK();
    }
#endif

}
}
