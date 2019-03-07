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

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_crop_and_resize)

//#include <ops/declarable/headers/parity_ops.h>
#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/image_resize.h>
namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(crop_and_resize, 4, 1, false, 0, 0) {

            auto image = INPUT_VARIABLE(0);
            auto boxes = INPUT_VARIABLE(1);
            auto boxIndexes = INPUT_VARIABLE(2);
            
            auto output = OUTPUT_VARIABLE(0);
            int width;
            int height;
            int method = 0; // bilinear
            double extrapolationVal = 0.;

            auto newImageSize = INPUT_VARIABLE(3);
            REQUIRE_TRUE(newImageSize->lengthOf() == 2, 0, "crop_and_resize: Resize params is a pair of values, not %i.", newImageSize->lengthOf());
            //REQUIRE_TRUE(block.numI() <= 1, 0, "crop_and_resize: Resize params already given by the second param. Int params are expensive.");
            //width = int(newImageSize->getScalar(0));
            //height = int(newImageSize->getScalar(1));
            if (block.numI() == 1) {
                method = INT_ARG(0);
            }

            if (block.numT() == 1) {
                extrapolationVal = T_ARG(0);
            }

            helpers::cropAndResizeFunctor(image, boxes, boxIndexes, newImageSize, method, extrapolationVal, output);
            return ND4J_STATUS_OK;
        }

        DECLARE_SHAPE_FN(crop_and_resize) {
            auto shapeList = SHAPELIST(); 
            auto in = inputShape->at(0);

            Nd4jLong* outputShape;

            int width;
            int height;
            auto newImageSize = INPUT_VARIABLE(3);
            REQUIRE_TRUE(newImageSize->lengthOf() == 2, 0, "crop_and_resize: Resize params is a pair of values, not %i.", newImageSize->lengthOf());
            //REQUIRE_TRUE(block.numI() <= 1, 0, "crop_and_resize: Resize params already given by the second param. Int params are expensive.");
            width = newImageSize->e<int>(0);
            height = newImageSize->e<int>(1);
            
            ALLOCATE(outputShape, block.getWorkspace(), shape::shapeInfoLength(4), Nd4jLong);
            outputShape[0] = 4;
            outputShape[1] = in[1];
            outputShape[2] = width;
            outputShape[3] = height;
            outputShape[4] = in[4];
            shape::updateStrides(outputShape, shape::order(in));
            ArrayOptions::setDataType(outputShape, ArrayOptions::dataType(in));
            shapeList->push_back(outputShape); 
            return shapeList;
        }
        DECLARE_TYPES(crop_and_resize) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }
    }
}

#endif
