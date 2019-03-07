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
#include <GraphExecutioner.h>
#include <graph/GraphHolder.h>
#include <graph/InferenceRequest.h>

using namespace nd4j;
using namespace nd4j::graph;

class ServerRelatedTests : public testing::Test {
public:
    ServerRelatedTests() {
        Environment::getInstance()->setDebug(true);
        Environment::getInstance()->setVerbose(true);
    }

    ~ServerRelatedTests() {
        Environment::getInstance()->setDebug(false);
        Environment::getInstance()->setVerbose(false);
    }
};

TEST_F(ServerRelatedTests, Basic_Output_Test_1) {
    flatbuffers::FlatBufferBuilder builder(4096);

    auto array1 = NDArrayFactory::create_<float>('c', {10, 10});
    auto array2 = NDArrayFactory::create_<float>('c', {10, 10});
    auto array3 = NDArrayFactory::create_<float>('c', {10, 10});

    array1->assign(1.0f);
    array2->assign(2.0f);
    array3->assign(3.0f);

    Variable var1(array1, "first", 1);
    Variable var2(array2, "second", 2);
    Variable var3(array3, "second indexed", 2, 1);

    ExecutionResult result({&var1, &var2, &var3});

    ASSERT_EQ(*array1, *result.at(0)->getNDArray());
    ASSERT_EQ(*array2, *result.at(1)->getNDArray());
    ASSERT_EQ(*array3, *result.at(2)->getNDArray());

    ASSERT_EQ(*array1, *result.byId("first")->getNDArray());
    ASSERT_EQ(*array2, *result.byId("second")->getNDArray());
    ASSERT_EQ(*array3, *result.byId("second indexed")->getNDArray());

    auto flatResult = result.asFlatResult(builder);
    builder.Finish(flatResult);
    auto ptr = builder.GetBufferPointer();
    auto received = GetFlatResult(ptr);

    ExecutionResult restored(received);
    ASSERT_EQ(3, restored.size());

    ASSERT_EQ(*array1, *restored.at(0)->getNDArray());
    ASSERT_EQ(*array2, *restored.at(1)->getNDArray());
    ASSERT_EQ(*array3, *restored.at(2)->getNDArray());

    ASSERT_EQ(*array1, *restored.byId("first")->getNDArray());
    ASSERT_EQ(*array2, *restored.byId("second")->getNDArray());
    ASSERT_EQ(*array3, *restored.byId("second indexed")->getNDArray());
}
#if GRAPH_FILES_OK
TEST_F(ServerRelatedTests, Basic_Execution_Test_1) {
    flatbuffers::FlatBufferBuilder builder(4096);
    auto oGraph = GraphExecutioner::importFromFlatBuffers("./resources/reduce_dim_false.fb");
    oGraph->printOut();

    auto exp = NDArrayFactory::create<float>('c', {3}, {3.f, 3.f, 3.f});

    GraphHolder::getInstance()->registerGraph(11901L, oGraph);

    auto cGraph = GraphHolder::getInstance()->cloneGraph(11901L);

    ASSERT_TRUE(oGraph != cGraph);

    auto flatResult = GraphExecutioner::execute(cGraph, builder, nullptr);

    builder.Finish(flatResult);
    auto ptr = builder.GetBufferPointer();
    auto received = GetFlatResult(ptr);

    ExecutionResult restored(received);
    ASSERT_EQ(1, restored.size());

    ASSERT_EQ(exp, *restored.at(0)->getNDArray());

    delete cGraph;

    GraphHolder::getInstance()->dropGraphAny(11901L);
}

TEST_F(ServerRelatedTests, Basic_Execution_Test_2) {
    flatbuffers::FlatBufferBuilder builder(4096);
    flatbuffers::FlatBufferBuilder otherBuilder(4096);
    auto oGraph = GraphExecutioner::importFromFlatBuffers("./resources/reduce_dim_false.fb");
    oGraph->printOut();

    auto input0 = NDArrayFactory::create<float>('c', {3, 3}, {2.f,2.f,2.f, 2.f,2.f,2.f, 2.f,2.f,2.f});
    auto exp = NDArrayFactory::create<float>('c', {3}, {6.f, 6.f, 6.f});

    GraphHolder::getInstance()->registerGraph(11902L, oGraph);

    auto cGraph = GraphHolder::getInstance()->cloneGraph(11902L);

    ASSERT_TRUE(oGraph != cGraph);

    // mastering InferenceRequest
    InferenceRequest ir(11902L);
    ir.appendVariable(1, 0, &input0);

    auto af = ir.asFlatInferenceRequest(otherBuilder);
    otherBuilder.Finish(af);
    auto fptr = otherBuilder.GetBufferPointer();
    auto fir = GetFlatInferenceRequest(fptr);

    auto flatResult = GraphExecutioner::execute(cGraph, builder, fir);

    builder.Finish(flatResult);
    auto ptr = builder.GetBufferPointer();
    auto received = GetFlatResult(ptr);

    ExecutionResult restored(received);
    ASSERT_EQ(1, restored.size());

    ASSERT_EQ(exp, *restored.at(0)->getNDArray());

    delete cGraph;

    GraphHolder::getInstance()->dropGraphAny(11902L);
}

TEST_F(ServerRelatedTests, BasicExecutionTests_3) {
    flatbuffers::FlatBufferBuilder builder(4096);
    flatbuffers::FlatBufferBuilder otherBuilder(4096);
    auto oGraph = GraphExecutioner::importFromFlatBuffers("./resources/reduce_dim_false.fb");
    oGraph->printOut();

    auto input0 = NDArrayFactory::create<float>('c', {3, 3}, {2.f,2.f,2.f, 2.f,2.f,2.f, 2.f,2.f,2.f});
    auto exp = NDArrayFactory::create<float>('c', {3}, {6.f, 6.f, 6.f});

    GraphHolder::getInstance()->registerGraph(11903L, oGraph);

    // mastering InferenceRequest
    InferenceRequest ir(11903L);
    ir.appendVariable(1, 0, &input0);

    auto af = ir.asFlatInferenceRequest(otherBuilder);
    otherBuilder.Finish(af);
    auto fptr = otherBuilder.GetBufferPointer();
    auto fir = GetFlatInferenceRequest(fptr);


    auto flatResult = GraphHolder::getInstance()->execute(fir->id(), builder, fir);

    builder.Finish(flatResult);
    auto ptr = builder.GetBufferPointer();
    auto received = GetFlatResult(ptr);

    ExecutionResult restored(received);
    ASSERT_EQ(1, restored.size());

    ASSERT_EQ(exp, *restored.at(0)->getNDArray());

    GraphHolder::getInstance()->dropGraphAny(11903L);
}
#endif
