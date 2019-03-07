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
//  @author GS <sgazeos@gmail.com>
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_dynamic_partition)

#include <ops/declarable/CustomOperations.h>
#include <array>
#include <ops/declarable/helpers/dynamic.h>

namespace nd4j {
namespace ops {
    CUSTOM_OP_IMPL(dynamic_partition, 2, 1, false, 0, 1) {
        auto input = INPUT_VARIABLE(0);
        auto indices = INPUT_VARIABLE(1);

        // input->printShapeInfo("input");
        // indices->printShapeInfo("indices");

        REQUIRE_TRUE(input->rankOf() >= indices->rankOf(), 0,
                     "dynamic_partition: data tensor rank should be non-lesser than indices\' tensor, but %i < %i given,",
                     input->rankOf(), indices->rankOf());
        for (int dim = 0; dim < indices->rankOf(); dim++) {
            REQUIRE_TRUE(input->sizeAt(dim) == indices->sizeAt(dim), 0,
                         "dynamic_partition: dimensions should be equals for data and indices tensors, but at axis[%i] %i != %i given",
                         dim, input->sizeAt(dim), indices->sizeAt(dim));
        }

        auto numPartition = INT_ARG(0);
        std::vector<NDArray *> outputList(numPartition);
        for (int o = 0; o < numPartition; ++o) {
            outputList[o] = OUTPUT_VARIABLE(o);
        }
        helpers::dynamicPartitionFunctor(input, indices, outputList);

        return Status::OK();
    }

    DECLARE_SHAPE_FN(dynamic_partition) {
        auto numPartition = INT_ARG(0);
        auto indices = INPUT_VARIABLE(1);
        std::vector<int> partitionSizes(numPartition, 0);
        auto in = inputShape->at(0);
        auto idx = inputShape->at(1);
        for (int i = 0; i < numPartition; i++) {
            for (int e = 0; e < indices->lengthOf(); ++e)
                if (indices->e<Nd4jLong>(e) == i)
                    partitionSizes[i]++;
        }

        auto shapes = SHAPELIST();
        int outRank = shape::rank(in) - shape::rank(idx) + 1;
        for (int e = 0; e < numPartition; e++) {
            Nd4jLong *newShape;
            ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength(outRank), Nd4jLong);
            //shape::shapeVector(partitionSizes[e], newShape);
            newShape[0] = outRank;
            newShape[1] = partitionSizes[e];
            for (int i = 1; i < outRank; ++i)
                newShape[i + 1] = shape::sizeAt(in, outRank + i - 1);

            shape::updateStrides(newShape, shape::order(in));
            ArrayOptions::setDataType(newShape, ArrayOptions::dataType(in));
            shapes->push_back(newShape);
        }

        return shapes;
    }

    DECLARE_TYPES(dynamic_partition) {
        getOpDescriptor()
                ->setAllowedInputTypes(nd4j::DataType::ANY)
                ->setAllowedOutputTypes({ALL_FLOATS, ALL_INTS});
    }

    DECLARE_TYPES(dynamic_partition_bp) {
        getOpDescriptor()
                ->setAllowedInputTypes(nd4j::DataType::ANY)
                ->setSameMode(true);
    }

    CUSTOM_OP_IMPL(dynamic_partition_bp, 3, 2, false, 0, 1) {
        auto input = INPUT_VARIABLE(0);
        auto indices = INPUT_VARIABLE(1);
        //auto gradOut = ;
        auto numPartition = INT_ARG(0);

        std::vector<NDArray*> outputList(2); // only for output
        std::vector<NDArray*> gradOutList(numPartition);
        for (Nd4jLong e = 0; e < numPartition; e++) {
            gradOutList[e] = INPUT_VARIABLE(e + 2);
        }
        outputList[0] = OUTPUT_VARIABLE(0);
        outputList[1] = OUTPUT_VARIABLE(1);

        helpers::dynamicPartitionFunctorBP(input, indices, gradOutList, outputList);

        return ND4J_STATUS_OK;
    }

    DECLARE_SHAPE_FN(dynamic_partition_bp) {
        auto numPartition = INT_ARG(0);
        auto indices = INPUT_VARIABLE(1);
        std::vector<int> partitionSizes(numPartition, 0);

        auto shapes = SHAPELIST();
        // just copy shape info from input and indices to output
        for (Nd4jLong i = 0; i < 2; i++) {
            Nd4jLong *newShape;
            COPY_SHAPE(inputShape->at(i), newShape);
            shapes->push_back(newShape);
        }

        return shapes;
    }
}
}

#endif