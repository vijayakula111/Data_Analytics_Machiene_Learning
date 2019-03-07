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
// @author raver119@gmail.com
//

#include "testlayers.h"
#include <Context.h>
#include <iomanip>
#include <Variable.h>
#include <VariableSpace.h>
#include <ops/declarable/OpRegistrator.h>
#include <ops/declarable/CustomOperations.h>
#include <helpers/helper_hash.h>
#include <NDArray.h>
#include <array/NDArrayList.h>
#include <NativeOps.h>
#include <ops/gemm.h>

using namespace nd4j;
using namespace nd4j::graph;

class DeclarableOpsTests1 : public testing::Test {
public:

    const int bS = 2;       // batch size
    const int iD = 1;       // input depth (number of picture channels, for example rgb=3)
    const int iH = 28;      // picture height in pixels
    const int iW = 28;      // picture width in pixels
    const int oD = 3;       // output depth (= N for dense layer)
    const int kH = 5;       // kernel height in pixels
    const int kW = 5;       // kernel width in pixels
    const int sH = 1;       // stride step in horizontal direction
    const int sW = 1;       // stride step in vertical direction
    const int pH = 0;       // padding height
    const int pW = 0;       // padding width
    const int dH = 2;       // dilation height
    const int dW = 2;       // dilation width
    const int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     // output height
    const int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;     // output width

    DeclarableOpsTests1() {
        printf("\n");
    }
};

template <typename T>
class TypedDeclarableOpsTests1 : public testing::Test {
public:

    const int bS = 2;       // batch size
    const int iD = 1;       // input depth (number of picture channels, for example rgb=3)
    const int iH = 28;      // picture height in pixels
    const int iW = 28;      // picture width in pixels
    const int oD = 3;       // output depth (= N for dense layer)
    const int kH = 5;       // kernel height in pixels
    const int kW = 5;       // kernel width in pixels
    const int sH = 1;       // stride step in horizontal direction
    const int sW = 1;       // stride step in vertical direction
    const int pH = 0;       // padding height
    const int pW = 0;       // padding width
    const int dH = 2;       // dilation height
    const int dW = 2;       // dilation width
    const int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     // output height
    const int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;     // output width

    TypedDeclarableOpsTests1() {
        printf("\n");
    }
};

typedef ::testing::Types<double, float> TestingTypes;
TYPED_TEST_CASE(TypedDeclarableOpsTests1, TestingTypes);

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, BasicInitialization1) {
    auto concat = new nd4j::ops::concat();
    std::string expName("concat");
    ASSERT_EQ(expName, *(concat->getOpName()));

    auto x0 = NDArrayFactory::create_<float>('c', {1, 5});
    auto x1 = NDArrayFactory::create_<float>('c', {1, 5});
    auto x2 = NDArrayFactory::create_<float>('c', {1, 5});
    auto x3 = NDArrayFactory::create_<float>('c', {1, 5});
    auto x4 = NDArrayFactory::create_<float>('c', {1, 5});

    x0->assign(1.0f);
    x1->assign(1.0f);
    x2->assign(1.0f);
    x3->assign(1.0f);
    x4->assign(1.0f);

    auto variableSpace = new VariableSpace();

    variableSpace->putVariable(-1, x0);
    variableSpace->putVariable(-2, x1);
    variableSpace->putVariable(-3, x2);
    variableSpace->putVariable(-4, x3);
    variableSpace->putVariable(-5, x4);

    auto nodeVar = new Variable();

    variableSpace->putVariable(1, nodeVar);

    Context block(1, variableSpace);
    block.getIArguments()->push_back(1);
    block.fillInputs({-1, -2, -3, -4, -5});

    ASSERT_FALSE(nodeVar->hasNDArray());

    Nd4jStatus result = concat->execute(&block);

    ASSERT_TRUE(nodeVar->hasNDArray());

    ASSERT_EQ(25, nodeVar->getNDArray()->lengthOf());

    ASSERT_NEAR(25.0, nodeVar->getNDArray()->reduceNumber(reduce::Sum).e<double>(0), 1e-5);

    ASSERT_EQ(ND4J_STATUS_OK, result);


    delete variableSpace;
    delete concat;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, BasicInitialization2) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation("concat");

    ASSERT_TRUE(op != nullptr);
    std::string expName("concat");
    ASSERT_EQ(expName, *(op->getOpName()));

    ASSERT_EQ(-1, op->getOpDescriptor()->getNumberOfInputs());
    ASSERT_EQ(1, op->getOpDescriptor()->getNumberOfOutputs());
}


TEST_F(DeclarableOpsTests1, BasicInitialization3) {
    auto op1 = nd4j::ops::OpRegistrator::getInstance()->getOperation("concat");
    std::string expName("concat");
    auto hash = nd4j::ops::HashHelper::getInstance()->getLongHash(expName);

    auto op2 = nd4j::ops::OpRegistrator::getInstance()->getOperation(hash);

    ASSERT_TRUE(op1 == op2);
}


TEST_F(DeclarableOpsTests1, SynonymInitialization2) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation("Mul");
    auto op2 = nd4j::ops::OpRegistrator::getInstance()->getOperation("multiply");

    ASSERT_TRUE(op != nullptr);
    std::string expName("multiply");
    ASSERT_EQ(expName, *(op->getOpName()));
    ASSERT_TRUE(op == op2);
}


TEST_F(DeclarableOpsTests1, TestTensorMmul1) {
    auto x = NDArrayFactory::create_<float>('c', {2, 3, 4});
    auto y = NDArrayFactory::create_<float>('c', {2, 3, 4});

    for (int i = 0; i < x->lengthOf(); i++) {
        x->p(i, i + 1);
        y->p(i, i + 1);
    }

    auto exp = NDArrayFactory::create<float>('c', {2, 2});
    exp.p(0, 650.0);
    exp.p(1, 1586.0);
    exp.p(2, 1586.0);
    exp.p(3, 4250.0);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(1, new Variable());
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2}); 
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);

    nd4j::ops::tensormmul tm;

    tm.execute(block);

    auto z = variableSpace->getVariable(1)->getNDArray();

    //z->printBuffer("Result: ");

    ASSERT_TRUE(exp.equalsTo(z));

    delete block;
    delete variableSpace;
}

TEST_F(DeclarableOpsTests1, TestTensorDot2) {
    auto x = NDArrayFactory::create_<float>('f', {2, 3, 4});
    auto y = NDArrayFactory::create_<float>('f', {2, 3, 4});

    for (int i = 0; i < x->lengthOf(); i++) {
        // x->p(i, i + 1);
        // y->p(i, i + 1);
        reinterpret_cast<float*>(x->getBuffer())[i] = i + 1;
        reinterpret_cast<float*>(y->getBuffer())[i] = i + 1;
    }

    NDArray exp('c', {2, 2}, nd4j::DataType::FLOAT32);
    exp.p(0, 2300.0);
    exp.p(1, 2444.0);
    exp.p(2, 2444.0);
    exp.p(3, 2600.0);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(1, new Variable());
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2});
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);

    nd4j::ops::tensormmul tm;

    tm.execute(block);

    auto z = variableSpace->getVariable(1)->getNDArray();

    //z->printBuffer("Result: ");

    ASSERT_TRUE(exp.equalsTo(z));

    delete block;
    delete variableSpace;
}

TEST_F(DeclarableOpsTests1, TestTensorDot3) {
    auto x = NDArrayFactory::create_<float>('c', {2, 3, 4});
    auto y = NDArrayFactory::create_<float>('f', {2, 3, 4});

    for (int i = 0; i < x->lengthOf(); i++) {
        x->p(i, i + 1);
        // y->p(i, i + 1);
        reinterpret_cast<float*>(y->getBuffer())[i] = i + 1;
    }

    auto exp = NDArrayFactory::create_<float>('f', {2, 2});   
    reinterpret_cast<float*>(exp->getBuffer())[0] = 1090.0;
    reinterpret_cast<float*>(exp->getBuffer())[1] = 2818.0;
    reinterpret_cast<float*>(exp->getBuffer())[2] = 1168.0;
    reinterpret_cast<float*>(exp->getBuffer())[3] = 3040.0;

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(1, new Variable());
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2});
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);

    nd4j::ops::tensormmul tm;

    tm.execute(block);

    auto z = variableSpace->getVariable(1)->getNDArray();

    // z->printIndexedBuffer("Result: ");

    ASSERT_TRUE(exp->equalsTo(z));

    delete exp;
    delete block;
    delete variableSpace;
}

TEST_F(DeclarableOpsTests1, TestTensorDot4) {
    auto x =  NDArrayFactory::create_<float>('f', {2, 3, 4});
    auto y =  NDArrayFactory::create_<float>('c', {2, 3, 4});

    for (int i = 0; i < x->lengthOf(); i++) {
        // x->p(i, i + 1);
        reinterpret_cast<float*>(x->getBuffer())[i] = i + 1;
        y->p(i, i + 1);
    }

    auto exp = NDArrayFactory::create_<float>('f', {2, 2});
    // exp->p(0, 1090.0);
    // exp->p(1, 1168.0);
    // exp->p(2, 2818.0);
    // exp->p(3, 3040.0);
    reinterpret_cast<float*>(exp->getBuffer())[0] = 1090.0;
    reinterpret_cast<float*>(exp->getBuffer())[1] = 1168.0;
    reinterpret_cast<float*>(exp->getBuffer())[2] = 2818.0;
    reinterpret_cast<float*>(exp->getBuffer())[3] = 3040.0;

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(1, new Variable());
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2});
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);

    nd4j::ops::tensormmul tm;

    tm.execute(block);

    auto z = variableSpace->getVariable(1)->getNDArray();

    //z->printBuffer("Result: ");

    ASSERT_TRUE(exp->equalsTo(z));

    delete exp;
    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, DivergentCheck1) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperation("switch");

    ASSERT_TRUE(op != nullptr);
    std::string expName("Switch");
    ASSERT_EQ(expName, *(op->getOpName()));
    ASSERT_TRUE(op->getOpDescriptor()->isDivergent());
    ASSERT_EQ(2, op->getOpDescriptor()->getNumberOfOutputs());
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, AddMatrices1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto y = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto exp = NDArrayFactory::create_<float>('c', {5, 3});
    x->assign(2);
    y->assign(1);
    exp->assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::add addOp;
 
    addOp.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));  

    delete exp;
    delete block;
    delete variableSpace;

}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, AddVectorVector1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {1, 15});
    auto y = NDArrayFactory::create_<float>  ('c', {1, 15});
    auto exp = NDArrayFactory::create_<float>('c', {1, 15});
    x->assign(2);
    y->assign(1);
    exp->assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::add addOp;
 
    addOp.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));  

    delete exp;
    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, AddMatrixScalar1) {

    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(2);
    y->assign(1);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::add addOp;
 
    addOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, AddScalarScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {1, 1});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 1});
    x->assign(2);
    y->assign(1);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::add addOp;
 
    addOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, SubtractMatrices1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {5, 3});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(3);
    y->assign(1);
    exp.assign(2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::subtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));


    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, SubtractTest_1) {

    auto x = NDArrayFactory::create_<float>('c', {1, 6});
    auto y = NDArrayFactory::create_<float>('c', {1, 6});
    auto exp = NDArrayFactory::create<float>('c', {1, 6});
    x->assign(3);
    y->assign(1);
    exp.assign(2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::subtract subOp;

    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));


    delete variableSpace;
    delete block;
}
//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, SubtractTest_2) {

    auto x = NDArrayFactory::create<float>('c', {3, 4, 5, 1});
    auto y = NDArrayFactory::create<float>('c', {1, 6});
//    auto y({6}, {1,1,1,1,1,1});
    auto exp = NDArrayFactory::create<float>('c', {3, 4, 5, 6});
    x.assign(3);
    y.assign(1);
    exp.assign(2);


    nd4j::ops::subtract subOp;

    auto res = subOp.execute({&x, &y}, {}, {});

    ASSERT_TRUE(res->status() == ND4J_STATUS_OK);

    ASSERT_TRUE(res->at(0)->equalsTo(&exp));

    delete res;
}

TEST_F(DeclarableOpsTests1, TestRng1) {
/*
    Nd4jLong *buffer = new Nd4jLong[100000];

    NativeOps nativeOps;

    nd4j::random::RandomBuffer *rng = (nd4j::random::RandomBuffer *) nativeOps.initRandom(nullptr, 123, 100000, (Nd4jPointer) buffer);

    if (rng == nullptr)
        throw std::runtime_error("RNG initialization failed");

    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1});
    block->setRNG(rng);
    block->getTArguments()->push_back(0.0f);
    block->getTArguments()->push_back(1.0f);

    nd4j::ops::randomuniform uniform;

    Nd4jStatus status = uniform.execute(block);

    ASSERT_EQ(ND4J_STATUS_OK, status);

    ASSERT_TRUE(x->sumNumber() > 0.0);

    nativeOps.destroyRandom((Nd4jPointer) rng);
    delete[] buffer;

    delete variableSpace;
    delete block;
    */
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, MergeSumTest1) {

    auto x = NDArrayFactory::create_<float>('c', {5, 5});
    auto y = NDArrayFactory::create_<float>('c', {5, 5});
    auto z = NDArrayFactory::create_<float>('c', {5, 5});
    auto exp = NDArrayFactory::create<float>('c', {5, 5});
    x->assign(3);
    y->assign(1);
    z->assign(2);
    exp.assign(6);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(-3, z);
    variableSpace->putVariable(1, new Variable(NDArrayFactory::create_<float>('c', {5, 5})));
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2, -3});

    nd4j::ops::mergeadd merge;

    merge.execute(block);

    auto res = variableSpace->getVariable(1)->getNDArray();

    ASSERT_TRUE(res->equalsTo(&exp));

    delete variableSpace;
    delete block;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ClipByValue1) {

    auto x = NDArrayFactory::create_<float>('c', {5, 5});
    auto exp = NDArrayFactory::create<float>('c', {5, 5});
    x->assign(4);
    x->p(0, -1);
    x->p(1, 2);
    exp.assign(3);
    exp.p(0, 0);
    exp.p(1, 2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, new Variable());
    auto block = new Context(1, variableSpace, true);
    block->getTArguments()->push_back(0.0f);
    block->getTArguments()->push_back(3.0f);
    block->fillInputs({-1});

    nd4j::ops::clipbyvalue clip;

    clip.execute(block);

    x->printIndexedBuffer("Result");
    exp.printIndexedBuffer("Expect");
    ASSERT_TRUE(x->equalsTo(&exp));


    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, MergeMaxTest1) {

    auto x = NDArrayFactory::create_<float>('c', {5, 5});
    auto y = NDArrayFactory::create_<float>('c', {5, 5});
    auto z = NDArrayFactory::create_<float>('c', {5, 5});
    auto exp = NDArrayFactory::create<float>('c', {5, 5});
    x->assign(3);
    y->assign(1);
    z->assign(2);
    exp.assign(3);

    auto zu = NDArrayFactory::create<float>('c', {5, 5});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(-3, z);
    variableSpace->putVariable(1, new Variable(NDArrayFactory::create_<float>('c', {5, 5})));
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2, -3});

    nd4j::ops::mergemax merge;

    merge.execute(block);

    auto res = variableSpace->getVariable(1)->getNDArray();
    
    ASSERT_TRUE(res->equalsTo(&exp));

    delete block;
    delete variableSpace;

}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, SubtractVectorVector1) {
    
    auto x = NDArrayFactory::create_<float>('c', {1, 15});
    auto y = NDArrayFactory::create_<float>('c', {1, 15});
    auto exp = NDArrayFactory::create<float>('c', {1, 15});
    x->assign(3);
    y->assign(1);
    exp.assign(2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::subtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp)); 

    delete block;
    delete variableSpace;

}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, SubtractMatrixScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(3);
    y->assign(1);
    exp.assign(2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::subtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp)); 

    delete block;
    delete variableSpace;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, SubtractScalarScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {1, 1});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 1});
    x->assign(3);
    y->assign(1);
    exp.assign(2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::subtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp)); 

    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractMatrices1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {5, 3});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(3.f);
    y->assign(1.f);
    exp.assign(-2.f);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversesubtract subOp;
 
    subOp.execute(block);
    x->printIndexedBuffer("Output Subtract");
    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractTest_1) {

    auto x = NDArrayFactory::create<float>('c', {1, 6});
    auto y = NDArrayFactory::create<float>('c', {1, 6});
    auto exp = NDArrayFactory::create<float>('c', {1, 6});
    x.assign(3.f);
    y.assign(1.f);
    exp.assign(-2.f);

    nd4j::ops::reversesubtract subOp;

    auto res = subOp.execute({&x, &y}, {}, {});

    ASSERT_TRUE(res->status() == ND4J_STATUS_OK);
    ASSERT_TRUE(res->at(0)->equalsTo(&exp));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractTest_2) {

//    auto x('c', {1, 6});
    auto x = NDArrayFactory::create<float>('c', {1, 6});
    auto y = NDArrayFactory::create<float>('c', {3, 4, 5, 1});
    auto exp = NDArrayFactory::create<float>('c', {3, 4, 5, 6});
    auto z(exp);
    x.assign(3.f);
    y.assign(1.f);
    exp.assign(-2.f);
    x.applyTrueBroadcast(BROADCAST(ReverseSubtract), &y, &z, true);
//    x.printIndexedBuffer("ReverseSubtract Legacy");
    ASSERT_TRUE(exp.equalsTo(&z));

    nd4j::ops::reversesubtract subOp;

    auto res = subOp.execute({&x, &y}, {}, {});

    ASSERT_TRUE(res->status() == ND4J_STATUS_OK);
    //res->at(0)->printIndexedBuffer("OUtput REVERSED SUB");
    ASSERT_TRUE(res->at(0)->equalsTo(&exp));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractTest_3) {

//    auto x('c', {1, 6});
    auto x = NDArrayFactory::create<float>('c', {6});
    auto y = NDArrayFactory::create<float>('c', {3, 4, 5, 1});
    auto exp = NDArrayFactory::create<float>('c', {3, 4, 5, 6});
    auto z(exp);
    x.assign(1);
    y.assign(3);
    exp.assign(2);
    x.applyTrueBroadcast(BROADCAST(ReverseSubtract), &y, &z, true);
    ASSERT_TRUE(z.equalsTo(&exp));
    nd4j::ops::reversesubtract subOp;

    auto res = subOp.execute({&x, &y}, {}, {});
    ASSERT_TRUE(res->status() == ND4J_STATUS_OK);
    ASSERT_TRUE(res->at(0)->equalsTo(&exp));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseModTest_1) {

//    auto x('c', {1, 6});
    auto x = NDArrayFactory::create<double>('c', {6});
    auto y = NDArrayFactory::create<double>('c', {3, 4, 5, 1});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 5, 6});
    auto z(exp);
    x.assign(2.);
    y.assign(9.f);
    exp.assign(1.f);
    y.applyTrueBroadcast(BROADCAST(Mod), &x, &z, true);
    z.printIndexedBuffer("MOD1");
    ASSERT_TRUE(exp.equalsTo(&z));
    x.applyTrueBroadcast(BROADCAST(ReverseMod), &y, &exp, true);
    ASSERT_TRUE(exp.equalsTo(&z));

    nd4j::ops::reversemod subOp;

    auto res = subOp.execute({&x, &y}, {}, {});

    ASSERT_TRUE(res->status() == ND4J_STATUS_OK);
    ASSERT_TRUE(res->at(0)->equalsTo(&exp));
    ASSERT_TRUE(exp.equalsTo(&z));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseModTest_2) {

//    auto x('c', {1, 6});
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {3, 4, 5});
    auto exp = NDArrayFactory::create<float>('c', {3, 4, 5});
    auto z(exp);
    x.assign(2.f);
    y.assign(9.f);
    exp.assign(1.f);
    x.applyTrueBroadcast(BROADCAST(ReverseMod), &y, &z, true);
    ASSERT_TRUE(z.equalsTo(&exp));
    x.applyTrueBroadcast(BROADCAST(ReverseMod), &y, &exp, true);
    ASSERT_TRUE(z.equalsTo(&exp));

    nd4j::ops::reversemod subOp;

    auto res = subOp.execute({&x, &y}, {}, {});

    ASSERT_TRUE(res->status() == ND4J_STATUS_OK);
//    res->at(0)->printIndexedBuffer("OUtput REVERSED MOD2");
    ASSERT_TRUE(res->at(0)->equalsTo(&exp));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractVectorVector1) {
    
    auto x = NDArrayFactory::create_<float>   ('c', {1, 15});
    auto y = NDArrayFactory::create_<float>   ('c', {1, 15});
    auto exp = NDArrayFactory::create_<float> ('c', {1, 15});
    x->assign(3);
    y->assign(1);
    exp->assign(-2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversesubtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractMatrixScalar1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto y = NDArrayFactory::create_<float>  ('c', {1, 1});
    auto exp = NDArrayFactory::create_<float>('c', {5, 3});
    x->assign(3);
    y->assign(1);
    exp->assign(-2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversesubtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseSubtractScalarScalar1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {1, 1});
    auto y = NDArrayFactory::create_<float>  ('c', {1, 1});
    auto exp = NDArrayFactory::create_<float>('c', {1, 1});
    x->assign(3);
    y->assign(1);
    exp->assign(-2);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversesubtract subOp;
 
    subOp.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, MultiplyMatrices1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto y = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto exp = NDArrayFactory::create_<float>('c', {5, 3});
    x->assign(2);
    y->assign(3);
    exp->assign(6);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::multiply mul;
 
    mul.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, MultiplyVectorVector1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {1, 15});
    auto y = NDArrayFactory::create_<float>  ('c', {1, 15});
    auto exp = NDArrayFactory::create_<float>('c', {1, 15});
    x->assign(2);
    y->assign(3);
    exp->assign(6);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::multiply mul;
 
    mul.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, MultiplyMatrixScalar) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto y = NDArrayFactory::create_<float>  ('c', {1, 1});
    auto exp = NDArrayFactory::create_<float>('c', {5, 3});
    x->assign(2);
    y->assign(3);
    exp->assign(6);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::multiply mul;
 
    mul.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, MultiplyScalarScalar1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {1, 1});
    auto y = NDArrayFactory::create_<float>  ('c', {1, 1});
    auto exp = NDArrayFactory::create_<float>('c', {1, 1});
    x->assign(2);
    y->assign(3);
    exp->assign(6);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::multiply mul;
 
    mul.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete block;
    delete variableSpace;
    delete exp;
}

TEST_F(DeclarableOpsTests1, TestMatMul1) {
    auto x = NDArrayFactory::create_<float>('c', {3, 5});
    x->linspace(1);

    auto y = NDArrayFactory::create_<float>('c', {5, 3});
    y->linspace(1);

    float _expB[]{135.0f, 310.0f, 485.0f, 150.0f, 350.0f, 550.0f, 165.0f, 390.0f, 615.0f};
    Nd4jLong _expS[] {2, 3, 3, 1, 3, 0, 1, 102}; // expected shape
    ArrayOptions::setDataType(_expS, nd4j::DataType::FLOAT32);
    NDArray exp(_expB, _expS);
    exp.triggerAllocationFlag(false, false);


    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    variableSpace->putVariable(1, new Variable());

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2});

    nd4j::ops::matmul op;

    Nd4jStatus status = op.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    ASSERT_TRUE(variableSpace->hasVariable(1));

    auto result = variableSpace->getVariable(1)->getNDArray();

    ASSERT_TRUE(result->equalsTo(&exp));

    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestSoftMax_bp_1) {

    auto input = NDArrayFactory::create_<double>('c', {2, 2});
    for (int e = 0; e < input->lengthOf(); e++)
        input->p(e, e+1);

    auto epsilon = NDArrayFactory::create_<double>('c', {2, 2});
    epsilon->p(0, 0.1f);
    epsilon->p(1, 0.2f);
    epsilon->p(2, 0.3f);
    epsilon->p(3, 0.4f);

    auto output = NDArrayFactory::create_<double>('c', {2, 2});
    output->assign(1.0f);

    auto exp = NDArrayFactory::create_<double>('c', {2, 2});
    exp->p(0, -0.019661194f);
    exp->p(1, 0.019661194f);
    exp->p(2, -0.019661194f);
    exp->p(3, 0.019661194f);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);
    variableSpace->putVariable(-2, epsilon);
    variableSpace->putVariable(1, output);
    //variableSpace->putVariable(42, exp);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2});

    nd4j::ops::softmax_bp op;

    Nd4jStatus status = op.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);

    ASSERT_TRUE(output->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;

}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, BroadcastDivideTest_1) {

    auto  x = NDArrayFactory::create<float>('c', {3, 4, 5, 1});
    auto  y = NDArrayFactory::create<float>('c', {1, 6});
    auto  exp = NDArrayFactory::create<float>('c', {3, 4, 5, 6});
    x.assign(6);
    y.assign(2);
    exp.assign(3);

    nd4j::ops::divide div;

    auto res = div.execute({&x, &y}, {}, {});

    ASSERT_EQ(res->status(), ND4J_STATUS_OK);
    ASSERT_TRUE(res->at(0)->equalsTo(exp));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, BroadcastReverseDivideTest_1) {
 
    auto  x = NDArrayFactory::create<float>('c', {3, 4, 5, 1});
    auto  y = NDArrayFactory::create<float>('c', {1, 6});
    auto  exp = NDArrayFactory::create<float>('c', {3, 4, 5, 6});
    x.assign(3.f);
    y.assign(6.f);
    exp.assign(2.f);

    nd4j::ops::reversedivide div;

    auto res = div.execute({&x, &y}, {}, {});

    ASSERT_EQ(res->status(), ND4J_STATUS_OK);
 
    ASSERT_TRUE(res->at(0)->equalsTo(exp));
    auto z(exp);
    x.applyTrueBroadcast(BROADCAST(ReverseDivide), &y, &z, true);
    y.applyTrueBroadcast(BROADCAST(Divide), &x, &exp, true);

    ASSERT_TRUE(z.equalsTo(&exp));

    delete res;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, DivideMatrices1) {
    
    auto x = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto y = NDArrayFactory::create_<float>  ('c', {5, 3});
    auto exp = NDArrayFactory::create_<float>('c', {5, 3});
    x->assign(6);
    y->assign(2);
    exp->assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::divide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(exp));

    delete variableSpace;
    delete block;
    delete exp;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, DivideVectorVector1) {
    
    auto x = NDArrayFactory::create_<float>('c', {1, 15});
    auto y = NDArrayFactory::create_<float>('c', {1, 15});
    auto exp = NDArrayFactory::create<float>('c', {1, 15});
    x->assign(6);
    y->assign(2);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::divide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, DivideMatrixScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(6);
    y->assign(2);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::divide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp)); 

    delete block;
    delete variableSpace;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, DivideScalarScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 1});
    auto y = NDArrayFactory::create_<float>('c', {5, 1});
    auto exp = NDArrayFactory::create<float>('c', {5, 1});
    x->assign(6);
    y->assign(2);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::divide div;
 
    div.execute(block);

    //x->printBuffer("x");
    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseDivideMatrices1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {5, 3});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(2);
    y->assign(6);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversedivide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseDivideVectorVector1) {
    
    auto x = NDArrayFactory::create_<float>('c', {1, 15});
    auto y = NDArrayFactory::create_<float>('c', {1, 15});
    auto exp = NDArrayFactory::create<float>('c', {1, 15});
    x->assign(2);
    y->assign(6);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversedivide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseDivideMatrixScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {5, 3});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {5, 3});
    x->assign(2);
    y->assign(6);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversedivide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, ReverseDivideScalarScalar1) {
    
    auto x = NDArrayFactory::create_<float>('c', {1, 1});
    auto y = NDArrayFactory::create_<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 1});
    x->assign(2);
    y->assign(6);
    exp.assign(3);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reversedivide div;
 
    div.execute(block);

    ASSERT_TRUE(x->equalsTo(&exp));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reshapeas1) {
    const std::vector<Nd4jLong> xShape = {5,4,3};
    const std::vector<Nd4jLong> yShape = {3,5,4};
    
    auto x = NDArrayFactory::create_<float>('c', xShape);
    auto y = NDArrayFactory::create_<float>('f', yShape);


    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(-2, y);
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1, -2});

    nd4j::ops::reshapeas reshape;
 
    reshape.execute(block);

    ASSERT_TRUE(x->isSameShape(y));

    delete variableSpace;
    delete block;
}

TEST_F(DeclarableOpsTests1, Test_Cast_1) {
    // TODO: right now there's no real cast implementation, but genera idea should be the same: arrays equality to be expected
    auto x = NDArrayFactory::create<float>('c', {5, 5});
    auto yExp = NDArrayFactory::create<float16>('c', {5, 5});
    x.linspace(1);
    yExp.linspace(1);
    nd4j::ops::cast op;

    auto result = op.execute({&x}, {}, {3});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);
    z->printIndexedBuffer("OUtput");
    yExp.printIndexedBuffer("Expect");
    z->printShapeInfo("OUt shape");
    yExp.printShapeInfo("Exp shape");
    ASSERT_TRUE(yExp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestRegistrator1) {
    auto res = nd4j::ops::OpRegistrator::getInstance()->getAllCustomOperations();

    // nd4j_printf("Ops: %s\n", res)
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestLegacyExecution1) {
    NativeOps nativeOps;

    auto x = NDArrayFactory::create_<float>('c', {10, 10});
    x->assign(1.0f);

    auto y = NDArrayFactory::create_<float>('c', {10, 10});
    y->assign(2.0f);

    auto z = NDArrayFactory::create_<float>('c', {10, 10});

    auto exp = NDArrayFactory::create_<float>('c', {10, 10});
    exp->assign(3.0f);
    z->assign(120.0f);
    std::string opName("add");

    auto hash = nd4j::ops::HashHelper::getInstance()->getInstance()->getLongHash(opName);

    auto inputBuffers = new Nd4jPointer[2];
    auto inputShapes = new Nd4jPointer[2];

    inputBuffers[0] = (Nd4jPointer) x->getBuffer();
    inputBuffers[1] = (Nd4jPointer) y->getBuffer();

    inputShapes[0] = (Nd4jPointer) x->getShapeInfo();
    inputShapes[1] = (Nd4jPointer) y->getShapeInfo();

    auto outputBuffers = new Nd4jPointer[1];
    auto outputShapes = new Nd4jPointer[1];

    outputBuffers[0] = (Nd4jPointer) z->getBuffer();
    outputShapes[0] = (Nd4jPointer) z->getShapeInfo();


    //auto status = nativeOps.execCustomOp(nullptr, hash, inputBuffers, inputShapes, 2, outputBuffers, outputShapes, 1, nullptr, 0, nullptr, 0, false);
    auto status = nativeOps.execCustomOp(nullptr, hash, inputBuffers, inputShapes, 2, outputBuffers, outputShapes, 1, nullptr, 0, nullptr, 0, nullptr, 0, false);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    z->printIndexedBuffer("Output add");
    ASSERT_NEAR(2.0f, y->meanNumber().e<float>(0), 1e-5);
    ASSERT_NEAR(1.0f, x->meanNumber().e<float>(0), 1e-5);
    ASSERT_NEAR(3.0f, z->meanNumber().e<float>(0), 1e-5);

    delete x;
    delete y;
    delete z;
    delete exp;
    delete[] inputBuffers;
    delete[] inputShapes;
    delete[] outputBuffers;
    delete[] outputShapes;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestLegacyExecution2) {
    NativeOps nativeOps;

    auto x = NDArrayFactory::create_<float>('c', {10, 10});
    x->assign(1.0f);

    auto y = NDArrayFactory::create_<float>('c', {10, 10});
    y->assign(2.0f);

    auto z = NDArrayFactory::create_<float>('c', {10, 10});

    auto exp = NDArrayFactory::create_<float>('c', {10, 10});
    exp->assign(3.0);

    std::string opName("add");

    auto hash = nd4j::ops::HashHelper::getInstance()->getInstance()->getLongHash(opName);

    auto inputBuffers = new Nd4jPointer[2];
    auto inputShapes = new Nd4jPointer[2];

    inputBuffers[0] = (Nd4jPointer) x->getBuffer();
    inputBuffers[1] = (Nd4jPointer) y->getBuffer();

    inputShapes[0] = (Nd4jPointer) x->getShapeInfo();
    inputShapes[1] = (Nd4jPointer) y->getShapeInfo();

    auto outputBuffers = new Nd4jPointer[1];
    auto outputShapes = new Nd4jPointer[1];

    nativeOps.execCustomOp(nullptr, hash, inputBuffers, inputShapes, 2, outputBuffers, outputShapes, 1, nullptr, 0, nullptr, 0, nullptr, 0, true);

    ASSERT_NEAR(2.0, y->meanNumber().e<float>(0), 1e-5);
    ASSERT_NEAR(3.0, x->meanNumber().e<float>(0), 1e-5);


    delete x;
    delete y;
    delete z;
    delete exp;
    delete[] inputBuffers;
    delete[] inputShapes;
    delete[] outputBuffers;
    delete[] outputShapes;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestGemv1) {
    auto xBuffer = new float[15]{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f};
    auto xShape = new Nd4jLong[8] {2, 5, 3, 3, 1, 0, 1, 99};
    ArrayOptions::setDataType(xShape, nd4j::DataType::FLOAT32);
    auto x = new NDArray(xBuffer, xShape);

    auto yBuffer = new float[3]{2.f, 4.f, 6.f};
    auto yShape = new Nd4jLong[8] {2, 3, 1, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(yShape, nd4j::DataType::FLOAT32);

    auto y = new NDArray(yBuffer, yShape);

    auto z = NDArrayFactory::create_<float>('f', {5, 1});

    auto expBuffer = new float[5]{28.00,  64.00,  100.00,  136.00,  172.00};
    auto exp = new NDArray(expBuffer, z->getShapeInfo());

    nd4j::blas::GEMV<float, float, float>::op('f',  x->rows(), x->columns(), 1.0f, x->getBuffer(), y->rows(), y->getBuffer(), 1, 0.0, z->getBuffer(), 1);

    //z->printBuffer();

    ASSERT_TRUE(z->equalsTo(exp));

    delete []xBuffer; delete []xShape; delete x; delete []yBuffer; delete []yShape; delete y; delete z; delete []expBuffer; delete exp;

}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reshape1) {
    const std::vector<Nd4jLong> xShape = {5,4,3};
    const std::vector<Nd4jLong> yShape = {3,5,4};    
    
    auto x = NDArrayFactory::create_<float>('c', xShape);
    auto y = NDArrayFactory::create_<float>('f', yShape);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    
    auto block = new Context(1, variableSpace, true);
    block->fillInputs({-1});    
    std::vector<int>* arguments = block->getIArguments();
    arguments->push_back(-y->ordering());
    arguments->push_back(3);
    arguments->push_back(5);
    arguments->push_back(4);
    
    nd4j::ops::reshape reshape;
    
    reshape.execute(block);

    ASSERT_TRUE(x->isSameShape(y)); 

    delete y;
    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reshape2) {
    const std::vector<Nd4jLong> xShape = {5,4,3};
    const std::vector<Nd4jLong> yShape = {3,5,4};    
    
    auto x = NDArrayFactory::create_<float>('c', xShape);
    auto y = NDArrayFactory::create_<float>('f', yShape);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, new Variable());
    
    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});    
    std::vector<int>* arguments = block->getIArguments();
    arguments->push_back(-y->ordering());
    arguments->push_back(3);
    arguments->push_back(5);
    arguments->push_back(4);
    
    nd4j::ops::reshape reshape;
    
    Nd4jStatus status = reshape.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();

    ASSERT_TRUE(result->isSameShape(y));    

    delete y;
    delete block;
    delete variableSpace;
}

TEST_F(DeclarableOpsTests1, Reshape3) {
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});

    nd4j::ops::reshape op;
    auto result = op.execute({&x}, {}, {-99, 3, 4, 5});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(x.isSameShape(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, Reshape4) {
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});

    nd4j::ops::reshape op;
    auto result = op.execute({&x}, {}, {3, 4, 5});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(x.isSameShape(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, Reshape5) {
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});

    nd4j::ops::reshape op;
    auto result = op.execute({&x}, {}, {5, 4, 3});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    delete result;
}

TEST_F(DeclarableOpsTests1, Reshape6){
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});
    auto exp = NDArrayFactory::create<float>('c', {4, 15});

    nd4j::ops::reshape op;
    auto result = op.execute({&x}, {}, {4, -1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(z->isSameShape(exp));

    delete result;

}

TEST_F(DeclarableOpsTests1, Reshape7){
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});
    auto exp = NDArrayFactory::create<float>('c', {60});

    nd4j::ops::reshape op;
    auto result = op.execute({&x}, {}, {-1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(z->isSameShape(exp));

    delete result;

}

TEST_F(DeclarableOpsTests1, TestScatterUpdate1) {
    auto matrix  = NDArrayFactory::create_<float>('c', {3, 2});
    auto updates = NDArrayFactory::create_<float>('c', {2, 2});
    updates->assign(1.0);

    //updates.printBuffer("Updates");

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, matrix);
    variableSpace->putVariable(-2, updates);
    variableSpace->putVariable(1, new Variable(&matrix));

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1, -2});

    std::vector<int>* arguments = block->getIArguments();
    arguments->push_back(0);
    arguments->push_back(1);
    arguments->push_back(1);
    arguments->push_back(2);
    arguments->push_back(1);
    arguments->push_back(2);

    nd4j::ops::scatter_update op;


    Nd4jStatus result = op.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, result);

    delete block;
    delete variableSpace;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Repeat1) {
    
    float eBuffer[8] = {1.0,2.0,1.0,2.0,3.0,4.0,3.0,4.0};
    Nd4jLong eShape[8] = {2, 4, 2, 2, 1, 0, 1, 99};
    ArrayOptions::setDataType(eShape, nd4j::DataType::FLOAT32);
    auto x = NDArrayFactory::create_<float>('c', {2, 2});
    auto exp = new NDArray(eBuffer, eShape);
    for (int e = 0; e < x->lengthOf(); e++)
        x->p(e, e + 1);
    
    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, new Variable());

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});    
    std::vector<int>* arguments = block->getIArguments();   
    *arguments = {2};           // set repeats
    arguments->push_back(0);    // set dimension

    nd4j::ops::repeat repeat;

    Nd4jStatus status = repeat.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();

    ASSERT_TRUE(exp->equalsTo(result));

    delete exp;
    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Transpose1) {

    auto x = NDArrayFactory::create_<float>('c', {3,5,2});
    auto exp = NDArrayFactory::create_<float>('f', {2,5,3});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);

    auto block = new Context(1, variableSpace, true);  // in-place
    block->fillInputs({-1});
    nd4j::ops::transpose transpose;

    Nd4jStatus status = transpose.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    // ASSERT_TRUE(x.isSameShapeStrict(&exp));

    for (int e = 0; e < x->rankOf() * 2 + 2; e++) {
        ASSERT_EQ(x->getShapeInfo()[e], exp->getShapeInfo()[e]);
    }
//  ASSERT_EQ(x.getShapeInfo()[x.rankOf() * 2 + 2],-exp.getShapeInfo()[x.rankOf() * 2 + 2]);
    ASSERT_EQ(x->getShapeInfo()[x->rankOf() * 2 + 3], exp->getShapeInfo()[x->rankOf() * 2 + 3]);

    delete exp;
    delete block;
    delete variableSpace;

}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Transpose2) {
    auto x = NDArrayFactory::create_<float>('c', {3,5,2});
    auto exp = NDArrayFactory::create_<float>('f', {2,5,3});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, new Variable());

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});
    nd4j::ops::transpose transpose;

    Nd4jStatus  status = transpose.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);

    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // ASSERT_TRUE(result->isSameShapeStrict(&exp));
    for (int e = 0; e < result->rankOf() * 2 + 2; e++) {
        ASSERT_EQ(result->getShapeInfo()[e], exp->getShapeInfo()[e]);
    }
    //ASSERT_EQ(result->getShapeInfo()[x.rankOf() * 2 + 2],-exp.getShapeInfo()[x.rankOf() * 2 + 2]);
    ASSERT_EQ(result->getShapeInfo()[x->rankOf() * 2 + 3], exp->getShapeInfo()[x->rankOf() * 2 + 3]);

    delete exp;
    delete block;
    delete variableSpace;
}


//////////////////////////////////////////////////////////////////////
// in-place
TEST_F(DeclarableOpsTests1, Permute1) {

    Nd4jLong shapeX[]   = {3, 5, 10, 15, 150, 15, 1, 0, 1, 99};
    Nd4jLong shapeExp[] = {3, 15, 5, 10, 1, 150, 15, 0, 0, 99};
    const std::vector<int> perm = {2, 0, 1};
    ArrayOptions::setDataType(shapeX, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shapeExp, nd4j::DataType::FLOAT32);

    auto x = new NDArray(shapeX,true);
    auto exp = new NDArray(shapeExp,true);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);

    auto block = new Context(1, variableSpace, true);  // in-place
    block->fillInputs({-1});
    std::vector<int>* arguments = block->getIArguments();   
    *arguments = perm;      // set dimensions to be permuted
    
    nd4j::ops::permute permute;
    Nd4jStatus status = permute.execute(block); 
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    ASSERT_TRUE(x->isSameShapeStrict(exp)); 

    delete exp;
    delete block;
    delete variableSpace;
}

//////////////////////////////////////////////////////////////////////
// not-in-place
TEST_F(DeclarableOpsTests1, Permute2) {      

    Nd4jLong shapeX[]   = {3, 5, 10, 15, 150, 15, 1, 0, 1, 99};
    Nd4jLong shapeExp[] = {3, 15, 5, 10, 1, 150, 15, 0, 0, 99};
    const std::vector<int> perm = {2, 0, 1};

    ArrayOptions::setDataType(shapeX, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shapeExp, nd4j::DataType::FLOAT32);

    auto x = new NDArray(shapeX, true);
    auto exp = new NDArray(shapeExp, true);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, new Variable());

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});
    auto arguments = block->getIArguments();   
    *arguments = perm;      // set dimensions to be permuted
    
    nd4j::ops::permute permute;
    Nd4jStatus status = permute.execute(block);
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    
    ASSERT_EQ(ND4J_STATUS_OK, status);  
    ASSERT_TRUE(result->isSameShapeStrict(exp));    

    delete block;        
    delete variableSpace;
    delete exp;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestArgumentsValidation1) {
    Nd4jLong shapeX[]   = {3, 5, 10, 15, 150, 15, 1, 0, 1, 99};
    Nd4jLong shapeExp[] = {3, 15, 5, 10, 1, 150, 15, 0, -1, 99};

    ArrayOptions::setDataType(shapeX, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shapeExp, nd4j::DataType::FLOAT32);

    const std::vector<int> perm = {2, 0, 1};
    auto x = new NDArray(shapeX);
    auto exp = new NDArray(shapeExp);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, new Variable());

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});

    nd4j::ops::im2col permute;
    Nd4jStatus status = permute.execute(block);

    ASSERT_TRUE(status != 0);

    delete exp;
    delete block;        
    delete variableSpace;    
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestReductionShape1) {
    auto input = NDArrayFactory::create_<float>('c', {4, 5, 5, 10, 10});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});

    // kernel params
    block->getIArguments()->push_back(MAX_INT);

    nd4j::ops::testreduction testop;

    auto inP = new Nd4jLong[shape::shapeInfoLength(input->getShapeInfo())];
    memcpy(inP, input->getShapeInfo(), shape::shapeInfoByteLength(input->rankOf()));

    auto inshape = new ShapeList(inP);

    auto shapes = testop.calculateOutputShape(inshape, *block);

    ASSERT_EQ(1,shapes->size());
    ASSERT_EQ(0,shapes->at(0)[0]); // scalar shape has rank 0
    ASSERT_EQ(8192,shapes->at(0)[1]);
    ASSERT_EQ(1,shapes->at(0)[2]);

    delete[] inP;
    shapes->destroy();
    delete variableSpace;
    delete block;
    delete inshape;
    delete shapes;

}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestReductionShape2) {
    auto input = NDArrayFactory::create_<float>('c', {4, 5, 5, 10, 10});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});

    // kernel params
    //block->getIArguments()->push_back(4);
    block->getIArguments()->push_back(1);
    block->getIArguments()->push_back(2);
    block->getIArguments()->push_back(3);
    block->getIArguments()->push_back(4);

    nd4j::ops::testreduction testop;

    auto inshapes = new ShapeList(input->getShapeInfo());
    auto shapes = testop.calculateOutputShape(inshapes, *block);
    ASSERT_EQ(1,shapes->size());
    ASSERT_EQ(1,shapes->at(0)[0]);
    ASSERT_EQ(4,shapes->at(0)[1]);
    ASSERT_EQ(1,shapes->at(0)[2]);

    shapes->destroy();
    delete variableSpace;
    delete block;
    delete shapes;
    delete inshapes;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, TestCustomShape1) {
    auto input = NDArrayFactory::create_<float>('c', {2, 3, 4});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});

    nd4j::ops::testcustom test;

    auto inshapes = new ShapeList(input->getShapeInfo());
    auto shapes = test.calculateOutputShape(inshapes, *block);

    //input.printShapeInfo("input");
    //shape::printShapeInfoLinear(shape);

    ASSERT_EQ(input->getShapeInfo()[0]    , shapes->at(0)[0]);
    ASSERT_EQ(input->getShapeInfo()[1] * 2, shapes->at(0)[1]);
    ASSERT_EQ(input->getShapeInfo()[2] * 2, shapes->at(0)[2]);
    ASSERT_EQ(input->getShapeInfo()[3] * 2, shapes->at(0)[3]);

    shapes->destroy();
    delete variableSpace;
    delete block;
    delete shapes;
    delete inshapes;
}


//////////////////////////////////////////////////////////////////////
/*
TEST_F(DeclarableOpsTests1, Sum1) {

    float xBuff[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int xShape[]  = {2, 4, 2, 2, 1, 0, 1, 99};
    float expBuff[] = {16, 20};
    int expShape[]  = {2, 1, 2, 2, 1, 0, 1, 99};

    const std::vector<int> dimensions = {1,0};

    auto x = NDArrayFactory::create_<float>(xBuff, xShape);
    auto z = NDArrayFactory::create_<float>(1, 2, 'c');
    auto exp(expBuff, expShape);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    variableSpace->putVariable(1, z);

    auto block = new Context(1, variableSpace, false);  // not-in-place
    block->fillInputs({-1});
    std::vector<int>* arguments = block->getIArguments();
    *arguments = dimensions;

    nd4j::ops::sum<float> sum;
    Nd4jStatus status = sum.execute(block);

    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();

    ASSERT_EQ(ND4J_STATUS_OK, status);
    ASSERT_TRUE(result->equalsTo(&exp));

    delete block;
    delete variableSpace;
}
*/

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Maxpool2d_test1) {

    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dH,dW, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Maxpool2d_test2) {

    const int bS = 2;  
    const int iD = 1;  
    const int iH = 28; 
    const int iW = 28; 
    const int kH = 5;  
    const int kW = 5;  
    const int sH = 1;  
    const int sW = 1;  
    const int pH = 0;  
    const int pW = 0;  
    const int dH = 1;  
    const int dW = 1;  
    const int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     // output height
    const int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;     // output width


    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dH,dW, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Maxpool2d_test3) {

    const int bS = 2;  
    const int iD = 1;  
    const int iH = 28; 
    const int iW = 28; 
    const int kH = 5;  
    const int kW = 5;  
    const int sH = 1;  
    const int sW = 1;  
    const int pH = 0;  
    const int pW = 0;  
    const int dH = 1;  
    const int dW = 1;  
    const int oH = (int) nd4j::math::nd4j_ceil<float, int>(iH * 1.f / sH);
    const int oW = (int) nd4j::math::nd4j_ceil<float, int>(iW * 1.f / sW);


    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dH,dW, 1};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Maxpool2d_test4) {

    const int bS = 2;  
    const int iD = 1;  
    const int iH = 24; 
    const int iW = 24; 
    const int kH = 3;  
    const int kW = 3;  
    const int sH = 1;  
    const int sW = 1;  
    const int pH = 0;  
    const int pW = 0;  
    const int dH = 1;  
    const int dW = 1;  
    const int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     // output height
    const int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;     // output width


    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dH,dW, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Maxpool2d_test5) {

    const int bS = 2;  
    const int iD = 1;  
    const int iH = 24; 
    const int iW = 24; 
    const int kH = 3;  
    const int kW = 3;  
    const int sH = 1;  
    const int sW = 1;  
    const int pH = 0;  
    const int pW = 0;  
    const int dH = 1;  
    const int dW = 1;  
    const int oH = (int) nd4j::math::nd4j_ceil<float, int>(iH * 1.f / sH);
    const int oW = (int) nd4j::math::nd4j_ceil<float, int>(iW * 1.f / sW);


    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dH,dW, 1};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Avgpool2d_test1) {

    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 0, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::avgpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    ASSERT_TRUE(exp.isSameShape(result));


    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Avgpool2d_test2) {
    const int bS = 2;  
    const int iD = 1;  
    const int iH = 28; 
    const int iW = 28; 
    const int kH = 5;  
    const int kW = 5;  
    const int sH = 1;  
    const int sW = 1;  
    const int pH = 0;  
    const int pW = 0;  
    const int dH = 1;  
    const int dW = 1;  
    const int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     // output height
    const int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;     // output width


    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 0, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::avgpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Avgpool2d_test3) {
    const int bS = 2;  
    const int iD = 1;  
    const int iH = 28; 
    const int iW = 28; 
    const int kH = 5;  
    const int kW = 5;  
    const int sH = 1;  
    const int sW = 1;  
    const int pH = 0;  
    const int pW = 0;  
    const int dH = 1;  
    const int dW = 1;  
    const int oH = (int) nd4j::math::nd4j_ceil<float, int>(iH * 1.f / sH);
    const int oW = (int) nd4j::math::nd4j_ceil<float, int>(iW * 1.f / sW);


    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 1, 0, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::avgpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    // result->printShapeInfo();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Pnormpool2d1) {

    auto x = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto exp = NDArrayFactory::create<float>('c',{bS,iD,oH,oW});
    // auto z('c',{bS,iD,oH,oW});

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 1, 0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode; 9 - extraParam0 for pnorm case;

    nd4j::ops::pnormpool2d pooling;
    Nd4jStatus status = pooling.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

/*/////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, IsMax1) {

    float xBuff[]   = {1,2,3,4,5,6,7,8,9};
    Nd4jLong xShape[]    = {2,3,3,3,1,0,1,99};
    bool expBuff[] = {0,0,1,0,0,1,0,0,1};
    ArrayOptions::setDataType(xShape, nd4j::DataType::BOOL);

    auto x = new NDArray(xBuff, xShape);
    NDArray exp(expBuff, xShape);

    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, x);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    std::vector<int>* argI = block->getIArguments();
//    *argI = {1};                                        // dimensions
    argI->push_back(1); // = {1};                                        // dimensions

    nd4j::ops::ismax ismaxOp;
    Nd4jStatus status = ismaxOp.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    result->printIndexedBuffer("IS_MAX");
    ASSERT_TRUE(exp.equalsTo(result));

    delete variableSpace;
    delete block;
}
*/

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, IsMax1) {
    NDArray x('c', {3, 3}, nd4j::DataType::FLOAT32);
//    NDArray exp('c', {3, 3}, nd4j::DataType::BOOL);
    NDArray exp('c', {3, 3}, nd4j::DataType::FLOAT32);
    x.linspace(1);
    exp.p<bool>(0, 2, true);
    exp.p<bool>(1, 2, true);
    exp.p<bool>(2, 2, true);

    nd4j::ops::ismax ismaxOp;
    auto result = ismaxOp.execute({&x}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto res = result->at(0);
    res->printIndexedBuffer("IS_MAX");
    ASSERT_TRUE(exp.equalsTo(res));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Maxpool2d_bp1) {

    auto input = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto epsilon = NDArrayFactory::create_<float>('c', {bS,iD,oH,oW});
    auto exp     = NDArrayFactory::create<float>('c', {bS,iD,iH,iW});
    
    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);
    variableSpace->putVariable(-2, epsilon);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    block->fillInputs({-2});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 0, 0};   // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d_bp bp;
    Nd4jStatus status = bp.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, AvgPool2dBP) {

    auto input = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto epsilon = NDArrayFactory::create_<float>('c', {bS,iD,oH,oW});
    auto exp     = NDArrayFactory::create<float>('c', {bS,iD,iH,iW});
    
    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);
    variableSpace->putVariable(-2, epsilon);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    block->fillInputs({-2});
    std::vector<int>* argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 1, 0};   // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode, 9 - extraParam0 (unnecessary for avg mode), 10 - data format

    nd4j::ops::avgpool2d_bp bp;
    Nd4jStatus status = bp.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, PnormPool2dBP) {

    auto input = NDArrayFactory::create_<float>('c', {bS,iD,iH,iW});
    auto epsilon = NDArrayFactory::create_<float>('c', {bS,iD,oH,oW});
    auto exp     = NDArrayFactory::create<float>('c', {bS,iD,iH,iW});
    
    auto variableSpace = new VariableSpace();
    variableSpace->putVariable(-1, input);
    variableSpace->putVariable(-2, epsilon);
    // variableSpace->putVariable(1, &z);

    auto block = new Context(1, variableSpace, false);
    block->fillInputs({-1});
    block->fillInputs({-2});
    auto argI = block->getIArguments();
    *argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 3};   // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode; 9 - divisor    
    std::vector<double>* argT = block->getTArguments();
    *argT = {0.000001};

    nd4j::ops::pnormpool2d_bp bp;
    Nd4jStatus status = bp.execute(block);
    ASSERT_EQ(ND4J_STATUS_OK, status);
    
    auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
    ASSERT_TRUE(exp.isSameShape(result));

    delete variableSpace;
    delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, CompactLaunchTests1) {
    Nd4jLong _expS[] = {4, 2, 3, 8, 8, 192, 64, 8, 1, 8192, 1, 99};
    float _expB[] = {6276.0,   12831.0,   19668.0,   26790.0,   27012.0,   20703.0,   14100.0,    7200.0,    13719.0,   28023.0,   42918.0,   58410.0,   58902.0,   45105.0,   30693.0,   15660.0,    22389.0,   45696.0,   69930.0,   95100.0,   95910.0,   73386.0,   49899.0,   25440.0,    32346.0,   65970.0,  100884.0,  137100.0,  138276.0,  105726.0,   71838.0,   36600.0,    33726.0,   68790.0,  105204.0,  142980.0,  144156.0,  110226.0,   74898.0,   38160.0,    27555.0,   56154.0,   85806.0,  116520.0,  117474.0,   89748.0,   60933.0,   31020.0,    19917.0,   40557.0,   61926.0,   84030.0,   84714.0,   64671.0,   43875.0,   22320.0,    10752.0,   21879.0,   33384.0,   45270.0,   45636.0,   34815.0,   23604.0,   12000.0,    7551.0,   15456.0,   23718.0,   32340.0,   32562.0,   24978.0,   17025.0,    8700.0,    16569.0,   33873.0,   51918.0,   70710.0,   71202.0,   54555.0,   37143.0,   18960.0,    27114.0,   55371.0,   84780.0,  115350.0,  116160.0,   88911.0,   60474.0,   30840.0,    39246.0,   80070.0,  122484.0,  166500.0,  167676.0,  128226.0,   87138.0,   44400.0,    40626.0,   82890.0,  126804.0,  172380.0,  173556.0,  132726.0,   90198.0,   45960.0,    33180.0,   67629.0,  103356.0,  140370.0,  141324.0,  107973.0,   73308.0,   37320.0,    23967.0,   48807.0,   74526.0,  101130.0,  101814.0,   77721.0,   52725.0,   26820.0,    12927.0,   26304.0,   40134.0,   54420.0,   54786.0,   41790.0,   28329.0,   14400.0,    8826.0,   18081.0,   27768.0,   37890.0,   38112.0,   29253.0,   19950.0,   10200.0,    19419.0,   39723.0,   60918.0,   83010.0,   83502.0,   64005.0,   43593.0,   22260.0,    31839.0,   65046.0,   99630.0,  135600.0,  136410.0,  104436.0,   71049.0,   36240.0,    46146.0,   94170.0,  144084.0,  195900.0,  197076.0,  150726.0,  102438.0,   52200.0,    47526.0,   96990.0,  148404.0,  201780.0,  202956.0,  155226.0,  105498.0,   53760.0,    38805.0,   79104.0,  120906.0,  164220.0,  165174.0,  126198.0,   85683.0,   43620.0,    28017.0,   57057.0,   87126.0,  118230.0,  118914.0,   90771.0,   61575.0,   31320.0,    15102.0,   30729.0,   46884.0,   63570.0,   63936.0,   48765.0,   33054.0,   16800.0,    17220.0,   34863.0,   52932.0,   71430.0,   72228.0,   54831.0,   36996.0,   18720.0,    36327.0,   73527.0,  111606.0,  150570.0,  152214.0,  115521.0,   77925.0,   39420.0,    57381.0,  116112.0,  176202.0,  237660.0,  240198.0,  182250.0,  122907.0,   62160.0,    80442.0,  162738.0,  246900.0,  332940.0,  336420.0,  255198.0,  172062.0,   87000.0,    84702.0,  171318.0,  259860.0,  350340.0,  353820.0,  268338.0,  180882.0,   91440.0,    66867.0,  135210.0,  205038.0,  276360.0,  279042.0,  211572.0,  142581.0,   72060.0,    46845.0,   94701.0,  143574.0,  193470.0,  195306.0,  148047.0,   99747.0,   50400.0,    24576.0,   49671.0,   75288.0,  101430.0,  102372.0,   77583.0,   52260.0,   26400.0,    22095.0,   44688.0,   67782.0,   91380.0,   92178.0,   69906.0,   47121.0,   23820.0,    46377.0,   93777.0,  142206.0,  191670.0,  193314.0,  146571.0,   98775.0,   49920.0,    72906.0,  147387.0,  223452.0,  301110.0,  303648.0,  230175.0,  155082.0,   78360.0,    101742.0,  205638.0,  311700.0,  419940.0,  423420.0,  320898.0,  216162.0,  109200.0,    106002.0,  214218.0,  324660.0,  437340.0,  440820.0,  334038.0,  224982.0,  113640.0,    83292.0,  168285.0,  254988.0,  343410.0,  346092.0,  262197.0,  176556.0,   89160.0,    58095.0,  117351.0,  177774.0,  239370.0,  241206.0,  182697.0,  122997.0,   62100.0,    30351.0,   61296.0,   92838.0,  124980.0,  125922.0,   95358.0,   64185.0,   32400.0,    26970.0,   54513.0,   82632.0,  111330.0,  112128.0,   84981.0,   57246.0,   28920.0,    56427.0,  114027.0,  172806.0,  232770.0,  234414.0,  177621.0,  119625.0,   60420.0,    88431.0,  178662.0,  270702.0,  364560.0,  367098.0,  278100.0,  187257.0,   94560.0,    123042.0,  248538.0,  376500.0,  506940.0,  510420.0,  386598.0,  260262.0,  131400.0,    127302.0,  257118.0,  389460.0,  524340.0,  527820.0,  399738.0,  269082.0,  135840.0,    99717.0,  201360.0,  304938.0,  410460.0,  413142.0,  312822.0,  210531.0,  106260.0,    69345.0,  140001.0,  211974.0,  285270.0,  287106.0,  217347.0,  146247.0,   73800.0,    36126.0,   72921.0,  110388.0,  148530.0,  149472.0,  113133.0,   76110.0,   38400.0,};

    NDArray exp(_expB, _expS);

    auto input = NDArrayFactory::create<float>('c', {2, 3, 4, 4});
    auto weights = NDArrayFactory::create<float>('c', {3, 3, 5, 5});

    input.linspace(1);
    weights.linspace(1);
    weights.permutei({2,3,1,0});

    nd4j::ops::deconv2d op;
    auto result = op.execute({&input, &weights}, {}, {5, 5, 1, 1, 0, 0, 1, 1, 0, 0});

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, CompactLaunchTests2) {
    Nd4jLong _expS[] = {4, 2, 3, 8, 8, 192, 64, 8, 1, 16384, 1, 99};
    double _expB[] = {6276.0,   12831.0,   19668.0,   26790.0,   27012.0,   20703.0,   14100.0,    7200.0,    13719.0,   28023.0,   42918.0,   58410.0,   58902.0,   45105.0,   30693.0,   15660.0,    22389.0,   45696.0,   69930.0,   95100.0,   95910.0,   73386.0,   49899.0,   25440.0,    32346.0,   65970.0,  100884.0,  137100.0,  138276.0,  105726.0,   71838.0,   36600.0,    33726.0,   68790.0,  105204.0,  142980.0,  144156.0,  110226.0,   74898.0,   38160.0,    27555.0,   56154.0,   85806.0,  116520.0,  117474.0,   89748.0,   60933.0,   31020.0,    19917.0,   40557.0,   61926.0,   84030.0,   84714.0,   64671.0,   43875.0,   22320.0,    10752.0,   21879.0,   33384.0,   45270.0,   45636.0,   34815.0,   23604.0,   12000.0,    7551.0,   15456.0,   23718.0,   32340.0,   32562.0,   24978.0,   17025.0,    8700.0,    16569.0,   33873.0,   51918.0,   70710.0,   71202.0,   54555.0,   37143.0,   18960.0,    27114.0,   55371.0,   84780.0,  115350.0,  116160.0,   88911.0,   60474.0,   30840.0,    39246.0,   80070.0,  122484.0,  166500.0,  167676.0,  128226.0,   87138.0,   44400.0,    40626.0,   82890.0,  126804.0,  172380.0,  173556.0,  132726.0,   90198.0,   45960.0,    33180.0,   67629.0,  103356.0,  140370.0,  141324.0,  107973.0,   73308.0,   37320.0,    23967.0,   48807.0,   74526.0,  101130.0,  101814.0,   77721.0,   52725.0,   26820.0,    12927.0,   26304.0,   40134.0,   54420.0,   54786.0,   41790.0,   28329.0,   14400.0,    8826.0,   18081.0,   27768.0,   37890.0,   38112.0,   29253.0,   19950.0,   10200.0,    19419.0,   39723.0,   60918.0,   83010.0,   83502.0,   64005.0,   43593.0,   22260.0,    31839.0,   65046.0,   99630.0,  135600.0,  136410.0,  104436.0,   71049.0,   36240.0,    46146.0,   94170.0,  144084.0,  195900.0,  197076.0,  150726.0,  102438.0,   52200.0,    47526.0,   96990.0,  148404.0,  201780.0,  202956.0,  155226.0,  105498.0,   53760.0,    38805.0,   79104.0,  120906.0,  164220.0,  165174.0,  126198.0,   85683.0,   43620.0,    28017.0,   57057.0,   87126.0,  118230.0,  118914.0,   90771.0,   61575.0,   31320.0,    15102.0,   30729.0,   46884.0,   63570.0,   63936.0,   48765.0,   33054.0,   16800.0,    17220.0,   34863.0,   52932.0,   71430.0,   72228.0,   54831.0,   36996.0,   18720.0,    36327.0,   73527.0,  111606.0,  150570.0,  152214.0,  115521.0,   77925.0,   39420.0,    57381.0,  116112.0,  176202.0,  237660.0,  240198.0,  182250.0,  122907.0,   62160.0,    80442.0,  162738.0,  246900.0,  332940.0,  336420.0,  255198.0,  172062.0,   87000.0,    84702.0,  171318.0,  259860.0,  350340.0,  353820.0,  268338.0,  180882.0,   91440.0,    66867.0,  135210.0,  205038.0,  276360.0,  279042.0,  211572.0,  142581.0,   72060.0,    46845.0,   94701.0,  143574.0,  193470.0,  195306.0,  148047.0,   99747.0,   50400.0,    24576.0,   49671.0,   75288.0,  101430.0,  102372.0,   77583.0,   52260.0,   26400.0,    22095.0,   44688.0,   67782.0,   91380.0,   92178.0,   69906.0,   47121.0,   23820.0,    46377.0,   93777.0,  142206.0,  191670.0,  193314.0,  146571.0,   98775.0,   49920.0,    72906.0,  147387.0,  223452.0,  301110.0,  303648.0,  230175.0,  155082.0,   78360.0,    101742.0,  205638.0,  311700.0,  419940.0,  423420.0,  320898.0,  216162.0,  109200.0,    106002.0,  214218.0,  324660.0,  437340.0,  440820.0,  334038.0,  224982.0,  113640.0,    83292.0,  168285.0,  254988.0,  343410.0,  346092.0,  262197.0,  176556.0,   89160.0,    58095.0,  117351.0,  177774.0,  239370.0,  241206.0,  182697.0,  122997.0,   62100.0,    30351.0,   61296.0,   92838.0,  124980.0,  125922.0,   95358.0,   64185.0,   32400.0,    26970.0,   54513.0,   82632.0,  111330.0,  112128.0,   84981.0,   57246.0,   28920.0,    56427.0,  114027.0,  172806.0,  232770.0,  234414.0,  177621.0,  119625.0,   60420.0,    88431.0,  178662.0,  270702.0,  364560.0,  367098.0,  278100.0,  187257.0,   94560.0,    123042.0,  248538.0,  376500.0,  506940.0,  510420.0,  386598.0,  260262.0,  131400.0,    127302.0,  257118.0,  389460.0,  524340.0,  527820.0,  399738.0,  269082.0,  135840.0,    99717.0,  201360.0,  304938.0,  410460.0,  413142.0,  312822.0,  210531.0,  106260.0,    69345.0,  140001.0,  211974.0,  285270.0,  287106.0,  217347.0,  146247.0,   73800.0,    36126.0,   72921.0,  110388.0,  148530.0,  149472.0,  113133.0,   76110.0,   38400.0,};
    NDArray exp(_expB, _expS);
    exp.triggerAllocationFlag(false, false);


    auto input = NDArrayFactory::create<double>('c', {2, 3, 4, 4});
    auto weights = NDArrayFactory::create<double>('c', {3, 3, 5, 5});
    auto z = NDArrayFactory::create<double>('c', {2, 3, 8, 8});

    input.linspace(1);
    weights.linspace(1);
    weights.permutei({2,3,1,0});

    nd4j::ops::deconv2d op;
    auto result = op.execute({&input, &weights}, {&z}, {}, {5, 5, 1, 1, 0, 0, 1, 1, 0, 0},{});

    ASSERT_EQ(ND4J_STATUS_OK, result);

    ASSERT_TRUE(exp.isSameShape(&z));
    ASSERT_TRUE(exp.equalsTo(&z));
}

////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, batchnorm_test1) {
    
    auto input    = NDArrayFactory::create<double>('c', {2,3,2,3,2});
    auto mean     = NDArrayFactory::create<double>('c', {2,3,2,3,2});
    auto variance = NDArrayFactory::create<double>('c', {2,3,2,3,2});
    auto gamma    = NDArrayFactory::create<double>('c', {2,3,2,3,2});
    auto beta     = NDArrayFactory::create<double>('c', {2,3,2,3,2});
    
    auto expected = NDArrayFactory::create<double>('c', {2,3,2,3,2}, {-0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, 0.49088821, 0.66059214, 0.83029607, 1., 1.16970393, 1.33940786, 1.50911179, 1.67881572, 1.84851965, 2.01822358, 2.18792751, 2.35763144, 2.52733537, 2.6970393 , 2.86674323, 3.03644717, 3.2061511 , 3.37585503, 3.54555896, 3.71526289, 3.88496682, 4.05467075, 4.22437468, 4.39407861, 4.56378254, 4.73348647, 4.9031904 , 5.07289433, 5.24259826, 5.41230219, 5.58200612, 5.75171005, 5.92141398, 6.09111791, 6.26082184, 6.43052577, 6.6002297 , 6.76993364, 6.93963757, 7.1093415 , 7.27904543, 7.44874936, 7.61845329, 7.78815722, 7.95786115, 8.12756508, 8.29726901, 8.46697294, 8.63667687, 8.8063808 , 8.97608473, 9.14578866, 9.31549259, 9.48519652, 9.65490045, 9.82460438, 9.99430831,10.16401224,10.33371617,10.50342011,10.67312404,10.84282797,11.0125319 ,11.18223583,11.35193976,11.52164369});

    input.linspace(0.1, 0.1);
    mean.assign(1.);
    variance.assign(0.5);
    gamma.assign(1.2);
    beta.assign(1.);

    nd4j::ops::batchnorm op;

    auto results = op.execute({&input, &mean, &variance, &gamma, &beta}, {1e-5}, {1,1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}


TEST_F(DeclarableOpsTests1, batchnorm_test2) {
    
    auto input    = NDArrayFactory::create<double>('c', {2,3,1,3,1});
    auto mean     = NDArrayFactory::create<double>('c', {1,3,2,1,2});
    auto variance = NDArrayFactory::create<double>('c', {2,1,2,3,2});
    auto gamma    = NDArrayFactory::create<double>('c', {2,3,2,3,1});
    auto beta     = NDArrayFactory::create<double>('c', {1,3,2,1,2});
    
    auto expected = NDArrayFactory::create<double>('c', {2,3,2,3,2}, {-0.52733537,-0.52733537,-0.35763144,-0.35763144,-0.18792751,-0.18792751, -0.52733537,-0.52733537,-0.35763144,-0.35763144,-0.18792751,-0.18792751, -0.01822358,-0.01822358, 0.15148035, 0.15148035, 0.32118428, 0.32118428, -0.01822358,-0.01822358, 0.15148035, 0.15148035, 0.32118428, 0.32118428, 0.49088821, 0.49088821, 0.66059214, 0.66059214, 0.83029607, 0.83029607, 0.49088821, 0.49088821, 0.66059214, 0.66059214, 0.83029607, 0.83029607, 1.        , 1.        , 1.16970393, 1.16970393, 1.33940786, 1.33940786, 1.        , 1.        , 1.16970393, 1.16970393, 1.33940786, 1.33940786, 1.50911179, 1.50911179, 1.67881572, 1.67881572, 1.84851965, 1.84851965, 1.50911179, 1.50911179, 1.67881572, 1.67881572, 1.84851965, 1.84851965, 2.01822358, 2.01822358, 2.18792751, 2.18792751, 2.35763144, 2.35763144, 2.01822358, 2.01822358, 2.18792751, 2.18792751, 2.35763144, 2.35763144});

    input.linspace(0.1, 0.1);
    mean.assign(1.);
    variance.assign(0.5);
    gamma.assign(1.2);
    beta.assign(1.);

    nd4j::ops::batchnorm op;

    auto results = op.execute({&input, &mean, &variance, &gamma, &beta}, {1e-5}, {1,1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, batchnorm_test3) {
    
    auto input    = NDArrayFactory::create<double>('c', {2,3,2,3,2});
    auto mean     = NDArrayFactory::create<double>('c', {2,3,2});
    auto variance = NDArrayFactory::create<double>('c', {2,3,1,3,1});
    auto gamma    = NDArrayFactory::create<double>('c', {1,1});
    auto beta     = NDArrayFactory::create<double>('c', {1,2});
    
    auto expected = NDArrayFactory::create<double>('c', {2,3,2,3,2}, {-0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, 0.49088821, 0.66059214, 0.83029607, 1., 1.16970393, 1.33940786, 1.50911179, 1.67881572, 1.84851965, 2.01822358, 2.18792751, 2.35763144, 2.52733537, 2.6970393 , 2.86674323, 3.03644717, 3.2061511 , 3.37585503, 3.54555896, 3.71526289, 3.88496682, 4.05467075, 4.22437468, 4.39407861, 4.56378254, 4.73348647, 4.9031904 , 5.07289433, 5.24259826, 5.41230219, 5.58200612, 5.75171005, 5.92141398, 6.09111791, 6.26082184, 6.43052577, 6.6002297 , 6.76993364, 6.93963757, 7.1093415 , 7.27904543, 7.44874936, 7.61845329, 7.78815722, 7.95786115, 8.12756508, 8.29726901, 8.46697294, 8.63667687, 8.8063808 , 8.97608473, 9.14578866, 9.31549259, 9.48519652, 9.65490045, 9.82460438, 9.99430831,10.16401224,10.33371617,10.50342011, 10.67312404,10.84282797,11.0125319 ,11.18223583,11.35193976,11.52164369});

    input.linspace(0.1, 0.1);
    mean.assign(1.);
    variance.assign(0.5);
    gamma.assign(1.2);
    beta.assign(1.);

    nd4j::ops::batchnorm op;

    auto results = op.execute({&input, &mean, &variance, &gamma, &beta}, {1e-5}, {1,1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, batchnorm_test4) {
    
    auto input    = NDArrayFactory::create<double>('c', {3,2});
    auto mean    = NDArrayFactory::create<double>('c', {2,3,2});
    auto variance= NDArrayFactory::create<double>('c', {2,3,1,3,2});
    auto gamma   = NDArrayFactory::create<double>('c', {1,1});
    auto beta    = NDArrayFactory::create<double>('c', {1,2});
    
    auto expected= NDArrayFactory::create<double>('c', {2,3,2,3,2}, {-0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428, -0.52733537,-0.35763144,-0.18792751,-0.01822358, 0.15148035, 0.32118428});

    input.linspace(0.1, 0.1);
    mean.assign(1.);
    variance.assign(0.5);
    gamma.assign(1.2);
    beta.assign(1.);

    nd4j::ops::batchnorm op;

    auto results = op.execute({&input, &mean, &variance, &gamma, &beta}, {1e-5}, {1,1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, sru_old_test1) {

    const int bS = 2;
    const int K = 3;    
    const int N = 4;
    double expStateBuff[] =  {0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715};
    double expOutputBuff[] = {1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656};

    auto input = NDArrayFactory::create<double>('c', {bS,K,N});
    auto weights = NDArrayFactory::create<double>('c', {3*K,K});
    auto bias = NDArrayFactory::create<double>('c', {1,2*K});
    auto init = NDArrayFactory::create<double>('c', {bS,K});
    auto mask = NDArrayFactory::create<double>('c', {bS,K});
    auto expState = NDArrayFactory::create<double>('c', {bS,K,N});
    auto expOut = NDArrayFactory::create<double>('c', {bS,K,N});
   
    input.assign(1.5);
    weights.assign(0.5); 
    bias.assign(0.3) ;
    init.assign(1.);
    mask.assign(1.);
    expState.setBuffer(expStateBuff);
    expOut.setBuffer(expOutputBuff);    

    nd4j::ops::sru_old op;
    auto  results = op.execute({&input, &weights, &bias, &init, &mask}, {}, {});
    ASSERT_TRUE(results->size() == 2);    

    auto state  = results->at(0);
    auto output = results->at(1);
    // state->printBuffer();
    expState.printIndexedBuffer("EXP STATE");
    state->printIndexedBuffer("OUT STATE");
    ASSERT_TRUE(expState.equalsTo(state));
    ASSERT_TRUE(expOut.equalsTo(output));
    
    delete results;
}

//////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, sru_test1) {

    const int bS = 2;
    const int K = 3;    
    const int N = 4;
    double expOutputBuff[] = {0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715};
    double expStateBuff[]  = {1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656};

    auto input = NDArrayFactory::create<double>('c', {bS,K,N});
    auto weights = NDArrayFactory::create<double>('c', {3*K,K});
    auto bias = NDArrayFactory::create<double>('c', {2*K});
    auto init = NDArrayFactory::create<double>('c', {bS,K});
    auto mask = NDArrayFactory::create<double>('c', {bS,K});
    auto expState = NDArrayFactory::create<double>('c', {bS,K,N});
    auto expOut = NDArrayFactory::create<double>('c', {bS,K,N});
   
    input.assign(1.5);
    weights.assign(0.5); 
    bias.assign(0.3) ;
    init.assign(1.);
    mask.assign(1.);
    expState.setBuffer(expStateBuff);
    expOut.setBuffer(expOutputBuff);    

    nd4j::ops::sru op;
    auto results = op.execute({&input, &weights, &bias, &init, &mask}, {}, {});
    ASSERT_TRUE(results->size() == 2);    

    auto output = results->at(0);
    auto state  = results->at(1);

    ASSERT_TRUE(expState.equalsTo(state));
    ASSERT_TRUE(expOut.equalsTo(output));
    
    delete results;
}

//////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, sru_logic_test1) {

    const int bS = 2;
    const int K = 3;    
    const int N = 4;
    double expOutputBuff[] = {0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715};
    double expStateBuff[]  = {1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656, 1.090533, 1.174509, 1.252403, 1.324656};

    auto input = NDArrayFactory::create<double>('c', {bS,K,N});
    auto weights = NDArrayFactory::create<double>('c', {3*K,K});
    auto bias = NDArrayFactory::create<double>('c', {1,2*K});
    auto init = NDArrayFactory::create<double>('c', {bS,K});
    auto mask = NDArrayFactory::create<double>('c', {bS,K});
    auto expState = NDArrayFactory::create<double>('c', {bS,K,N});
    auto expOut = NDArrayFactory::create<double>('c', {bS,K,N});
   
    input.assign(1.5);
    weights.assign(0.5); 
    bias.assign(0.3) ;
    init.assign(1.);
    mask.assign(1.);
    expState.setBuffer(expStateBuff);
    expOut.setBuffer(expOutputBuff);    

    nd4j::ops::sru_logic op;
    auto results = op.execute({&input, &weights, &bias, &init, &mask}, {}, {});
    ASSERT_TRUE(results->size() == 2);    

    auto output = results->at(0);
    auto state  = results->at(1);

    ASSERT_TRUE(expState.equalsTo(state));
    ASSERT_TRUE(expOut.equalsTo(output));
    
    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, sru_bp) {

    const int bS = 2;
    const int K = 3;    
    const int N = 4;
    double expGradXBuff[] = {-0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165};    
    double expGradWBuff[] = {0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, 0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215};
    double expGradBBuff[] = {-0.7043748, -0.7043748, -0.7043748, -0.2128962, -0.2128962, -0.2128962};
    double expGradInitBuff[] = {1.1421, 1.1421, 1.1421, 1.1421, 1.1421, 1.1421};
    double stateBuff[] = {0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715};                       

    auto input = NDArrayFactory::create<double>('c', {bS,K,N});
    auto weights = NDArrayFactory::create<double>('c', {3*K,K});
    auto bias = NDArrayFactory::create<double>('c', {1,2*K});
    auto init = NDArrayFactory::create<double>('c', {bS,K});
    auto mask = NDArrayFactory::create<double>('c', {bS,K});
    auto state = NDArrayFactory::create<double>('c', {bS,K,N});
    auto inGradCt = NDArrayFactory::create<double>('c', {bS,K});
    auto inGradH = NDArrayFactory::create<double>('c', {bS,K,N});

    auto expGradX = NDArrayFactory::create<double>('c', {bS,K,N});
    expGradX.setBuffer(expGradXBuff);
    auto expGradW = NDArrayFactory::create<double>('c', {bS,3*K,K});
    expGradW.setBuffer(expGradWBuff);
    auto expGradB = NDArrayFactory::create<double>('c', {1,2*K});
    expGradB.setBuffer(expGradBBuff);
    auto expGradInit = NDArrayFactory::create<double>('c', {bS,K});
    expGradInit.setBuffer(expGradInitBuff);

    input.assign(1.5);
    weights.assign(0.5); 
    bias.assign(0.3) ;    
    mask.assign(1.);
    init.assign(1.);
    state.setBuffer(stateBuff);
    inGradCt.assign(0.5);
    inGradH.assign(0.5);
    
    nd4j::ops::sru_bp bp;
    auto resultsBP = bp.execute({&input, &weights, &bias, &init, &state, &inGradCt, &inGradH, &mask}, {}, {});
    ASSERT_TRUE(resultsBP->size() == 4);    

    auto gradX    = resultsBP->at(0);
    auto gradW    = resultsBP->at(1);
    auto gradB    = resultsBP->at(2);
    auto gradInit = resultsBP->at(3);
    expGradX.printIndexedBuffer("Exp GRAD");
    gradX->printIndexedBuffer("Res GRAD");
    ASSERT_TRUE(expGradX.equalsTo(gradX,1e-4)); 
    ASSERT_TRUE(expGradW.equalsTo(gradW));
    ASSERT_TRUE(expGradB.equalsTo(gradB));
    ASSERT_TRUE(expGradInit.equalsTo(gradInit));
    
    delete resultsBP;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, sru_bp_logic1) {

    const int bS = 2;
    const int K = 3;    
    const int N = 4;
    double expGradXBuff[] = {-0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165, -0.0259303, -0.03869125, -0.0302272, -0.02299165};    
    double expGradWBuff[] = {0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, 0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, 0.42526005,  0.42526005,  0.42526005, -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.5282811 , -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215, -0.15967215};
    double expGradBBuff[] = {-0.7043748, -0.7043748, -0.7043748, -0.2128962, -0.2128962, -0.2128962};
    double expGradInitBuff[] = {1.1421, 1.1421, 1.1421, 1.1421, 1.1421, 1.1421};
    double stateBuff[] = {0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715, 0.847983, 0.874549, 0.896109, 0.913715};                       

    auto input = NDArrayFactory::create<double>('c', {bS,K,N});
    auto weights = NDArrayFactory::create<double>('c', {3*K,K});
    auto bias = NDArrayFactory::create<double>('c', {1,2*K});
    auto init = NDArrayFactory::create<double>('c', {bS,K});
    auto mask = NDArrayFactory::create<double>('c', {bS,K});
    auto state = NDArrayFactory::create<double>('c', {bS,K,N});
    auto inGradCt = NDArrayFactory::create<double>('c', {bS,K});
    auto inGradH = NDArrayFactory::create<double>('c', {bS,K,N});

    auto expGradX = NDArrayFactory::create<double>('c', {bS,K,N});
    expGradX.setBuffer(expGradXBuff);
    auto expGradW = NDArrayFactory::create<double>('c', {bS,3*K,K});
    expGradW.setBuffer(expGradWBuff);
    auto expGradB = NDArrayFactory::create<double>('c', {1,2*K});
    expGradB.setBuffer(expGradBBuff);
    auto expGradInit = NDArrayFactory::create<double>('c', {bS,K});
    expGradInit.setBuffer(expGradInitBuff);

    input.assign(1.5);
    weights.assign(0.5); 
    bias.assign(0.3) ;    
    mask.assign(1.);
    init.assign(1.);
    state.setBuffer(stateBuff);
    inGradCt.assign(0.5);
    inGradH.assign(0.5);
    
    nd4j::ops::sru_bp_logic bp;
    auto resultsBP = bp.execute({&input, &weights, &bias, &init, &state, &inGradCt, &inGradH, &mask}, {}, {});
    ASSERT_TRUE(resultsBP->size() == 4);    

    auto gradX    = resultsBP->at(0);
    auto gradW    = resultsBP->at(1);
    auto gradB    = resultsBP->at(2);
    auto gradInit = resultsBP->at(3);

    ASSERT_TRUE(expGradX.equalsTo(gradX, 1e-4)); 
    ASSERT_TRUE(expGradW.equalsTo(gradW));
    ASSERT_TRUE(expGradB.equalsTo(gradB));
    ASSERT_TRUE(expGradInit.equalsTo(gradInit));
    
    delete resultsBP;
}

//////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, sru_bi_1) {

    const int bS = 2;
    const int K = 3;    
    const int N = 4;
    double expStateBuff[] =  {1.02857, 1.02857, 1.02857, 1.11288, 1.11288, 1.11288, 1.02857, 1.02857, 1.02857, 1.11288, 1.11288, 1.11288, 1.0569, 1.0569, 1.0569, 1.08501, 1.08501, 1.08501, 1.0569, 1.0569, 1.0569, 1.08501, 1.08501, 1.08501, 1.08501, 1.08501, 1.08501, 1.0569, 1.0569, 1.0569, 1.08501, 1.08501, 1.08501, 1.0569, 1.0569, 1.0569, 1.11288, 1.11288, 1.11288, 1.02857, 1.02857, 1.02857, 1.11288, 1.11288, 1.11288, 1.02857, 1.02857, 1.02857};
    double expOutputBuff[] = {0.779265, 0.779265, 0.779265, 0.810752, 0.810752, 0.810752, 0.779265, 0.779265, 0.779265, 0.810752, 0.810752, 0.810752, 0.790317, 0.790317, 0.790317, 0.800804, 0.800804, 0.800804, 0.790317, 0.790317, 0.790317, 0.800804, 0.800804, 0.800804, 0.800804, 0.800804, 0.800804, 0.790317, 0.790317, 0.790317, 0.800804, 0.800804, 0.800804, 0.790317, 0.790317, 0.790317, 0.810752, 0.810752, 0.810752, 0.779265, 0.779265, 0.779265, 0.810752, 0.810752, 0.810752, 0.779265, 0.779265, 0.779265};

    auto input = NDArrayFactory::create<double>('c', {N,bS,2*K});
    auto weights = NDArrayFactory::create<double>('c', {2*K,6*K});
    auto bias = NDArrayFactory::create<double>('c', {1,4*K});
    auto init = NDArrayFactory::create<double>('c', {bS,2*K});
    auto mask = NDArrayFactory::create<double>('c', {bS,2*K});
    auto expState = NDArrayFactory::create<double>('c', {N,bS,2*K});
    auto expOut = NDArrayFactory::create<double>('c', {N,bS,2*K});
   
    input.assign(1.5);    
    weights.assign(0.5); 
    bias.assign(0.3) ;
    init.assign(1.);
    mask.assign(1.);
    expState.setBuffer(expStateBuff);
    expOut.setBuffer(expOutputBuff);    

    nd4j::ops::sru_bi op;
    auto results = op.execute({&input, &weights, &bias, &init, &mask}, {}, {});
    ASSERT_TRUE(results->size() == 2);    

    auto output = results->at(0);
    auto state = results->at(1);
    
    ASSERT_TRUE(expState.equalsTo(state));
    ASSERT_TRUE(expOut.equalsTo(output));
    
    delete results;
}

TEST_F(DeclarableOpsTests1, sru_bi_bp_1) {

    const int bS = 2;
    const int K = 3;    
    const int N = 3;
    double expGradXBuff[] = {0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129, 0.00408129};    
    double expGradInitBuff[] = {1.05121, 1.05121, 1.05121, 1.02676, 1.02676, 1.02676, 1.05121, 1.05121, 1.05121, 1.02676, 1.02676, 1.02676};
    double expGradWBuff[] = {0.02595354,-0.090096 ,-0.00882456,0.02595354,-0.090096 ,-0.0088245, 0.02595354,-0.090096 ,-0.00882456,0.01651665,-0.0559437,-0.0084390, 0.01651665,-0.0559437,-0.00843906,0.01651665,-0.0559437,-0.00843906, 0.02595354,-0.090096 ,-0.00882456,0.02595354,-0.090096 ,-0.0088245, 0.02595354,-0.090096 ,-0.00882456,0.01651665,-0.0559437,-0.0084390, 0.01651665,-0.0559437,-0.00843906,0.01651665,-0.0559437,-0.00843906, 0.02595354,-0.090096 ,-0.00882456,0.02595354,-0.090096 ,-0.0088245, 0.02595354,-0.090096 ,-0.00882456,0.01651665,-0.0559437,-0.0084390, 0.01651665,-0.0559437,-0.00843906,0.01651665,-0.0559437,-0.00843906, 0.02595354,-0.090096 ,-0.00882456,0.02595354,-0.090096 ,-0.0088245, 0.02595354,-0.090096 ,-0.00882456,0.01651665,-0.0559437,-0.0084390, 0.01651665,-0.0559437,-0.00843906,0.01651665,-0.0559437,-0.00843906, 0.02595354,-0.090096 ,-0.00882456,0.02595354,-0.090096 ,-0.0088245, 0.02595354,-0.090096 ,-0.00882456,0.01651665,-0.0559437,-0.0084390, 0.01651665,-0.0559437,-0.00843906,0.01651665,-0.0559437,-0.00843906, 0.02595354,-0.090096 ,-0.00882456,0.02595354,-0.090096 ,-0.0088245, 0.02595354,-0.090096 ,-0.00882456,0.01651665,-0.0559437,-0.0084390, 0.01651665,-0.0559437,-0.00843906,0.01651665,-0.0559437,-0.00843906, 0.02124567,-0.0731508,-0.00868926,0.02124567,-0.0731508,-0.0086892, 0.02124567,-0.0731508,-0.00868926,0.02084955,-0.0712011,-0.0085608, 0.02084955,-0.0712011,-0.00856086,0.02084955,-0.0712011,-0.00856086, 0.02124567,-0.0731508,-0.00868926,0.02124567,-0.0731508,-0.0086892, 0.02124567,-0.0731508,-0.00868926,0.02084955,-0.0712011,-0.0085608, 0.02084955,-0.0712011,-0.00856086,0.02084955,-0.0712011,-0.00856086, 0.02124567,-0.0731508,-0.00868926,0.02124567,-0.0731508,-0.0086892, 0.02124567,-0.0731508,-0.00868926,0.02084955,-0.0712011,-0.0085608, 0.02084955,-0.0712011,-0.00856086,0.02084955,-0.0712011,-0.00856086, 0.02124567,-0.0731508,-0.00868926,0.02124567,-0.0731508,-0.0086892, 0.02124567,-0.0731508,-0.00868926,0.02084955,-0.0712011,-0.0085608, 0.02084955,-0.0712011,-0.00856086,0.02084955,-0.0712011,-0.00856086, 0.02124567,-0.0731508,-0.00868926,0.02124567,-0.0731508,-0.0086892, 0.02124567,-0.0731508,-0.00868926,0.02084955,-0.0712011,-0.0085608, 0.02084955,-0.0712011,-0.00856086,0.02084955,-0.0712011,-0.00856086, 0.02124567,-0.0731508,-0.00868926,0.02124567,-0.0731508,-0.0086892, 0.02124567,-0.0731508,-0.00868926,0.02084955,-0.0712011,-0.0085608, 0.02084955,-0.0712011,-0.00856086,0.02084955,-0.0712011,-0.00856086, 0.01671156,-0.0570699,-0.00856086,0.01671156,-0.0570699,-0.0085608, 0.01671156,-0.0570699,-0.00856086,0.02534988,-0.0880002,-0.0086892, 0.02534988,-0.0880002,-0.00868926,0.02534988,-0.0880002,-0.00868926, 0.01671156,-0.0570699,-0.00856086,0.01671156,-0.0570699,-0.0085608, 0.01671156,-0.0570699,-0.00856086,0.02534988,-0.0880002,-0.0086892, 0.02534988,-0.0880002,-0.00868926,0.02534988,-0.0880002,-0.00868926, 0.01671156,-0.0570699,-0.00856086,0.01671156,-0.0570699,-0.0085608, 0.01671156,-0.0570699,-0.00856086,0.02534988,-0.0880002,-0.0086892, 0.02534988,-0.0880002,-0.00868926,0.02534988,-0.0880002,-0.00868926, 0.01671156,-0.0570699,-0.00856086,0.01671156,-0.0570699,-0.0085608, 0.01671156,-0.0570699,-0.00856086,0.02534988,-0.0880002,-0.0086892, 0.02534988,-0.0880002,-0.00868926,0.02534988,-0.0880002,-0.00868926, 0.01671156,-0.0570699,-0.00856086,0.01671156,-0.0570699,-0.0085608, 0.01671156,-0.0570699,-0.00856086,0.02534988,-0.0880002,-0.0086892, 0.02534988,-0.0880002,-0.00868926,0.02534988,-0.0880002,-0.00868926, 0.01671156,-0.0570699,-0.00856086,0.01671156,-0.0570699,-0.0085608, 0.01671156,-0.0570699,-0.00856086,0.02534988,-0.0880002,-0.0086892, 0.02534988,-0.0880002,-0.00868926,0.02534988,-0.0880002,-0.00868926};
    double expGradBBuff[] = {-0.0734389, -0.0734389, -0.0734389, -0.0717151, -0.0717151, -0.0717151, -0.0734389, -0.0734389, -0.0734389, -0.0717151, -0.0717151, -0.0717151, -0.00869156, -0.00869156, -0.00869156, -0.00856306, -0.00856306, -0.00856306, -0.00869156, -0.00869156, -0.00869156, -0.00856306, -0.00856306, -0.00856306};
    double stateBuff[] = {1.028569, 1.028569, 1.028569, 1.112884, 1.112884, 1.112884, 1.028569, 1.028569, 1.028569, 1.112884, 1.112884, 1.112884, 1.056905, 1.056905, 1.056905, 1.085009, 1.085009, 1.085009, 1.056905, 1.056905, 1.056905, 1.085009, 1.085009, 1.085009, 1.085009, 1.085009, 1.085009, 1.056905, 1.056905, 1.056905, 1.085009, 1.085009, 1.085009, 1.056905, 1.056905, 1.056905, 1.112884, 1.112884, 1.112884, 1.028569, 1.028569, 1.028569, 1.112884, 1.112884, 1.112884, 1.028569, 1.028569, 1.028569};
    
    auto input = NDArrayFactory::create<double>('c', {N,bS,2*K});
    auto weights = NDArrayFactory::create<double>('c', {2*K,6*K});
    auto bias = NDArrayFactory::create<double>('c', {1,4*K});
    auto init = NDArrayFactory::create<double>('c', {bS,2*K});
    auto mask = NDArrayFactory::create<double>('c', {bS,2*K});
    auto state = NDArrayFactory::create<double>('c', {N,bS,2*K});
    auto inGradCt = NDArrayFactory::create<double>('c', {bS,2*K});
    auto inGradH = NDArrayFactory::create<double>('c', {N,bS,2*K});
    
    auto gradBias = NDArrayFactory::create<double>('c', {bS,4*K});
    gradBias.setBuffer(expGradBBuff);

    auto expGradX = NDArrayFactory::create<double>('c', {N,bS,2*K});
    expGradX.setBuffer(expGradXBuff);
    auto expGradW = NDArrayFactory::create<double>('c', {N,2*K,6*K});
    expGradW.setBuffer(expGradWBuff);
    auto expGradB = NDArrayFactory::create<double>('c', {1,4*K});
    gradBias.reduceAlongDimension(reduce::Sum, &expGradB, {0}, false, true);    // [bS x 4K] -> [1 x 4K]
    auto expGradInit = NDArrayFactory::create<double>('c', {bS,2*K});
    expGradInit.setBuffer(expGradInitBuff);

    input.assign(1.5);
    weights.assign(0.5);
    bias.assign(0.3) ;    
    mask.assign(1.);
    init.assign(1.);
    state.setBuffer(stateBuff);
    inGradCt.assign(0.5);
    inGradH.assign(0.5);
    
    nd4j::ops::sru_bi_bp bp;
    auto resultsBP = bp.execute({&input, &weights, &bias, &init, &state, &inGradCt, &inGradH, &mask}, {}, {});
    ASSERT_TRUE(resultsBP->size() == 4);    

    auto gradX    = resultsBP->at(0);
    auto gradW    = resultsBP->at(1);
    auto gradB    = resultsBP->at(2);
    auto gradInit = resultsBP->at(3);

    ASSERT_TRUE(expGradX.equalsTo(gradX)); 
    ASSERT_TRUE(expGradW.equalsTo(gradW));
    ASSERT_TRUE(expGradB.equalsTo(gradB));
    ASSERT_TRUE(expGradInit.equalsTo(gradInit));
    
    delete resultsBP;
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests1, Maxpool2d_bp2) {
    
    int bS=2, iD=1, iH=4,iW=4, oD=3, kH=2,kW=2, sH=1,sW=1, pH=0,pW=0, dH=1,dW=1;
    int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     
    int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;    

    TypeParam epsilonBuff[]  = {6., 7., 8., 10., 11., 12., 14., 15., 16., 22., 23., 24., 26., 27., 28., 30., 31., 32.};
    TypeParam expectedBuff[] = {0., 0., 0., 0.,0., 6., 7., 8.,0.,10.,11.,12.,0.,14.,15.,16.,0., 0., 0., 0.,0.,22.,23.,24.,0.,26.,27.,28.,0.,30.,31.,32.};

    auto input    = NDArrayFactory::create<TypeParam>('c', {bS,iD,iH,iW});
    auto epsilon  = NDArrayFactory::create<TypeParam>('c', {bS,iD,oH,oW});
    auto expected = NDArrayFactory::create<TypeParam>('c', {bS,iD,iH,iW});


    input.linspace(1.);
    epsilon.setBuffer(epsilonBuff);
    expected.setBuffer(expectedBuff);
    
    std::initializer_list<Nd4jLong> argI = {kH,kW, sH,sW, pH,pW, dW,dH, 0, 0, 0};   // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 - dilation Height/Width; 8 - same mode;

    nd4j::ops::maxpool2d_bp op;
    auto results = op.execute({&input, &epsilon}, {}, argI);
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests1, Avgpool2d_bp2) {
    
    int bS=2, iD=1, iH=4,iW=4, oD=3, kH=2,kW=2, sH=1,sW=1, pH=0,pW=0, dH=1,dW=1;
    int oH = (iH - kH - (kH-1)*(dH-1) + 2*pH)/sH + 1;     
    int oW = (iW - kW - (kW-1)*(dW-1) + 2*pW)/sW + 1;    

    TypeParam epsilonBuff[] = {3.5 , 4.5 , 5.5, 7.5 , 8.5 , 9.5, 11.5, 12.5, 13.5, 19.5, 20.5, 21.5, 23.5, 24.5, 25.5, 27.5, 28.5, 29.5};
    TypeParam expectedBuff[] = {0.875, 2., 2.5,  1.375, 2.75 , 6., 7.,  3.75, 4.75 ,10., 11., 5.75, 2.875, 6., 6.5, 3.375, 4.875, 10.,10.5, 5.375, 10.75, 22.,23., 11.75, 12.75, 26.,27., 13.75, 6.875, 14.,14.5, 7.375};

    auto input    = NDArrayFactory::create<TypeParam>('c', {bS,iD,iH,iW});
    auto epsilon  = NDArrayFactory::create<TypeParam>('c', {bS,iD,oH,oW});
    auto expected = NDArrayFactory::create<TypeParam>('c', {bS,iD,iH,iW});


    input.linspace(1.);
    epsilon.setBuffer(epsilonBuff);
    expected.setBuffer(expectedBuff);
    
    std::initializer_list<Nd4jLong> argI = {kH,kW, sH,sW, pH,pW, dW,dH, 1, 1, 0};   

    nd4j::ops::avgpool2d_bp op;
    auto results = op.execute({&input, &epsilon}, {}, argI);
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

TEST_F(DeclarableOpsTests1, ArgMax1) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    x.linspace(1);
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {3});
    exp.assign(4);

    nd4j::ops::argmax op;

    auto result = op.execute({&x}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(DeclarableOpsTests1, ArgMax2) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    x.linspace(1);
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {5});
    exp.assign(2);

    nd4j::ops::argmax op;

    auto result = op.execute({&x}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(DeclarableOpsTests1, ArgMax3) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    auto dim = NDArrayFactory::create<float>('c', {1, 1}, {0.});
    x.linspace(1);
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {5});
    exp.assign(2);

    nd4j::ops::argmax op;

    auto result = op.execute({&x, &dim}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, ArgMax4) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    auto dim = NDArrayFactory::create<float>('c', {1, 1}, {1});
    x.linspace(1);
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {3});
    exp.assign(4);

    nd4j::ops::argmax op;

    auto result = op.execute({&x, &dim}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(DeclarableOpsTests1, ArgMax5) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    auto dim = NDArrayFactory::create<float>('c', {1, 2}, {0, 1});
    x.linspace(1);
    auto exp = NDArrayFactory::create<Nd4jLong>(14);


    nd4j::ops::argmax op;

    auto result = op.execute({&x, &dim}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, ArgMax6) {
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5});
    auto dim = NDArrayFactory::create<float>(-1.f);
    x.linspace(1);


    nd4j::ops::argmax op;

    auto expected = op.execute({&x}, {}, {2});
    ASSERT_EQ(Status::OK(), expected->status());
    auto exp = expected->at(0);


    auto result = op.execute({&x, &dim}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);

    ASSERT_EQ(*exp, *z);

    delete result;
    delete expected;
}


TEST_F(DeclarableOpsTests1, ArgMin1) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    x.linspace(1);
//    auto exp('c', {3, 1});
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {3});
    exp.assign(0.0f);

    nd4j::ops::argmin op;

    auto result = op.execute({&x}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(DeclarableOpsTests1, SquareTests1) {
    auto x = NDArrayFactory::create<float>('c', {3, 5});
    x.linspace(1);

    auto exp = NDArrayFactory::create<float>('c', {3, 5});
    exp.linspace(1);
    exp *= exp;

    nd4j::ops::square op;

    auto result = op.execute({&x}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, OneHotTests_1) {
    auto indices = NDArrayFactory::create<float>('c', {1, 4}, {0.0f, 2.0f, -1.0f, 1.0f});

    auto exp = NDArrayFactory::create<float>('c', {4, 3}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f});

    nd4j::ops::onehot op;

    auto result = op.execute({&indices}, {1.0f, 0.0f}, {-1, 3});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, OneHotTests_2) {
    auto indices = NDArrayFactory::create<float>('c', {2, 2}, {0.f, 2.f, 1.f, -1.f});

    auto exp = NDArrayFactory::create<float>('c', {2, 2, 3}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f});

    nd4j::ops::onehot op;
    auto result = op.execute({&indices}, {1.0f, 0.0f}, {-1, 3});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, OneHotTests_3) {
    auto indices = NDArrayFactory::create<float>('c', {4}, {0.0f, 2.0f, -1.0f, 1.0f});

    auto exp = NDArrayFactory::create<float>('c', {4, 3}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f});

    nd4j::ops::onehot op;

    auto result = op.execute({&indices}, {1.0f, 0.0f}, {-1, 3});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, OneHotTests_4) {
    auto indices = NDArrayFactory::create<float>('c', {4}, {0.0f, 2.0f, -1.0f, 1.0f});
    auto depth = NDArrayFactory::create<float>(3.0f);

    auto exp = NDArrayFactory::create<float>('c', {4, 3}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f});

    nd4j::ops::onehot op;

    auto result = op.execute({&indices, &depth}, {1.0f, 0.0f}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, OneHotTests_5) {
    auto indices = NDArrayFactory::create<float>('c', {4}, {0.0f, 2.0f, -1.0f, 1.0f});
    auto depth = NDArrayFactory::create<float>(3.0f);
    auto on = NDArrayFactory::create<float>(1.0f);
    auto off = NDArrayFactory::create<float>(0.0f);

    auto exp = NDArrayFactory::create<float>('c', {4, 3}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f});

    nd4j::ops::onehot op;

    auto result = op.execute({&indices, &depth, &on, &off}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests1, FillAs_1) {
    auto x = NDArrayFactory::create<float>('c', {2, 2});
    x.assign(117);

    float scalar = 119.f;

    nd4j::ops::fill_as op;
    auto result = op.execute({&x}, {scalar}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(x.isSameShape(result->at(0)));

    ASSERT_NEAR(scalar, result->at(0)->meanNumber().e<float>(0), 1e-5f);

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, LRN1) {
    nd4j::ops::lrn lrn;

    lrn.getOpName();
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_1) {

    float buff1[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12};
    float buff2[]   = {13,14,16,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,16,16,17,18,19,20,21,22,23,24};
    Nd4jLong shape1[]    = {2, 3, 4, 4, 1, 0, 1, 99};
    Nd4jLong shape2[]    = {2, 3, 4, 4, 1, 0, 1, 99};
    Nd4jLong expShape[]  = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shape2, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray input2(buff2, shape2);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input2}, {}, {0});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;

}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_2) {

    float buff1[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12};
    float buff2[]   = {13,14,16,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1,  2,  3,  4, 13, 14, 16, 16, 5,  6,  7,  8, 17, 18, 19, 20, 9, 10, 11, 12, 21, 22, 23, 24};
    Nd4jLong shape1[]    = {2, 3, 4, 4, 1, 0, 1, 99};
    Nd4jLong shape2[]    = {2, 3, 4, 4, 1, 0, 1, 99};
    Nd4jLong expShape[]  = {3, 3, 2, 4, 8, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shape2, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray input2(buff2, shape2);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input2}, {}, {1});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_3) {

    float buff1[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12};
    float buff2[]   = {13,14,16,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,16,16,17,18,19,20,21,22,23,24};
    Nd4jLong shape1[]    = {2, 1, 12, 12, 1, 0, 1, 99};
    Nd4jLong shape2[]    = {2, 1, 12, 12, 1, 0, 1, 99};
    Nd4jLong expShape[]  = {3, 2, 1, 12, 12, 12, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shape2, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray input2(buff2, shape2);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input2}, {}, {0});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_4) {

    float buff1[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12};
    float buff2[]   = {13,14,16,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,16,16,17,18,19,20,21,22,23,24};
    Nd4jLong shape1[]    = {2, 1, 12, 12, 1, 0, 1, 99};
    Nd4jLong shape2[]    = {2, 1, 12, 12, 1, 0, 1, 99};
    Nd4jLong expShape[]  = {3, 1, 2, 12, 24, 12, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shape2, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray input2(buff2, shape2);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input2}, {}, {1});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_5) {

    float buff1[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12};
    float buff2[]   = {13,14,16,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,16,16,17,18,19,20,21,22,23,24};
    Nd4jLong shape1[]    = {2, 12, 1, 1,  1, 0, 1, 99};
    Nd4jLong shape2[]    = {2, 12, 1, 1,  1, 0, 1, 99};
    Nd4jLong expShape[]  = {3, 2, 12, 1, 12, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shape2, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray input2(buff2, shape2);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input2}, {}, {0});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_6) {

    float buff1[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12};
    float buff2[]   = {13,14,16,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1 ,13 ,2 ,14 ,3 ,16 ,4 ,16 ,5 ,17 ,6 ,18 ,7 ,19 ,8 ,20 ,9 ,21 ,10 ,22 ,11 ,23 ,12 ,24};
    Nd4jLong shape1[]    = {2, 12, 1, 1, 12, 0, 1, 99};
    Nd4jLong shape2[]    = {2, 12, 1, 1, 12, 0, 1, 99};
    Nd4jLong expShape[]  = {3, 12, 2, 1, 2, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(shape2, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray input2(buff2, shape2);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input2}, {}, {1});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_7) {

    float buff1[]   = {1};    
    float expBuff[] = {1, 1, 1};
    Nd4jLong shape1[]    = {2, 1, 1, 1, 1, 0, 1, 99};    
    Nd4jLong expShape[]  = {3, 3, 1, 1, 1, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input1, &input1}, {}, {0});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_8) {

    float buff1[]   = {1};    
    float expBuff[] = {1, 1, 1};
    Nd4jLong shape1[]    = {1, 1, 1, 0, 1, 99};    
    Nd4jLong expShape[]  = {2, 3, 1, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input1, &input1}, {}, {0});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_9) {

    float buff1[]   = {1};    
    float expBuff[] = {1, 1, 1};
    Nd4jLong shape1[]    = {2, 1, 1, 1, 1, 0, 1, 99};    
    Nd4jLong expShape[]  = {3, 1, 3, 1, 3, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input1, &input1}, {}, {1});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Stack_10) {

    float buff1[]   = {1};    
    float expBuff[] = {1, 1, 1};
    Nd4jLong shape1[]    = {1, 1, 1, 0, 1, 99};    
    Nd4jLong expShape[]  = {2, 1, 3, 3, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input1, &input1}, {}, {1});
    auto output = results->at(0);

    //expected.printShapeInfo("exp");
    //output->printShapeInfo("out");

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}

TEST_F(DeclarableOpsTests1, Stack_11) {

    float buff1[]   = {1};
    float expBuff[] = {1, 1, 1};
    Nd4jLong shape1[]    = {1, 1, 1, 0, 1, 99};
    Nd4jLong expShape[]  = {2, 3, 1, 1, 1, 0, 1, 99};
    ArrayOptions::setDataType(shape1, nd4j::DataType::FLOAT32);
    ArrayOptions::setDataType(expShape, nd4j::DataType::FLOAT32);

    NDArray input1(buff1, shape1);
    NDArray expected(expBuff, expShape);

    nd4j::ops::stack op;
    auto results = op.execute({&input1, &input1, &input1}, {}, {});
    auto output = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete results;
}


TEST_F(DeclarableOpsTests1, Test_Range_Integer_1) {
    auto exp = NDArrayFactory::create<int>('c', {4});
    exp.linspace(1);

    nd4j::ops::range op;

    auto result = op.execute({}, {}, {1, 5, 1});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_EQ(1, result->size());

    auto array = result->at(0);    
    array->printIndexedBuffer("Range integer 1");
    ASSERT_TRUE(exp.isSameShape(array));
    ASSERT_TRUE(exp.equalsTo(array));

    delete result;
}


TEST_F(DeclarableOpsTests1, Test_Range_Integer_2) {
    auto exp = NDArrayFactory::create<float>('c', {4});
    exp.linspace(1);

    auto start = NDArrayFactory::create<float>('c', {1, 1});
    auto stop = NDArrayFactory::create<float>('c', {1, 1});
    auto step = NDArrayFactory::create<float>('c', {1, 1});
    start.p(0, 1.f);
    stop.p(0, 5.f);
    step.p(0, 1.f);

    nd4j::ops::range op;

    auto result = op.execute({&start, &stop, &step}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_EQ(1, result->size());

    auto array = result->at(0);    

    ASSERT_TRUE(exp.isSameShape(array));
    ASSERT_TRUE(exp.equalsTo(array));

    delete result;
}


TEST_F(DeclarableOpsTests1, Test_Range_Integer_3) {
    auto exp = NDArrayFactory::create<float>('c', {4});
    exp.linspace(1);

    nd4j::ops::range op;

    auto result = op.execute({}, {1.f, 5.f, 1.f}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_EQ(1, result->size());

    auto array = result->at(0);

    ASSERT_TRUE(exp.isSameShape(array));
    ASSERT_TRUE(exp.equalsTo(array));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test1) {
    auto input = NDArrayFactory::create<double>('c', {3, 3}, {-1, 1, -2, 2, -3, 3, -4, 4, 5});
    auto expOutput = NDArrayFactory::create<double>('c', {3, 3}, {1.14195199e-01, 8.43794734e-01, 4.20100661e-02, 2.68454951e-01, 1.80883523e-03, 7.29736214e-01, 9.02116571e-05, 2.68917160e-01, 7.30992629e-01});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test2) {
    auto input = NDArrayFactory::create<double>('c', {3, 3, 3}, {-1, 1, -2, 2, -3, 3, -4, 4, -5,5 ,-6,6, -7,7, -8,8, -9,9, -10,10, -11,11, -12,12, -13,13, 14});
    auto expOutput = NDArrayFactory::create<double>('c', {3, 3, 3}, {4.73142e-02,   4.73847e-02,   6.69062e-03, 9.50330e-01,   8.67881e-04,   9.92976e-01, 2.35563e-03,   9.51747e-01,   3.33106e-04, 4.74259e-02,   2.26032e-06,   4.74259e-02, 2.91395e-07,   9.99998e-01,   3.94360e-08, 9.52574e-01,   1.12535e-07,   9.52574e-01, 7.58256e-10,   4.74259e-02,   1.22325e-11, 1.00000e+00,   1.32293e-11,   1.19203e-01, 3.77513e-11,   9.52574e-01,   8.80797e-01});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {1}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test3) {
    auto input = NDArrayFactory::create<double>('c', {3, 3, 3}, {-1, 1, -2, 2, -3, 3, -4, 4, -5,5 ,-6,6, -7,7, -8,8, -9,9, -10,10, -11,11, -12,12, -13,13, 14});
    auto expOutput = NDArrayFactory::create<double>('c', {3, 3, 3}, {2.47262e-03,   1.23395e-04,   3.35350e-04, 1.23395e-04,   4.53979e-05,   1.23395e-04, 6.14417e-06,   1.23395e-04,   5.56530e-09, 9.97527e-01,   1.12521e-07,   9.99665e-01, 1.52281e-08,   9.99955e-01,   2.06090e-09, 9.99994e-01,   2.78912e-10,   6.69285e-03, 3.05146e-07,   9.99876e-01,   4.13855e-08, 9.99877e-01,   5.60254e-09,   9.99877e-01, 7.58251e-10,   9.99877e-01,   9.93307e-01});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {0}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test4) {
    auto input = NDArrayFactory::create<double>('c', {1, 5}, {-1, 1, -2, 2, 3});
    auto expOutput = NDArrayFactory::create<double>('c', {1, 5}, {0.01198,  0.08855,  0.00441,  0.24072,  0.65434});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {1}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test5) {
    auto input = NDArrayFactory::create<double>('c', {1, 5}, {-1, 1, -2, 2, 3});
    auto expOutput = NDArrayFactory::create<double>('c', {1, 5}, {1,  1,  1,  1,  1});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {0}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test6) {
    auto input = NDArrayFactory::create<double>('c', {5, 1}, {-1, 1, -2, 2, 3});
    auto expOutput = NDArrayFactory::create<double>('c', {5, 1}, {0.01198,  0.08855,  0.00441,  0.24072,  0.65434});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {0}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test7) {
    auto input = NDArrayFactory::create<double>('c', {5, 1}, {-1, 1, -2, 2, 3});
    auto expOutput = NDArrayFactory::create<double>('c', {5, 1}, {1,  1,  1,  1,  1});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {1}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, softmax_test8) {
    auto input = NDArrayFactory::create<double>('c', {5}, {-1, 1, -2, 2, 3});
    auto expOutput = NDArrayFactory::create<double>('c', {5}, {0.01198,  0.08855,  0.00441,  0.24072,  0.65434});

    nd4j::ops::softmax op;
    auto results = op.execute({&input}, {}, {}, {}, false, nd4j::DataType::DOUBLE);
    auto z = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expOutput.isSameShape(z));
    ASSERT_TRUE(expOutput.equalsTo(z));    

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Test_Stack_Edge_1) {
    float inBuff[]  = {1.0f, 2.0f, 3.0f};
    float expBuff[] = {1.0f, 2.0f, 3.0f};

    auto input = NDArrayFactory::create<float>(inBuff, 'c', {1, 3});

    auto exp = NDArrayFactory::create<float>(expBuff, 'c', {1, 1, 3});

    nd4j::ops::stack op;

    auto result = op.execute({&input}, {}, {0});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);    

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Test_Stack_Edge_2) {
    float inBuff[]  = {1.0f, 2.0f, 3.0f};
    float expBuff[] = {1.0f, 2.0f, 3.0f};

    auto input = NDArrayFactory::create<float>(inBuff, 'c', {1, 1, 3});

    auto exp = NDArrayFactory::create<float>(expBuff, 'c', {1, 1, 1, 3});

    nd4j::ops::stack op;

    auto result = op.execute({&input}, {}, {0});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Test_Stack_Edge_3) {
    float inBuff[]  = {1.0f, 2.0f, 3.0f};
    float expBuff[] = {1.0f, 2.0f, 3.0f};

    auto input = NDArrayFactory::create<float>(inBuff, 'c', {1, 3});

    auto exp = NDArrayFactory::create<float>(expBuff, 'c', {1, 1, 3});

    nd4j::ops::stack op;

    auto result = op.execute({&input}, {}, {1});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    //z->printShapeInfo();

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_1 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {24., 23., 22., 21., 20., 19., 18., 17., 16., 15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {0,1,2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_2 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {}, {}, true);

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(&input));
    ASSERT_TRUE(expected.equalsTo(&input));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_3 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 24., 23., 22., 21., 20., 19., 18., 17., 16., 15., 14., 13.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {1,2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_4 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {16,15,14,13,    20,19,18,17,       24,23,22,21,    4,3,2,1,    8,7,6,5,      12,11,10,9,};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {0,2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_5 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {21., 22., 23., 24., 17., 18., 19., 20., 13., 14., 15., 16., 9., 10., 11., 12., 5., 6., 7., 8., 1., 2., 3., 4.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {0,1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_6 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {4., 3., 2., 1., 8., 7., 6., 5., 12., 11., 10., 9., 16., 15., 14., 13., 20., 19., 18., 17., 24., 23., 22., 21.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {2}, {}, true);

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(&input));
    ASSERT_TRUE(expected.equalsTo(&input));

    delete results;
}


////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_7 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {9., 10., 11., 12., 5., 6., 7., 8., 1., 2., 3., 4., 21., 22., 23., 24., 17., 18., 19., 20., 13., 14., 15., 16.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}



//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_8 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 24., 23., 22., 21., 20., 19., 18., 17., 16., 15., 14., 13.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {2,1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_9 ) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    float expBuff[] = {13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.};
    Nd4jLong shapeInfo[] = {3, 2, 3, 4, 12, 4, 1, 0, 1, 99};
    ArrayOptions::setDataType(shapeInfo, nd4j::DataType::FLOAT32);

    NDArray input(inBuff, shapeInfo);
    NDArray expected(expBuff, shapeInfo);
    NDArray output(shapeInfo);

    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

TEST_F(DeclarableOpsTests1, Reverse_10 ) {
    auto x = NDArrayFactory::create<double>('c', {4, 3}, {1.5375735, 0.1592365, 0.09966054, 0.677872, 1.144433, -1.0355669, 0.48456487, -0.67863184, 0.85020787, 0.13950661, 0.20998026, -1.1660044});
    auto i = NDArrayFactory::create<int>('c', {1}, {-1});
    auto e = NDArrayFactory::create<double>('c', {4, 3}, {0.09966054, 0.1592365, 1.5375735,  -1.0355669, 1.144433, 0.677872,   0.85020787, -0.67863184, 0.48456487,  -1.1660044, 0.20998026, 0.13950661});

    nd4j::ops::reverse op;
    auto result = op.execute({&x, &i}, {}, {}, {}, false, nd4j::DataType::DOUBLE);

    auto z = result->at(0);

    ASSERT_TRUE(e.isSameShape(z));
    ASSERT_TRUE(e.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_11 ) {


    auto input = NDArrayFactory::create<float>('c', {2,3,4});
    auto expected = NDArrayFactory::create<float>('c', {2,3,4}, {24., 23., 22., 21., 20., 19., 18., 17., 16., 15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1.});

    input.linspace(1);
    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {0, 1, 2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_12 ) {


    auto input = NDArrayFactory::create<float>({0.f, 1.f, 2.f, 3.f, 4.f});
    auto expected = NDArrayFactory::create<float>({4.f, 3.f, 2.f, 1.f, 0.f});

    //input.linspace(1);
    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    //result->printIndexedBuffer("Result reverse");
    //expected.printIndexedBuffer("Expected reverse");
    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_13 ) {


    auto input = NDArrayFactory::create<float>({0.f, 1.f, 2.f, 3.f, 4.f});
    auto expected = NDArrayFactory::create<float>({4.f, 3.f, 2.f, 1.f, 0.f});

    //input.linspace(1);
    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {-1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests1, Reverse_14 ) {


    auto input = NDArrayFactory::create<double>({0.f, 1.f, 2.f, 3.f, 4.f});
    auto expected = NDArrayFactory::create<double>({0.f, 1.f, 2.f, 3.f, 4.f});

    //input.linspace(1);
    nd4j::ops::reverse op;
    auto results = op.execute({&input}, {}, {}, {}, false, nd4j::DataType::DOUBLE);

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

////////////////////////////////////////////////////////////////////
// CONSTANT mode 2D
TEST_F(DeclarableOpsTests1, Pad_1) {

    float inBuff[]  = {1,2,3,4,5,6};
    int padBuff[] = {1,1,2,2};
    float expBuff[] = {0,0,0,0,0,0,0, 0,0,1,2,3,0,0, 0,0,4,5,6,0,0, 0,0,0,0,0,0,0};    

    auto input    = NDArrayFactory::create<float>(inBuff,  'c', {2,3});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {2,2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4,7});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}


////////////////////////////////////////////////////////////////////
// REFLECT mode 2D
TEST_F(DeclarableOpsTests1, Pad_2) {

    float inBuff[]  = {1,2,3,4,5,6};
    int padBuff[] = {1,1,2,2};
    float expBuff[] = {6,5,4,5,6,5,4, 3,2,1,2,3,2,1, 6,5,4,5,6,5,4, 3,2,1,2,3,2,1};    

    auto input    = NDArrayFactory::create<float>(inBuff,  'c', {2,3});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {2,2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4,7});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}


////////////////////////////////////////////////////////////////////
// SYMMETRIC mode 2D
TEST_F(DeclarableOpsTests1, Pad_3) {

    float inBuff[]  = {1,2,3,4,5,6};
    int padBuff[] = {1,1,2,2};
    float expBuff[] = {2,1,1,2,3,3,2, 2,1,1,2,3,3,2, 5,4,4,5,6,6,5, 5,4,4,5,6,6,5};    

    auto input    = NDArrayFactory::create<float>(inBuff,  'c', {2,3});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {2,2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4,7});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}


////////////////////////////////////////////////////////////////////
// CONSTANT mode 3D
TEST_F(DeclarableOpsTests1, Pad_4) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    int padBuff[] = {1,1,2,2,2,2};
    float expBuff[] = {0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 1, 2, 3,0,0,0,0, 4, 5, 6,0,0,0,0, 7, 8, 9,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0,10,11,12,0,0,0,0,13,14,15,0,0,0,0,16,17,18,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0,0,0, 0, 0, 0,0,0};

    auto input    = NDArrayFactory::create<float>(inBuff,  'c', {2,3,3});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {3,2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4,7,7});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}



////////////////////////////////////////////////////////////////////
// REFLECT mode 3D
TEST_F(DeclarableOpsTests1, Pad_5) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    int padBuff[] = {1,1,2,2,2,2};
    float expBuff[] = {18,17,16,17,18,17,16, 15,14,13,14,15,14,13, 12,11,10,11,12,11,10, 15,14,13,14,15,14,13, 18,17,16,17,18,17,16, 15,14,13,14,15,14,13, 12,11,10,11,12,11,10, 9, 8, 7, 8, 9, 8, 7, 6, 5, 4, 5, 6, 5, 4, 3, 2, 1, 2, 3, 2, 1, 6, 5, 4, 5, 6, 5, 4, 9, 8, 7, 8, 9, 8, 7, 6, 5, 4, 5, 6, 5, 4, 3, 2, 1, 2, 3, 2, 1, 18,17,16,17,18,17,16, 15,14,13,14,15,14,13, 12,11,10,11,12,11,10, 15,14,13,14,15,14,13, 18,17,16,17,18,17,16, 15,14,13,14,15,14,13, 12,11,10,11,12,11,10, 9, 8, 7, 8, 9, 8, 7, 6, 5, 4, 5, 6, 5, 4, 3, 2, 1, 2, 3, 2, 1, 6, 5, 4, 5, 6, 5, 4, 9, 8, 7, 8, 9, 8, 7, 6, 5, 4, 5, 6, 5, 4, 3, 2, 1, 2, 3, 2, 1};                      
    auto input    = NDArrayFactory::create<float>(inBuff,  'c', {2,3,3});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {3,2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4,7,7});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}


////////////////////////////////////////////////////////////////////
// SYMMETRIC mode 3D
TEST_F(DeclarableOpsTests1, Pad_6) {

    float inBuff[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    int padBuff[] = {1,1,2,2,2,2};
    float expBuff[] = {5, 4, 4, 5, 6, 6, 5, 2, 1, 1, 2, 3, 3, 2, 2, 1, 1, 2, 3, 3, 2, 5, 4, 4, 5, 6, 6, 5, 8, 7, 7, 8, 9, 9, 8, 8, 7, 7, 8, 9, 9, 8, 5, 4, 4, 5, 6, 6, 5, 5, 4, 4, 5, 6, 6, 5, 2, 1, 1, 2, 3, 3, 2, 2, 1, 1, 2, 3, 3, 2, 5, 4, 4, 5, 6, 6, 5, 8, 7, 7, 8, 9, 9, 8, 8, 7, 7, 8, 9, 9, 8, 5, 4, 4, 5, 6, 6, 5, 14,13,13,14,15,15,14, 11,10,10,11,12,12,11, 11,10,10,11,12,12,11, 14,13,13,14,15,15,14, 17,16,16,17,18,18,17, 17,16,16,17,18,18,17, 14,13,13,14,15,15,14, 14,13,13,14,15,15,14, 11,10,10,11,12,12,11, 11,10,10,11,12,12,11, 14,13,13,14,15,15,14, 17,16,16,17,18,18,17, 17,16,16,17,18,18,17, 14,13,13,14,15,15,14};

    auto input    = NDArrayFactory::create<float>(inBuff,  'c', {2,3,3});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {3,2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4,7,7});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

////////////////////////////////////////////////////////////////////
// CONSTANT mode 4D
TEST_F(DeclarableOpsTests1, Pad_7)
{

    float inBuff[] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int padBuff[] = {1, 1, 1, 1, 1, 1, 1, 1};
    float expBuff[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 0, 0, 15, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto input = NDArrayFactory::create<float>(inBuff, 'c', {2, 2, 2, 2});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {4, 2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4, 4, 4, 4});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto *result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

////////////////////////////////////////////////////////////////////
// REFLECT mode 4D
TEST_F(DeclarableOpsTests1, Pad_8)
{

    float inBuff[] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int padBuff[] = {1, 1, 1, 1, 1, 1, 1, 1};
    float expBuff[] = {16, 15, 16, 15, 14, 13, 14, 13, 16, 15, 16, 15, 14, 13, 14, 13, 12, 11, 12, 11, 10, 9, 10, 9, 12, 11, 12, 11, 10, 9, 10, 9, 16, 15, 16, 15, 14, 13, 14, 13, 16, 15, 16, 15, 14, 13, 14, 13, 12, 11, 12, 11, 10, 9, 10, 9, 12, 11, 12, 11, 10, 9, 10, 9, 8, 7, 8, 7, 6, 5, 6, 5, 8, 7, 8, 7, 6, 5, 6, 5, 4, 3, 4, 3, 2, 1, 2, 1, 4, 3, 4, 3, 2, 1, 2, 1, 8, 7, 8, 7, 6, 5, 6, 5, 8, 7, 8, 7, 6, 5, 6, 5, 4, 3, 4, 3, 2, 1, 2, 1, 4, 3, 4, 3, 2, 1, 2, 1, 16, 15, 16, 15, 14, 13, 14, 13, 16, 15, 16, 15, 14, 13, 14, 13, 12, 11, 12, 11, 10, 9, 10, 9, 12, 11, 12, 11, 10, 9, 10, 9, 16, 15, 16, 15, 14, 13, 14, 13, 16, 15, 16, 15, 14, 13, 14, 13, 12, 11, 12, 11, 10, 9, 10, 9, 12, 11, 12, 11, 10, 9, 10, 9, 8, 7, 8, 7, 6, 5, 6, 5, 8, 7, 8, 7, 6, 5, 6, 5, 4, 3, 4, 3, 2, 1, 2, 1, 4, 3, 4, 3, 2, 1, 2, 1, 8, 7, 8, 7, 6, 5, 6, 5, 8, 7, 8, 7, 6, 5, 6, 5, 4, 3, 4, 3, 2, 1, 2, 1, 4, 3, 4, 3, 2, 1, 2, 1};    
    auto input = NDArrayFactory::create<float>(inBuff, 'c', {2, 2, 2, 2});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {4, 2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4, 4, 4, 4});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto *result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

//////////////////////////////////////////////////////////////////
// SYMMETRIC mode 4D 
TEST_F(DeclarableOpsTests1, Pad_9)
{

    float inBuff[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int padBuff[] = {1, 1, 1, 1, 1, 1, 1, 1};
    float expBuff[] = {1, 1, 2, 2, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 1, 1, 2, 2, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 5, 5, 6, 6, 5, 5, 6, 6, 7, 7, 8, 8, 7, 7, 8, 8, 5, 5, 6, 6, 5, 5, 6, 6, 7, 7, 8, 8, 7, 7, 8, 8, 1, 1, 2, 2, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 1, 1, 2, 2, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 5, 5, 6, 6, 5, 5, 6, 6, 7, 7, 8, 8, 7, 7, 8, 8, 5, 5, 6, 6, 5, 5, 6, 6, 7, 7, 8, 8, 7, 7, 8, 8, 9, 9, 10, 10, 9, 9, 10, 10, 11, 11, 12, 12, 11, 11, 12, 12, 9, 9, 10, 10, 9, 9, 10, 10, 11, 11, 12, 12, 11, 11, 12, 12, 13, 13, 14, 14, 13, 13, 14, 14, 15, 15, 16, 16, 15, 15, 16, 16, 13, 13, 14, 14, 13, 13, 14, 14, 15, 15, 16, 16, 15, 15, 16, 16, 9, 9, 10, 10, 9, 9, 10, 10, 11, 11, 12, 12, 11, 11, 12, 12, 9, 9, 10, 10, 9, 9, 10, 10, 11, 11, 12, 12, 11, 11, 12, 12, 13, 13, 14, 14, 13, 13, 14, 14, 15, 15, 16, 16, 15, 15, 16, 16, 13, 13, 14, 14, 13, 13, 14, 14, 15, 15, 16, 16, 15, 15, 16, 16};
    auto input = NDArrayFactory::create<float>(inBuff, 'c', {2, 2, 2, 2});
    auto paddings = NDArrayFactory::create<int>(padBuff, 'c', {4, 2});
    auto expected = NDArrayFactory::create<float>(expBuff, 'c', {4, 4, 4, 4});

    nd4j::ops::pad op;
    auto results = op.execute({&input, &paddings}, {}, {2});

    ASSERT_EQ(ND4J_STATUS_OK, results->status());

    auto *result = results->at(0);
    // result->printIndexedBuffer();

    ASSERT_TRUE(expected.isSameShapeStrict(result));
    ASSERT_TRUE(expected.equalsTo(result));

    delete results;
}

TEST_F(DeclarableOpsTests1, Test_Expose_1) {
    auto input0 = NDArrayFactory::create<float>('c', {2, 3}, {1, 2, 3, 6, 5, 4});
    auto input1 = NDArrayFactory::create<float>('c', {2, 3}, {3, 2, 1, 4, 5, 6});

    nd4j::ops::expose op;

    auto result = op.execute({&input0, &input1}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z0 = result->at(0);
    auto z1 = result->at(1);

    ASSERT_TRUE(input0.equalsTo(z0));
    ASSERT_TRUE(input1.equalsTo(z1));

    delete result;
}

TEST_F(DeclarableOpsTests1, Test_Expose_2) {
    auto list = new NDArrayList(0, true);

    auto var = new Variable(nullptr, "arraylist", -1, 0);
    var->setNDArrayList(list);

    VariableSpace variableSpace;
    variableSpace.putVariable(-1, var);
    variableSpace.trackList(list);

    Context block(1, &variableSpace);
    block.pickInput(-1);

    nd4j::ops::expose op;
    auto result = op.execute(&block);

    ASSERT_EQ(ND4J_STATUS_OK, result);
    ASSERT_TRUE(variableSpace.hasVariable(1));

    auto var1 = variableSpace.getVariable(1);

    ASSERT_EQ(var->variableType(), var1->variableType());

    auto list1 = var1->getNDArrayList();

    ASSERT_TRUE(list == list1);

}


