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
#if NOT_EXCLUDED(OP_reshape)

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        //////////////////////////////////////////////////////////////////////////
        // here iArgs is a vector with (optional) negative of order as first element:
        // ({-order, dim1, dim2, dim3, ...})
        CUSTOM_OP_IMPL(reshape, 1, 1, true, 0, -2) {
            auto x = INPUT_VARIABLE(0);

            if (block.width() == 1) {
                auto arguments = block.getIArguments();
                int argsSize = arguments->size();
                
                //Special case: empty.reshape(-1) -> return empty
                if (x->isEmpty()) {
                    REQUIRE_TRUE((int) arguments->size() == 1 && arguments->at(0) == -1, 0, "Reshape: when input is empty, iargs must be [-1]");
                    REQUIRE_TRUE(OUTPUT_VARIABLE(0)->isEmpty(), 0, "Reshape: when input is empty, output must also be empty");
                    return ND4J_STATUS_OK;    //No op
                }

                int e = 1;
                char order = (char) -(*arguments)[0];
                if (order != 'c' && order != 'f') {
                    order = 'c'; //x->ordering();
                    e = 0;
                }

                REQUIRE_TRUE(argsSize - e >= 1, 0, "Reshape arguments should have at least 1 dimension");

                std::vector<Nd4jLong> shapeNew;
                int e2 = e;
                for (; e < (int) arguments->size(); e++) {
                    if (arguments->at(e) == -1){
                        long shapeLength = 1;
                        for(; e2 < e; e2++){
                            shapeLength *= arguments->at(e2);
                        }
                        for(e2 = e + 1; e2 < arguments->size(); e2++){
                            shapeLength *= arguments->at(e2);
                        }
                        long realShape = x->lengthOf() / shapeLength;
                        shapeNew.push_back(realShape);
                    }
                    else{
                        shapeNew.push_back(arguments->at(e));
                    }

                }

                auto len = shape::prodLong(shapeNew.data(), shapeNew.size());
                REQUIRE_TRUE(len == x->lengthOf(), 0, "Reshape: lengths before and after reshape should match, but got %i vs %i", x->lengthOf(), len);

                if (Environment::getInstance()->isDebugAndVerbose()) {
                    nd4j_printv("Reshape: new shape", shapeNew);
                }

                if (block.isInplace()) {
                    if (x->reshapei(order, shapeNew)) {
                        STORE_RESULT(*x);
                        return ND4J_STATUS_OK;
                    }
                } else {
                    auto ret = OUTPUT_VARIABLE(0);
                    auto xr = x->reshape(order, shapeNew);
                    ret->assign(xr);
                    STORE_RESULT(*ret);
                    delete xr;
                    return ND4J_STATUS_OK;
                }
            } else if (block.width() == 2) {
                auto s = INPUT_VARIABLE(1);
                
                //Special case: empty.reshape(-1) -> return empty
                if (x->isEmpty()) {
                    REQUIRE_TRUE(s->lengthOf() == 1 && s->e<Nd4jLong>(0) == -1, 0, "Reshape: when input is empty, shape must be [-1]");
                    REQUIRE_TRUE(OUTPUT_VARIABLE(0)->isEmpty(), 0, "Reshape: when input is empty, output must also be empty");
                    return ND4J_STATUS_OK;    //No op
                }

                char order = 'c';
                if (block.numI() > 0)
                    order = (char) -INT_ARG(0);

                std::vector<Nd4jLong> shapeNew(s->lengthOf());

                for (int e = 0; e < (int) s->lengthOf(); e++) {
                    auto dim = s->e<Nd4jLong >(e);
                    if (dim == -1){
                        long shapeLength = 1;
                        for(int e2 = 0; e2 < e; e2++){
                            shapeLength *= s->e<Nd4jLong>(e2);
                        }
                        for(int e2 = e + 1; e2 < (int) s->lengthOf(); e2++){
                            REQUIRE_TRUE(s->e<Nd4jLong>(e2) != -1, 0, "Reshape : Only one unknown dimension (-1) is allowed.");
                            shapeLength *= s->e<Nd4jLong>(e2);
                        }
                        long realShape = x->lengthOf() / shapeLength;
                        shapeNew[e] = realShape;
                    }
                    else{
                        shapeNew[e] = dim;
                    }
                }

                if (Environment::getInstance()->isDebugAndVerbose()) {
                    nd4j_printv("Reshape: new shape", shapeNew);
                }

                if (block.isInplace()) {
                    if (x->reshapei(order, shapeNew)) {
                        STORE_RESULT(*x);
                        return Status::OK();
                    }
                } else {
                    auto ret = OUTPUT_VARIABLE(0);
                    if (s->isEmpty()) {
                        // just a scalar
                        ret->assign(x);
                    } else {
                        auto xr = x->reshape(order, shapeNew);
                        ret->assign(xr);
                        delete xr;
                    }

                    return Status::OK();
                }
            }

            return ND4J_STATUS_BAD_INPUT;
        }


        DECLARE_TYPES(reshape) {
            getOpDescriptor()
                    ->setAllowedInputTypes(0, nd4j::DataType::ANY)
                    ->setAllowedInputTypes(1, {ALL_INTS})
                    ->setSameMode(true);
        }

        DECLARE_SHAPE_FN(reshape) {
            auto inp = inputShape->at(0);

            // we can launch op using Int arguments
            if (inputShape->size() == 1) {
                std::vector<int> *arguments = block.getIArguments();

                int e = 1;
                char order = (char) -(*arguments)[0];
                if (order != 'c' && order != 'f') {
                    order = shape::order(inp);
                    e = 0;
                }
                
                //Special case: empty.reshape(-1) -> return empty
                if (INPUT_VARIABLE(0)->isEmpty()) {
                    REQUIRE_TRUE((int) arguments->size() == 1 && arguments->at(0) == -1, 0, "Reshape: when input is empty, iargs must be [-1]");
                    Nd4jLong *newShape = ShapeBuilders::createScalarShapeInfo(ArrayOptions::dataType(inp), block.getWorkspace());
                    ArrayOptions::setPropertyBit(newShape, ARRAY_EMPTY);
                    return SHAPELIST(newShape);
                }

                std::vector<int> shapeNew;

                int e2 = e;
                for (; e < (int) arguments->size(); e++) {
                    if ((int) arguments->at(e) == -1){

                        long shapeLength = 1;
                        for(; e2 < e; e2 ++){
                            shapeLength *= arguments->at(e2);
                        }
                        for(e2 = e + 1; e2 < arguments->size(); e2++){
                            REQUIRE_TRUE(arguments->at(e2) != -1, 0, "Reshape : Only one unknown dimension (-1) is allowed.");
                            shapeLength *= arguments->at(e2);
                        }

                        long realShape = shape::length(inp) / shapeLength;
                        shapeNew.push_back(realShape);
                    }
                    else{
                        shapeNew.push_back(arguments->at(e));
                    }
                }

                Nd4jLong *newShape;
                ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength((int) shapeNew.size()), Nd4jLong);


                newShape[0] = shapeNew.size();
                int cnt = 1;
                for (auto v: shapeNew)
                    newShape[cnt++] = v;

                shape::updateStrides(newShape, order);
                ArrayOptions::setDataType(newShape, ArrayOptions::dataType(inp));

                return SHAPELIST(newShape);
            } else {
                // or, with second input "as shape"
                auto x = INPUT_VARIABLE(0);
                auto y = INPUT_VARIABLE(1);

                // special case here
                if (y->isEmpty()) {
                    REQUIRE_TRUE(x->lengthOf() == 1, 0, "Reshape: new length doesn't match existing array");
                    return SHAPELIST(ShapeBuilders::createScalarShapeInfo(ArrayOptions::dataType(inp),block.getWorkspace()));
                }
                //Special case: empty.reshape(-1) -> return empty
                if (x->isEmpty()) {
                    REQUIRE_TRUE(y->lengthOf() == 1 && y->e<Nd4jLong>(0) == -1, 0, "Reshape: when input is empty, shape must be [-1]");
                    Nd4jLong *newShape = ShapeBuilders::createScalarShapeInfo(ArrayOptions::dataType(inp), block.getWorkspace());
                    ArrayOptions::setPropertyBit(newShape, ARRAY_EMPTY);
                    return SHAPELIST(newShape);
                }

                std::vector<Nd4jLong> shapeNew(y->lengthOf());

                for (int e = 0; e < (int) y->lengthOf(); e++) {
                    auto dim = y->e<Nd4jLong>(e);
                    if (dim == -1){
                        long shapeLength = 1;
                        for(int e2 = 0; e2 < e; e2++){
                            shapeLength *= y->e<Nd4jLong>(e2);
                        }
                        for(int e2 = e + 1; e2 < (int)y->lengthOf(); e2++){
                            REQUIRE_TRUE(y->e<Nd4jLong>(e2) != -1, 0, "Reshape : Only one unknown dimension (-1) is allowed.");
                            shapeLength *= y->e<Nd4jLong>(e2);
                        }
                        long realShape = shape::length(inp) / shapeLength;
                        shapeNew[e] = realShape;
                    }else {
                        shapeNew[e] = dim;
                    }
                }


                Nd4jLong *newShape;
                ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength(shapeNew.size()), Nd4jLong);

                shape::shapeBuffer(shapeNew.size(), ArrayOptions::dataType(inp), shapeNew.data(), newShape);

                return SHAPELIST(newShape);
            }
        }
    }
}

#endif