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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 24.08.2018
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_scatter_nd_sub)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/generic/helpers/ScatterHelper.h>

namespace nd4j {
namespace ops  {

OP_IMPL(scatter_nd_sub, 3, 1, true) {
    auto input   = INPUT_VARIABLE(0);
    auto indices = INPUT_VARIABLE(1);
    auto updates = INPUT_VARIABLE(2);

    auto output = OUTPUT_VARIABLE(0);

    bool lock = block.getBArguments()->empty() ? false : B_ARG(0);

    const int inRank  = input->rankOf();
    const int indRank = indices->rankOf();
    const int updRank = updates->rankOf();

    const Nd4jLong indLastDim = indices->sizeAt(-1);
    
    REQUIRE_TRUE(indLastDim <= inRank, 0, "SCATTER_ND_SUB OP: the last dimension of indices array must be <= input_array_rank, but got %i instead !", indLastDim);
    REQUIRE_TRUE(updRank == (indRank - 1 + inRank - indLastDim), 0, "SCATTER_ND_SUB OP: the equality updates_rank = (indices_rank - 1 + input_rank - last_indices_dimension) must be true for input arrays, but got instead: updates_rank = %i, indices_rank = %i, last_indices_dimension = %i !", updRank, indRank, indLastDim);

    std::vector<Nd4jLong> inShape  = input->getShapeAsVector();
    std::vector<Nd4jLong> updShape = updates->getShapeAsVector();
    std::vector<Nd4jLong> indShape = indices->getShapeAsVector();    
    std::vector<Nd4jLong> expectedUpdShape(std::begin(indShape), std::end(indShape) - 1);     
    if(inRank > indLastDim)
        std::move(std::begin(inShape) + indLastDim, std::end(inShape), std::back_inserter(expectedUpdShape));        
    REQUIRE_TRUE(expectedUpdShape == updShape, 0, "SCATTER_ND_SUB OP: wrong shape of updates array, expected is %s, but got %s instead !", ShapeUtils::shapeAsString(expectedUpdShape).c_str(), ShapeUtils::shapeAsString(updShape).c_str());

    if (!block.isInplace())
        output->assign(input);

    ScatterHelper::scatterND(pairwise::Subtract, *indices, *updates, *output, lock);

    return Status::OK();
}

    DECLARE_TYPES(scatter_nd_sub) {
        getOpDescriptor()                
            ->setAllowedInputTypes(0, {ALL_INTS, ALL_FLOATS})
            ->setAllowedInputTypes(1, {ALL_INTS})
            ->setAllowedInputTypes(2, {ALL_INTS, ALL_FLOATS})
            ->setAllowedOutputTypes({ALL_INTS, ALL_FLOATS});
    }

}
}

#endif
