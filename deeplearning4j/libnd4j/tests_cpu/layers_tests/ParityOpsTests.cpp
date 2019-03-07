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
// Created by raver119 on 12.10.2017.
//

#include "testlayers.h"
#include <NDArray.h>
#include <ops/declarable/CustomOperations.h>


using namespace nd4j;
using namespace nd4j::ops;

class ParityOpsTests : public testing::Test {
public:

};


TEST_F(ParityOpsTests, TestZeroAs1) {
    auto x = NDArrayFactory::create<float>('c', {10, 10});
    x.assign(1.0);

    auto exp = NDArrayFactory::create<float>('c', {10, 10});
    exp.assign(0.0f);

    nd4j::ops::zeros_as op;

    auto result = op.execute({&x}, {}, {});

    auto z = result->at(0);

    ASSERT_TRUE(z->isSameShape(&x));
    ASSERT_TRUE(z->equalsTo(&exp));

    delete result;
}

TEST_F(ParityOpsTests, TestMaximum1) {
    auto x = NDArrayFactory::create<float>('c', {10, 10});
    x.assign(1.0);

    auto y = NDArrayFactory::create<float>('c', {10, 10});
    y.assign(2.0);

    nd4j::ops::maximum op;

    auto result = op.execute({&x, &y}, {}, {});

    auto z = result->at(0);

    ASSERT_TRUE(y.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, TestMinimum1) {
    auto x = NDArrayFactory::create<float>('c', {10, 10});
    x.assign(1.0f);

    auto y = NDArrayFactory::create<float>('c', {10, 10});
    y.assign(-2.0f);


    nd4j::ops::minimum op;

    auto result = op.execute({&x, &y}, {}, {});

    auto z = result->at(0);

    ASSERT_TRUE(y.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, TestTear1) {
    auto input = NDArrayFactory::create<float>('c', {10, 5});
    auto tads = input.allTensorsAlongDimension({1});
    for (int e = 0; e < tads->size(); e++) {
        ASSERT_EQ(5, tads->at(e)->lengthOf());
        tads->at(e)->assign((float) e + 1);
    }

    nd4j::ops::tear op;

    auto result = op.execute({&input}, {}, {1});

    ASSERT_EQ(10, result->size());

    for (int e = 0; e < result->size(); e++)
        ASSERT_TRUE(tads->at(e)->equalsTo(result->at(e)));

    delete result;
    delete tads;
}

TEST_F(ParityOpsTests, TestUnstack1) {
    auto input = NDArrayFactory::create<float>('c', {10, 5});
    auto tads = input.allTensorsAlongDimension({1});
    for (int e = 0; e < tads->size(); e++) {
        ASSERT_EQ(5, tads->at(e)->lengthOf());
        tads->at(e)->assign((float) e + 1);
    }

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {0});

    ASSERT_EQ(10, result->size());

    // result->at(0)->printShapeInfo("rz");
    // tads->at(0)->printShapeInfo("re");

    for (int e = 0; e < result->size(); e++)
        ASSERT_TRUE(tads->at(e)->equalsTo(result->at(e)));

    delete result;
    delete tads;
}



TEST_F(ParityOpsTests, TestUnstack2) {
    auto input = NDArrayFactory::create<float>('c', {5,2,6});
    auto tads = input.allTensorsAlongDimension({0,1});
    for (int e = 0; e < tads->size(); e++) {
        ASSERT_EQ(10, tads->at(e)->lengthOf());
        tads->at(e)->assign((float) e + 1);
    }

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {2});

    ASSERT_EQ(6, result->size());

    for (int e = 0; e < result->size(); e++)
        ASSERT_TRUE(tads->at(e)->equalsTo(result->at(e)));

    delete result;
    delete tads;
}

TEST_F(ParityOpsTests, TestUnstack3) { 
    auto input = NDArrayFactory::create<float>('c', {3,2,3});
    auto exp = NDArrayFactory::create<float>('c', {3, 2}, {1.f, 4., 7., 10.f, 13.f,  16.f});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {2});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, TestUnstack4) { 
    auto input = NDArrayFactory::create<float>('c', {3,2,3});
    auto exp = NDArrayFactory::create<float>('c', {3, 3}, { 1, 2, 3, 7, 8, 9, 13, 14, 15.});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {1});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, TestUnstack5) { 
    auto input = NDArrayFactory::create<float>('c', {3,2,3});
    auto exp = NDArrayFactory::create<float>('c', {2, 3}, { 1, 2, 3, 4, 5, 6});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {0});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, TestUnstack6) { 
    auto input = NDArrayFactory::create<float>('c', {1, 1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 1}, {1});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {0});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, TestUnstack7) { 
    auto input = NDArrayFactory::create<float>('c', {1, 1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 1}, {1});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {1});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, TestUnstack8) { 
    auto input = NDArrayFactory::create<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1}, {1});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {0});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, TestUnstack9) {
    auto input = NDArrayFactory::create<float>('c', {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1}, {1});
    input.linspace(1);

    nd4j::ops::unstack op;

    auto result = op.execute({&input}, {}, {1});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, ExpandDimsTest1) {
    auto input = NDArrayFactory::create<float>('c', {5, 5});
    input.linspace(1);
    auto reshaped = input.reshape('c', {5, 1, 5});

    nd4j::ops::expand_dims op;
    auto result = op.execute({&input}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(reshaped->isSameShape(z));
    ASSERT_TRUE(reshaped->equalsTo(z));

    delete result;
    delete reshaped;

}


TEST_F(ParityOpsTests, ExpandDimsTest2) {
    auto input = NDArrayFactory::create<float>('c', {3, 4});
    input.linspace(1);
    auto reshaped = input.reshape('c', {1, 3, 4});

    nd4j::ops::expand_dims op;
    auto result = op.execute({&input}, {}, {0});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(reshaped->isSameShape(z));
    ASSERT_TRUE(reshaped->equalsTo(z));

    delete result;
    delete reshaped;

}


TEST_F(ParityOpsTests, ExpandDimsTest3) {
    auto input = NDArrayFactory::create<float>('c', {3, 4});
    input.linspace(1);
    auto reshaped = input.reshape('c', {3, 1, 4});

    nd4j::ops::expand_dims op;
    auto result = op.execute({&input}, {}, {-2});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(reshaped->isSameShape(z));
    ASSERT_TRUE(reshaped->equalsTo(z));

    delete result;
    delete reshaped;

}

TEST_F(ParityOpsTests, ExpandDimsTest4) {
    auto input = NDArrayFactory::create<float>('c', {3, 4});
    input.linspace(1);
    auto reshaped = input.reshape('c', {1, 3, 4});

    nd4j::ops::expand_dims op;
    auto result = op.execute({&input}, {}, {-3});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(reshaped->isSameShape(z));
    ASSERT_TRUE(reshaped->equalsTo(z));

    delete result;
    delete reshaped;

}


TEST_F(ParityOpsTests, Test_Shape_1) {
    auto x = NDArrayFactory::create<float>('c', {3, 4, 5, 6});
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {4}, {3, 4, 5, 6});

    nd4j::ops::shape_of op;
    auto result = op.execute({&x}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    z->printShapeInfo("z shape");
    z->printIndexedBuffer(" z buffr");

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, Test_Equals_1) {
    auto x = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {1, 5}, {1, 0, 3, 0, 5});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {1, 0, 1, 0, 1});

    nd4j::ops::equals op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false, nd4j::DataType::BOOL);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, Test_NotEquals_1) {
    auto x = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {1, 5}, {1, 0, 3, 0, 5});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {0, 1, 0, 1, 0});

    nd4j::ops::not_equals op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false, nd4j::DataType::BOOL);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Less_1) {
    auto x = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {1, 5}, {5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {1, 1, 0, 0, 0});

    nd4j::ops::less op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false, nd4j::DataType::BOOL);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_LessEquals_1) {
    auto x = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {1, 5}, {5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {1, 1, 1, 0, 0});

    nd4j::ops::less_equal op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false, nd4j::DataType::BOOL);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_GreaterEquals_1) {
    auto x = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {1, 5}, {5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {0, 0, 1, 1, 1});

    nd4j::ops::greater_equal op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false, nd4j::DataType::BOOL);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_GreaterEquals_2) {
    auto x = NDArrayFactory::create<double>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<double>('c', {1, 5}, {5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {0, 0, 1, 1, 1});

    nd4j::ops::greater_equal op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Greater_1) {
    auto x = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    auto y = NDArrayFactory::create<float>('c', {1, 5}, {5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<bool>('c', {1, 5}, {0, 0, 0, 1, 1});

    nd4j::ops::greater op;
    auto result = op.execute({&x, &y}, {}, {}, {}, false, nd4j::DataType::BOOL);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Where_1) {
    auto mask = NDArrayFactory::create<bool>('c', {3, 3}, {1, 1, 1,  0, 0, 0,  1, 1, 1});
    auto x = NDArrayFactory::create<float>('c', {3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto y = NDArrayFactory::create<float>('c', {3, 3}, {9, 8, 7, 6, 5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<float>('c', {3, 3}, {1, 2, 3, 6, 5, 4, 7, 8, 9});

    nd4j::ops::Where op;
    auto result = op.execute({&mask, &x, &y}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    // z->printIndexedBuffer("result");

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Where_2) {
    auto mask = NDArrayFactory::create<bool>('c', {1, 3}, {1, 0, 0});
    auto x = NDArrayFactory::create<float>('c', {3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto y = NDArrayFactory::create<float>('c', {3, 3}, {9, 8, 7, 6, 5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<float>('c', {3, 3}, {1, 2, 3, 6, 5, 4, 3, 2, 1});

    nd4j::ops::Where op;
    auto result = op.execute({&mask, &x, &y}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, Test_Where_3) {
    auto mask = NDArrayFactory::create<bool>('c', {2, 2, 3}, {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1});
    auto exp = NDArrayFactory::create<Nd4jLong>('c', {5, 3}, {0, 0, 1, 0, 0, 2, 0, 1, 1, 1, 0, 0, 1, 1, 2});

    nd4j::ops::Where op;
    auto result = op.execute({&mask}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    // z->printShapeInfo("z");

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Select_1) {
    auto mask = NDArrayFactory::create<bool>('c', {1, 3}, {1, 0, 0});
    auto x = NDArrayFactory::create<float>('c', {3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto y = NDArrayFactory::create<float>('c', {3, 3}, {9, 8, 7, 6, 5, 4, 3, 2, 1});
    auto exp = NDArrayFactory::create<float>('c', {3, 3}, {1, 2, 3, 6, 5, 4, 3, 2, 1});

    nd4j::ops::select op;
    auto result = op.execute({&mask, &x, &y}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Select_2) {
    auto mask = NDArrayFactory::create<bool>('c', {2, 2}, {1, 0, 1, 0});
    auto x = NDArrayFactory::create<float>('c', {2, 2}, {1, 2, 3, 4 });
    auto y = NDArrayFactory::create<float>('c', {2, 2}, {9, 8, 7, 6});
    auto exp = NDArrayFactory::create<float>('c', {2, 2}, {1, 8, 3, 6});

    nd4j::ops::select op;
    auto result = op.execute({&mask, &x, &y}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Select_3) {
    auto mask = NDArrayFactory::create<bool>('c', {1, 1}, {false}, nullptr);
    auto x = NDArrayFactory::create<float>('c', {1, 1}, {1});
    auto y = NDArrayFactory::create<float>('c', {1, 1}, {2});
    auto exp = NDArrayFactory::create<float>('c', {1, 1}, {2});

    nd4j::ops::select op;
    auto result = op.execute({&mask, &x, &y}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Reshape_TF_1) {
    auto x = NDArrayFactory::create<int>('c', {2, 2}, {1, 2, 3, 4});
    auto shape = NDArrayFactory::create<int>('c', {1, 3}, {1, 2, 2});

    auto exp = NDArrayFactory::create<int>('c', {1, 2, 2}, {1, 2, 3, 4});
    
    nd4j::ops::reshape op;

    auto result = op.execute({&x, &shape}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Bias_Add_1) {
    auto x = NDArrayFactory::create<float>('c', {10, 5});
    x.assign(0.0);
    auto bias = NDArrayFactory::create<float>('c', {1, 5}, {1, 2, 3, 4, 5});
    nd4j::ops::biasadd op;

    auto result = op.execute({&x, &bias}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);


    auto tads = z->allTensorsAlongDimension({1});
    for (int e = 0; e < tads->size(); e++) {
        ASSERT_TRUE(bias.equalsTo(tads->at(e)));
    }

    delete tads;
    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_1) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2}, {1, 2, 3, 4});
    NDArray idc('c', {1}, {0}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 2}, {1, 1});
    auto exp = NDArrayFactory::create<float>('c', {2, 2}, {2, 3, 3, 4});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_2) {
    auto vec = NDArrayFactory::create<float>('c', {4}, {1, 2, 3, 4});
    NDArray idc('c', {1, 4}, {0, 1, 2, 3}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 4}, {1, 1, 1, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 4}, {2, 3, 4, 5});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&vec, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_3) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    NDArray idc('c', {1}, {0}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 2, 2}, {1, 1, 1, 1});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {2, 3, 4, 5, 5, 6, 7, 8});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_4) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    NDArray idc('c', {1, 2}, {0, 0}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 2, 2, 2}, {1, 1, 1, 1, 1, 1, 1, 1});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {3, 4, 5, 6, 5, 6, 7, 8});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_5) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 3}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    NDArray idc('c', {2, 2}, {1, 1, 0, 0}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {2, 2, 2, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 3}, {9., 11., 13.,15., 17., 19., 9., 11., 13.,15., 17., 19.});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_6) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 1, 1, 1, 1, 1, 1, 1});
    NDArray idc('c', {2, 2}, {1, 1, 0, 0}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {2, 2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {7, 9, 11, 13, 7, 9, 11, 13});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, Test_Scatter_Add_7) {
    auto matrix = NDArrayFactory::create<float>('c', {10, 3}, {1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f,9.f,10.f,11.f,12.f,13.f,14.f,15.f,16.f,17.f,18.f,19.f,20.f,21.f,22.f,23.f,24.f,25.f,26.f,27.f,28.f,29.f,30.f});
    NDArray idc('c', {0}, {5}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {3}, {10.f, 20.f, 30.f});
    auto exp = NDArrayFactory::create<float>('c', {10, 3}, {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f,11.f,12.f, 13.f,14.f,15.f, 26.f,37.f,48.f, 19.f,20.f,21.f, 22.f,23.f,24.f, 25.f,26.f,27.f, 28.f,29.f,30.f});

    nd4j::ops::scatter_add op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMax_test1) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2}, {1, 2, 3, 4});
    NDArray idc('c', {1}, {0.}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 2}, {10, 1});
    auto exp = NDArrayFactory::create<float>('c', {2, 2}, {10, 2, 3, 4});

    nd4j::ops::scatter_max op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMax_test2) {
    auto vec = NDArrayFactory::create<float>('c', {4}, {1, 2, 3, 4});
    NDArray idc('c', {1, 4}, {0, 1, 2, 3}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 4}, {10, 1, 30, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 4}, {10, 2, 30, 4});

    nd4j::ops::scatter_max op;
    auto result = op.execute({&vec, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMax_test3) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    NDArray idc('c', {1}, {0}, nd4j::DataType::INT64);
    auto updates = NDArrayFactory::create<float>('c', {1, 2, 2}, {10, 1, 30, 1});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {10, 2, 30, 4, 5, 6, 7, 8});

    nd4j::ops::scatter_max op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMax_test4) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    NDArray idc('c', {1,2}, {0,0}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {1, 2, 2, 2}, {1,10,1,10, 1,1,10,1.});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 10, 10, 10, 5, 6, 7, 8});

    nd4j::ops::scatter_max op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);     

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMax_test5) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 3}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    NDArray idc('c', {2, 2}, {1, 1, 0, 0}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2, 2, 2, 3}, {2,10,1,10, 2,10,1,10, 2,10,1,10,  10,2,10,1, 10,2,10,1, 10,2,10,1.});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 3}, {10, 2, 10,   2, 10, 2,   2, 10, 2,   10, 2, 10});

    nd4j::ops::scatter_max op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

   ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMax_test6) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 1, 1, 1, 1, 1, 1, 1});
    NDArray idc('c', {2, 2}, {1, 1, 0, 0}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2, 2, 2, 2}, {0,2,0,2, 0,2,0,2, 2,0,2,0.,  2,0,2,0});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {2, 1, 2, 1, 1, 2, 1, 2});

    nd4j::ops::scatter_max op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

   ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


TEST_F(ParityOpsTests, scatterMin_test1) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2}, {1, 2, 3, 4});    
    NDArray idc('c', {1}, {0}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {1, 2}, {-1, 1});
    auto exp = NDArrayFactory::create<float>('c', {2, 2}, {-1, 1, 3, 4});

    nd4j::ops::scatter_min op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMin_test2) {
    auto vec = NDArrayFactory::create<float>('c', {4}, {1, 2, 3, 4});
    NDArray idc('c', {1, 4}, {0, 1, 2, 3}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {1, 4}, {10, 1, 30, 1});
    auto exp = NDArrayFactory::create<float>('c', {1, 4}, {1, 1, 3, 1});

    nd4j::ops::scatter_min op;
    auto result = op.execute({&vec, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMin_test3) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    NDArray idc('c', {1}, {0}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {1, 2, 2}, {10, 1, 30, 2});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 1, 3, 2, 5, 6, 7, 8});

    nd4j::ops::scatter_min op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ParityOpsTests, scatterMin_test4) {
    auto matrix = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    NDArray idc('c', {1,2}, {0,0}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {1, 2, 2, 2}, {1,10,1,10, 1,1,10,1.});
    auto exp = NDArrayFactory::create<float>('c', {2, 2, 2}, {1, 1, 1, 1, 5, 6, 7, 8});

    nd4j::ops::scatter_min op;
    auto result = op.execute({&matrix, &idc, &updates}, {}, {}, {true});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);     

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_test1) {    
    
    NDArray indices('c', {2, 1}, {1., 0.}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2, 4}, {10.f, 20.f, 30.f, 40.f, 50.f, 60.f, 70.f, 80.f});
    auto shape = NDArrayFactory::create<int>('c', {2}, {3, 4});
    auto exp = NDArrayFactory::create<float>('c', {3, 4}, {50.f, 60.f, 70.f, 80.f, 10.f, 20.f, 30.f, 40.f, 0.f,  0.f,  0.f,  0.f});

    nd4j::ops::scatter_nd op;
    auto result = op.execute({&indices, &updates, &shape}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_test2) {    
    
    NDArray indices('c', {3, 1}, {4., 2., 0.}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3, 4});
    auto shape = NDArrayFactory::create<int>('c', {2}, {5, 4});
    auto exp = NDArrayFactory::create<float>('c', {5, 4}, {9.f,10.f,11.f,12.f, 0.f, 0.f, 0.f, 0.f, 5.f, 6.f, 7.f, 8.f, 0.f, 0.f, 0.f, 0.f, 1.f, 2.f, 3.f, 4.f});
    updates.linspace(1.f);

    nd4j::ops::scatter_nd op;
    auto result = op.execute({&indices, &updates, &shape}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_test3) {    
    
    NDArray indices('c', {2, 3, 1}, {0., 2., 7., 3., 6., 9.}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,3, 3,4});
    auto shape = NDArrayFactory::create<int>('c', {3}, {10, 3, 4});
    auto exp = NDArrayFactory::create<float>('c', {10, 3, 4}, {1.f,  2.f,  3.f,  4., 5.f,  6.f,  7.f,  8., 9.f, 10.f, 11.f, 12., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0.,
                                        13.f, 14.f, 15.f, 16.,17.f, 18.f, 19.f, 20.,21.f, 22.f, 23.f, 24.,37.f, 38.f, 39.f, 40.,41.f, 42.f, 43.f, 44.,45.f, 46.f, 47.f, 48.,
                                         0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0.,
                                        49.f, 50.f, 51.f, 52.,53.f, 54.f, 55.f, 56.,57.f, 58.f, 59.f, 60.,25.f, 26.f, 27.f, 28.,29.f, 30.f, 31.f, 32.,33.f, 34.f, 35.f, 36.,
                                         0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0., 0.f,  0.f,  0.f,  0.,61.f, 62.f, 63.f, 64.,65.f, 66.f, 67.f, 68.,69.f, 70.f, 71.f, 72.,});
    updates.linspace(1.f);

    nd4j::ops::scatter_nd op;
    auto result = op.execute({&indices, &updates, &shape}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_test4) {    
        
    NDArray indices('c', {4, 1}, {4., 3., 1., 7.}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {4}, {9.f, 10.f, 11.f, 12.f});
    auto shape = NDArrayFactory::create<int>('c', {1}, {8});
    auto exp = NDArrayFactory::create<float>('c', {8}, {0.f, 11.f, 0.f, 10.f, 9.f, 0.f, 0.f, 12.f});
    
    nd4j::ops::scatter_nd op;
    auto result = op.execute({&indices, &updates, &shape}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_add_test1) {    
    
    auto input = NDArrayFactory::create<float>('c', {8}, {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f});
    NDArray indices('c', {4, 1}, {4., 3., 1., 7.}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {4}, {9.f, 10.f, 11.f, 12.f});
    auto exp = NDArrayFactory::create<float>('c', {8}, {1.f, 13.f, 3.f, 14.f, 14.f, 6.f, 7.f, 20.f});
    
    nd4j::ops::scatter_nd_add op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_add_test2) {    
    
    auto input = NDArrayFactory::create<float>('c', {6, 4});
    NDArray indices('c', {3, 3, 2}, {0.f,0.f, 1.f,1.f, 2.f,2.f, 3.f,3.f, 4.f,0.f, 5.f,1.f, 0.f,2.f, 1.f,3.f, 2.f,0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3,3});
    auto exp = NDArrayFactory::create<float>('c', {6,4}, {1.f,0.f,7.f,0.f, 0.f,2.f,0.f,8.f, 9.f,0.f,3.f,0.f, 0.f,0.f,0.f,4.f, 5.f,0.f,0.f,0.f, 0.f,6.f,0.f,0.f});

    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_add op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 
    // z->printIndexedBuffer();

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_add_test3) {
    
    auto input = NDArrayFactory::create<float>('c', {6, 4});
    NDArray indices('c', {2, 3, 1}, {5.f, 1.f, 2.f, 3.f, 4.f, 0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,3,4});
    auto exp = NDArrayFactory::create<float>('c', {6,4}, {21.f, 22.f, 23.f, 24.f, 5.f,  6.f,  7.f,  8.f, 9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f, 1.f,  2.f,  3.f,  4.f});

    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_add op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_add_test4) {
    
    auto input = NDArrayFactory::create<float>('c', {6, 4, 5});
    NDArray indices('c', {3, 3, 2}, {0.f,0.f, 1.f,1.f, 2.f,2.f, 3.f,3.f, 4.f,0.f, 5.f,1.f, 0.f,2.f, 1.f,3.f, 2.f,0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3,3,5});
    auto exp = NDArrayFactory::create<float>('c', {6,4,5}, {1.f,  2.f,  3.f,  4.f,  5.f, 0.f,  0.f,  0.f,  0.f,  0.f,31.f, 32.f, 33.f, 34.f, 35.f, 0.f,  0.f,  0.f,  0.f,  0.f,
                                      0.f,  0.f,  0.f,  0.f,  0.f, 6.f,  7.f,  8.f,  9.f, 10.f, 0.f,  0.f,  0.f,  0.f,  0.f,36.f, 37.f, 38.f, 39.f, 40.f,
                                     41.f, 42.f, 43.f, 44.f, 45.f, 0.f,  0.f,  0.f,  0.f,  0.f,11.f, 12.f, 13.f, 14.f, 15.f, 0.f,  0.f,  0.f,  0.f,  0.f,
                                      0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f,16.f, 17.f, 18.f, 19.f, 20.f,
                                     21.f, 22.f, 23.f, 24.f, 25.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f,
                                      0.f,  0.f,  0.f,  0.f,  0.f,26.f, 27.f, 28.f, 29.f, 30.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f});
    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_add op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_add_test5) {
    
    auto input = NDArrayFactory::create<float>('c', {6,5,4,3,2});
    NDArray indices('c', {2,2,3}, {0.f,0.f,0.f, 1.f,1.f,1.f, 2.f,2.f,2.f, 3.f,3.f,3.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,2,3,2});
    auto exp = NDArrayFactory::create<float>('c', {6,5,4,3,2}, { 1.f,  2.f, 3.f,  4.f, 5.f,  6.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 7.f,  8.f, 9.f, 10.f,11.f, 12.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,13.f, 14.f,15.f, 16.f,17.f, 18.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,19.f, 20.f,21.f, 22.f,23.f, 24.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f});
    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_add op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_sub_test1) {    
    
    auto input = NDArrayFactory::create<float>('c', {8}, {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f});
    NDArray indices('c', {4, 1}, {4.f, 3.f, 1.f, 7.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {4}, {9.f, 10.f, 11.f, 12.f});
    auto exp = NDArrayFactory::create<float>('c', {8}, {1.f, -9.f,  3.f, -6.f, -4.f,  6.f,  7.f, -4.f});
    
    nd4j::ops::scatter_nd_sub op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_sub_test2) {    
    
    auto input = NDArrayFactory::create<float>('c', {6, 4});
    NDArray indices('c', {3, 3, 2}, {0.f,0.f, 1.f,1.f, 2.f,2.f, 3.f,3.f, 4.f,0.f, 5.f,1.f, 0.f,2.f, 1.f,3.f, 2.f,0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3,3});
    auto exp = NDArrayFactory::create<float>('c', {6,4}, {-1.f,0.f,-7.f,0.f, 0.f,-2.f,0.f,-8.f, -9.f,0.f,-3.f,0.f, 0.f,0.f,0.f,-4.f, -5.f,0.f,0.f,0.f, 0.f,-6.f,0.f,0.f});

    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_sub op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 
    // z->printIndexedBuffer();

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_sub_test3) {
    
    auto input = NDArrayFactory::create<float>('c', {6, 4});
    NDArray indices('c', {2, 3, 1}, {5.f, 1.f, 2.f, 3.f,4.f, 0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,3,4});
    auto exp = NDArrayFactory::create<float>('c', {6,4}, {-21.f,-22.f,-23.f,-24., -5.f, -6.f, -7.f, -8., -9.f,-10.f,-11.f,-12., -13.f,-14.f,-15.f,-16., -17.f,-18.f,-19.f,-20., -1.f, -2.f, -3.f, -4.f});

    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_sub op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_sub_test4) {
    
    auto input = NDArrayFactory::create<float>('c', {6, 4, 5});    
    NDArray indices('c', {3, 3, 2}, {0.f,0.f, 1.f,1.f, 2.f,2.f, 3.f,3.f, 4.f,0.f, 5.f,1.f, 0.f,2.f, 1.f,3.f, 2.f,0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3,3,5});
    auto exp = NDArrayFactory::create<float>('c', {6,4,5}, {-1.f,  -2.f,  -3.f,  -4.f,  -5.f, 0.f,  0.f,  0.f,  0.f,  0.f,-31.f, -32.f, -33.f, -34.f, -35.f, 0.f,  0.f,  0.f,  0.f,  0.f,
                                      0.f,  0.f,  0.f,  0.f,  0.f, -6.f,  -7.f,  -8.f,  -9.f, -10.f, 0.f,  0.f,  0.f,  0.f,  0.f,-36.f, -37.f, -38.f, -39.f, -40.f,
                                     -41.f, -42.f, -43.f, -44.f, -45.f, 0.f,  0.f,  0.f,  0.f,  0.f,-11.f, -12.f, -13.f, -14.f, -15.f, 0.f,  0.f,  0.f,  0.f,  0.f,
                                      0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f,-16.f, -17.f, -18.f, -19.f, -20.f,
                                     -21.f, -22.f, -23.f, -24.f, -25.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f,
                                      0.f,  0.f,  0.f,  0.f,  0.f,-26.f, -27.f, -28.f, -29.f, -30.f, 0.f,  0.f,  0.f,  0.f,  0.f, 0.f,  0.f,  0.f,  0.f,  0.f});
    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_sub op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_sub_test5) {
    
    auto input = NDArrayFactory::create<float>('c', {6,5,4,3,2});
    NDArray indices('c', {2,2,3}, {0.f,0.f,0.f, 1.f,1.f,1.f, 2.f,2.f,2.f, 3.f,3.f,3.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,2,3,2});
    auto exp = NDArrayFactory::create<float>('c', {6,5,4,3,2}, { -1.f,  -2.f, -3.f,  -4.f, -5.f,  -6.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, -7.f,  -8.f, -9.f, -10.f,-11.f, -12.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,-13.f, -14.f,-15.f, -16.f,-17.f, -18.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,-19.f, -20.f,-21.f, -22.f,-23.f,-24.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f,
0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f, 0.f,  0.f});
    input = 0.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_sub op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}


////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_update_test1) {    
    
    auto input = NDArrayFactory::create<float>('c', {8}, {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f});
    NDArray indices('c', {4, 1}, {4.f, 3.f, 1.f, 7.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {4}, {9.f, 10.f, 11.f, 12.f});
    auto exp = NDArrayFactory::create<float>('c', {8}, {1.f, 11.f, 3.f, 10.f, 9.f, 6.f, 7.f, 12.f});
    
    nd4j::ops::scatter_nd_update op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_update_test2) {    
    
    auto input = NDArrayFactory::create<float>('c', {6, 4});
    NDArray indices('c', {3, 3, 2}, {0.f,0.f, 1.f,1.f, 2.f,2.f, 3.f,3.f, 4.f,0.f, 5.f,1.f, 0.f,2.f, 1.f,3.f, 2.f,0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3,3});
    auto exp = NDArrayFactory::create<float>('c', {6,4}, {1.f,-1.f,7.f,-1.f, -1.f,2.f,-1.f,8.f, 9.f,-1.f,3.f,-1.f, -1.f,-1.f,-1.f,4.f, 5.f,-1.f,-1.f,-1.f, -1.f,6.f,-1.f,-1.f});

    input = -1.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_update op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 
    // z->printIndexedBuffer();

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_update_test3) {
    
    auto input = NDArrayFactory::create<float>('c', {6, 4});
    NDArray indices('c', {2, 3, 1}, {5.f, 1.f, 2.f, 3.f, 4.f, 0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,3,4});
    auto exp = NDArrayFactory::create<float>('c', {6,4}, {21.f, 22.f, 23.f, 24.f, 5.f,  6.f,  7.f,  8.f, 9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f, 1.f,  2.f,  3.f,  4.f,});

    input = -1.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_update op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);     

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_update_test4) {
    
    auto input = NDArrayFactory::create<float>('c', {6, 4, 5});    
    NDArray indices('c', {3, 3, 2}, {0.f,0.f, 1.f,1.f, 2.f,2.f, 3.f,3.f, 4.f,0.f, 5.f,1.f, 0.f,2.f, 1.f,3.f, 2.f,0.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {3,3,5});
    auto exp = NDArrayFactory::create<float>('c', {6,4,5}, {1.f,  2.f,  3.f,  4.f,  5.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,31.f, 32.f, 33.f, 34.f, 35.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,
                                      -1.f,  -1.f,  -1.f,  -1.f,  -1.f, 6.f,  7.f,  8.f,  9.f, 10.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,36.f, 37.f, 38.f, 39.f, 40.f,
                                     41.f, 42.f, 43.f, 44.f, 45.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,11.f, 12.f, 13.f, 14.f, 15.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,
                                      -1.f,  -1.f,  -1.f,  -1.f,  -1.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,16.f, 17.f, 18.f, 19.f, 20.f,
                                     21.f, 22.f, 23.f, 24.f, 25.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f,
                                      -1.f,  -1.f,  -1.f,  -1.f,  -1.f,26.f, 27.f, 28.f, 29.f, 30.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f, -1.f,  -1.f,  -1.f,  -1.f,  -1.f});
    input = -1.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_update op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(ParityOpsTests, scatterND_update_test5) {
    
    auto input = NDArrayFactory::create<float>('c', {6,5,4,3,2});
    NDArray indices('c', {2,2,3}, {0.f,0.f,0.f, 1.f,1.f,1.f, 2.f,2.f,2.f, 3.f,3.f,3.f}, nd4j::DataType::INT32);
    auto updates = NDArrayFactory::create<float>('c', {2,2,3,2});
    auto exp = NDArrayFactory::create<float>('c', {6,5,4,3,2}, { 1.f,  2.f, 3.f,  4.f, 5.f,  6.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, 7.f,  8.f, 9.f, 10.f,11.f, 12.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,13.f, 14.f,15.f, 16.f,17.f, 18.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,19.f, 20.f,21.f, 22.f,23.f, 24.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f,
-1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f, -1.f,  -1.f});
    input = -1.f;
    updates.linspace(1.f);
    
    nd4j::ops::scatter_nd_update op;
    auto result = op.execute({&input, &indices, &updates}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0); 

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

