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
#include <NDArray.h>
#include <GraphExecutioner.h>
#include <ops/declarable/CustomOperations.h>

using namespace nd4j;
using namespace nd4j::ops;

class ListOperationsTests : public testing::Test {

};

TEST_F(ListOperationsTests, BasicTest_Write_1) {
    NDArrayList list(5);
    auto x = NDArrayFactory::create<double>('c', {1, 128});
    x.linspace(1);

    nd4j::ops::write_list op;

    auto result = op.execute(&list, {&x}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_EQ(1, list.elements());

    auto result2 = op.execute(&list, {&x}, {}, {2});

    ASSERT_EQ(2, list.elements());

    delete result;
    delete result2;
}

TEST_F(ListOperationsTests, BasicTest_Stack_1) {
    NDArrayList list(10);
    auto exp = NDArrayFactory::create<double>('c', {10, 100});
    auto tads = exp.allTensorsAlongDimension({1});
    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {1, 100});
        row->assign((double) e);
        list.write(e, row);
        tads->at(e)->assign(row);
    }

    nd4j::ops::stack_list op;

    auto result = op.execute(&list, {}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
    delete tads;
}


TEST_F(ListOperationsTests, BasicTest_Read_1) {
    NDArrayList list(10);
    auto exp = NDArrayFactory::create<double>('c', {1, 100});
    exp.assign(4.0f);

    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {1, 100});
        row->assign((double) e);
        list.write(e, row->dup());

        delete row;
    }

    nd4j::ops::read_list op;

    auto result = op.execute(&list, {}, {}, {4});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ListOperationsTests, BasicTest_Pick_1) {
    NDArrayList list(10);
    auto exp = NDArrayFactory::create<double>('c', {4, 100});

    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {1, 100});
        row->assign((double) e);
        list.write(e, row->dup());

        delete row;
    }

    auto tads = exp.allTensorsAlongDimension({1});
    tads->at(0)->assign(1.0f);
    tads->at(1)->assign(1.0f);
    tads->at(2)->assign(3.0f);
    tads->at(3)->assign(3.0f);


    nd4j::ops::pick_list op;
    auto result = op.execute(&list, {}, {}, {1, 1, 3, 3});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
    delete tads;
}

TEST_F(ListOperationsTests, BasicTest_Size_1) {
    NDArrayList list(10);
    auto exp = NDArrayFactory::create<int>(10);
    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {1, 100});
        row->assign((double) e);
        list.write(e, row->dup());

        delete row;
    }

    nd4j::ops::size_list op;

    auto result = op.execute(&list, {}, {}, {1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(ListOperationsTests, BasicTest_Create_1) {
    auto matrix = NDArrayFactory::create<double>('c', {3, 2});
    matrix.linspace(1);

    nd4j::ops::create_list op;

    auto result = op.execute(nullptr, {&matrix}, {}, {1, 1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    // we return flow as well
    ASSERT_EQ(1, result->size());

    delete result;
}

TEST_F(ListOperationsTests, BasicTest_Split_1) {
    NDArrayList list(0, true);

    auto exp0 = NDArrayFactory::create<double>('c', {2, 5});
    auto exp1 = NDArrayFactory::create<double>('c', {3, 5});
    auto exp2 = NDArrayFactory::create<double>('c', {5, 5});

    auto matrix = NDArrayFactory::create<double>('c', {10, 5});

    auto lengths = NDArrayFactory::create<double>('c', {1, 3});
    lengths.p(0, 2);
    lengths.p(1, 3);
    lengths.p(2, 5);

    auto tads = matrix.allTensorsAlongDimension({1});

    auto tads0 = exp0.allTensorsAlongDimension({1});
    auto tads1 = exp1.allTensorsAlongDimension({1});
    auto tads2 = exp2.allTensorsAlongDimension({1});

    int cnt0 = 0;
    int cnt1 = 0;
    int cnt2 = 0;
    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {1, 5});
        row->assign((double) e);
        tads->at(e)->assign(row);

        if (e < 2)
            tads0->at(cnt0++)->assign(row);
        else if (e < 5)
            tads1->at(cnt1++)->assign(row);
        else
            tads2->at(cnt2++)->assign(row);

        delete row;
    }

    nd4j::ops::split_list op;
    auto result = op.execute(&list, {&matrix, &lengths}, {}, {});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_EQ(3, list.height());

    ASSERT_TRUE(exp0.isSameShape(list.readRaw(0)));
    ASSERT_TRUE(exp0.equalsTo(list.readRaw(0)));

    ASSERT_TRUE(exp1.isSameShape(list.readRaw(1)));
    ASSERT_TRUE(exp1.equalsTo(list.readRaw(1)));

    ASSERT_TRUE(exp2.isSameShape(list.readRaw(2)));
    ASSERT_TRUE(exp2.equalsTo(list.readRaw(2)));

    delete result;
    delete tads;
    delete tads0;
    delete tads1;
    delete tads2;
}

TEST_F(ListOperationsTests, BasicTest_Scatter_1) {
    NDArrayList list(0, true);
    auto s = NDArrayFactory::create<double>(0.0);

    auto matrix = NDArrayFactory::create<double>('c', {10, 5});
    auto tads = matrix.allTensorsAlongDimension({1});
    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {1, 5});
        row->assign((double) e);
        tads->at(e)->assign(row);

        delete row;
    }
    auto indices = NDArrayFactory::create<double>('c', {1, 10});
    for (int e = 0; e < matrix.rows(); e++)
        indices.p(e, 9 - e);

    nd4j::ops::scatter_list op;
    auto result = op.execute(&list, {&indices, &matrix, &s}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    for (int e = 0; e < 10; e++) {
        auto row = tads->at(9 - e);
        auto chunk = list.readRaw(e);

        ASSERT_TRUE(chunk->isSameShape(row));

        ASSERT_TRUE(chunk->equalsTo(row));
    }

    delete tads;
    delete result;
}

TEST_F(ListOperationsTests, BasicTest_Clone_1) {
    auto list = new NDArrayList(0, true);

    VariableSpace variableSpace;
    auto var = new Variable(nullptr, nullptr, -1, 0);
    var->setNDArrayList(list);

    variableSpace.putVariable(-1, var);
    variableSpace.trackList(list);

    Context block(1, &variableSpace);
    block.pickInput(-1);

    nd4j::ops::clone_list op;

    ASSERT_TRUE(list == block.variable(0)->getNDArrayList());

    auto result = op.execute(&block);

    ASSERT_EQ(ND4J_STATUS_OK, result);

    auto resVar = variableSpace.getVariable(1);

    auto resList = resVar->getNDArrayList();

    ASSERT_TRUE( resList != nullptr);

    ASSERT_TRUE(list->equals(*resList));
}

TEST_F(ListOperationsTests, BasicTest_Gather_1) {
    NDArrayList list(0, true);
    for (int e = 0; e < 10; e++) {
        auto row = NDArrayFactory::create_<double>('c', {3});
        row->assign((double) e);
        list.write(e, row->dup());

        delete row;
    }

    auto exp = NDArrayFactory::create<double>('c', {10, 3});
    auto tads = exp.allTensorsAlongDimension({1});
    for (int e = 0; e < 10; e++) {
        auto tad = tads->at(9 - e);
        tad->assign(e);
    }

    auto indices = NDArrayFactory::create<double>('c', {1, 10});
    indices.linspace(9, -1);

    nd4j::ops::gather_list op;
    auto result = op.execute(&list, {&indices}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());
    ASSERT_EQ(1, result->size());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));

    //exp.printIndexedBuffer("e");
    //z->printIndexedBuffer("z");

    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
    delete tads;
}

TEST_F(ListOperationsTests, GraphTests_Sequential_1) {
    Graph graph;

    auto matrix  = NDArrayFactory::create_<float>('c', {3, 3});
    auto tads = matrix->allTensorsAlongDimension({1});
    for (int e = 0; e < tads->size(); e++) {
        tads->at(e)->assign((float) (e+1));
    }


    auto exp = NDArrayFactory::create<float>('c', {3, 3});
    auto tadsExp = exp.allTensorsAlongDimension({1});
    tadsExp->at(0)->assign(0.f);
    tadsExp->at(1)->assign(-1.f);
    tadsExp->at(2)->assign(-2.f);
    delete tadsExp;

    auto indices = NDArrayFactory::valueOf({1, 3}, 1.0f, 'c');
    //indices->linspace(0);


    auto variableSpace = graph.getVariableSpace();
    variableSpace->putVariable(-1, matrix);
    variableSpace->putVariable(-2, indices);


    auto nodeA = new Node(OpType_TRANSFORM_SAME, 0, 1, {-1});

    // creating list
    nd4j::ops::create_list opB;
    auto nodeB = new Node(&opB, 2, {1},{},{}, 0.0f, {}, {0, 1});
    //nodeB->setCustomOp(&opB);

    // filling list with matrix
    nd4j::ops::split_list opC;
    auto nodeC = new Node(&opC, 3, {2, 1, -2});
    //nodeC->setCustomOp(&opC);

    // reading chunks from List. We're adding op number 3 in inputs, to ensure graph will execute this node after split
    nd4j::ops::read_list opD;
    auto nodeD0 = new Node(&opD, 5, {2, 3}, {},{}, 0.0f, {}, {0});
    auto nodeD1 = new Node(&opD, 6, {2, 3}, {},{}, 0.0f, {}, {1});
    auto nodeD2 = new Node(&opD, 7, {2, 3}, {},{}, 0.0f, {}, {2});
    //nodeD0->setCustomOp(&opD);
    //nodeD1->setCustomOp(&opD);
    //nodeD2->setCustomOp(&opD);

    // using OneMinus on each chunk separately
    auto nodeE0 = new Node(OpType_TRANSFORM_SAME, nd4j::transform::OneMinus, 10, {5});
    auto nodeE1 = new Node(OpType_TRANSFORM_SAME, nd4j::transform::OneMinus, 11, {6});
    auto nodeE2 = new Node(OpType_TRANSFORM_SAME, nd4j::transform::OneMinus, 12, {7});

    // writing chunks back to the List
    nd4j::ops::write_list opF;
    auto nodeF0 = new Node(&opF, 15, {2, 10}, {},{}, 0.0f, {}, {0});
    auto nodeF1 = new Node(&opF, 16, {2, 11}, {},{}, 0.0f, {}, {1});
    auto nodeF2 = new Node(&opF, 17, {2, 12}, {},{}, 0.0f, {}, {2});

//    nodeF0->setCustomOp(&opF);
//    nodeF1->setCustomOp(&opF);
//    nodeF2->setCustomOp(&opF);

    // now we're stacking chunks back to matrix state 
    nd4j::ops::stack_list opG;
    auto nodeG = new Node(&opG, 20, {2, 15, 16, 17});
    //auto nodeG = new Node<float>(OpType_CUSTOM, 0, 20, {2});

//    nodeG->setCustomOp(&opG);


    graph.addNode(nodeA);
    graph.addNode(nodeB);
    graph.addNode(nodeC);
    graph.addNode(nodeD0);
    graph.addNode(nodeD1);
    graph.addNode(nodeD2);
    graph.addNode(nodeE0);
    graph.addNode(nodeE1);
    graph.addNode(nodeE2);

    graph.addNode(nodeF0);
    graph.addNode(nodeF1);
    graph.addNode(nodeF2);

    graph.addNode(nodeG);

    // let's also validate structural integrity
    graph.buildGraph();

    ASSERT_EQ(0, nodeA->getLayer());
    ASSERT_EQ(1, nodeB->getLayer());
    ASSERT_EQ(2, nodeC->getLayer());

    ASSERT_EQ(3, nodeD0->getLayer());
    ASSERT_EQ(3, nodeD1->getLayer());
    ASSERT_EQ(3, nodeD2->getLayer());

    ASSERT_EQ(4, nodeE0->getLayer());
    ASSERT_EQ(4, nodeE1->getLayer());
    ASSERT_EQ(4, nodeE2->getLayer());

    ASSERT_EQ(5, nodeF0->getLayer());
    ASSERT_EQ(5, nodeF1->getLayer());
    ASSERT_EQ(5, nodeF2->getLayer());

    ASSERT_EQ(6, nodeG->getLayer());

    auto result = GraphExecutioner::execute(&graph);
    ASSERT_EQ(ND4J_STATUS_OK, result);

    ASSERT_TRUE(variableSpace->hasVariable(2));
    auto list = variableSpace->getVariable(2)->getNDArrayList();

    ASSERT_TRUE(list != nullptr);

    ASSERT_EQ(3, list->height());
    ASSERT_EQ(3, list->elements());


    ASSERT_TRUE(variableSpace->hasVariable(20));

    auto stack = variableSpace->getVariable(20)->getNDArray();

    ASSERT_TRUE(stack != nullptr);

    ASSERT_TRUE(exp.isSameShape(stack));
    ASSERT_TRUE(exp.equalsTo(stack));

    delete tads;
}


TEST_F(ListOperationsTests, GraphTests_Sequential_2) {
    Graph graph;

    auto scalar = NDArrayFactory::create_<double>(0.0f);
    auto matrix = NDArrayFactory::create_<double>('c', {3, 3});
    auto tads = matrix->allTensorsAlongDimension({1});
    for (int e = 0; e < tads->size(); e++) {
        tads->at(e)->assign((float) (e+1));
    }


    auto exp = NDArrayFactory::create<double>('c', {3, 3});
    auto tadsExp = exp.allTensorsAlongDimension({1});
    tadsExp->at(0)->assign(0.f);
    tadsExp->at(1)->assign(-1.f);
    tadsExp->at(2)->assign(-2.f);

    //auto indices = NDArray<float>::valueOf({1, 3}, 1.0f, 'c');
    auto indices = NDArrayFactory::create_<double>('c', {1, 3});
    indices->linspace(0);


    auto variableSpace = graph.getVariableSpace();
    variableSpace->putVariable(-1, matrix);
    variableSpace->putVariable(-2, indices);
    variableSpace->putVariable(-3, scalar);


    auto nodeA = new Node(OpType_TRANSFORM_SAME, 0, 1, {-1});

    // creating list
    nd4j::ops::create_list opB;
    auto nodeB = new Node(&opB, 2, {1},{},{}, 0.0f, {}, {0, 1});
//    nodeB->setCustomOp(&opB);

    // filling list with matrix
    nd4j::ops::scatter_list opC;
    auto nodeC = new Node(&opC, 3, {2, -2, 1, -3});
    
    //nodeC->setCustomOp(&opC);

    nd4j::ops::read_list opD;
    auto nodeD0 = new Node(&opD, 5, {2, 3}, {},{}, 0.0f, {}, {0});
    auto nodeD1 = new Node(&opD, 6, {2, 3, 15}, {},{}, 0.0f, {}, {1});
    auto nodeD2 = new Node(&opD, 7, {2, 3, 16}, {},{}, 0.0f, {}, {2});

//    nodeD0->setCustomOp(&opD);
//    nodeD1->setCustomOp(&opD);
//    nodeD2->setCustomOp(&opD);


    // using OneMinus on each chunk separately
    auto nodeE0 = new Node(OpType_TRANSFORM_SAME, nd4j::transform::OneMinus, 10, {5});
    auto nodeE1 = new Node(OpType_TRANSFORM_SAME, nd4j::transform::OneMinus, 11, {6});
    auto nodeE2 = new Node(OpType_TRANSFORM_SAME, nd4j::transform::OneMinus, 12, {7});

    // writing chunks back to the List
    nd4j::ops::write_list opF;
    auto nodeF0 = new Node(&opF, 15, {2, 10}, {},{}, 0.0f, {}, {0});
    auto nodeF1 = new Node(&opF, 16, {2, 11}, {},{}, 0.0f, {}, {1});
    auto nodeF2 = new Node(&opF, 17, {2, 12}, {},{}, 0.0f, {}, {2});

//    nodeF0->setCustomOp(&opF);
//    nodeF1->setCustomOp(&opF);
//    nodeF2->setCustomOp(&opF);

    // now we're gathering chunks back to matrix state 
    nd4j::ops::pick_list opG;
    auto nodeG = new Node(&opG, 20, {2, -2, 15, 16, 17});
    //auto nodeG = new Node<float>(OpType_CUSTOM, 0, 20, {2});

    //nodeG->setCustomOp(&opG);

    graph.addNode(nodeA);
    graph.addNode(nodeB);
    graph.addNode(nodeC);
    graph.addNode(nodeD0);
    graph.addNode(nodeD1);
    graph.addNode(nodeD2);
    graph.addNode(nodeE0);
    graph.addNode(nodeE1);
    graph.addNode(nodeE2);

    graph.addNode(nodeF0);
    graph.addNode(nodeF1);
    graph.addNode(nodeF2);

    graph.addNode(nodeG);

    // let's also validate structural integrity
    graph.buildGraph();

    ASSERT_EQ(0, nodeA->getLayer());
    ASSERT_EQ(1, nodeB->getLayer());
    ASSERT_EQ(2, nodeC->getLayer());

    ASSERT_EQ(3, nodeD0->getLayer());
    ASSERT_EQ(4, nodeE0->getLayer());
    ASSERT_EQ(5, nodeF0->getLayer());

    ASSERT_EQ(6, nodeD1->getLayer());
    ASSERT_EQ(7, nodeE1->getLayer());
    ASSERT_EQ(8, nodeF1->getLayer());

    ASSERT_EQ(9, nodeD2->getLayer());
    ASSERT_EQ(10, nodeE2->getLayer());
    ASSERT_EQ(11, nodeF2->getLayer());

    ASSERT_EQ(12, nodeG->getLayer());


    auto result = GraphExecutioner::execute(&graph);
    ASSERT_EQ(ND4J_STATUS_OK, result);

    ASSERT_TRUE(variableSpace->hasVariable(2));
    auto list = variableSpace->getVariable(2)->getNDArrayList();

    ASSERT_TRUE(list != nullptr);

    ASSERT_EQ(3, list->height());
    ASSERT_EQ(3, list->elements());

    ASSERT_TRUE(variableSpace->hasVariable(20));
    
    auto stack = variableSpace->getVariable(20)->getNDArray();
    
    ASSERT_TRUE(stack != nullptr);

    ASSERT_TRUE(exp.isSameShape(stack));
    ASSERT_TRUE(exp.equalsTo(stack));

    delete tadsExp;
    delete tads;
}