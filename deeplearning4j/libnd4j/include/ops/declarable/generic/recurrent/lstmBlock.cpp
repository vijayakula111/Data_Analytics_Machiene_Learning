/*******************************************************************************
 * Copyright (c) 2015-2019 Skymind, Inc.
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

// lstmBlock: Full LSTM layer in one op
// @author Alex Black
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_lstmBlock)

#include <ops/declarable/CustomOperations.h>
#include<ops/declarable/helpers/lstmBlock.h>

namespace nd4j {
namespace ops  {


//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(lstmBlock, 9, 7, false, 2, 2) {
    auto maxTSLength = INPUT_VARIABLE(0);
    auto x     = INPUT_VARIABLE(1);                   // input [seqLen, bS, inSize] at time t
    auto cLast = INPUT_VARIABLE(2);                   // previous cell state  [bS, numUnits], time t-1
    auto yLast = INPUT_VARIABLE(3);                   // previous output [bS, numUnits], time t-1

    auto W    = INPUT_VARIABLE(4);                    // Weights - concatenated (input-to-hidden, hidden-to-hidden weights)  weights, [(inSize+numUnits), 4*numUnits]
    auto Wci  = INPUT_VARIABLE(5);                    // weights - cell peephole (t-1) connections to input modulation gate, [numUnits]
    auto Wcf  = INPUT_VARIABLE(6);                    // weights - cell peephole (t-1) connections to forget gate, [numUnits]
    auto Wco  = INPUT_VARIABLE(7);                    // weights - cell peephole (t) connections to output gate, [numUnits]
    auto b    = INPUT_VARIABLE(8);                    // biases, [4*numUnits]

    auto i   =  OUTPUT_VARIABLE(0);                   // Output - input modulation gate activations [seqLen, bS, numUnits]
    auto c   =  OUTPUT_VARIABLE(1);                      // Activations, cell state (pre tanh) [seqLen, bs, numUnits]
    auto f   =  OUTPUT_VARIABLE(2);                   // Output - forget gate activations [seqLen, bs, numUnits]
    auto o   =  OUTPUT_VARIABLE(3);                   // Output - output gate activations [seqLen, bs, numUnits]
    auto z   =  OUTPUT_VARIABLE(4);                   // Output - input gate activations [seqLen, bs, numUnits]
    auto h   =  OUTPUT_VARIABLE(5);                   // Cell state, post tanh [seqLen, bs, numUnits]
    auto y   =  OUTPUT_VARIABLE(6);                   // current cell output [seqLen, bS, numProj], time t
    
    const int peephole   = INT_ARG(0);                     // if 1, provide peephole connections
    const int dataFormat = INT_ARG(1);                     // 0=TNS=[seqLen,mb,size]; 1=NST=[mb,size,seqLen]; 2=NTS=[mb,seqLen,size]
    const double forgetBias   = T_ARG(0);
    const double clippingCellValue  = T_ARG(1);       // clipping value for ct, if it is not equal to zero, then cell state is clipped

    REQUIRE_TRUE(x->rankOf()==3, 0, "lstmBlock: Input array 1 (x) rank must be got input with rank %i", x->rankOf());
    REQUIRE_TRUE(cLast->rankOf()==2 && yLast->rankOf()==2, 0, "lstmBlock: Input ranks must be 2 for inputs 2/3 (cLast, yLast) - got %i, %i", cLast->rankOf(), yLast->rankOf());
    REQUIRE_TRUE(W->rankOf()==2, 0, "lstmBlock: Weights array rank must be 2");
    REQUIRE_TRUE(b->rankOf()==1, 0, "lstmBlock: Biases must be rank 1");
    REQUIRE_TRUE(i->rankOf()==3 && c->rankOf()==3 && f->rankOf()==3 && o->rankOf()==3 && z->rankOf()==3 && h->rankOf()==3 && y->rankOf()==3,
                 0, "lstmBlock: Output arrays must all be rank 3");

    helpers::lstmBlockTimeLoop(maxTSLength, x, cLast, yLast,   W, Wci, Wcf, Wco, b,    i, c, f, o, z, h, y, {(double)peephole, forgetBias, clippingCellValue}, dataFormat);

    return Status::OK();
}

DECLARE_TYPES(lstmBlock) {
    getOpDescriptor()
            ->setAllowedInputTypes(nd4j::DataType::ANY)
            ->setAllowedOutputTypes({ALL_FLOATS});
}


DECLARE_SHAPE_FN(lstmBlock) {
    auto x     = inputShape->at(1);
    auto cLast = inputShape->at(2);
    auto yLast = inputShape->at(3);
    auto W     = inputShape->at(4);
    auto b     = inputShape->at(8);

    REQUIRE_TRUE(shape::rank(x)==3, 0, "lstmBlock: Input array 1 (x) rank must be got input with rank %i", shape::rank(x));
    REQUIRE_TRUE(shape::rank(cLast)==2 && shape::rank(yLast)==2, 0, "lstmBlock: Input ranks must be 2 for inputs 2/3 (cLast, yLast) - got %i, %i", shape::rank(cLast), shape::rank(yLast));
    REQUIRE_TRUE(shape::rank(W)==2, 0, "lstmBlock: Weights array rank must be 2");
    REQUIRE_TRUE(shape::rank(b)==1, 0, "lstmBlock: Biases must be rank 1");


    const int dataFormat = INT_ARG(1);                     // 0=TNS=[seqLen,mb,size]; 1=NST=[mb,size,seqLen]; 2=NTS=[mb,seqLen,size]
    int bs;
    int t;
    int nOut = cLast[2];    //rank, bs, nOut, ...]

    Nd4jLong *s(nullptr);
    ALLOCATE(s, block.getWorkspace(), shape::shapeInfoLength(3), Nd4jLong);      // [time, bS, nOut]
    s[0] = 3;
    if(dataFormat == 0){
        //[rank, seqLen, bs, nIn, ...]
        s[1] = x[1];    //seqLen
        s[2] = x[2];    //bS
        s[3] = nOut;
    } else if(dataFormat==1){
        //[rank, bs, nIn, seqLen, ...]
        s[1] = x[1];    //bS
        s[2] = nOut;
        s[3] = x[3];    //seqLen
    } else {
        //[rank, bs, seqLen, nIn, ...]
        s[1] = x[1];    //bS
        s[2] = x[2];    //seqLen
        s[3] = nOut;

    }
    ShapeUtils::updateStridesAndType(s, x, 'c');

    //7 outputs, all same shape/type
    return SHAPELIST(s, s, s, s, s, s, s);
}   

}
}

#endif