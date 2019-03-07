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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 01.11.2017.
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_stack)

#include <ops/declarable/CustomOperations.h>
#include<ops/declarable/helpers/stack.h>

namespace nd4j {
namespace ops {

CUSTOM_OP_IMPL(stack, -1, 1, false, 0, 0) {
	auto input = INPUT_VARIABLE(0);
	auto output = OUTPUT_VARIABLE(0);
	int dim  = block.getIArguments()->size() > 0 ? INT_ARG(0) : 0;
	if(dim < 0)
 		dim += input->rankOf() + 1;
	
	// input validation
	// check whether shapes of all input array are the same				
	for (int i = 0; i < (int) block.width() - 1; ++i)
		REQUIRE_TRUE(shape::equalsSoft((INPUT_VARIABLE(i))->getShapeInfo(), (INPUT_VARIABLE(i+1))->getShapeInfo()), 0, "STACK op: the shapes of all input arrays must be the same !");
 	
	REQUIRE_TRUE(dim <= input->rankOf(), 0, "STACK op: the input dimension parameter must be <= rank of input arrays shapes (rank=%i), but got %i instead !", input->shapeOf(), dim);

 	
 	std::vector<NDArray*> inArrs(block.width());
 	for(int i = 0; i < block.width(); ++i)
		inArrs[i] = INPUT_VARIABLE(i);
	
	helpers::stack(inArrs, *output, dim);
	
	// remove unity from output shape if input arrays are vectors 
	// if(input->isVector())	{
	// 	std::vector<int> outShape(output->shapeOf(), output->shapeOf() + output->rankOf());		
	// 	outShape.erase(find(outShape.begin(), outShape.end(), 1));
	// 	output->reshapei(output->ordering(), outShape);
	// 	if(dim != 0 && (int)block.width() == 1)			// such is implemented by tensorFlow
	// 		output->permutei({1, 0});
	// 	output->getShapeInfo()[output->rankOf()*2 + 2] = 1;		
	// }
  	
  	return Status::OK();
}
DECLARE_SYN(pack, stack);
DECLARE_SYN(Pack, stack);

	DECLARE_TYPES(stack) {
		//getOpDescriptor()->setSameMode(true);
		getOpDescriptor()
		    ->setAllowedInputTypes(DataType::ANY)
		    ->setAllowedOutputTypes(DataType::ANY);

	}

DECLARE_SHAPE_FN(stack) {
	
	// check whether input dimension is within rank range
	auto inShapeInfo = inputShape->at(0);
	int rank = shape::rank(inShapeInfo);
    int dim  = block.getIArguments()->size() > 0 ? INT_ARG(0) : 0;
    if(dim < 0 )
		dim += rank + 1;

	 REQUIRE_TRUE(dim <= inShapeInfo[0], 0, "STACK op: the input dimension parameter must be <= rank of input arrays shapes (rank=%i), but got %i instead !", inShapeInfo[0], dim);
	
	if(rank == 0) {
  		return SHAPELIST(ShapeBuilders::createVectorShapeInfo(ArrayOptions::dataType(inShapeInfo), block.width(), block.workspace()));
	}
	
	//the rank of output ShapeInfo is larger by one compared to input ShapeInfo
	std::vector<Nd4jLong> outShape(inShapeInfo + 1, inShapeInfo + 1 + rank);
	
	// insert (int) block.width() at dim position of input shape to get output shape	
	outShape.insert(outShape.begin() + Nd4jLong(dim), (Nd4jLong) block.width());
	
	// evaluate output ShapeInfo
	int newRank = outShape.size();
	Nd4jLong* outShapeInfo = nullptr;
  	ALLOCATE(outShapeInfo, block.getWorkspace(), shape::shapeInfoLength(newRank), Nd4jLong);
  	outShapeInfo[0] = newRank;
  	
  	for(int i=1; i <= newRank; ++i)
  		outShapeInfo[i] = outShape[i-1];
  	
  	shape::updateStrides(outShapeInfo, shape::order(inShapeInfo));
  	ArrayOptions::setDataType(outShapeInfo, ArrayOptions::dataType(inShapeInfo));

  	
  	return SHAPELIST(outShapeInfo);
}

// 1) 1х4 + 1х4 = 2х1х4 (along dim=0) = 2x4 
// 2) 1х4 + 1х4 = 1х2х4 (along dim=1) = 2x4 
// 3) 4х1 + 4х1 = 2х4x1 (along dim=0) = 2x4 
// 4) 4х1 + 4х1 = 4х2x1 (along dim=1) = 4x2 

}
}

#endif