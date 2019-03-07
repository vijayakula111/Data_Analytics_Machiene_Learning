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
// Created by GS <sgazeos@gmail.com>
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_embedding_lookup)

#include <ops/declarable/CustomOperations.h>
#include <helpers/ShapeUtils.h>
#include <vector>
#include <numeric>


namespace nd4j {
namespace ops {


//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(embedding_lookup, 2, 1, false, 0, 1) {
    auto input   = INPUT_VARIABLE(0); // lookup param
    auto indeces = INPUT_VARIABLE(1); // indeces, as is
    auto output  = OUTPUT_VARIABLE(0); //

    if (block.width() > 2) { // multiple input
        indeces = INPUT_VARIABLE(block.width() - 1);
        std::vector<int> dims(input->rankOf());
        int i = output->rankOf() - input->rankOf();
        for (auto& v: dims){
            v = i++;
        }

        std::unique_ptr<ResultSet> outputView(output->allTensorsAlongDimension(dims));
        REQUIRE_TRUE(block.width() > output->sizeAt(0), 0, "embedding_lookup: input list should be greater then %i, but %i given.",
                    output->sizeAt(0), block.width()
                );
        for (Nd4jLong e = 0; e < indeces->lengthOf(); ++e) {
            Nd4jLong thisIndex = (*indeces).e<Nd4jLong>(e);
            input   = INPUT_VARIABLE(thisIndex); // lookup param

            outputView->at(e)->assign(input);
        }
    }
    else {
        int indexRank = indeces->rankOf();
        REQUIRE_TRUE(indexRank > 0, 0, "embeded_lookup: input array of indexes can't be single scalar, the requirement is: rank > 0 !");

        int inputRank = input->rankOf();
        int lastIndDim = indeces->lengthOf();
        int partition_mode = INT_ARG(0); // partition_mode == 0 - i.e. 'mod' , 1 - 'div'

        nd4j::ops::gather op;

        std::unique_ptr<ResultSet> result(op.execute({input, indeces}, {}, {0}, {}));
        REQUIRE_TRUE(result->status() == Status::OK(), 0, "embedding_lookup: cannot retrieve results from gather op.");
        REQUIRE_TRUE(result->at(0)->isSameShape(output), 0, "embedding_lookup: wrong shape of return from gather op.");
        output->assign(result->at(0));
    }
    return Status::OK();
}

DECLARE_TYPES(embedding_lookup) {
    getOpDescriptor()
            ->setAllowedInputTypes(nd4j::DataType::ANY)
            ->setAllowedOutputTypes(nd4j::DataType::ANY);
}

DECLARE_SHAPE_FN(embedding_lookup) {

    auto inShapeInfo = inputShape->at(0);
    auto indecesShapeInfo = inputShape->at(1);
    int inRank = shape::rank(inShapeInfo);
    if (inputShape->size() == 2u) {
        int outRank = inRank;

        Nd4jLong *outShapeInfo = nullptr;

        ALLOCATE(outShapeInfo, block.getWorkspace(), shape::shapeInfoLength(outRank), Nd4jLong);
        std::vector<Nd4jLong> shapeInfo(outRank);

        shapeInfo[0] = indecesShapeInfo[1]; // vector - how many elements
        for (int e = 1; e < outRank; e++)
            shapeInfo[e] = shape::sizeAt(inShapeInfo, e);
        if (shape::order(inShapeInfo) == 'c')
            shape::shapeBuffer(outRank, block.dataType(), shapeInfo.data(), outShapeInfo);
        else
            shape::shapeBufferFortran(outRank, block.dataType(), shapeInfo.data(), outShapeInfo);

        return SHAPELIST(outShapeInfo);
    }

    Nd4jLong *outShapeInfo = nullptr;
    int outRank = inRank + 1;
    ALLOCATE(outShapeInfo, block.getWorkspace(), shape::shapeInfoLength(outRank), Nd4jLong);
    std::vector<Nd4jLong> shapeInfo(outRank);
    auto indeces = INPUT_VARIABLE(block.width() - 1);
    shapeInfo[0] = indeces->lengthOf(); // vector - how many elements
    for (int e = 1; e < outRank; e++)
        shapeInfo[e] = shape::sizeAt(inShapeInfo, e);
    if (shape::order(inShapeInfo) == 'c')
        shape::shapeBuffer(outRank, block.dataType(), shapeInfo.data(), outShapeInfo);
    else
        shape::shapeBufferFortran(outRank, block.dataType(), shapeInfo.data(), outShapeInfo);
    ArrayOptions::setDataType(outShapeInfo, ArrayOptions::dataType(inShapeInfo));
    return SHAPELIST(outShapeInfo);
}




}
}

#endif