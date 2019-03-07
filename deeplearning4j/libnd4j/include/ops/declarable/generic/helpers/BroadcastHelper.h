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

#ifndef LIBND4J_BROADCAST_HELPER_H
#define LIBND4J_BROADCAST_HELPER_H

#include <NDArray.h>
#include <helpers/ShapeUtils.h>
#include <ops/BroadcastOpsTuple.h>
#include <ops/BroadcastBoolOpsTuple.h>
#include <NDArrayFactory.h>

namespace nd4j {
    namespace ops {
        class BroadcastHelper {
        public: 
            static FORCEINLINE NDArray* broadcastApply(nd4j::BroadcastOpsTuple op, NDArray* x, NDArray* y, NDArray* z, void *extraArgs = nullptr) {
                std::unique_ptr<NDArray> ptr;
                if (!Environment::getInstance()->isExperimentalBuild()) {
                    if (y->dataType() != x->dataType()) {
                        y = y->cast(x->dataType());
                        std::unique_ptr<NDArray> ptr2(y);
                        ptr.swap(ptr2);
                    }
                }

                if (!x->isScalar() && !y->isScalar() && x->isSameShape(y)) {
				    x->applyPairwiseTransform(op.p, y, z, nullptr);
                } else if (!x->isScalar() && y->isScalar()) {
                    x->applyScalarArr(op.s, const_cast<const NDArray*>(y), z);
                } else if (x->isScalar() && !y->isScalar()) {
                    if (z->isSameShape(y)) {
                        z->assign(x);
                        z->applyPairwiseTransform(op.p, *y, extraArgs);
                        return z;
                    } else {
                        auto v = y->getShapeAsVector();
                        auto tZ = NDArrayFactory::valueOf(v, y, y->ordering());
                        tZ->applyPairwiseTransform(op.p, *y, extraArgs);
                        return tZ;
                    }
                } else if (x->isScalar() && y->isScalar()) { // x->isScalar() && y->isScalar()
				    x->applyScalarArr(op.s, const_cast<const NDArray*>(y), z, nullptr);
			    } else if (ShapeUtils::areShapesBroadcastable(*x, *y)) {
                    x->applyTrueBroadcast(op, y, z, true, extraArgs);
                    return z;
                } else {
                    auto sx = ShapeUtils::shapeAsString(x);
                    auto sy = ShapeUtils::shapeAsString(y);
                    nd4j_printf("Broadcast: shapes should be equal, or broadcastable. But got %s vs %s instead\n", sx.c_str(), sy.c_str());
                    return nullptr;
                }

                return z;
            }

            static FORCEINLINE NDArray* broadcastApply(nd4j::BroadcastBoolOpsTuple op, NDArray* x, NDArray* y, NDArray* z, void *extraArgs = nullptr) {

                if (!x->isScalar() && !y->isScalar() && x->isSameShape(y)) {
                    x->applyPairwiseTransform(op.p, y, z, nullptr);
                } else if (!x->isScalar() && y->isScalar()) {
                    x->applyScalarArr(op.s, const_cast<const NDArray*>(y), z);
                } else if (x->isScalar() && !y->isScalar()) {
                    if (z->isSameShape(y)) {
                        //z->assign(x);
                        x->applyPairwiseTransform(op.p, y, z, extraArgs);
                        return z;
                    } else {
                        auto v = y->getShapeAsVector();
                        auto tZ = NDArrayFactory::valueOf(v, y, y->ordering());
                        //tZ->applyPairwiseTransform(op.p, *y, extraArgs);
                        return tZ;
                    }
                } else if (x->isScalar() && y->isScalar()) { // x->isScalar() && y->isScalar()
                    x->applyScalarArr(op.s, const_cast<const NDArray*>(y), z, nullptr);
                } else if (ShapeUtils::areShapesBroadcastable(*x, *y)) {
                    x->applyTrueBroadcast(op, y, z, true, extraArgs);
                    return z;
                } else {
                    auto sx = ShapeUtils::shapeAsString(x);
                    auto sy = ShapeUtils::shapeAsString(y);
                    nd4j_printf("Broadcast: shapes should be equal, or broadcastable. But got %s vs %s instead\n", sx.c_str(), sy.c_str());
                    return nullptr;
                }

                return z;
            }
        };
    }
}

#endif