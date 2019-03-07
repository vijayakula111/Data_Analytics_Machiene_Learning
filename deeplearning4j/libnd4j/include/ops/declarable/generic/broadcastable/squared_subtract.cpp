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
// Created by raver119 on 23.11.17.
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_squaredsubtract)

#include <ops/declarable/generic/helpers/BroadcastHelper.h>
#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        BROADCASTABLE_OP_IMPL(squaredsubtract, 0, 0) {
            auto x = INPUT_VARIABLE(0);
            auto y = INPUT_VARIABLE(1);
            auto z = OUTPUT_VARIABLE(0);

            BROADCAST_CHECK_EMPTY(x,y,z);

            auto tZ = BroadcastHelper::broadcastApply(BROADCAST(SquaredSubtract), x, y, z);
            if (tZ == nullptr)
                return ND4J_STATUS_KERNEL_FAILURE;
            else if (tZ != z) {
                OVERWRITE_RESULT(tZ);
            }

            return Status::OK();
        }
        DECLARE_SYN(squareddifference, squaredsubtract);

        DECLARE_TYPES(squaredsubtract) {
            getOpDescriptor()
                    ->setAllowedInputTypes(0, DataType::ANY)
                    ->setAllowedInputTypes(1, DataType::ANY)
                    ->setAllowedOutputTypes(0, DataType::INHERIT);
        }

        CUSTOM_OP_IMPL(squaredsubtract_bp, 3, 2, false, 0, 0) {
            auto x = INPUT_VARIABLE(0);
            auto y = INPUT_VARIABLE(1);
            auto epsNext = INPUT_VARIABLE(2);

            auto gradX = OUTPUT_VARIABLE(0);
            auto gradY = OUTPUT_VARIABLE(1);

            /*
            auto lambdaX = LAMBDA_TTT(_e, _x, _y) {
                return _e * (T) 2.0 * (_x - _y) ;
            };

            auto lambdaY = LAMBDA_TTT(_e, _x, _y) {
                return _e * (T) 2.0 * (_y - _x);
            };
            */

            auto ts = NDArrayFactory::create(x->dataType(), 2, block.workspace());


            if (x->isSameShape(y)) {
                // PWT case case

                // X gradient
                //epsNext->applyTriplewiseLambda(x, y, lambdaX, gradX);
                gradX->assign((*epsNext) * ts * ((*x) - (*y)));

                // Y gradient
                //epsNext->applyTriplewiseLambda(x, y, lambdaY, gradY);
                gradY->assign((*epsNext) * ts * ((*y) - (*x)));

            } else if (y->isScalar()) {
                // scalar case
                auto tmpX = x->reduceNumber(reduce::Sum);
                gradY->assign(tmpX);
                
                //epsNext->applyPairwiseLambda(x, lambdaS, gradX);
                gradX->assign(epsNext * ts * ((*x) - (*y)));
            } else {
                // broadcast case

                auto preX = x->dup();
                auto preY = y->dup();

                auto targetShape = epsNext->getShapeAsVector();

                preX->tileToShape(targetShape);
                preY->tileToShape(targetShape);


                //epsNext->applyTriplewiseLambda(x, y, lambdaX, preX);
                //epsNext->applyTriplewiseLambda(x, y, lambdaY, preY);
                auto resX = (*epsNext) * ts * ((*x) - (*y));
                preX->assign(resX);
                auto resY = (*epsNext) * ts * ((*y) - (*x));
                preY->assign(resY);

                auto axisX = ShapeUtils::evalBroadcastBackwardAxis(x->shapeInfo(), epsNext->shapeInfo());
                auto axisY = ShapeUtils::evalBroadcastBackwardAxis(y->shapeInfo(), epsNext->shapeInfo());

                if (axisX.size() > 0) {
                    auto sum = preX->reduceAlongDimension(reduce::Sum, axisX);
                    gradX->assign(sum);
                    delete sum;
                } else 
                    gradX->assign(preX);

                if (axisY.size() > 0) {
                    auto sum = preY->reduceAlongDimension(reduce::Sum, axisY);
                    gradY->assign(sum);
                    delete sum;
                } else
                    gradY->assign(preY);


                delete preX;
                delete preY;
            }

            return Status::OK();
        }

        DECLARE_SHAPE_FN(squaredsubtract_bp) {
            auto x = inputShape->at(0);
            auto y = inputShape->at(1);
            auto e = inputShape->at(2);

            // eps always has shape of x
            // grad always has shape of y

            Nd4jLong *shapeE;
            Nd4jLong *shapeG;

            COPY_SHAPE(x, shapeE);
            COPY_SHAPE(y, shapeG);

            auto shapeList = SHAPELIST(shapeE, shapeG);

            return shapeList;
        }

        DECLARE_TYPES(squaredsubtract_bp) {
            getOpDescriptor()
                    ->setAllowedInputTypes(DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }

    }
}

#endif