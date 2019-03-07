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
// Created by raver119 on 29/10/17.
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_transpose)

#include <ops/declarable/CustomOperations.h>
#include <helpers/ShapeUtils.h>

namespace nd4j {
namespace ops {

    //////////////////////////////////////////////////////////////////////////
    CUSTOM_OP_IMPL(transpose, 1, 1, true, 0, 0) {
        auto x = INPUT_VARIABLE(0);
        if (block.width() == 1) {
            if (block.isInplace()) {
                x->transposei();
                STORE_RESULT(*x);
            } else {
                auto output = OUTPUT_VARIABLE(0);
                auto t = x->transpose();
                output->assign(t);
                STORE_RESULT(*output);
                delete t;
            }
        } else {
            // this is tf-mode transpose, that's nd4j permute
            bool replace = false;
            std::vector<int> arguments(*block.getIArguments());

            auto w = block.width();
            auto a = arguments.size();

            if (w == 2 && a == 0) {
                auto axis = INPUT_VARIABLE(1);
                for (int e = 0; e < axis->lengthOf(); e++) {
                    auto ax = axis->e<int>(e);
                    if (ax < 0)
                        ax += x->rankOf();

                    arguments.emplace_back(ax);
                }

                replace = true;
            } else if (a == 0) {
                for (int e = x->rankOf() - 1; e >= 0; e--)
                    arguments.emplace_back(e);
            }

            // 0D edge case
            if (x->rankOf() == 0) {
                REQUIRE_TRUE(arguments.size() == 1, 0, "Permute: only one axis is allowed for scalar");
                auto output = OUTPUT_VARIABLE(0);
                if (!block.isInplace())
                    output->assign(x);

                return Status::OK();
            }

            if(block.isInplace()) {		// in-place
                x->permutei(arguments);
                STORE_RESULT(x);
            } else {
                auto input = x->permute(arguments);

                auto output = OUTPUT_VARIABLE(0);
                output->assign(input);

                delete input;
             }
        }
        return Status::OK();
    }

    DECLARE_TYPES(transpose) {
        getOpDescriptor()
                ->setAllowedInputTypes(nd4j::DataType::ANY)
                ->setSameMode(true);
    }

    DECLARE_SHAPE_FN(transpose) {
        if (block.width() == 1) {
            auto outputShapeInfo = ShapeUtils::evalTranspShapeInfo(*INPUT_VARIABLE(0), block.workspace());
            return SHAPELIST(outputShapeInfo);
        } else {
            // this is basically permute mode
            auto shapeList = SHAPELIST();
            auto arguments = block.getIArguments();
            if (shape::rank(inputShape->at(0)) == 0) {
                Nd4jLong *newshape;
                ALLOCATE(newshape, block.getWorkspace(), shape::shapeInfoLength(inputShape->at(0)), Nd4jLong);
                newshape[0] = 0;
                newshape[1] = 0;
                newshape[2] = 1;
                newshape[3] = 99;
                ArrayOptions::copyDataType(newshape, inputShape->at(0));
                shapeList->push_back(newshape);
            } else if (arguments->size() > 0 || inputShape->size() > 1) {
                auto axis = arguments->size() > 0 ? *arguments : (INPUT_VARIABLE(1))->template asVectorT<int>();
                auto outputShapeInfo = ShapeUtils::evalPermShapeInfo(axis.data(), axis.size(), *INPUT_VARIABLE(0), block.workspace());
                shapeList->push_back(outputShapeInfo);
            } else if (inputShape->size() == 2) {
                // dead end
                auto axis = INPUT_VARIABLE(1);
                auto axisV = axis->template asVectorT<Nd4jLong>();
                auto newshape = ShapeUtils::evalPermShapeInfo(axisV.data(), axisV.size(), *INPUT_VARIABLE(0), block.workspace());
                shapeList->push_back(newshape);
            } else {
                int rank = shape::rank(inputShape->at(0));
                for (int e = rank - 1; e >= 0; e--)
                    arguments->emplace_back(e);

                auto outputShapeInfo = ShapeUtils::evalPermShapeInfo(arguments->data(), arguments->size(), *INPUT_VARIABLE(0), block.workspace());
                shapeList->push_back(outputShapeInfo);
            }

            return shapeList;
        }
    }
}
}

#endif