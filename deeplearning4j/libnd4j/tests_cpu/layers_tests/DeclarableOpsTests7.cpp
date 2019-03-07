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
// Created by raver119 on 09.02.18.
//


#include "testlayers.h"
#include <ops/declarable/CustomOperations.h>
#include <helpers/helper_hash.h>
#include <NDArray.h>
#include <array/NDArrayList.h>


using namespace nd4j;
using namespace nd4j::graph;


class DeclarableOpsTests7 : public testing::Test {
public:

    DeclarableOpsTests7() {
        printf("\n");
        fflush(stdout);
    }
};


TEST_F(DeclarableOpsTests7, Test_CHOOSE_SCALAR_LARGE) {
    double inputData[150] = {
            0,  0.51,  0.68,  0.69,  0.86,  0.91,  0.96,  0.97,  0.97,  1.03,  1.13,  1.16,  1.16,  1.17,  1.19,  1.25,  1.25,  1.26,  1.27,  1.28,  1.29,  1.29,  1.29,  1.30,  1.31,  1.32,  1.33,  1.33,  1.35,  1.35,  1.36,  1.37,  1.38,  1.40,  1.41,  1.42,  1.43,  1.44,  1.44,  1.45,  1.45,  1.47,  1.47,  1.51,  1.51,  1.51,  1.52,  1.53,  1.56,  1.57,  1.58,  1.59,  1.61,  1.62,  1.63,  1.63,  1.64,  1.64,  1.66,  1.66,  1.67,  1.67,  1.70,  1.70,  1.70,  1.72,  1.72,  1.72,  1.72,  1.73,  1.74,  1.74,  1.76,  1.76,  1.77,  1.77,  1.80,  1.80,  1.81,  1.82,  1.83,  1.83,  1.84,  1.84,  1.84,  1.85,  1.85,  1.85,  1.86,  1.86,  1.87,  1.88,  1.89,  1.89,  1.89,  1.89,  1.89,  1.91,  1.91,  1.91,  1.92,  1.94,  1.95,  1.97,  1.98,  1.98,  1.98,  1.98,  1.98,  1.99,  2,  2,  2.01,  2.01,  2.02,  2.03,  2.03,  2.03,  2.04,  2.04,  2.05,  2.06,  2.07,  2.08,  2.08,  2.08,  2.08,  2.09,  2.09,  2.10,  2.10,  2.11,  2.11,  2.11,  2.12,  2.12,  2.13,  2.13,  2.14,  2.14,  2.14,  2.14,  2.15,  2.15,  2.16,  2.16,  2.16,  2.16,  2.16,  2.17
    };

    auto x = NDArrayFactory::create<double>(inputData,'c',{1,149});
    nd4j::ops::choose op;
    //greater than test
    auto result = op.execute({&x}, {0.0},{3});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(1);
    auto array = *z;
    z->printIndexedBuffer("CHOOSE test");
    ASSERT_EQ(148,array.e<double>(0));
    //ASSERT_TRUE(exp.isSameShape(z));

    delete result;

}

TEST_F(DeclarableOpsTests7, Test_CHOOSE_SCALAR_ZERO) {
    std::vector<double> data;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
    }



    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    nd4j::ops::choose op;
    //greater than test
    auto result = op.execute({&x}, {0.0},{3});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(1);
    auto array = *z;
    ASSERT_EQ(3,array.e<double>(0));
    //ASSERT_TRUE(exp.isSameShape(z));

    delete result;

}


TEST_F(DeclarableOpsTests7, Test_CHOOSE_SCALAR) {
    std::vector<double> data;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
    }



    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    auto scalar = NDArrayFactory::create<double>('c',{1,1},{0.0});
    nd4j::ops::choose op;
    //greater than test
    auto result = op.execute({&x,&scalar}, {1.0},{3});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);
    ASSERT_EQ(4,z->lengthOf());
    //ASSERT_TRUE(exp.isSameShape(z));

    delete result;

}


TEST_F(DeclarableOpsTests7, Test_CHOOSE_SCALAR_LEFT) {
    std::vector<double> data;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
    }



    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    auto scalar = NDArrayFactory::create<double>('c',{1,1},{0.0});
    nd4j::ops::choose op;
    //greater than test
    auto result = op.execute({&scalar,&x}, {1.0},{3});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);
    ASSERT_EQ(4,z->lengthOf());
    //ASSERT_TRUE(exp.isSameShape(z));

    delete result;

}


TEST_F(DeclarableOpsTests7, Test_CHOOSE_ONLY_SCALAR) {
    std::vector<double> data;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
    }



    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    nd4j::ops::choose op;
    //greater than test
    auto result = op.execute({&x}, {1.0},{3});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);
    ASSERT_EQ(4,z->lengthOf());
    //ASSERT_TRUE(exp.isSameShape(z));

    delete result;

}


TEST_F(DeclarableOpsTests7, Test_CHOOSE_ONLY_SCALAR_GTE) {
    std::vector<double> data;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
    }



    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    nd4j::ops::choose op;
    //greater than test
    auto result = op.execute({&x}, {1.0},{5});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);
    ASSERT_EQ(4,z->lengthOf());
    //ASSERT_TRUE(exp.isSameShape(z));

    delete result;

}


TEST_F(DeclarableOpsTests7, TEST_WHERE) {
    std::vector<double> data;
    std::vector<bool> mask;
    std::vector<double> put;
    std::vector<double> resultData;
    std::vector<double> assertion;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
        if(i >  1) {
            assertion.push_back(5.0);
            mask.push_back(true);
        }
        else {
            assertion.push_back(i);
            mask.push_back(false);
        }

        put.push_back(5.0);
        resultData.push_back(0.0);
    }




    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    auto maskArr = NDArrayFactory::create<bool>('c',{1,4},mask);
    auto putArr = NDArrayFactory::create<double>('c',{1,4},put);
    auto resultArr = NDArrayFactory::create<double>('c',{1,4},resultData);
    nd4j::ops::where_np op;
    //greater than test
    //            Nd4jStatus execute(std::initializer_list<NDArray<T>*> inputs, std::initializer_list<NDArray<T>*> outputs , std::initializer_list<T> tArgs, std::initializer_list<int> iArgs, bool isInplace = false);

    auto result = op.execute({&maskArr,&x,&putArr},{&resultArr}, {},{3}, {}, false);
    ASSERT_EQ(Status::OK(), result);
    for(int i = 0; i < 4; i++)
        ASSERT_EQ(assertion[i],resultArr.e<double>(i));
    // auto z = result->at(0);
    //ASSERT_EQ(4,z->lengthOf());
    //ASSERT_TRUE(exp.isSameShape(z));


}

TEST_F(DeclarableOpsTests7, TEST_WHERE_MASK) {
    double x[300] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    double z[300] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    bool mask[300] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    double put[200] = {0.99666107,0.9867112,0.97686064,0.9671082,0.95745337,0.9478948,0.9384318,0.92906314,0.9197881,0.91060543,0.9015147,0.8925147,0.8836044,0.8747831,0.86605,0.85740393,0.8488442,0.84037,0.83198035,0.8236745,0.8154515,0.8073106,0.79925096,0.79127187,0.7833724,0.77555174,0.76780915,0.7601439,0.75255525,0.7450422,0.7376043,0.73024046,0.72295034,0.715733,0.7085876,0.7015135,0.69451016,0.68757665,0.6807124,0.6739167,0.66718876,0.66052806,0.6539338,0.6474054,0.6409421,0.6345435,0.6282087,0.6219371,0.6157281,0.60958105,0.6034956,0.59747064,0.5915059,0.5856007,0.57975453,0.5739667,0.5682366,0.5625637,0.5569475,0.5513874,0.54588276,0.540433,0.53503764,0.5296962,0.52440816,0.51917285,0.5139898,0.5088585,0.50377846,0.4987491,0.4937699,0.48884052,0.48396033,0.47912875,0.47434545,0.4696099,0.46492168,0.46028027,0.45568514,0.4511359,0.44663212,0.4421733,0.43775895,0.43338865,0.42906195,0.42477852,0.4205379,0.41633952,0.41218308,0.40806815,0.40399432,0.3999611,0.3959682,0.39201516,0.38810158,0.384227,0.38039115,0.37659356,0.37283397,0.3691119,0.36542687,0.36177874,0.35816705,0.3545914,0.35105142,0.34754673,0.34407702,0.34064204,0.33724132,0.3338745,0.33054137,0.3272415,0.32397458,0.32074028,0.3175382,0.31436813,0.31122974,0.3081226,0.30504647,0.30200112,0.2989862,0.29600134,0.29304633,0.2901207,0.28722438,0.28435695,0.2815181,0.27870762,0.27592525,0.27317056,0.27044344,0.26774356,0.26507056,0.2624243,0.25980446,0.25721073,0.25464293,0.25210077,0.249584,0.24709237,0.24462552,0.24218333,0.23976555,0.23737194,0.23500215,0.23265606,0.23033342,0.22803394,0.22575743,0.2235036,0.22127232,0.21906327,0.21687631,0.21471114,0.21256764,0.21044552,0.20834461,0.20626466,0.20420544,0.20216681,0.20014854,0.19815037,0.19617215,0.19421372,0.19227484,0.19035533,0.18845497,0.18657354,0.18471093,0.18286693,0.18104129,0.17923392,0.17744459,0.17567308,0.1739193,0.17218304,0.17046405,0.16876228,0.16707748,0.16540948,0.16375816,0.16212334,0.16050482,0.15890247,0.15731607,0.15574552,0.15419069,0.15265137,0.15112738,0.14961864,0.14812498,0.14664622,0.1451822,0.14373279,0.14229788,0.14087726,0.13947085,0.13807845,0.13669999,0.13533528};
    double assertion[300] = {1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,1.000000000000000000e+00,9.966611049434810354e-01,9.867111603284486332e-01,9.768605487739230320e-01,9.671082786103732953e-01,9.574533680683808834e-01,9.478948451798039354e-01,9.384317476799283186e-01,9.290631229105962285e-01,9.197880277243004610e-01,9.106055283892373620e-01,9.015147004953073528e-01,8.925146288610534828e-01,8.836044074415293492e-01,8.747831392370875037e-01,8.660499362030764647e-01,8.574039191604412302e-01,8.488442177072155204e-01,8.403699701308978698e-01,8.319803233217017979e-01,8.236744326866727306e-01,8.154514620646623468e-01,8.073105836421510251e-01,7.992509778699116163e-01,7.912718333805045523e-01,7.833723469065965173e-01,7.755517232000953554e-01,7.678091749520912224e-01,7.601439227135980969e-01,7.525551948170853267e-01,7.450422272987937689e-01,7.376042638218265335e-01,7.302405556000080011e-01,7.229503613225031211e-01,7.157329470791886639e-01,7.085875862867698771e-01,7.015135596156351072e-01,6.945101549174396149e-01,6.875766671534137009e-01,6.807123983233853703e-01,6.739166573955123196e-01,6.671887602367149173e-01,6.605280295438040739e-01,6.539337947752965619e-01,6.474053920839111242e-01,6.409421642497381555e-01,6.345434606140767375e-01,6.282086370139332576e-01,6.219370557171712832e-01,6.157280853583116942e-01,6.095811008749726367e-01,6.034954834449430816e-01,5.974706204238864338e-01,5.915059052836644238e-01,5.856007375512777280e-01,5.797545227484157682e-01,5.739666723316099173e-01,5.682366036329845604e-01,5.625637398015992385e-01,5.569475097453767676e-01,5.513873480736106725e-01,5.458826950400470501e-01,5.404329964865340896e-01,5.350377037872348085e-01,5.296962737933965659e-01,5.244081687786711354e-01,5.191728563849821176e-01,5.139898095689314772e-01,5.088585065487419845e-01,5.037784307517284565e-01,4.987490707622945774e-01,4.937699202704479151e-01,4.888404780208293054e-01,4.839602477622509946e-01,4.791287381977387683e-01,4.743454629350723484e-01,4.696099404378203390e-01,4.649216939768630041e-01,4.602802515824001017e-01,4.556851459964368911e-01,4.511359146257447605e-01,4.466320994952920342e-01,4.421732472021388527e-01,4.377589088697927955e-01,4.333886401030203062e-01,4.290620009431086457e-01,4.247785558235752101e-01,4.205378735263185508e-01,4.163395271382073215e-01,4.121830940081024908e-01,4.080681557043087104e-01,4.039942979724505667e-01,3.999611106937689398e-01,3.959681878438343627e-01,3.920151274516718853e-01,3.881015315592946102e-01,3.842270061816405180e-01,3.803911612669100828e-01,3.765936106572991271e-01,3.728339720501240850e-01,3.691118669593352886e-01,3.654269206774144463e-01,3.617787622376523182e-01,3.581670243768036999e-01,3.545913434981138868e-01,3.510513596347161203e-01,3.475467164133922426e-01,3.440770610186974499e-01,3.406420441574410929e-01,3.372413200235238606e-01,3.338745462631242389e-01,3.305413839402346898e-01,3.272414975025391692e-01,3.239745547476344245e-01,3.207402267895853032e-01,3.175381880258169032e-01,3.143681161043347383e-01,3.112296918912743071e-01,3.081225994387726264e-01,3.050465259531625062e-01,3.020011617634821843e-01,2.989862002903017069e-01,2.960013380148582840e-01,2.930462744485015647e-01,2.901207121024425017e-01,2.872243564578055852e-01,2.843569159359789489e-01,2.815181018692606840e-01,2.787076284717992514e-01,2.759252128108221624e-01,2.731705747781537075e-01,2.704434370620155681e-01,2.677435251191103149e-01,2.650705671469821278e-01,2.624242940566549609e-01,2.598044394455423789e-01,2.572107395706292876e-01,2.546429333219200064e-01,2.521007621961529055e-01,2.495839702707757235e-01,2.470923041781825646e-01,2.446255130802063582e-01,2.421833486428674187e-01,2.397655650113727777e-01,2.373719187853666479e-01,2.350021689944260528e-01,2.326560770738031469e-01,2.303334068404078172e-01,2.280339244690317291e-01,2.257573984688081292e-01,2.235035996599082919e-01,2.212723011504689752e-01,2.190632783137518302e-01,2.168763087655291855e-01,2.147111723416972873e-01,2.125676510761114746e-01,2.104455291786438698e-01,2.083445930134591173e-01,2.062646310775079761e-01,2.042054339792348794e-01,2.021667944174980747e-01,2.001485071607009836e-01,1.981503690261307848e-01,1.961721788595043592e-01,1.942137375147174327e-01,1.922748478337968081e-01,1.903553146270518526e-01,1.884549446534251604e-01,1.865735466010380594e-01,1.847109310679319050e-01,1.828669105430000552e-01,1.810412993871116094e-01,1.792339138144224131e-01,1.774445718738737465e-01,1.756730934308744496e-01,1.739193001491673995e-01,1.721830154728755669e-01,1.704640646087285105e-01,1.687622745084652875e-01,1.670774738514141378e-01,1.654094930272448083e-01,1.637581641188943782e-01,1.621233208856623365e-01,1.605047987464754966e-01,1.589024347633189727e-01,1.573160676248336609e-01,1.557455376300762306e-01,1.541906866724424563e-01,1.526513582237501165e-01,1.511273973184814046e-01,1.496186505381822129e-01,1.481249659960175158e-01,1.466461933214808777e-01,1.451821836452561187e-01,1.437327895842310799e-01,1.422978652266598532e-01,1.408772661174743090e-01,1.394708492437411185e-01,1.380784730202649913e-01,1.366999972753347725e-01,1.353352832366127023e-01};
    Nd4jLong threeHundredShapePointer[8] = {2,1,300,1,1,0,1,99};
    Nd4jLong twoHundredShapePointer[8] = {2,1,200,1,1,0,1,99};
    nd4j::ops::where_np op;
    ArrayOptions::setDataType(threeHundredShapePointer, nd4j::DataType::DOUBLE);
    ArrayOptions::setDataType(twoHundredShapePointer, nd4j::DataType::DOUBLE);

    NDArray xArr(x,threeHundredShapePointer);
    NDArray putArr(put,twoHundredShapePointer);
    NDArray resultArr(z,threeHundredShapePointer);

    resultArr.assign(0.0);
    ArrayOptions::setDataType(threeHundredShapePointer, nd4j::DataType::BOOL);
    NDArray maskArr(mask,threeHundredShapePointer);

    ArrayOptions::setDataType(threeHundredShapePointer, nd4j::DataType::DOUBLE);
    NDArray assertArr(assertion, threeHundredShapePointer);
    Nd4jStatus result = op.execute({&maskArr, &xArr, &putArr},{&resultArr},{},{},{});
    ASSERT_EQ(Status::OK(),result);
    ASSERT_TRUE(assertArr.isSameShape(resultArr));
    ASSERT_TRUE (assertArr.equalsTo(resultArr));
}

TEST_F(DeclarableOpsTests7, TEST_WHERE_SCALAR) {
    std::vector<double> data;
    std::vector<bool> mask;
    std::vector<double> put;
    std::vector<double> resultData;
    std::vector<double> assertion;
    for(Nd4jLong i = 0; i < 4; i++) {
        data.push_back(i);
        if(i >  1) {
            assertion.push_back(5.0);
            mask.push_back(true);
        }
        else {
            assertion.push_back(i);
            mask.push_back(false);
        }

        resultData.push_back(0.0);
    }


    put.push_back(5.0);


    auto x = NDArrayFactory::create<double>('c',{1,4},data);
    auto maskArr = NDArrayFactory::create<bool>('c',{1,4},mask);
    auto putArr = NDArrayFactory::create<double>('c',{1,1},put);
    auto resultArr = NDArrayFactory::create<double>('c',{1,4},resultData);
    nd4j::ops::where_np op;
    //greater than test
    //            Nd4jStatus execute(std::initializer_list<NDArray<T>*> inputs, std::initializer_list<NDArray<T>*> outputs , std::initializer_list<T> tArgs, std::initializer_list<int> iArgs, bool isInplace = false);

    auto result = op.execute({&maskArr,&x,&putArr},{&resultArr}, {},{3}, {}, false);
    // ASSERT_EQ(Status::OK(), result->status());
    for(int i = 0; i < 4; i++)
        ASSERT_EQ(assertion[i],resultArr.e<double>(i));
    // auto z = result->at(0);
    //ASSERT_EQ(4,z->lengthOf());
    //ASSERT_TRUE(exp.isSameShape(z));


}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestMatrixDiagPart_1) {
    auto x = NDArrayFactory::create<double>('c', {2, 4, 4}, {1., 0., 0., 0., 0., 2., 0., 0., 0., 0., 3., 0., 0., 0., 0., 4.,5., 0., 0., 0., 0., 6., 0., 0., 0., 0., 7., 0., 0., 0., 0., 8.});

    auto z = NDArrayFactory::create<double>('c', {2, 4}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0});

    nd4j::ops::matrix_diag_part op;

    auto result = op.execute({&x}, {}, {});

    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(z.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestMatrixDiagPart_2) {
    auto x = NDArrayFactory::create<double>('c', {2, 3, 4}, {1., 0., 0., 0., 0., 2., 0., 0., 0., 0., 3., 0.,5., 0., 0., 0., 0., 6., 0., 0., 0., 0., 7., 0.});

    auto z = NDArrayFactory::create<double>('c', {2, 3}, {1.0, 2.0, 3.0, 5.0, 6.0, 7.0});

    nd4j::ops::matrix_diag_part op;

    auto result = op.execute({&x}, {}, {});

    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(z.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestMatrixDiag_1) {
    auto z = NDArrayFactory::create<double>('c', {2, 4, 4}, {1., 0., 0., 0., 0., 2., 0., 0., 0., 0., 3., 0., 0., 0., 0., 4.,5., 0., 0., 0., 0., 6., 0., 0., 0., 0., 7., 0., 0., 0., 0., 8.});

    auto x = NDArrayFactory::create<double>('c', {2, 4}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0});

    nd4j::ops::matrix_diag op;

    auto result = op.execute({&x}, {}, {});

    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(z.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestMatrixDiag_2) {
    auto z = NDArrayFactory::create<double>('c', {2, 3, 3}, {1., 0., 0., 0., 2., 0., 0., 0., 3.,5., 0., 0., 0., 6., 0.,0., 0., 7.});
    auto x = NDArrayFactory::create<double>('c', {2, 3}, {1.0, 2.0, 3.0, 5.0, 6.0, 7.0});

    nd4j::ops::matrix_diag op;

    auto result = op.execute({&x}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(z.equalsTo(result->at(0)));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRandomCrop_1) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4}, {1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1. });
    auto shape = NDArrayFactory::create<int>({1, 2, 3});
    nd4j::ops::random_crop op;

    auto result = op.execute({&x, &shape}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
//    ASSERT_TRUE(z.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRandomCrop_2) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4}, {1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1. });
    auto shape = NDArrayFactory::create<Nd4jLong>({2, 2, 2});
    nd4j::ops::random_crop op;

    auto result = op.execute({&x, &shape}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
//    ASSERT_TRUE(z.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Dynamic_Stitch_119) {
    auto indices0 = NDArrayFactory::create<double>('c', {2}, {1.0f, 10.f});
    auto indices1 = NDArrayFactory::create<double>('c', {2, 3}, {0.f, 7.f, 9.f, 5.f, 8.f, 3.f});
    auto indices2 = NDArrayFactory::create<double>('c', {3, 1}, {6.f, 4.f, 2.f});
    auto data0 = NDArrayFactory::create<double>('c', {2,5,4}, {1.f, 2.f, 3.f, 4.f,5.f, 6.f, 7.f, 8.f,9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f,21.f, 22.f, 23.f, 24.f,
                                        25.f, 26.f, 27.f, 28.f,29.f, 30.f, 31.f, 32.f,33.f, 34.f, 35.f, 36.f,37.f, 38.f, 39.f, 40.f});

    auto data1 = NDArrayFactory::create<double>('c', {2,3,5,4},{1.f, 2.f, 3.f, 4.f,5.f, 6.f, 7.f, 8.f,9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f,21.f, 22.f, 23.f, 24.f,25.f, 26.f, 27.f, 28.f,
                                        29.f, 30.f, 31.f, 32.f,33.f, 34.f, 35.f, 36.f,37.f, 38.f, 39.f, 40.f,41.f, 42.f, 43.f, 44.f,45.f, 46.f, 47.f, 48.f,49.f, 50.f, 51.f, 52.f,53.f, 54.f, 55.f, 56.f,
                                        57.f, 58.f, 59.f, 60.f,61.f, 62.f, 63.f, 64.f,65.f, 66.f, 67.f, 68.f,69.f, 70.f, 71.f, 72.f,73.f, 74.f, 75.f, 76.f,77.f, 78.f, 79.f, 80.f,81.f, 82.f, 83.f, 84.f,
                                        85.f, 86.f, 87.f, 88.f,89.f, 90.f, 91.f, 92.f,93.f, 94.f, 95.f, 96.f,97.f, 98.f, 99.f, 100.f,101.f, 102.f, 103.f, 104.f,105.f, 106.f, 107.f, 108.f,109.f, 110.f, 111.f, 112.f,
                                        113.f, 114.f, 115.f, 116.f,117.f, 118.f, 119.f, 120.f});

    auto data2 = NDArrayFactory::create<double>('c', {3,1,5,4}, {1.f, 2.f, 3.f, 4.f,5.f, 6.f, 7.f, 8.f,9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f,21.f, 22.f, 23.f, 24.f,
                                        25.f, 26.f, 27.f, 28.f,29.f, 30.f, 31.f, 32.f,33.f, 34.f, 35.f, 36.f,37.f, 38.f, 39.f, 40.f,41.f, 42.f, 43.f, 44.f,45.f, 46.f, 47.f, 48.f,
                                        49.f, 50.f, 51.f, 52.f,53.f, 54.f, 55.f, 56.f,57.f, 58.f, 59.f, 60.f});

    auto exp = NDArrayFactory::create<double>('c', {11, 5, 4}, {1.f,   2.f,   3.f,   4.f,5.f,   6.f,   7.f,   8.f,9.f,  10.f,  11.f,  12.f,13.f,  14.f,  15.f,  16.f,17.f,  18.f,  19.f,  20.f,1.f,   2.f,   3.f,   4.f,
                                          5.f,   6.f,   7.f,   8.f,9.f,  10.f,  11.f,  12.f,13.f,  14.f,  15.f,  16.f,17.f,  18.f,  19.f,  20.f,41.f,  42.f,  43.f,  44.f,45.f,  46.f,  47.f,  48.f,
                                         49.f,  50.f,  51.f,  52.f,53.f,  54.f,  55.f,  56.f,57.f,  58.f,  59.f,  60.f,101.f, 102.f, 103.f, 104.f,105.f, 106.f, 107.f, 108.f,109.f, 110.f, 111.f, 112.f,
                                        113.f, 114.f, 115.f, 116.f,117.f, 118.f, 119.f, 120.f,21.f,  22.f,  23.f,  24.f,25.f,  26.f,  27.f,  28.f,29.f,  30.f,  31.f,  32.f,33.f,  34.f,  35.f,  36.f,
                                         37.f,  38.f,  39.f,  40.f,61.f,  62.f,  63.f,  64.f,65.f,  66.f,  67.f,  68.f,69.f,  70.f,  71.f,  72.f,73.f,  74.f,  75.f,  76.f,77.f,  78.f,  79.f,  80.f,
                                          1.f,   2.f,   3.f,   4.f,5.f,   6.f,   7.f,   8.f,9.f,  10.f,  11.f,  12.f,13.f,  14.f,  15.f,  16.f,17.f,  18.f,  19.f,  20.f,21.f,  22.f,  23.f,  24.f,
                                         25.f,  26.f,  27.f,  28.f,29.f,  30.f,  31.f,  32.f,33.f,  34.f,  35.f,  36.f,37.f,  38.f,  39.f,  40.f,81.f,  82.f,  83.f,  84.f,85.f,  86.f,  87.f,  88.f,
                                         89.f,  90.f,  91.f,  92.f,93.f,  94.f,  95.f,  96.f,97.f,  98.f,  99.f, 100.f,41.f,  42.f,  43.f,  44.f,45.f,  46.f,  47.f,  48.f,49.f,  50.f,  51.f,  52.f,
                                         53.f,  54.f,  55.f,  56.f,57.f,  58.f,  59.f,  60.f,21.f,  22.f,  23.f,  24.f,25.f,  26.f,  27.f,  28.f,29.f,  30.f,  31.f,  32.f,33.f,  34.f,  35.f,  36.f,37.f,  38.f,  39.f,  40.f});

    nd4j::ops::dynamic_stitch op;
    auto result = op.execute({&indices0, &indices1, &indices2, &data0, &data1, &data2}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());
//    result->at(0)->printIndexedBuffer("Output");
//    exp.printIndexedBuffer("Expect");
//    result->at(0)->printShapeInfo("Output shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));
    delete result;
}
////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Dynamic_Stitch_Prof_1) {
    auto indices0 = NDArrayFactory::create<double>('c', {2}, {1.0f, 10.f});
    auto indices1 = NDArrayFactory::create<double>('c', {2, 3}, {0.f, 7.f, 9.f, 5.f, 8.f, 3.f});
    auto indices2 = NDArrayFactory::create<double>('c', {3, 1}, {6.f, 4.f, 2.f});
    auto data0 = NDArrayFactory::create<double>('c', {2,5,4}, {1.f, 2.f, 3.f, 4.f,5.f, 6.f, 7.f, 8.f,9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f,21.f, 22.f, 23.f, 24.f,
                                        25.f, 26.f, 27.f, 28.f,29.f, 30.f, 31.f, 32.f,33.f, 34.f, 35.f, 36.f,37.f, 38.f, 39.f, 40.f});

    auto data1 = NDArrayFactory::create<double>('c', {2,3,5,4},{1.f, 2.f, 3.f, 4.f,5.f, 6.f, 7.f, 8.f,9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f,21.f, 22.f, 23.f, 24.f,25.f, 26.f, 27.f, 28.f,
                                        29.f, 30.f, 31.f, 32.f,33.f, 34.f, 35.f, 36.f,37.f, 38.f, 39.f, 40.f,41.f, 42.f, 43.f, 44.f,45.f, 46.f, 47.f, 48.f,49.f, 50.f, 51.f, 52.f,53.f, 54.f, 55.f, 56.f,
                                        57.f, 58.f, 59.f, 60.f,61.f, 62.f, 63.f, 64.f,65.f, 66.f, 67.f, 68.f,69.f, 70.f, 71.f, 72.f,73.f, 74.f, 75.f, 76.f,77.f, 78.f, 79.f, 80.f,81.f, 82.f, 83.f, 84.f,
                                        85.f, 86.f, 87.f, 88.f,89.f, 90.f, 91.f, 92.f,93.f, 94.f, 95.f, 96.f,97.f, 98.f, 99.f, 100.f,101.f, 102.f, 103.f, 104.f,105.f, 106.f, 107.f, 108.f,109.f, 110.f, 111.f, 112.f,
                                        113.f, 114.f, 115.f, 116.f,117.f, 118.f, 119.f, 120.f});

    auto data2 = NDArrayFactory::create<double>('c', {3,1,5,4}, {1.f, 2.f, 3.f, 4.f,5.f, 6.f, 7.f, 8.f,9.f, 10.f, 11.f, 12.f,13.f, 14.f, 15.f, 16.f,17.f, 18.f, 19.f, 20.f,21.f, 22.f, 23.f, 24.f,
                                        25.f, 26.f, 27.f, 28.f,29.f, 30.f, 31.f, 32.f,33.f, 34.f, 35.f, 36.f,37.f, 38.f, 39.f, 40.f,41.f, 42.f, 43.f, 44.f,45.f, 46.f, 47.f, 48.f,
                                        49.f, 50.f, 51.f, 52.f,53.f, 54.f, 55.f, 56.f,57.f, 58.f, 59.f, 60.f});

    auto exp = NDArrayFactory::create<double>('c', {11, 5, 4}, {1.f,   2.f,   3.f,   4.f,5.f,   6.f,   7.f,   8.f,9.f,  10.f,  11.f,  12.f,13.f,  14.f,  15.f,  16.f,17.f,  18.f,  19.f,  20.f,1.f,   2.f,   3.f,   4.f,
                                          5.f,   6.f,   7.f,   8.f,9.f,  10.f,  11.f,  12.f,13.f,  14.f,  15.f,  16.f,17.f,  18.f,  19.f,  20.f,41.f,  42.f,  43.f,  44.f,45.f,  46.f,  47.f,  48.f,
                                         49.f,  50.f,  51.f,  52.f,53.f,  54.f,  55.f,  56.f,57.f,  58.f,  59.f,  60.f,101.f, 102.f, 103.f, 104.f,105.f, 106.f, 107.f, 108.f,109.f, 110.f, 111.f, 112.f,
                                        113.f, 114.f, 115.f, 116.f,117.f, 118.f, 119.f, 120.f,21.f,  22.f,  23.f,  24.f,25.f,  26.f,  27.f,  28.f,29.f,  30.f,  31.f,  32.f,33.f,  34.f,  35.f,  36.f,
                                         37.f,  38.f,  39.f,  40.f,61.f,  62.f,  63.f,  64.f,65.f,  66.f,  67.f,  68.f,69.f,  70.f,  71.f,  72.f,73.f,  74.f,  75.f,  76.f,77.f,  78.f,  79.f,  80.f,
                                          1.f,   2.f,   3.f,   4.f,5.f,   6.f,   7.f,   8.f,9.f,  10.f,  11.f,  12.f,13.f,  14.f,  15.f,  16.f,17.f,  18.f,  19.f,  20.f,21.f,  22.f,  23.f,  24.f,
                                         25.f,  26.f,  27.f,  28.f,29.f,  30.f,  31.f,  32.f,33.f,  34.f,  35.f,  36.f,37.f,  38.f,  39.f,  40.f,81.f,  82.f,  83.f,  84.f,85.f,  86.f,  87.f,  88.f,
                                         89.f,  90.f,  91.f,  92.f,93.f,  94.f,  95.f,  96.f,97.f,  98.f,  99.f, 100.f,41.f,  42.f,  43.f,  44.f,45.f,  46.f,  47.f,  48.f,49.f,  50.f,  51.f,  52.f,
                                         53.f,  54.f,  55.f,  56.f,57.f,  58.f,  59.f,  60.f,21.f,  22.f,  23.f,  24.f,25.f,  26.f,  27.f,  28.f,29.f,  30.f,  31.f,  32.f,33.f,  34.f,  35.f,  36.f,37.f,  38.f,  39.f,  40.f});

    nd4j::ops::dynamic_stitch op;
    auto result = op.execute({&indices0, &indices1, &indices2, &data0, &data1, &data2}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());
//    result->at(0)->printIndexedBuffer("Output");
//    exp.printIndexedBuffer("Expect");
//    result->at(0)->printShapeInfo("Output shape");
    auto res = result->at(0);
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));
    int numOfCases = 100;
    auto timeStart = std::chrono::system_clock::now();
    
    for (int i = 0; i < numOfCases; i++) {
       op.execute({&indices0, &indices1, &indices2, &data0, &data1, &data2}, {res}, {}, {}, {});
    }

    auto timeEnd = std::chrono::system_clock::now();
    auto outerTime = std::chrono::duration_cast<std::chrono::microseconds> (timeEnd - timeStart).count();
    //nd4j_printf("dynamic_stitch: Process with %i iterations was load: %lld us.\n", numOfCases, outerTime / numOfCases);

    delete result;
}

////////////////////////////////////////////////////////////////////////////////

TEST_F(DeclarableOpsTests7, Test_Dynamic_Stitch_119_1) {
    auto indices0 = NDArrayFactory::create<double>('c', {2}, {1.0f, 10.f});
    auto indices1 = NDArrayFactory::create<double>('c', {2, 3}, {0,7,9, 5,8,3});
    auto indices2 = NDArrayFactory::create<double>('c', {3, 1}, {6, 4, 2});

    auto data0 = NDArrayFactory::create<double>('c', {2,5,4});
    auto data1 = NDArrayFactory::create<double>('c', {2,3,5,4});
    auto data2 = NDArrayFactory::create<double>('c', {3,1,5,4});

    nd4j::ops::dynamic_stitch op;
    auto result = op.execute({&indices0, &indices1, &indices2, &data0, &data1, &data2}, {}, {});

    ASSERT_EQ(Status::OK(), result->status());

    delete result;
}


TEST_F(DeclarableOpsTests7, Test_Dynamic_Partition_119) {
    auto x = NDArrayFactory::create<double>('c', {5, 4, 11});
    auto y = NDArrayFactory::create<double>('c', {5, 4}, {0,1,2,3, 1,0,2,3, 2,3,1,0, 2,1,0,3, 0,1,2,3});
    auto e = NDArrayFactory::create<double>('c', {5, 11});
    x.assign(1.f);
    e.assign(1.f);
    nd4j::ops::dynamic_partition op;
    auto result = op.execute({&x, &y}, {}, {4});
    ASSERT_EQ(Status::OK(), result->status());
    ASSERT_EQ(4, result->size());
    auto z = result->at(0);
//    z->printShapeInfo("Output shape info");
//    z->printIndexedBuffer("Output1");
//    result->at(1)->printIndexedBuffer("Output2");
//    result->at(2)->printIndexedBuffer("Output3");
//    result->at(3)->printIndexedBuffer("Output4");
    ASSERT_TRUE(e.isSameShape(z));

    delete result;
}

TEST_F(DeclarableOpsTests7, Test_Dynamic_Partition_119_1) {
    auto x = NDArrayFactory::create<double>('c', {3, 4, 2}, {10, 20,11, 21,12, 22,13, 23,14, 24,15, 25,16, 26,17, 27,18, 28,19, 29,20, 30,21, 31});

    auto y = NDArrayFactory::create<double>('c', {3, 4}, {0,0,0,0, 2,2,2,2, 2,1,1,1});
    auto e = NDArrayFactory::create<double>('c', {4, 2}, {10, 20, 11, 21, 12, 22, 13, 23});

//    x.assign(1.f);
//    e.assign(1.f);
    nd4j::ops::dynamic_partition op;
    auto result = op.execute({&x, &y}, {}, {3});
    ASSERT_EQ(Status::OK(), result->status());
    ASSERT_EQ(3, result->size());
    auto z = result->at(0);
//    z->printShapeInfo("Output shape info");
//    result->at(1)->printShapeInfo("Shape2");
//    result->at(2)->printShapeInfo("Shape3");
//    result->at(3)->printShapeInfo("Shape4");
//    z->printIndexedBuffer("Output1");
//    result->at(1)->printIndexedBuffer("Output2");
//    result->at(2)->printIndexedBuffer("Output3");
//    result->at(3)->printIndexedBuffer("Output4");
    ASSERT_TRUE(e.isSameShape(z));

    delete result;
}


TEST_F(DeclarableOpsTests7, Test_Gather_Vector_Case_119) {
    auto input = NDArrayFactory::create<double>('c', {4}, {2.f, 3.f, 4.f, 5.f});
    auto indices = NDArrayFactory::create<int>('c', {2}, {0, 2});
    auto exp = NDArrayFactory::create<double>('c', {2}, {2.f, 4.f});

    nd4j::ops::gather op;
    auto result = op.execute({&input, &indices}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

TEST_F(DeclarableOpsTests7, Test_SequenceMask_1) {
    auto input = NDArrayFactory::create<double>('c', {4, 4},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f});
    auto exp = NDArrayFactory::create<double>('c', {4, 4, 16}, {1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  0.f,  0.f,  0.f,  0.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  1.f,  0.f,  0.f,  0.f,  0.f, 0.f,1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  1.f,  1.f,  0.f,  0.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  1.f,  1.f,  1.f,  0.f,  0.f, 0.f,1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  0.f, 0.f,
                                        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f, 0.f,1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f, 1.f });

    nd4j::ops::sequence_mask op;
    auto result = op.execute({&input}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);
//    z->printIndexedBuffer("Output");
//    z->printShapeInfo("Shape");
    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;

}

TEST_F(DeclarableOpsTests7, Test_SequenceMask_2) {
    auto input = NDArrayFactory::create<double>('c', {2, 2, 2},   {10., 20., 30., 4., 0., 6., 7., 8.});
    auto exp = NDArrayFactory::create<double>('c', {2, 2, 2, 30}, {   1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
    1., 1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
    1., 1., 1., 1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,    1., 1., 1., 1., 1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
    1., 1., 1., 1., 1., 1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.});

    nd4j::ops::sequence_mask op;
    auto result = op.execute({&input}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());

    auto z = result->at(0);
//    z->printIndexedBuffer("Output");
//    z->printShapeInfo("Shape");
    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMax_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({2.5, 9.0, 3.0, 9.0, 4.2});

    nd4j::ops::segment_max op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMaxBP_1) {
    auto x = NDArrayFactory::create<double>({1.8,   2.5,  4.,  9., 2.1, 2.4, 3.,   9., 2.1, 2.1, 0.7, 0.1,  3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({0., 1., 0., 2., 0., 0., 3., 4., 0., 0.,0., 0., 0., 5., 0.,0.});
    auto eps = NDArrayFactory::create<double>('c', {5});
    nd4j::ops::segment_max_bp op;
    eps.linspace(1);
    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMax_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.  });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {2.1, 2.5, 4.0, 9.0,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::segment_max op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMaxBP_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5, 4., 9., 2.1, 2.4, 3., 9., 2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.  });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto eps = NDArrayFactory::create<double>('c', {3, 4}, {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.});
//    NDArray<double> exp('c', {3, 4}, {2.1, 2.5, 4.0, 9.0,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto exp = NDArrayFactory::create<double>('c', {4, 4}, {0., 2., 3., 4., 1., 0., 0., 4., 5., 6., 7., 8., 9., 10., 11., 12.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::segment_max_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 2);
    //exp.printIndexedBuffer("BP Max Expect");
    //result->at(0)->printIndexedBuffer("BP Max Output");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMax_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
     91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {91. , 82. , 37. , 64.,55.1, 46.4, 73. , 28.,119.1, 12.1,112.7, 13.1,14. ,114.2, 16.2,117.,51. , 42. , 87. , 44.,
                     55.1, 56.4, 93. , 28.,119.1, 82.1,112.7,113.1,114. ,114.2,116.2,117.,91. , 82. , 37. , 64.,55.1, 46.4, 73. , 28., 119.1, 12.1,112.7, 13.1,14. ,114.2, 16.2,117. });

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::segment_max op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMax_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,
                                        15.1,  56.4,  93. ,   28.,109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,
                                        119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
                     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
                    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                     31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                      0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                      0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,
                    119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_max op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMax_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,  4.,  9., 2.1, 2.4, 3., 9., 2.1, 2.1, 0.7, 0.1,  3.,  4.2,  2.2, 1.});
    auto idx = NDArrayFactory::create<double>({4.0, 4.0,  1.,  1.,  1.,  1., 2., 3.,  3.,  3., 4.,   4.,  4.,   4.,   0., 0.});
    auto exp = NDArrayFactory::create<double>({2.2, 9., 3., 9., 4.2});

    nd4j::ops::unsorted_segment_max op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMaxBP_1) {
    auto x = NDArrayFactory::create<double>({1.8,   2.5,  4.,  9., 2.1, 2.4, 3.,   9., 2.1, 2.1, 0.7, 0.1,  3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({0., 1., 0., 2., 0., 0., 3., 4., 0., 0.,0., 0., 0., 5., 0.,0.});
    auto eps = NDArrayFactory::create<double>('c', {5});
    nd4j::ops::segment_max_bp op;
    eps.linspace(1);
    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMaxBP_2) {
    auto x = NDArrayFactory::create<double>({3., 1.8,   2.5,  4.,  9., 2.1, 2.4, 9., 2.1, 2.1, 0.7, 0.1,  3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({3., 0., 1., 0., 2., 0., 0., 4., 0., 0.,0., 0., 0., 5., 0.,0.});
    auto eps = NDArrayFactory::create<double>('c', {5});
    nd4j::ops::segment_max_bp op;
    eps.linspace(1);
    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //exp.printIndexedBuffer("Expect");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMax_2) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,  4.,  9., 2.1, 2.4, 3., 9., 2.1, 2.1, 0.7, 0.1,  3.,  4.2,  2.2, 1.});
    auto idx = NDArrayFactory::create<double>({4.0, 4.0,  1.,  1.,  1.,  1., 3., 3.,  3.,  3., 4.,   4.,  4.,   4.,   0., 0.});
    auto exp = NDArrayFactory::create<double>({2.2, 9., -DataTypeUtils::max<double>(), 9., 4.2});

    nd4j::ops::unsorted_segment_max op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMax_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.  });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {2.1, 2.5, 4.0, 9.0,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::unsorted_segment_max op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
    //exp.printIndexedBuffer("Expect");
    //result->at(0)->printIndexedBuffer("Output");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMax_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  8., 2.1, 2.1, 11.7, 0.1, 3., 4.2, 2.2, 1.  });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 0.0, 2.0});
    double principalMax = DataTypeUtils::max<double>();
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {2.1, 2.5, 11.7, 9.0,
                                      -principalMax, -principalMax, -principalMax, -principalMax,
                                      3., 4.2, 2.2, 1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::unsorted_segment_max op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
    //exp.printIndexedBuffer("Expect");
    //result->at(0)->printIndexedBuffer("Output");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMin_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({1.8, 2.1, 3.,  2.1, 0.1});

    nd4j::ops::segment_min op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMinBP_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  9., 2.1, 2.1, 0.7, 0.1, 3., 4.2,  2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({ 1.,   0.,  0., 0.,  2.,   0.,  3.,  0.,  4.,  4.,  0., 5.,  0.,  0.,  0.,  0.});
    auto eps = NDArrayFactory::create<double>('c', {5});
    eps.linspace(1);
    nd4j::ops::segment_min_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output1");
    //exp.printIndexedBuffer("Expecte");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMinBP_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  9., 2.1, 2.1, 0.7, 0.1, 3., 4.2,  2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({ 1.,   0.,  0., 0.,  2.,   0.,  3.,  0.,  4.,  4.,  0., 5.,  0.,  0.,  0.,  0.});
    auto eps = NDArrayFactory::create<double>('c', {5});
    eps.linspace(1);
    nd4j::ops::unsorted_segment_min_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output1");
    //exp.printIndexedBuffer("Expecte");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMinBP_2) {
    auto x = NDArrayFactory::create<double>({3., 1.8, 2.5,  4.,  9., 2.1, 2.4,  9., 2.1, 2.1, 0.7, 0.1, 3., 4.2,  2.2, 1.});
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({3., 1.,   0.,  0., 0.,  2.,   0.,  0.,  4.,  4.,  0., 5.,  0.,  0.,  0.,  0.});
    auto eps = NDArrayFactory::create<double>('c', {5});
    eps.linspace(1);
    nd4j::ops::unsorted_segment_min_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output1");
    //exp.printIndexedBuffer("Expecte");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMin_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<int>({0, 0, 1, 2});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {1.8, 2.4, 3. , 9.,2.1, 2.1, 0.7, 0.1,3. , 4.2, 2.2, 1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::segment_min op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMinBP_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<Nd4jLong>({0, 0, 1, 2});
    auto eps = NDArrayFactory::create<double>('c', {3, 4}, {1., 2., 3. , 4., 5., 6., 7., 8., 9., 10., 11., 12.});
    auto exp = NDArrayFactory::create<double>('c', {4, 4}, {1., 0.,  0.,  4., 0., 2.,  3.,  4., 5., 6., 7., 8., 9., 10., 11., 12.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::segment_min_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 2);
    exp.printIndexedBuffer("Expect");
    result->at(0)->printIndexedBuffer("Output");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMin_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,109.1,  82.1,  12.7, 113.1,
    114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,
    119.1,  12.1, 112.7,  13.1,    14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<int>({0, 1, 1, 2});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,31. ,  22. ,  67. ,  24. ,
                 15.1,  46.4,  73. ,  28. ,109.1,  12.1,  12.7,  13.1,14. ,  14.2,  16.2,  11. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_min op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMin_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
     91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
                     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
                    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                     31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                      0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                      0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_min op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMin_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({1.8, 2.1, 3.,  2.1, 0.1});

    nd4j::ops::unsorted_segment_min op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMin_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {1.8, 2.4, 3. , 9.,2.1, 2.1, 0.7, 0.1,3. , 4.2, 2.2, 1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::unsorted_segment_min op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMin_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, { 91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,109.1,  82.1,  12.7, 113.1,
            114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,
            119.1,  12.1, 112.7,  13.1,    14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,31. ,  22. ,  67. ,  24. ,
                                         15.1,  46.4,  73. ,  28. ,109.1,  12.1,  12.7,  13.1,14. ,  14.2,  16.2,  11. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_min op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMin_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {91.,  82.,  37.,   64., 55.1,  46.4,  73.,  28., 119.1,  12.1, 112.7,  13.1,  14., 114.2,  16.2, 117.,
            51.,  42.,  67.,   24., 15.1,  56.4,  93.,  28., 109.1,  82.1,  12.7, 113.1, 114.,  14.2, 116.2,  11.,
            31.,  22.,  87.,   44., 55.1,  46.4,  73.,  28., 119.1,  12.1, 112.7,  13.1,  14., 114.2,  16.2, 117.,
            91.,  82.,  37.,   64., 55.1,  46.4,  73.,  28., 119.1,  12.1, 112.7,  13.1,  14., 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    double principalMax = DataTypeUtils::max<double>();

    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
            91.,  82.,  37.,  64., 55.1,  46.4,  73.,    28., 119.1, 12.1, 112.7, 13.1,  14., 114.2, 16.2, 117., 51.,
            42.,  67.,  24., 15.1, 56.4,   93.,  28.,  109.1,  82.1, 12.7, 113.1, 114., 14.2, 116.2, 11.,
            principalMax,   principalMax,   principalMax, principalMax,   principalMax,   principalMax,  principalMax,
            principalMax,   principalMax, principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,
            31.,  22.,  87.,  44., 55.1, 46.4,  73.,  28., 119.1, 12.1, 112.7,  13.1, 14., 114.2, 16.2, 117.,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            principalMax,   principalMax,   principalMax,   principalMax,   principalMax,   principalMax,
            91.,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_min op;

    auto result = op.execute({&x, &idx}, {}, {8});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    // exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMean_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({2.15,      4.375,     3.,        4.4,       1.8666667});

    nd4j::ops::segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

TEST_F(DeclarableOpsTests7, TestSegmentMean_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {    1.95,     2.45,       3.5,       9.,    2.1,       2.1,       0.7,      0.1,    3. ,       4.2,       2.2,      1.});

    nd4j::ops::segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    result->at(0)->printIndexedBuffer("Output");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMeanBP_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto eps = NDArrayFactory::create<double>('c', {3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4, 4}, { 0.5, 1., 1.5, 2.,  0.5, 1., 1.5, 2., 5., 6., 7., 8., 9., 10., 11., 12.});
    eps.linspace(1);

    nd4j::ops::segment_mean_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 2);
//    exp.printIndexedBuffer("Expect");
//    result->at(0)->printIndexedBuffer("Output");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMean_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
     91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
             91.  ,      82.  ,      37.  ,      64. ,55.1 ,      46.4 ,      73.  ,      28. ,119.1 ,      12.1 ,     112.7 ,      13.1,14.  ,     114.2 ,      16.2 ,     117. ,
             41.  ,      32.  ,      77.  ,      34. ,35.1 ,      51.4 ,      83.  ,      28. ,114.1 ,      47.1 ,      62.7,      63.1,64.  ,      64.2 ,      66.2 ,      64. ,
             91.  ,      82.  ,      37.  ,      64. ,55.1 ,      46.4 ,      73.  ,      28. ,119.1 ,      12.1 ,     112.7 ,      13.1,14.  ,     114.2 ,      16.2 ,     117. });

    nd4j::ops::segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMean_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
     91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
             91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
             31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
              0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
              0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,
            119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMean_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({2.15,      4.375,     3.,        4.4,       1.8666667});

    nd4j::ops::unsorted_segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentMeanBP_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,      2.,     3.,        4.,       5.});
    auto exp = NDArrayFactory::create<double>({1./2.,  1./2., 2./4., 2./4., 2./4., 2./4, 3., 4./3., 4./3., 4./3.,
                         5./6., 5./6., 5./6., 5./6., 5./6., 5./6.});
    nd4j::ops::segment_mean_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMeanBP_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,      2.,     3.,        4.,       5.});
    auto exp = NDArrayFactory::create<double>({1./2.,  1./2., 2./4., 2./4., 2./4., 2./4, 3., 4./3., 4./3., 4./3.,
                         5./6., 5./6., 5./6., 5./6., 5./6., 5./6.});
    nd4j::ops::unsorted_segment_mean_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMeanBP_2) {
    auto x = NDArrayFactory::create<double>({3.,1.8, 2.5,4.,  9., 2.1, 2.4,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,      2.,     3.,        4.,       5.});
    auto exp = NDArrayFactory::create<double>({3., 1./2.,  1./2., 2./4., 2./4., 2./4., 2./4, 4./3., 4./3., 4./3.,
                         5./6., 5./6., 5./6., 5./6., 5./6., 5./6.});
    nd4j::ops::unsorted_segment_mean_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMean_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {    1.95,     2.45,       3.5,       9.,    2.1,       2.1,       0.7,      0.1,    3. ,       4.2,       2.2,      1.});

    nd4j::ops::unsorted_segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    result->at(0)->printIndexedBuffer("Output");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMean_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
            91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
            91.  ,      82.  ,      37.  ,      64. ,55.1 ,      46.4 ,      73.  ,      28. ,119.1 ,      12.1 ,     112.7 ,      13.1,14.  ,     114.2 ,      16.2 ,     117. ,
            41.  ,      32.  ,      77.  ,      34. ,35.1 ,      51.4 ,      83.  ,      28. ,114.1 ,      47.1 ,      62.7,      63.1,64.  ,      64.2 ,      66.2 ,      64. ,
            91.  ,      82.  ,      37.  ,      64. ,55.1 ,      46.4 ,      73.  ,      28. ,119.1 ,      12.1 ,     112.7 ,      13.1,14.  ,     114.2 ,      16.2 ,     117. });

    nd4j::ops::unsorted_segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentMean_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
            91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,
            119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_mean op;

    auto result = op.execute({&x, &idx}, {}, {8});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSqrtN_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({3.0405593, 8.75,      3.,        7.621024,  4.5723805});

    nd4j::ops::unsorted_segment_sqrt_n op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSqrtN_BP_1) {
    auto x = NDArrayFactory::create<double>({3.,1.8, 2.5,4.,  9., 2.1, 2.4,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1., 2.,      3.,        4.,  5.});
//    NDArray<double> exp({3.0405593, 8.75,      3.,        7.621024,  4.5723805});
    auto exp = NDArrayFactory::create<double>({3., 0.707107, 0.707107, 1., 1., 1., 1., 2.309401, 2.309401, 2.309401, 2.041241, 2.041241, 2.041241, 2.041241, 2.041241, 2.041241});
    nd4j::ops::unsorted_segment_sqrt_n_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Hello Out:");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSqrtN_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, { 2.7577164,  3.4648232,  4.9497476, 12.727922,
                                             2.1,        2.1,        0.7,       0.1,
                                             3. ,        4.2,        2.2,       1.
    });

    nd4j::ops::unsorted_segment_sqrt_n op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    result->at(0)->printIndexedBuffer("Output");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSqrtN_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
            91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
            91.  ,      82.  ,      37.  ,      64. ,55.1 ,      46.4 ,      73.  ,      28. ,119.1 ,      12.1 ,     112.7 ,      13.1,14.  ,     114.2 ,      16.2 ,     117. ,
            57.982758,  45.254833, 108.89445,   48.083263,     49.638893,  72.69058,  117.37973,   39.59798,    161.36177,   66.60946,   88.67119,   89.23688,  90.50967,   90.79251,   93.62093,   90.50967,
            91.  ,      82.  ,      37.  ,      64. ,55.1 ,      46.4 ,      73.  ,      28. ,119.1 ,      12.1 ,     112.7 ,      13.1,14.  ,     114.2 ,      16.2 ,     117. });

    nd4j::ops::unsorted_segment_sqrt_n op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSqrtN_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
            91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,
            119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_sqrt_n op;

    auto result = op.execute({&x, &idx}, {}, {8});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSqrtN_5) {
    auto x = NDArrayFactory::create<double>({1.,2.,5.,7.,3.,1.,3.,4.});
    auto idx = NDArrayFactory::create<double>({3.,1.,0.,0.,2.,0.,3.,2.});
    //NDArray<double> exp({1.7320508075688772, 1.,      1.4142135623730951,        1.4142135623730951});
    auto exp = NDArrayFactory::create<double>({7.5055537, 2.,        4.9497476, 2.828427});
    nd4j::ops::unsorted_segment_sqrt_n op;

    auto result = op.execute({&x, &idx}, {}, {4});
    ASSERT_EQ(result->status(), Status::OK());
   // result->at(0)->printIndexedBuffer("Output");
   // exp.printIndexedBuffer("Expect");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentSum_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({4.3,  17.5,  3.,  13.2,  11.2});

    nd4j::ops::segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentSumBP_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,  2.,  3.,  4.,  5.});
    auto exp = NDArrayFactory::create<double>({ 1.,  1.,  2.,  2.,  2.,  2.,  3.,  4.,  4.,  4.,  5.,  5.,  5.,  5.,  5.,  5.});
    nd4j::ops::segment_sum_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSumBP_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,  2.,  3.,  4.,  5.});
    auto exp = NDArrayFactory::create<double>({ 1.,  1.,  2.,  2.,  2.,  2.,  3.,  4.,  4.,  4.,  5.,  5.,  5.,  5.,  5.,  5.});
    nd4j::ops::unsorted_segment_sum_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSumBP_2) {
    auto x = NDArrayFactory::create<double>({3.,1.8, 2.5,4.,  9., 2.1, 2.4,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,  2.,  3.,  4.,  5.});
    auto exp = NDArrayFactory::create<double>({ 3., 1.,  1.,  2.,  2.,  2.,  2.,  4.,  4.,  4.,  5.,  5.,  5.,  5.,  5.,  5.});
    nd4j::ops::unsorted_segment_sum_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentSum_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {3.9 ,       4.9,        7. ,       18.,2.1 ,       2.1,        0.7,        0.1,3.  ,       4.2,        2.2,        1.});

    nd4j::ops::segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentSumBP_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {4, 4}, {1. , 2.,  3.,  4., 1. , 2., 3.,  4., 5.,  6., 7., 8., 9., 10., 11., 12.});
    auto eps = NDArrayFactory::create<double>('c', {3, 4});
    eps.linspace(1);

    nd4j::ops::segment_sum_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 2);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentSum_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
    109.1,  82.1,  12.7, 113.1,    114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
     91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
                91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,82. ,       64. ,      154. ,       68.  ,
             70.2,      102.8,      166. ,       56.  ,228.2,       94.2,      125.4,     126.2 ,128. ,      128.4,      132.4,      128.  ,91. ,  82. ,  37. ,  64. ,
                  55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentSum_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
     91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
                     91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
                    109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                     31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                      0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
                      0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,
                    119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSum_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({4.3,  17.5,  3.,  13.2,  11.2});

    nd4j::ops::unsorted_segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSum_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,2.1, 2.4,  3.,  9.,2.1, 2.1, 0.7, 0.1,3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {3.9 ,       4.9,        7. ,       18.,2.1 ,       2.1,        0.7,        0.1,3.  ,       4.2,        2.2,        1.});

    nd4j::ops::unsorted_segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSum_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
            109.1,  82.1,  12.7, 113.1,    114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
            91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,82. ,       64. ,      154. ,       68.  ,
            70.2,      102.8,      166. ,       56.  ,228.2,       94.2,      125.4,     126.2 ,128. ,      128.4,      132.4,      128.  ,91. ,  82. ,  37. ,  64. ,
            55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentSum_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,   24.,15.1,  56.4,  93. ,   28.,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,31. ,  22. ,  87.,   44. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,
            91. ,  82. ,  37.,   64. ,55.1,  46.4,  73.,   28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 3.0, 7.0});
    auto exp = NDArrayFactory::create<double>('c', {8, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,51. ,  42. ,  67. ,  24. ,15.1,  56.4,  93. ,  28. ,
            109.1,  82.1,  12.7, 113.1,114. ,  14.2, 116.2,  11. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            31. ,  22. ,  87. ,  44. ,55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,
            0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,0. ,   0. ,   0. ,   0. ,91. ,  82. ,  37. ,  64. ,55.1,  46.4,  73. ,  28. ,
            119.1,  12.1, 112.7,  13.1,14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_sum op;

    auto result = op.execute({&x, &idx}, {}, {8});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentProd_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5, 4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({4.5,    181.44,     3.,      39.69,     1.9404});

    nd4j::ops::segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentProdBP_1) {
    auto x = NDArrayFactory::create<double>({  1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  9., 2.1, 2.1, 0.7, 0.1,  3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,    2.,     3.,      4.,     5.});
    auto exp = NDArrayFactory::create<double>({2.5, 1.8, 90.72, 40.32, 172.8, 151.2, 3., 17.64, 75.6, 75.6, 13.86, 97.02, 3.234, 2.31, 4.41, 9.702});
    nd4j::ops::segment_prod_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("ProdBP Output");
    //exp.printIndexedBuffer("ProdBP Expect");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProdBP_1) {
    auto x = NDArrayFactory::create<double>({  1.8, 2.5,  4.,  9., 2.1, 2.4,  3.,  9., 2.1, 2.1, 0.7, 0.1,  3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,    2.,     3.,      4.,     5.});
    auto exp = NDArrayFactory::create<double>({2.5, 1.8, 90.72, 40.32, 172.8, 151.2, 3., 17.64, 75.6, 75.6, 13.86, 97.02, 3.234, 2.31, 4.41, 9.702});
    nd4j::ops::segment_prod_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("ProdBP Output");
    //exp.printIndexedBuffer("ProdBP Expect");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProdBP_2) {
    auto x = NDArrayFactory::create<double>({ 3.,  1.8, 2.5,  4.,  9., 2.1, 2.4,  9., 2.1, 2.1, 0.7, 0.1,  3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto eps = NDArrayFactory::create<double>({1.,    2.,     3.,      4.,     5.});
    auto exp = NDArrayFactory::create<double>({1., 2.5, 1.8, 90.72, 40.32, 172.8, 151.2, 17.64, 75.6, 75.6, 13.86, 97.02, 3.234, 2.31, 4.41, 9.702});
    nd4j::ops::segment_prod_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Unsorted ProdBP Output");
    //exp.printIndexedBuffer("Unsorted ProdBP Expect");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentProd_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {
        1.8, 2.5,  4.,  9.,        2.1, 2.4,  3.,  9.,        2.1, 2.1, 0.7, 0.1,         3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {        3.78,       6. ,       12.  ,      81.,        2.1 ,       2.1,        0.7 ,       0.1,        3.  ,       4.2,        2.2 ,       1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentProdBP_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {1.8, 2.5,  4.,  9.,
            2.1, 2.4,  3.,  9.,
            2.1, 2.1, 0.7, 0.1,
            3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto eps = NDArrayFactory::create<double>('c', {3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4, 4}, {2.1, 4.8,  9.,  36., 1.8, 5., 12., 36., 5., 6., 7., 8., 9., 10., 11., 12.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}
    eps.linspace(1);
    nd4j::ops::segment_prod_bp op;

    auto result = op.execute({&x, &idx, &eps}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 2);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestSegmentProd_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
     91. ,  82. ,  37. ,  64. ,     55.1,  46.4,  73. ,  28. ,    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. ,     51. ,  42. ,  67. ,   24.,
     15.1,  56.4,  93. ,   28.,    109.1,  82.1,  12.7, 113.1,    114. ,  14.2, 116.2,  11. ,     31. ,  22. ,  87.,   44. ,     55.1,  46.4,  73.,   28. ,
    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. ,     91. ,  82. ,  37.,   64. ,     55.1,  46.4,  73.,   28. ,    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
                 91. ,  82. ,  37. ,  64. , 55.1,  46.4,  73. ,  28. ,  119.1,  12.1, 112.7,  13.1, 14. , 114.2,  16.2, 117. ,
                1581.0, 924.0, 5829.0, 1056.0,832.01001, 2616.9602, 6789.0, 784.0, 12993.810, 993.41003, 1431.2899, 1481.61, 1596.0, 1621.64, 1882.4401, 1287.0,
                 91. ,  82. ,  37. ,  64. ,   55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1, 14. , 114.2,  16.2, 117. });

    nd4j::ops::segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {});
    ASSERT_EQ(result->status(), Status::OK());
    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProd_1) {
    auto x = NDArrayFactory::create<double>({1.8, 2.5,4.,  9., 2.1, 2.4,3.,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({4.5,    181.44,     3.,      39.69,     1.9404});

    nd4j::ops::unsorted_segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProd_11) {
    auto x = NDArrayFactory::create<double>({3.,1.8, 2.5,4.,  9., 2.1, 2.4,9., 2.1, 2.1,0.7, 0.1, 3., 4.2, 2.2, 1.});
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0});
    auto exp = NDArrayFactory::create<double>({4.5,    181.44,     3.,      39.69,     1.9404});

    nd4j::ops::unsorted_segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {5});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}
////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProd_2) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {
            1.8, 2.5,  4.,  9.,        2.1, 2.4,  3.,  9.,        2.1, 2.1, 0.7, 0.1,         3., 4.2, 2.2, 1.    });
    auto idx = NDArrayFactory::create<double>({0.0, 0.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {        3.78,       6. ,       12.  ,      81.,        2.1 ,       2.1,        0.7 ,       0.1,        3.  ,       4.2,        2.2 ,       1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::unsorted_segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProd_12) {
    auto x = NDArrayFactory::create<double>('c', {4, 4}, {
            3., 4.2, 2.2, 1.,
            1.8, 2.5,  4.,  9.,        2.1, 2.4,  3.,  9.,        2.1, 2.1, 0.7, 0.1    });
    auto idx = NDArrayFactory::create<double>({2.0, 0.0, 0.0, 1.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {        3.78,       6. ,       12.  ,      81.,        2.1 ,       2.1,        0.7 ,       0.1,        3.  ,       4.2,        2.2 ,       1.});

    //{ 2.1, 2.5,  4.,  9., 2.1, 2.1, 0.7, 0.1, 3.,  4.2, 2.2, 1.}

    nd4j::ops::unsorted_segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_EQ(result->size(), 1);
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProd_3) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,     55.1,  46.4,  73. ,  28. ,    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. ,     51. ,  42. ,  67. ,   24.,
            15.1,  56.4,  93. ,   28.,    109.1,  82.1,  12.7, 113.1,    114. ,  14.2, 116.2,  11. ,     31. ,  22. ,  87.,   44. ,     55.1,  46.4,  73.,   28. ,
            119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. ,     91. ,  82. ,  37.,   64. ,     55.1,  46.4,  73.,   28. ,    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({0.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
            91. ,  82. ,  37. ,  64. , 55.1,  46.4,  73. ,  28. ,  119.1,  12.1, 112.7,  13.1, 14. , 114.2,  16.2, 117. ,
            1581.0, 924.0, 5829.0, 1056.0,832.01001, 2616.9602, 6789.0, 784.0, 12993.810, 993.41003, 1431.2899, 1481.61, 1596.0000, 1621.6399, 1882.4401, 1287.0,
            91. ,  82. ,  37. ,  64. ,   55.1,  46.4,  73. ,  28. ,119.1,  12.1, 112.7,  13.1, 14. , 114.2,  16.2, 117. });

    nd4j::ops::unsorted_segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProd_4) {
    auto x = NDArrayFactory::create<double>('c', {4, 4, 4}, {
            91. ,  82. ,  37. ,  64. ,     55.1,  46.4,  73. ,  28. ,    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. ,     51. ,  42. ,  67. ,   24.,
            15.1,  56.4,  93. ,   28.,    109.1,  82.1,  12.7, 113.1,    114. ,  14.2, 116.2,  11. ,     31. ,  22. ,  87.,   44. ,     55.1,  46.4,  73.,   28. ,
            119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. ,     91. ,  82. ,  37.,   64. ,     55.1,  46.4,  73.,   28. ,    119.1,  12.1, 112.7,  13.1,     14. , 114.2,  16.2, 117. });

// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<double>({1.0, 1.0, 1.0, 2.0});
    auto exp = NDArrayFactory::create<double>('c', {3, 4, 4}, {
            1., 1., 1., 1.,      1., 1.,1.,1.,     1.,1.,1.,1.,     1.,1.,1.,1.,

            143871.0, 75768.0, 215673.0, 67584.,     45843.75, 121426.96, 495597.0, 21952.0,
            1547562.8, 12020.262, 161306.38, 19409.092,  22344.0, 185191.27, 30495.531, 150579.0,

            91., 82., 37., 64,     55.1, 46.400002, 73, 28,    119.1, 12.1, 112.7, 13.1,    14.0, 114.2, 16.2, 117.0});

    nd4j::ops::unsorted_segment_prod op;

    auto result = op.execute({&x, &idx}, {}, {3});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
//    result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestUnsortedSegmentProdBP_4) {
    auto x = NDArrayFactory::create<double>('c', {8}, {
            5,1,7,2,3,4,1,3});
    auto gradO = NDArrayFactory::create<double>('c', {4}, {1.0,2.0,3.0,4.0});
// ----------------------------------------------------------------

    auto idx = NDArrayFactory::create<int>({0,0,0,1,2,2,3,3});
    auto exp = NDArrayFactory::create<double>('c', {8}, {
            7.000000, 35.000000, 5.000000, 2.000000, 12.000000, 9.000000, 12.000000, 4.000000
    });
//            1., 1., 1., 1.,      1., 1.,1.,1.,     1.,1.,1.,1.,     1.,1.,1.,1.,
//
//            143871.0, 75768.0, 215673.0, 67584.,     45843.75, 121426.96, 495597.0, 21952.0,
//            1547562.8, 12020.262, 161306.38, 19409.092,  22344.0, 185191.27, 30495.531, 150579.0,
//
//            91., 82., 37., 64,     55.1, 46.400002, 73, 28,    119.1, 12.1, 112.7, 13.1,    14.0, 114.2, 16.2, 117.0});

    nd4j::ops::unsorted_segment_prod_bp op;

    auto result = op.execute({&x, &idx, &gradO}, {}, {4});
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
    //exp.printIndexedBuffer("Expect");
//    exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_1) {
    auto x = NDArrayFactory::create<double>('c', {2,4, 4, 4}, {
     91.,  82.,  37.,  64.,     55.,  46.,  73.,  28.,   119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     51.,  42.,  67.,  24.,     15.,  56.,  93.,  28.,   109.,  82.,  12., 113.,    114.,  14., 116.,  11.,
     31.,  22.,  87.,  44.,     55.,  46.,  73.,  28.,   119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     91.,  82.,  37.,  64.,    55.1, 46.4,  73.,  28.,   119.,  12., 112.,  13.,     14., 114., 16.2, 117.,
     91.,  82.,  37.,  64.,     55.,  46.,  73.,  28.,   119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     51.,  42.,  67.,  24.,     15.,  56.,  93.,  28.,   109.,  82.,  12., 113.,    114.,  14., 116.,  11.,
     31.,  22.,  87.,  44.,     55.,  46.,  73.,  28.,   119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     91.,  82.,  37.,  64.,    55.1, 46.4,  73.,  28.,   119.,  12., 112.,  13.,    140., 110., 160.,  107.});

// ----------------------------------------------------------------


    auto exp = NDArrayFactory::create<double>('c', {2, 4, 4, 4}, {
     91.,  82.,  37.,  64.,     55.,  46.,  73.,  28.,    119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     51.,  42.,  67.,  24.,     15.,  56.,  93.,  28.,    109.,  82.,  12., 113.,    114.,  14., 116.,  11.,
     31.,  22.,  87.,  44.,     55.,  46.,  73.,  28.,    119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     91.,  82.,  37.,  64.,    55.1, 46.4,  73.,  28.,    119.,  12., 112.,  13.,     14., 114., 16.2, 117.,
     91.,  82.,  37.,  64.,     55.,  46.,  73.,  28.,    119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     51.,  42.,  67.,  24.,     15.,  56.,  93.,  28.,    109.,  82.,  12., 113.,    114.,  14., 116.,  11.,
     31.,  22.,  87.,  44.,     55.,  46.,  73.,  28.,    119.,  12., 112.,  13.,     14., 114.,  16., 117.,
     91.,  82.,  37.,  64.,    55.1,  46.4, 73.,  28.,    119.,  12., 112.,  13.,    140., 110., 160., 107.});

    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {1,1,1,1,1,1,0});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_2) {
    auto x = NDArrayFactory::create<double>('c', {3, 3, 4, 3}, {
     11.,  12.,  13.,     12.,  13.,  14.,     15.,  16.,  17.,     18.,  19.,  10.,
      1.,   2.,   3.,      2.,   3.,   4.,     21.,  22.,  23.,     22.,  23.,  24.,
      5.,   6.,   7.,      8.,   9.,   0.,     35.,  36.,  37.,     38.,  39.,  40.,
      9.,   8.,   7.,      6.,   5.,   4.,     49.,  48.,  47.,     46.,  45.,  44.,
      3.,   2.,   1.,      0.,   1.,   2.,     53.,  52.,  51.,     50.,  51.,  52.,
     15.,  16.,  17.,     18.,  19.,  10.,    135., 136., 137.,    138., 139., 140.,
    211.,  12.,  13.,     12., 213.,  14.,     15., 216.,  17.,    128.,  19.,  10.,
     21.,   2.,   3.,      2.,   3.,  24.,     21.,  22., 223.,     22., 223.,  24.,
     25.,   6.,   7.,      8.,   9.,  20.,     35.,  36., 327.,     38., 239.,  40.});

//Images shape is  (3, 3, 4, 3)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
auto exp = NDArrayFactory::create<double>('c', {3, 1, 1, 12}, {
     11.,  12.,  13.,  12.,  13.,  14.,   1.,   2.,   3.,   2.,   3.,   4.,
      9.,   8.,   7.,   6.,   5.,   4.,   3.,   2.,   1.,   0.,   1.,   2.,
    211.,  12.,  13.,  12., 213.,  14.,  21.,   2.,   3.,   2.,   3.,  24.
 });
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {2,2, 3,3, 1,1,0});
    ASSERT_EQ(result->status(), Status::OK());

    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_3) {
    auto x = NDArrayFactory::create<double>('c', {3, 3, 4, 3}, {
     11.,  12.,  13.,     12.,  13.,  14.,     15.,  16.,  17.,     18.,  19.,  10.,
      1.,   2.,   3.,      2.,   3.,   4.,     21.,  22.,  23.,     22.,  23.,  24.,
      5.,   6.,   7.,      8.,   9.,   0.,     35.,  36.,  37.,     38.,  39.,  40.,
      9.,   8.,   7.,      6.,   5.,   4.,     49.,  48.,  47.,     46.,  45.,  44.,
      3.,   2.,   1.,      0.,   1.,   2.,     53.,  52.,  51.,     50.,  51.,  52.,
     15.,  16.,  17.,     18.,  19.,  10.,    135., 136., 137.,    138., 139., 140.,
    211.,  12.,  13.,     12., 213.,  14.,     15., 216.,  17.,    128.,  19.,  10.,
     21.,   2.,   3.,      2.,   3.,  24.,     21.,  22., 223.,     22., 223.,  24.,
     25.,   6.,   7.,      8.,   9.,  20.,     35.,  36., 327.,     38., 239.,  40.});

//Images shape is  (3, 3, 4, 3)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
auto exp = NDArrayFactory::create<double>('c', {3, 1, 2, 6}, {
     11.,  12.,  13.,   5.,   6.,   7.,
     15.,  16.,  17.,  35.,  36.,  37.,

      9.,   8.,   7.,  15.,  16.,  17.,
     49.,  48.,  47., 135., 136., 137.,

    211.,  12.,  13.,  25.,   6.,   7.,
     15., 216.,  17.,  35.,  36., 327.
 });
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {2,1,3,2,2,2,0});
    ASSERT_EQ(result->status(), Status::OK());
//    x.printIndexedBuffer("images");
//    nd4j_printf("input params: ksize = [1, 2, 1, 1], strides = [1, 3, 2, 1], rates = [1, 2, 2, 1]\n", "");
//    result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");

    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_4) {
    auto x = NDArrayFactory::create<double>('c', {3, 3, 4, 3}, {
     11.,  12.,  13.,     12.,  13.,  14.,     15.,  16.,  17.,     18.,  19.,  10.,
      1.,   2.,   3.,      2.,   3.,   4.,     21.,  22.,  23.,     22.,  23.,  24.,
      5.,   6.,   7.,      8.,   9.,   0.,     35.,  36.,  37.,     38.,  39.,  40.,
      9.,   8.,   7.,      6.,   5.,   4.,     49.,  48.,  47.,     46.,  45.,  44.,
      3.,   2.,   1.,      0.,   1.,   2.,     53.,  52.,  51.,     50.,  51.,  52.,
     15.,  16.,  17.,     18.,  19.,  10.,    135., 136., 137.,    138., 139., 140.,
    211.,  12.,  13.,     12., 213.,  14.,     15., 216.,  17.,    128.,  19.,  10.,
     21.,   2.,   3.,      2.,   3.,  24.,     21.,  22., 223.,     22., 223.,  24.,
     25.,   6.,   7.,      8.,   9.,  20.,     35.,  36., 327.,     38., 239.,  40.});

//Images shape is  (3, 3, 4, 3)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
auto exp = NDArrayFactory::create<double>('c', {3, 3, 4, 3}, {
     11.,  12.,  13.,     12.,  13.,  14.,     15.,  16.,  17.,     18.,   19.,  10.,
      1.,   2.,   3.,      2.,   3.,   4.,     21.,  22.,  23.,     22.,  23.,  24.,
      5.,   6.,   7.,      8.,   9.,   0.,     35.,  36.,  37.,     38.,  39.,  40.,
      9.,   8.,   7.,      6.,   5.,   4.,     49.,  48.,  47.,     46.,  45.,  44.,
      3.,   2.,   1.,      0.,   1.,   2.,     53.,  52.,  51.,     50.,  51.,  52.,
     15.,  16.,  17.,     18.,  19.,  10.,    135., 136., 137.,    138., 139., 140.,
    211.,  12.,  13.,     12., 213.,  14.,     15., 216.,  17.,    128.,  19.,  10.,
     21.,   2.,   3.,      2.,   3.,  24.,     21.,  22., 223.,     22., 223.,  24.,
     25.,   6.,   7.,      8.,   9.,  20.,     35.,  36., 327.,     38., 239.,  40.});
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {1,1,1,1,1,1,0});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_5) {
    auto x = NDArrayFactory::create<double>('c', {3, 3, 4, 3}, {
 11.,  12.,  13.,   12.,  13.,  14.,   15.,  16.,  17.,    18.,  19.,  10.,
  1.,   2.,   3.,    2.,   3.,   4.,   21.,  22.,  23.,    22.,  23.,  24.,
  5.,   6.,   7.,    8.,   9.,   0.,   35.,  36.,  37.,   38.,  39.,  40.,
  9.,   8.,   7.,    6.,   5.,   4.,   49.,  48.,  47.,   46.,  45.,  44.,
  3.,   2.,   1.,    0.,   1.,   2.,   53.,  52.,  51.,   50.,  51.,  52.,
 15.,  16.,  17.,   18.,  19.,  10.,  135., 136., 137.,  138., 139., 140.,
211.,  12.,  13.,   12., 213.,  14.,   15., 216.,  17.,  128.,  19.,  10.,
 21.,   2.,   3.,    2.,   3.,  24.,   21.,  22., 223.,   22., 223.,  24.,
 25.,   6.,   7.,    8.,   9.,  20.,   35.,  36., 327.,   38., 239.,  40.});

//Images shape is  (3, 3, 4, 3)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
auto exp = NDArrayFactory::create<double>('c', {3, 1, 1, 18}, {
  11.,  12.,  13.,  15.,  16.,  17.,   1.,   2.,   3.,  21.,  22.,  23.,   5.,  6.,   7.,  35.,  36.,  37.,
   9.,   8.,   7.,  49.,  48.,  47.,   3.,   2.,   1.,  53.,  52.,  51.,  15., 16.,  17., 135., 136., 137.,
 211.,  12.,  13.,  15., 216.,  17.,  21.,   2.,   3.,  21.,  22., 223.,  25.,  6.,   7.,  35.,  36., 327.

//Patch shape is (3, 1, 2, 18)

 });
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {3,2,3,2,1,2,0});
    ASSERT_EQ(result->status(), Status::OK());
//    result->at(0)->printIndexedBuffer("Output");
    //result->at(0)->printShapeInfo("Out Shape");
//    exp.printIndexedBuffer("Expect");
    //exp.printShapeInfo("Exp Shape");
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_6) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,     12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,
    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,     22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42
});

//Images shape is  (3, 3, 4, 3)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
auto exp = NDArrayFactory::create<double>('c', {2, 1, 4, 4}, {
    11.11, 11.12, 12.11, 12.12,     11.21, 11.22, 12.21, 12.22,     11.31, 11.32, 12.31, 12.32,     11.41, 11.42, 12.41, 12.42,
    21.11, 21.12, 22.11, 22.12, 21.21, 21.22, 22.21, 22.22,    21.31, 21.32, 22.31, 22.32,     21.41, 21.42, 22.41, 22.42
 });
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {2,1, 1,1, 1,1,0});
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.isSameShape(result->at(0)));
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_SGO_7) {
    auto x = NDArrayFactory::create<double>('c', {1, 3, 3, 1});
    x.linspace(1);

//Images shape is  (1, 3, 3, 4)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
    auto exp = NDArrayFactory::create<double>('c', {1, 3, 3, 4}, {
            1., 2., 4., 5.,    2., 3., 5., 6.,    3., 0., 6., 0.,
    4., 5., 7., 8.,    5., 6., 8., 9.,    6., 0., 9., 0.,    7., 8., 0., 0.,    8., 9., 0., 0.,   9., 0., 0., 0. });
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {2,2, 1,1, 1,1, 1}); // equiv TF ksizes=[1,2,2,1], strides=[1,1,1,1], rates=[1,1,1,1], padding="SAME"
    ASSERT_EQ(result->status(), Status::OK());
    auto output = result->at(0);
//    output->printBuffer("Output");
//    exp.printBuffer("Expect");
//    for (Nd4jLong e = 0; e < exp.lengthOf(); e++)
//        if (exp.e<double>(e) != output->e<double>(e))
//            printf("%lld ", e);
//        printf("\n");
    //result->at(1)->printBuffer("OUtput2");
    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestExtractImagePatches_SGO_8) {
    auto x = NDArrayFactory::create<double>('c', {1, 3, 3, 2});
    x.linspace(1);

//Images shape is  (1, 3, 3, 4)
//[1, 1, 1, 1]
//[1, 3, 2, 1]
    auto exp = NDArrayFactory::create<double>('c', {1, 3, 3, 8}, {
            1,  2,  3,  4,  7,  8,  9, 10,  3,  4,  5,  6,  9, 10, 11, 12,  5,  6,  0,  0, 11, 12,  0,  0,
            7,  8,  9, 10, 13, 14, 15, 16,  9, 10, 11, 12, 15, 16, 17, 18, 11, 12,  0,  0, 17, 18,  0,  0,
            13, 14, 15, 16, 0, 0, 0, 0, 15, 16, 17, 18, 0, 0, 0, 0, 17, 18, 0, 0, 0, 0, 0, 0 });
// ----------------------------------------------------------------
    nd4j::ops::extract_image_patches op;

    auto result = op.execute({&x}, {}, {2,2, 1,1, 1,1, 1}); // equiv TF ksizes=[1,2,2,1], strides=[1,1,1,1], rates=[1,1,1,1], padding="SAME"
    ASSERT_EQ(result->status(), Status::OK());
    auto output = result->at(0);
//    output->printBuffer("Output");
//    exp.printBuffer("Expect");
//    for (Nd4jLong e = 0; e < exp.lengthOf(); e++)
//        if (exp.e<double>(e) != output->e<double>(e))
//            printf("%lld ", e);
//    printf("\n");
    //result->at(1)->printBuffer("OUtput2");
    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_1) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,     12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,
    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,     22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42
});

auto exp = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    22.21, 22.22, 22.31, 22.32, 22.41, 22.42, 11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,
    12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42, 21.11, 21.12, 21.21, 21.22, 21.31, 21.32,
    21.41, 21.42, 22.11, 22.12
 });
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {6}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_2) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,     12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,
    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,     22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42
});

auto exp = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
         12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,
         22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42,    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42
 });
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {-8}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());
    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_3) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,     12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,
    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,     22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42
});

auto exp = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
         12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,
         22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42,    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42
 });
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {-40}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_4) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,     12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,
    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,     22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42
});

auto exp = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    22.21, 22.22, 22.31, 22.32, 22.41, 22.42, 11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,
    12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42, 21.11, 21.12, 21.21, 21.22, 21.31, 21.32,
    21.41, 21.42, 22.11, 22.12
 });

//    22.21, 22.22, 22.31, 22.32, 22.41, 22.42, 11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,
//    12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42, 21.11, 21.12, 21.21, 21.22, 21.31, 21.32
//    21.41, 21.42, 22.11, 22.12
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {38}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());
    //result->at(0)->printIndexedBuffer("Output 4");
    //exp.printIndexedBuffer("Expect 4");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_4_inplace) {
    auto x = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,     12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42,
    21.11, 21.12, 21.21, 21.22, 21.31, 21.32, 21.41, 21.42,     22.11, 22.12, 22.21, 22.22, 22.31, 22.32, 22.41, 22.42
});

auto exp = NDArrayFactory::create<double>('c', {2, 2, 4, 2}, {
    22.21, 22.22, 22.31, 22.32, 22.41, 22.42, 11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,
    12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42, 21.11, 21.12, 21.21, 21.22, 21.31, 21.32,
    21.41, 21.42, 22.11, 22.12
 });

//    22.21, 22.22, 22.31, 22.32, 22.41, 22.42, 11.11, 11.12, 11.21, 11.22, 11.31, 11.32, 11.41, 11.42,
//    12.11, 12.12, 12.21, 12.22, 12.31, 12.32, 12.41, 12.42, 21.11, 21.12, 21.21, 21.22, 21.31, 21.32
//    21.41, 21.42, 22.11, 22.12
// ----------------------------------------------------------------
    nd4j::ops::roll op;
    NDArray* y = nullptr;
    auto result = op.execute({&x}, {y}, {}, {38}, {}, true, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result, Status::OK());
    //x.printIndexedBuffer("Output 4 inplace");
    //exp.printIndexedBuffer("Expect 4 inplace");

    ASSERT_TRUE(exp.equalsTo(&x));

//    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_5) {
    auto x = NDArrayFactory::create<double>('c', {3, 4}, {
    0., 1., 2., 3., 4, 5., 6., 7., 8., 9., 10., 11.
});

auto exp = NDArrayFactory::create<double>('c', {3, 4}, {
    2., 3., 0., 1., 6., 7., 4., 5., 10., 11., 8., 9.
//     4,  5,  6,  7, 8,  9, 10, 11, 0,  1,  2,  3
});
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {2, 1}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());

    //result->at(0)->printIndexedBuffer("Output");
    //exp.printIndexedBuffer("Expect");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_6) {
    auto x = NDArrayFactory::create<double>('c', {2, 3, 2}, {
    0., 1., 2., 3., 4, 5., 6., 7., 8., 9., 10., 11.
});

auto exp = NDArrayFactory::create<double>('c', {2, 3, 2}, {
    1., 0., 3., 2., 5., 4., 7., 6., 9., 8., 11., 10.
});
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {1, 2}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());

    //result->at(0)->printIndexedBuffer("Output");
    //exp.printIndexedBuffer("Expect");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_7) {
    auto x = NDArrayFactory::create<double>('c', {2, 3, 2}, {
    0., 1., 2., 3.,  4.,  5., 6., 7., 8., 9., 10., 11.
});

auto exp = NDArrayFactory::create<double>('c', {2, 3, 2}, {
    11., 10., 7., 6., 9., 8., 5.,  4., 1., 0., 3., 2.
});
// ----------------------------------------------------------------
    nd4j::ops::roll op;

    auto result = op.execute({&x}, {}, {1, 2, 1, 0}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result->status(), Status::OK());

    //result->at(0)->printIndexedBuffer("Output");
    //exp.printIndexedBuffer("Expect");

    ASSERT_TRUE(exp.equalsTo(result->at(0)));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_8) {
    auto x = NDArrayFactory::create<double>('c', {2, 3, 2}, {
    0., 1., 2., 3.,  4.,  5., 6., 7., 8., 9., 10., 11.
});

auto exp = NDArrayFactory::create<double>('c', {2, 3, 2}, {
    11., 10., 7., 6., 9., 8., 5.,  4., 1., 0., 3., 2.
});
// ----------------------------------------------------------------
    nd4j::ops::roll op;
    NDArray* y = nullptr;
    auto result = op.execute({&x}, {y}, {}, {1, 2, 1, 0}, {}, true, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result, Status::OK());

    //x.printIndexedBuffer("Output");
    //exp.printIndexedBuffer("Expect");

    ASSERT_TRUE(exp.equalsTo(&x));

//    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, TestRoll_9) {
    auto x = NDArrayFactory::create<double>('c', {2, 3, 3}, {
    0., 1., 2., 3.,  4.,  5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17.
});

auto exp = NDArrayFactory::create<double>('c', {2, 3, 3}, {
    6.,  7.,  8., 0., 1., 2., 3., 4., 5., 15., 16., 17., 9., 10., 11., 12., 13., 14.
});
// ----------------------------------------------------------------
    nd4j::ops::roll op;
    NDArray* y = nullptr;
    auto result = op.execute({&x}, {y}, {}, {1, 1}, {}, true, nd4j::DataType::DOUBLE);
    ASSERT_EQ(result, Status::OK());

    //x.printIndexedBuffer("Output");
    //exp.printIndexedBuffer("Expect");

    ASSERT_TRUE(exp.equalsTo(&x));

//    delete result;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, maxpool2d_bp_test1) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,  pH=0,pW=0,  dH=1,dW=1;
    int oH=2,oW=2;
    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 0;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iC, iH, iW});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, iC, oH, oW});
    auto expected = NDArrayFactory::create<double>('c', {bS, iC, iH, iW}, {0. , 0. , 0. ,0. , 0. , 0. ,0. , 0.1, 0.2,0. , 0.3, 0.4,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0.5, 0.6,0. , 0.7, 0.8,
                                                     0. , 0. , 0. ,0. , 0. , 0. ,0. , 0.9, 1. ,0. , 1.1, 1.2,0. , 0. , 0. ,0. , 0. , 0. ,0. , 1.3, 1.4,0. , 1.5, 1.6,
                                                     0. , 0. , 0. ,0. , 0. , 0. ,0. , 1.7, 1.8,0. , 1.9, 2. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 2.1, 2.2,0. , 2.3, 2.4});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::maxpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, maxpool2d_bp_test2) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,  pH=1,pW=1,  dH=1,dW=1;
    int oH=4,oW=4;
    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 0;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iC, iH, iW});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, iC, oH, oW});
    auto expected = NDArrayFactory::create<double>('c', {bS, iC, iH, iW}, {0. ,  0. ,  0. , 0.1,  0.2,  0.7, 0.5,  0.6,  1.5, 2.2,  2.4,  5.4, 0. ,  0. ,  0. , 1.7,  1.8,  3.9, 2.1,  2.2,  4.7, 5.4,  5.6, 11.8,
                                                     0. ,  0. ,  0. , 3.3,  3.4,  7.1, 3.7,  3.8,  7.9, 8.6,  8.8, 18.2, 0. ,  0. ,  0. , 4.9,  5. , 10.3, 5.3,  5.4, 11.1,11.8, 12. , 24.6,
                                                     0. ,  0. ,  0. , 6.5,  6.6, 13.5, 6.9,  7. , 14.3,15. , 15.2, 31. , 0. ,  0. ,  0. , 8.1,  8.2, 16.7, 8.5,  8.6, 17.5,18.2, 18.4, 37.4});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::maxpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, maxpool2d_bp_test3) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,   pH=0,pW=0,   dH=1,dW=1;
    int oH=4,oW=3;
    int paddingMode = 1;             // 1-SAME,  0-VALID
    int dataFormat  = 1;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iH, iW, iC});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, oH, oW, iC});
    auto expected = NDArrayFactory::create<double>('c', {bS, iH, iW, iC}, {0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0.1, 0.2, 0.3, 1.1, 1.3, 1.5, 0. , 0. , 0. , 1. , 1.1, 1.2, 2.9, 3.1, 3.3,
                                                     0. , 0. , 0. , 4.7, 4.9, 5.1,11.2,11.6,12. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. , 3.7, 3.8, 3.9, 8.3, 8.5, 8.7,
                                                     0. , 0. , 0. , 4.6, 4.7, 4.8,10.1,10.3,10.5, 0. , 0. , 0. ,11.9,12.1,12.3,25.6,26. ,26.4});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::maxpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);
    
    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, maxpool2d_bp_test4) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,   pH=0,pW=0,   dH=1,dW=1;
    int oH=2,oW=2;
    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 1;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iH, iW, iC});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, oH, oW, iC});
    auto expected = NDArrayFactory::create<double>('c', {bS, iH, iW, iC}, {0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0.1, 0.2, 0.3,0.4, 0.5, 0.6,
                                                     0. , 0. , 0. ,0.7, 0.8, 0.9,1. , 1.1, 1.2,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,0. , 0. , 0. ,
                                                     0. , 0. , 0. ,1.3, 1.4, 1.5,1.6, 1.7, 1.8,0. , 0. , 0. ,1.9, 2. , 2.1,2.2, 2.3, 2.4});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::maxpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);
    
    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}
 
 
//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, pnormpool2d_bp_test1) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,  pH=0,pW=0,  dH=1,dW=1;
    int oH=2,oW=2;
    int pnorm = 3;
    double eps = 0.;

    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 0;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iC, iH, iW});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, iC, oH, oW});
    auto expected = NDArrayFactory::create<double>('c', {bS, iC, iH, iW}, {9.661570e-04,9.671602e-03,1.306569e-02,3.679184e-02,1.297220e-01,1.040181e-01,1.126750e-01,3.320884e-01,2.340406e-01,1.333333e-01,3.352886e-01,2.070211e-01,
                                                     8.991618e-02,2.160601e-01,1.283173e-01,2.744226e-01,6.364498e-01,3.662123e-01,3.869788e-01,8.808994e-01,4.984556e-01,2.613189e-01,5.818475e-01,3.225517e-01,
                                                     2.065654e-01,4.553546e-01,2.501175e-01,5.190718e-01,1.131343e+00,6.148388e-01,6.362602e-01,1.377521e+00,7.439550e-01,3.833026e-01,8.227519e-01,4.407146e-01,
                                                     3.261206e-01,6.969233e-01,3.717564e-01,7.627507e-01,1.620991e+00,8.600952e-01,8.814538e-01,1.866888e+00,9.873542e-01,5.046682e-01,1.064004e+00,5.602558e-01,
                                                     4.464697e-01,9.389536e-01,4.932274e-01,1.005908e+00,2.108550e+00,1.104095e+00,1.125322e+00,2.354009e+00,1.230180e+00,6.258913e-01,1.305581e+00,6.804127e-01,
                                                     5.671396e-01,1.181128e+00,6.145977e-01,1.248783e+00,2.595083e+00,1.347494e+00,1.368600e+00,2.840157e+00,1.472778e+00,7.470673e-01,1.547362e+00,8.008900e-01});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::pnormpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {eps}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, pnorm, dataFormat});
    auto output = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    expected.printBuffer("Expected");
    output->printBuffer("Outputed");
    ASSERT_TRUE(expected.equalsTo(output));
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, pnormpool2d_bp_test2) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,  pH=0,pW=0,  dH=1,dW=1;
    int oH=2,oW=2;
    int pnorm = 2;
    double eps = 0.;

    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 0;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iC, iH, iW});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, iC, oH, oW});
    auto expected = NDArrayFactory::create<double>('c', {bS, iC, iH, iW}, {0.007931,0.042891,0.040544,0.09369 ,0.276841,0.191675,0.163957,0.442946,0.287512,0.154919,0.373153,0.221172,
                                                     0.15901 ,0.365232,0.207846,0.428282,0.959455,0.534076,0.508585,1.128771,0.623089,0.319794,0.698063,0.379547,
                                                     0.321068,0.692438,0.372316,0.757521,1.620323,0.864566,0.838684,1.787943,0.951023,0.483194,1.023434,0.541058,
                                                     0.483937,1.019414,0.536145,1.085348,2.276996,1.192917,1.166749,2.443606,1.278126,0.646499,1.349361,0.703463,
                                                     0.647021,1.346249,0.699745,1.412654,2.932174,1.520512,1.494153,3.098146,1.604985,0.809791,1.675544,0.866229,
                                                     0.810192,1.673009,0.863237,1.739711,3.58665 ,1.847753,1.82126 ,3.752188,1.931741,0.973081,2.001861,1.029173});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::pnormpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {eps}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, pnorm, dataFormat});
    auto output = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, avgpool2d_bp_test1) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,  pH=0,pW=0,  dH=1,dW=1;
    int oH=2,oW=2;
    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 0;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iC, iH, iW});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, iC, oH, oW});
    auto expected = NDArrayFactory::create<double>('c', {bS, iC, iH, iW}, {0.016667,0.05    ,0.033333,0.066667,0.166667,0.1     ,0.066667,0.166667,0.1     ,0.05    ,0.116667,0.066667,
                                                     0.083333,0.183333,0.1     ,0.2     ,0.433333,0.233333,0.2     ,0.433333,0.233333,0.116667,0.25    ,0.133333,
                                                     0.15    ,0.316667,0.166667,0.333333,0.7     ,0.366667,0.333333,0.7     ,0.366667,0.183333,0.383333,0.2     ,
                                                     0.216667,0.45    ,0.233333,0.466667,0.966667,0.5     ,0.466667,0.966667,0.5     ,0.25    ,0.516667,0.266667,
                                                     0.283333,0.583333,0.3     ,0.6     ,1.233333,0.633333,0.6     ,1.233333,0.633333,0.316667,0.65    ,0.333333,
                                                     0.35    ,0.716667,0.366667,0.733333,1.5     ,0.766667,0.733333,1.5     ,0.766667,0.383333,0.783333,0.4     });
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::avgpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, avgpool2d_bp_test2) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,  pH=1,pW=1,  dH=1,dW=1;
    int oH=4,oW=4;
    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 0;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iC, iH, iW});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, iC, oH, oW});
    auto expected = NDArrayFactory::create<double>('c', {bS, iC, iH, iW}, {0.233333,0.3     ,0.366667,0.55    ,0.65    ,0.75    ,0.95    ,1.05    ,1.15    ,0.766667,0.833333,0.9     ,
                                                     1.3     ,1.366667,1.433333,2.15    ,2.25    ,2.35    ,2.55    ,2.65    ,2.75    ,1.833333,1.9     ,1.966667,
                                                     2.366667,2.433333,2.5     ,3.75    ,3.85    ,3.95    ,4.15    ,4.25    ,4.35    ,2.9     ,2.966667,3.033333,
                                                     3.433333,3.5     ,3.566667,5.35    ,5.45    ,5.55    ,5.75    ,5.85    ,5.95    ,3.966667,4.033333,4.1     ,
                                                     4.5     ,4.566667,4.633333,6.95    ,7.05    ,7.15    ,7.35    ,7.45    ,7.55    ,5.033333,5.1     ,5.166667,
                                                     5.566667,5.633333,5.7     ,8.549999,8.65    ,8.75    ,8.95    ,9.05    ,9.150001,6.1     ,6.166667,6.233334});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::avgpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);

    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, avgpool2d_bp_test3) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,   pH=0,pW=0,   dH=1,dW=1;
    int oH=4,oW=3;
    int paddingMode = 1;             // 1-SAME,  0-VALID
    int dataFormat  = 1;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iH, iW, iC});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, oH, oW, iC});
    auto expected = NDArrayFactory::create<double>('c', {bS, iH, iW, iC}, {0.19167, 0.23333, 0.275, 0.50833, 0.59167, 0.675, 1.2  , 1.325, 1.45 ,0.50833,0.56667, 0.625, 1.19167,1.30833, 1.425, 2.4  ,2.575, 2.75 ,
                                                     1.18333, 1.24167, 1.3  , 2.54167, 2.65833, 2.775, 4.425, 4.6  , 4.775,1.01667,1.05833, 1.1  , 2.15833,2.24167, 2.325, 3.675,3.8  , 3.925,
                                                     1.69167, 1.73333, 1.775, 3.50833, 3.59167, 3.675, 5.7  , 5.825, 5.95 ,2.60833,2.66667, 2.725, 5.39167,5.50833, 5.625, 8.7  ,8.875, 9.05 ,
                                                     3.28333, 3.34167, 3.4  , 6.74167, 6.85833, 6.975,10.725,10.9  ,11.075,2.51667,2.55833, 2.6  , 5.15833,5.24167, 5.325, 8.175,8.3  , 8.425});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::avgpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 0, dataFormat});
    auto output = results->at(0);
    
    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}


//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, avgpool2d_bp_test4) {

    int bS=2, iH=4,iW=3,  iC=3,  kH=3,kW=2,  sH=1,sW=1,   pH=0,pW=0,   dH=1,dW=1;
    int oH=2,oW=2;
    int paddingMode = 0;             // 1-SAME,  0-VALID
    int dataFormat  = 1;             // 1-NDHWC, 0-NCDHW

    auto input    = NDArrayFactory::create<double>('c', {bS, iH, iW, iC});
    auto gradO    = NDArrayFactory::create<double>('c', {bS, oH, oW, iC});
    auto expected = NDArrayFactory::create<double>('c', {bS, iH, iW, iC}, {0.01667,0.03333,0.05,0.08333,0.11667,0.15,0.06667,0.08333,0.1,0.13333,0.16667,0.2 ,0.36667,0.43333,0.5 ,0.23333,0.26667,0.3,
                                                     0.13333,0.16667,0.2 ,0.36667,0.43333,0.5 ,0.23333,0.26667,0.3,0.11667,0.13333,0.15,0.28333,0.31667,0.35,0.16667,0.18333,0.2,
                                                     0.21667,0.23333,0.25,0.48333,0.51667,0.55,0.26667,0.28333,0.3,0.53333,0.56667,0.6 ,1.16667,1.23333,1.3 ,0.63333,0.66667,0.7,
                                                     0.53333,0.56667,0.6 ,1.16667,1.23333,1.3 ,0.63333,0.66667,0.7,0.31667,0.33333,0.35,0.68333,0.71667,0.75,0.36667,0.38333,0.4});
    input.linspace(1.);
    gradO.linspace(0.1, 0.1);
    
    nd4j::ops::avgpool2d_bp op;
    auto results = op.execute({&input, &gradO}, {}, {kH,kW,  sH,sW,  pH,pW,  dH,dW,  paddingMode, 1, dataFormat});
    auto output = results->at(0);
    
    ASSERT_EQ(Status::OK(), results->status());
    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));    
    
    delete results;
}

 
////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test1) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                    23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                    41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                    63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                    82., 90.,  91., 89., 92., 34.,  35., 33., 36.});
    auto expected = NDArrayFactory::create<double>(50.);

    nd4j::ops::percentile op;

    auto result = op.execute({&input}, {50.}, {});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test2) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});
    auto expected = NDArrayFactory::create<double>('c', {1,1,1}, {11.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             1}, {});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test3) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});
    auto expected = NDArrayFactory::create<double>('c', {1,1,1}, {10.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  0,             1}, {});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test4) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});
    auto expected = NDArrayFactory::create<double>('c', {1,1,1}, {11.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  1,             1}, {});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test5) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});

    auto expected = NDArrayFactory::create<double>('c', {1,1,4}, {12., 7., 11., 10.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  0,             1}, {0,1});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test6) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});

    auto expected = NDArrayFactory::create<double>('c', {1,1,4}, {16., 14., 15., 13.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  1,             1}, {0,1});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test7) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});

    auto expected = NDArrayFactory::create<double>('c', {1,1,4}, {12., 7., 11., 10.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             1}, {0,1});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test8) {

    const int dim0=5, dim1=5, dim2=4;

    auto input = NDArrayFactory::create<double>('c', {dim0, dim1, dim2}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});

    auto expected = NDArrayFactory::create<double>('c', {4}, {12., 7., 11., 10.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             0}, {0,1});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test9) {

    const int dim0=100;

    auto input = NDArrayFactory::create<double>('c', {dim0}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});

    auto expected = NDArrayFactory::create<double>(11.);

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             0}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test10) {

    const int dim0=100;

    auto input = NDArrayFactory::create<double>('c', {dim0}, {6.,   7.,  83., 81., 84., 86.,  87., 85., 88.,  5.,  8., 78.,  79., 77., 80., 10.,  16., 18.,  19., 17., 20., 22.,
                                                   23., 21., 24., 26.,  27., 25., 28., 30.,  31., 29., 32., 38., 11.,  9., 12., 14.,  15., 13., 39., 37., 40., 42.,  43.,
                                                   41., 44., 46.,  47., 45., 48., 50.,  51., 49., 52., 54.,  55., 53., 56., 58.,  59., 57., 60., 98.,  99., 97.,100., 62.,
                                                   63., 61., 64., 66.,  67., 65., 68., 70.,  71., 69., 72., 74.,  75., 73., 76., 2.,   3.,  1.,  4., 94.,  95., 93., 96.,
                                                   82., 90.,  91., 89., 92., 34.,  35., 33., 36.});

    auto expected = NDArrayFactory::create<double>('c', {1}, {11.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             1}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test11) {

    const int dim0=1;

    auto input = NDArrayFactory::create<double>('c', {dim0}, {100.});

    auto expected = NDArrayFactory::create<double>('c', {1}, {100.});

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             1}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, percentile_test12) {

    const int dim0=1;

    auto input = NDArrayFactory::create<double>('c', {dim0}, {100.});

    auto expected = NDArrayFactory::create<double>(100.);

    nd4j::ops::percentile op;
                                       //q,  interpolation, keepDims
    auto result = op.execute({&input}, {10,  2,             0}, {});
    auto output = result->at(0);

    ASSERT_TRUE(expected.isSameShape(output));
    ASSERT_TRUE(expected.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, transpose_test3) {

    auto input = NDArrayFactory::create<double>('c', {5, 3},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f});
    auto exp = NDArrayFactory::create<double>('c', {3, 5},   {1.f, 4.f, 7.f, 10.f, 13.f, 2.f, 5.f, 8.f, 11.f, 14.f, 3.f, 6.f, 9.f, 12.f, 15.f});

    nd4j::ops::transpose op;
    auto result = op.execute({&input}, {}, {});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, fill_test2) {

    auto x = NDArrayFactory::create<int>('c', {1,2},  {2, 2});
    auto v = NDArrayFactory::create<double>(42.);
    auto exp = NDArrayFactory::create<double>('c', {2, 2},{42.f, 42.f, 42.f, 42.f});
    
    nd4j::ops::fill op;
    auto result = op.execute({&x, &v}, {}, {});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    auto z = result->at(0);    

    ASSERT_TRUE(exp.isSameShape(z));
    ASSERT_TRUE(exp.equalsTo(z));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, fill_test3) {

    auto x = NDArrayFactory::create<int>('c', {2},  {2, 2});
    auto v = NDArrayFactory::create<double>(42.);
    auto exp = NDArrayFactory::create<double>('c', {2, 2}, {42.f, 42.f, 42.f, 42.f});
        
    nd4j::ops::fill op;
    auto result = op.execute({&x, &v}, {}, {});
    auto output = result->at(0);    

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, clipbynorm_test3) {
    
    auto x = NDArrayFactory::create<double>('c', {3, 5});
    auto unities = NDArrayFactory::create<double>('c', {3, 1}, {1., 1., 1.});
    auto scale = NDArrayFactory::create<double>('c', {3, 1}, {1.1, 1., 0.9});

    x.linspace(100.);

    auto xNorm1 = x.reduceAlongDims(reduce::Norm2, {1}, true);
    x /= xNorm1;
    xNorm1 = x.reduceAlongDims(reduce::Norm2,{1}, true);

    ASSERT_TRUE(unities.isSameShape(xNorm1));
    ASSERT_TRUE(unities.equalsTo(xNorm1));

    x *= scale;
    xNorm1 = x.reduceAlongDims(reduce::Norm2, {1}, true);

    nd4j::ops::clipbynorm op;
    auto result = op.execute({&x}, {1.0}, {1}, {}, false, nd4j::DataType::DOUBLE);
    auto z = result->at(0);

    auto zNorm1 = z->reduceAlongDims(reduce::Norm2, {1}, true);
    auto exp = NDArrayFactory::create<double>('c', {3, 1}, {1., 1., xNorm1.e<double>(2)});
        
    ASSERT_TRUE(exp.isSameShape(&zNorm1));
    ASSERT_TRUE(exp.equalsTo(&zNorm1));

    delete result;
}


////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, cumsum_test1) {
    
    auto inputC = NDArrayFactory::create<double>('c', {3, 5},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.});
    auto axis = NDArrayFactory::create<Nd4jLong>(1);

    auto expFF = NDArrayFactory::create<double>('c', {3, 5}, {1.,  3.,  6., 10., 15., 6., 13., 21., 30., 40., 11., 23., 36., 50., 65.});
    auto expTF = NDArrayFactory::create<double>('c', {3, 5}, {0., 1., 3.,  6., 10., 0.,  6., 13., 21., 30., 0., 11., 23., 36., 50.});

    auto expFT = NDArrayFactory::create<double>('c', {3, 5}, {15, 14, 12, 9, 5,40, 34, 27, 19, 10,65, 54, 42, 29, 15});    //+++
    auto expTT = NDArrayFactory::create<double>('c', {3, 5}, {14, 12, 9, 5, 0,34, 27, 19, 10, 0,54, 42, 29, 15, 0});

    int exclusive, reverse;    

    //************************************//
    exclusive = 0; reverse = 0;

    nd4j::ops::cumsum op;
    auto result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    auto z = result->at(0);    
    ASSERT_TRUE(expFF.equalsTo(z));
    delete result;

    //************************************//
    exclusive = 1; reverse = 0;

    result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    z = result->at(0);    
    ASSERT_TRUE(expTF.equalsTo(z));
    delete result;

    //************************************//
    exclusive = 0; reverse = 1;

    result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    z = result->at(0);    
    ASSERT_TRUE(expFT.equalsTo(z));
    delete result;

    //************************************//
    exclusive = 1; reverse = 1;

    result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    z = result->at(0);    
    ASSERT_TRUE(expTT.equalsTo(z));
    delete result;   
   
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, cumprod_test1) {
    
    auto inputC = NDArrayFactory::create<double>('c', {3, 5},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.});
    auto axis = NDArrayFactory::create<Nd4jLong>(1);

    auto expFF = NDArrayFactory::create<double>('c', {3, 5}, {1.,   2.,   6.,    24.,   120., 6.,  42., 336.,  3024., 30240.,11., 132.,1716., 24024.,360360.});
    auto expTF = NDArrayFactory::create<double>('c', {3, 5}, {1, 1, 2, 6, 24,1, 6, 42, 336, 3024,1, 11, 132, 1716, 24024});

    auto expFT = NDArrayFactory::create<double>('c', {3, 5}, {120, 120, 60, 20, 5,30240, 5040, 720, 90, 10,360360, 32760, 2730, 210, 15});    //+++
    auto expTT = NDArrayFactory::create<double>('c', {3, 5}, {120, 60, 20, 5, 1,5040, 720, 90, 10, 1,32760, 2730, 210, 15, 1});

    int exclusive, reverse;    

    //************************************//
    exclusive = 0; reverse = 0;

    nd4j::ops::cumprod op;
    auto result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
    ASSERT_TRUE(expFF.equalsTo(z));
    delete result;

    //************************************//
    exclusive = 1; reverse = 0;

    result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    z = result->at(0);    
    ASSERT_TRUE(expTF.equalsTo(z));
    delete result;

    //************************************//
    exclusive = 0; reverse = 1;

    result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    z = result->at(0);    
    ASSERT_TRUE(expFT.equalsTo(z));
    delete result;

    //************************************//
    exclusive = 1; reverse = 1;

    result = op.execute({&inputC, &axis}, {}, {exclusive, reverse}, {}, false, nd4j::DataType::DOUBLE);
    ASSERT_EQ(Status::OK(), result->status());    
    z = result->at(0);    
    ASSERT_TRUE(expTT.equalsTo(z));
    delete result;   
   
}
 
////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test1) {

    auto input = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {1, 1, 2, 2});

    auto exp = NDArrayFactory::create<double>('c', {4, 7}, {2, 1, 1, 2, 3, 3, 2, 2, 1, 1, 2, 3, 3, 2, 5, 4, 4, 5, 6, 6, 5, 5, 4, 4, 5, 6, 6, 5});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test2) {

    auto input = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {1, 1, 2, 2});

    auto exp = NDArrayFactory::create<double>('c', {4, 7}, {6, 5, 4, 5, 6, 5, 4, 3, 2, 1, 2, 3, 2, 1, 6, 5, 4, 5, 6, 5, 4, 3, 2, 1, 2, 3, 2, 1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test3) {

    auto input = NDArrayFactory::create<double>('c', {3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {1,2}, {2, 2});

    auto exp = NDArrayFactory::create<double>('c', {7}, {2, 1, 1, 2, 3, 3, 2});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);
    
    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test4) {

    auto input = NDArrayFactory::create<double>('c', {3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {2}, {2, 3});

    auto exp = NDArrayFactory::create<double>('c', {8}, {2, 1, 1, 2, 3, 3, 2, 1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test5) {

    auto input = NDArrayFactory::create<double>('c', {3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {2}, {2, 2});

    auto exp = NDArrayFactory::create<double>('c', {7}, {3, 2, 1, 2, 3, 2, 1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);
    output->printBuffer("Output");
    exp.printBuffer("Expected");
    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test6) {

    auto input = NDArrayFactory::create<double>(1.);
    auto paddings = NDArrayFactory::create<int>('c', {1,2,1,1}, {1, 1});

    auto exp = NDArrayFactory::create<double>('c', {3}, {1,1,1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test7) {

    auto input = NDArrayFactory::create<double>(1.);
    auto paddings = NDArrayFactory::create<int>('c', {2}, {1, 1});

    auto exp = NDArrayFactory::create<double>('c', {3}, {1,1,1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test8) {

    auto input = NDArrayFactory::create<double>('c', {1,3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {1, 1, 3, 3});

    auto exp = NDArrayFactory::create<double>('c', {3,9}, {3, 2, 1, 1, 2, 3, 3, 2, 1, 3, 2, 1, 1, 2, 3, 3, 2, 1, 3, 2, 1, 1, 2, 3, 3, 2, 1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test9) {

    auto input = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {2, 2, 3, 3});

    auto exp = NDArrayFactory::create<double>('c', {6, 9}, {6, 5, 4, 4, 5, 6, 6, 5, 4, 3, 2, 1, 1, 2, 3, 3, 2, 1, 3, 2, 1, 1, 2, 3, 3, 2, 1, 6, 5, 4, 4, 5, 6, 6, 5, 4, 6, 5, 4, 4, 5, 6, 6, 5, 4, 3, 2, 1, 1, 2, 3, 3, 2, 1});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test10) {

    auto input = NDArrayFactory::create<double>('c', {1,3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {0, 0, 0, 0});

    auto exp = NDArrayFactory::create<double>('c', {1,3}, {1., 2., 3.});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test11) {

    auto input = NDArrayFactory::create<double>('c', {1,3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {0, 0, 0, 0});

    auto exp = NDArrayFactory::create<double>('c', {1,3}, {1., 2., 3.});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test12) {

    auto input = NDArrayFactory::create<double>('c', {3},  {1., 2., 3.});
    auto paddings = NDArrayFactory::create<int>('c', {2,1}, {0, 0});

    auto exp = NDArrayFactory::create<double>('c', {3}, {1., 2., 3.});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test13) {

    auto input = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});
    auto paddings = NDArrayFactory::create<int>('c', {2, 2}, {0, 0, 0, 0});

    auto exp = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test14) {

    auto input = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});
    auto paddings = NDArrayFactory::create<Nd4jLong>('c', {2, 2}, {1, 0, 0, 1});

    auto exp = NDArrayFactory::create<double>('c', {3, 4},    {4, 5, 6, 5, 1, 2, 3, 2, 4, 5, 6, 5});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test15) {

    auto input = NDArrayFactory::create<double>('c', {2, 3},    {1., 2., 3., 4., 5., 6.});
    auto paddings = NDArrayFactory::create<Nd4jLong>('c', {2, 2}, {1, 1, 0, 0});

    auto exp = NDArrayFactory::create<double>('c', {4, 3},  {1, 2, 3, 1, 2, 3, 4, 5, 6, 4, 5, 6});

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {1});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, mirrorPad_test16) {

    auto input = NDArrayFactory::create<double>('c', {4,3,2});
    auto paddings = NDArrayFactory::create<int>('c', {3,2}, {3,3,2,2,1,1});

    auto exp = NDArrayFactory::create<double>('c', {10,7,4},  {24., 23., 24., 23.,22., 21., 22., 21.,20., 19., 20., 19.,22., 21., 22., 21.,24., 23., 24., 23.,22., 21., 22., 21.,20., 19., 20., 19.,18., 17., 18., 17.,16., 15., 16., 15.,14., 13., 14., 13.,16., 15., 16., 15.,18., 17., 18., 17.,16., 15., 16., 15.,14., 13., 14., 13.,
            12., 11., 12., 11.,10.,  9., 10.,  9., 8.,  7.,  8.,  7.,10.,  9., 10.,  9.,12., 11., 12., 11.,10.,  9., 10.,  9., 8.,  7.,  8.,  7., 6.,  5.,  6.,  5., 4.,  3.,  4.,  3., 2.,  1.,  2.,  1., 4.,  3.,  4.,  3., 6.,  5.,  6.,  5., 4.,  3.,  4.,  3., 2.,  1.,  2.,  1.,
            12., 11., 12., 11.,10.,  9., 10.,  9., 8.,  7.,  8.,  7.,10.,  9., 10.,  9.,12., 11., 12., 11.,10.,  9., 10.,  9., 8.,  7.,  8.,  7.,18., 17., 18., 17.,16., 15., 16., 15.,14., 13., 14., 13.,16., 15., 16., 15.,18., 17., 18., 17.,16., 15., 16., 15.,14., 13., 14., 13.,
            24., 23., 24., 23.,22., 21., 22., 21.,20., 19., 20., 19.,22., 21., 22., 21.,24., 23., 24., 23.,22., 21., 22., 21.,20., 19., 20., 19.,18., 17., 18., 17.,16., 15., 16., 15.,14., 13., 14., 13.,16., 15., 16., 15.,18., 17., 18., 17.,16., 15., 16., 15.,14., 13., 14., 13.,
            12., 11., 12., 11.,10.,  9., 10.,  9., 8.,  7.,  8.,  7.,10.,  9., 10.,  9.,12., 11., 12., 11.,10.,  9., 10.,  9., 8.,  7.,  8.,  7., 6.,  5.,  6.,  5., 4.,  3.,  4.,  3., 2.,  1.,  2.,  1., 4.,  3.,  4.,  3., 6.,  5.,  6.,  5., 4.,  3.,  4.,  3., 2.,  1.,  2.,  1.});
    input.linspace(1.);

    nd4j::ops::mirror_pad op;
    auto result = op.execute({&input, &paddings}, {}, {0});
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_1) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 5},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.});
    auto exp = NDArrayFactory::create<double>(120.f);
    //************************************//

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&input}, {}, {});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
    //z->printIndexedBuffer("Result is ");
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_2) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 5},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.});
    auto exp = NDArrayFactory::create<double>({15.f, 40.f, 65.f});
    //************************************//

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&input}, {}, {1});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_1) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 5},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.});
    auto exp = NDArrayFactory::create<double>(1307674368000.f);
    //************************************//

    nd4j::ops::reduce_prod op;
    auto result = op.execute({&input}, {}, {});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
    //z->printIndexedBuffer("Result is ");
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_2) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 5},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.});
    auto exp = NDArrayFactory::create<double>({120.f, 30240.f, 360360.f});
    //************************************//

    nd4j::ops::reduce_prod op;
    auto result = op.execute({&input}, {}, {1});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_01) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {66.f, 72.f, 78.f, 84.f});
    x.linspace(1);

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_02) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {66.f, 72.f, 78.f, 84.f});
    x.linspace(1);

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {1.}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_3) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {68.f, 100.f, 132.f});
    x.linspace(1);

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_4) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {1,3,1}, {68.f, 100.f, 132.f});
    x.linspace(1);

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {1.}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_5) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>(300.f);
    x.linspace(1);

    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_6) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>(300.f);
    x.linspace(1);
           
    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_7) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,1}, {300.f});
    x.linspace(1);
//    x.printIndexedBuffer("Input with shape (2, 3, 4) is");       
    nd4j::ops::reduce_sum op;
    auto result = op.execute({&x}, {1.}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_01) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>('c', {2}, {10395.f, 46080.f});
    x.linspace(1);

    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_02) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>('c', {1,1,2}, {10395.f, 46080.f});
    x.linspace(1);

    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {1.}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_3) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>('c', {3}, {112.f, 1080.f, 3960.f});
    x.linspace(1);

    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_4) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>('c', {1,3,1}, {112.f, 1080.f, 3960.f});
    x.linspace(1);

    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {1.}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_5) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>(479001600.f);
    x.linspace(1);
    
    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_6) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>(479001600.f);
    x.linspace(1);
           
    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_7) {

    auto x = NDArrayFactory::create<double>('c', {2,3,2});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {479001600.f});
    x.linspace(1);
//    x.printIndexedBuffer("Input with shape (2, 3, 4) is");       
    nd4j::ops::reduce_prod op;
    auto result = op.execute({&x}, {1.}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

TEST_F(DeclarableOpsTests7, Test_Matmul_Once_Again) {
    auto x = NDArrayFactory::create<double>('c', {1, 2}, {2.0f, 2.0f});
    auto y = NDArrayFactory::create<double>('c', {2, 1}, {2.0f, 2.0f});
    auto exp = NDArrayFactory::create<double>('c', {1, 1}, {8.0f});

    nd4j::ops::matmul op;
    auto result = op.execute({&x, &y}, {}, {});
    ASSERT_EQ(Status::OK(), result->status());

    ASSERT_EQ(exp, *result->at(0));

    delete result;
}

TEST_F(DeclarableOpsTests7, Test_Pnorm_Once_Again) {
    auto input = NDArrayFactory::create<double>('c', {1, 1, 5, 5}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 5, 5}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0});

    nd4j::ops::pnormpool2d op;
    auto result = op.execute({&input}, {}, {1,1,  1,1,  0,0,  1,1,1,  3,  0});
    ASSERT_EQ(Status::OK(), result->status());

    ASSERT_EQ(exp, *result->at(0));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    x.linspace(1);

    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {1.f, 2.f, 3.f, 4.f});
    x.linspace(1);

    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {1.}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {1.f, 5.f, 9.f});
    x.linspace(1);

    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,3,1}, {1.f, 5.f, 9.f});
    x.linspace(1);

    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {1.}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(1.f);
    x.linspace(1);
    
    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(1.f);
    x.linspace(1);
           
    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {1.f});
    x.linspace(1);
//    x.printIndexedBuffer("Input with shape (2, 3, 4) is");
    nd4j::ops::reduce_min op;
    auto result = op.execute({&x}, {1.}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {21.f, 22.f, 23.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
//    output->printShapeInfo("Output shape");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_2) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {21.f, 22.f, 23.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {1.}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {16.f, 20.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,3,1}, {16.f, 20.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {1.}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(24.f);
    x.linspace(1);
    
    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(24.f);
    x.linspace(1);
           
    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {24.f});
    x.linspace(1);
//    x.printIndexedBuffer("Input with shape (2, 3, 4) is");       
    nd4j::ops::reduce_max op;
    auto result = op.execute({&x}, {1.}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {66.f, 72.f, 78.f, 84.f});
    x.linspace(1);

    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_2) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {66.f, 72.f, 78.f, 84.f});
    x.linspace(1);

    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {1.}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {68.f, 100.f, 132.f});
    x.linspace(1);

    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,3,1}, {68.f, 100.f, 132.f});
    x.linspace(1);

    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {1.}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(300.f);
    x.linspace(1);
    
    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(300.f);
    x.linspace(1);
           
    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {300.f});
    x.linspace(1);
//    x.printIndexedBuffer("Input with shape (2, 3, 4) is");       
    nd4j::ops::reduce_norm1 op;
    auto result = op.execute({&x}, {1.}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {31.7175f, 33.823071f, 35.97221f, 38.15757f});
    x.linspace(1);

    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_2) {

    auto x = NDArrayFactory::create<double>('c', {2,3,4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {31.7175f, 33.823071f, 35.97221f, 38.15757f});
    x.linspace(1);

    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {1.}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {29.597298f, 39.344631f, 49.759422f});
    x.linspace(1);

    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,3,1}, {29.597298f, 39.344631f, 49.759422f});
    x.linspace(1);

    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {1.}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(70.f);
    x.linspace(1);
    
    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(70.f);
    x.linspace(1);
           
    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {70.f});
    x.linspace(1);
//    x.printIndexedBuffer("Input with shape (2, 3, 4) is");       
    nd4j::ops::reduce_norm2 op;
    auto result = op.execute({&x}, {1.}, {0,1,2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {21.f, 22.f, 23.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {21.f, 22.f, 23.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {1.f}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {16.f, 20.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {}, {0,2});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 3, 1}, {16.f, 20.f, 24.f});
    x.linspace(1);

    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {1.f}, {0,2});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(24.f);
    x.linspace(1);
    
    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(24.f);
    x.linspace(1);
    
    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {}, {0, 1, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {24.f});
    x.linspace(1);
    
    nd4j::ops::reduce_norm_max op;
    auto result = op.execute({&x}, {1.f}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {4}, {1006.f, 1144.f, 1294.f, 1456.f});
    x.linspace(1);

    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1,1,4}, {1006.f, 1144.f, 1294.f, 1456.f});
    x.linspace(1);

    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {1.f}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3}, {876.f, 1548.f, 2476.f});
    x.linspace(1);

    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {}, {0,2});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 3, 1}, {876.f, 1548.f, 2476.f});
    x.linspace(1);

    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {1.f}, {0,2});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(4900.f);
    x.linspace(1);
    
    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>(4900.f);
    x.linspace(1);
    
    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {}, {0, 1, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto exp = NDArrayFactory::create<double>('c', {1, 1, 1}, {4900.f});
    x.linspace(1);
    
    nd4j::ops::reduce_sqnorm op;
    auto result = op.execute({&x}, {1.f}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_BP_1) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.});
    auto eps = NDArrayFactory::create<double>(0.5f);
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,0.5f});
    //************************************//

    nd4j::ops::reduce_sum_bp op;
    auto result = op.execute({&input, &eps}, {}, {});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_BP_2) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.});
    auto eps = NDArrayFactory::create<double>('c', {1, 1}, {0.5f});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {0.5f, 0.5f, 0.5f, 0.5f,
                                     0.5f, 0.5f, 0.5f, 0.5f, 
                                     0.5f, 0.5f, 0.5f,0.5f});
    //************************************//

    nd4j::ops::reduce_sum_bp op;
    auto result = op.execute({&input, &eps}, {1.f}, {});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//  z->printIndexedBuffer("Result is ");
//  z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_BP_3) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {1.f, 2.f, 3.f, 4.f,
                                     1.f, 2.f, 3.f, 4.f, 
                                     1.f, 2.f, 3.f, 4.f});
    //************************************//

    nd4j::ops::reduce_sum_bp op;
    auto result = op.execute({&input, &eps}, {}, {0});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Sum_BP_4) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.});
    auto eps = NDArrayFactory::create<double>('c', {1, 4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {1.f, 2.f, 3.f, 4.f,
                                     1.f, 2.f, 3.f, 4.f, 
                                     1.f, 2.f, 3.f, 4.f});
    //************************************//

    nd4j::ops::reduce_sum_bp op;
    auto result = op.execute({&input, &eps}, {1.f}, {0});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_BP_1) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 5},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f});
    auto eps = NDArrayFactory::create<double>(1307674368000.f);
    //************************************//
//    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,0.5f});
    //************************************//
    auto exp = NDArrayFactory::create<double>('c', {3, 5},   {1710012166826558903812096.f, 855006083413279451906048.f, 570004067618451974258688.f,
                                       427503041706639725953024.f, 342002454982589992140800.f, 285002033809225987129344.f, 
                                       244287457550765131825152.f, 213751520853319862976512.f, 190001355872817324752896.f, 
                                       171001227491294996070400.f, 155455648254341989531648.f, 142501016904612993564672.f, 
                                       131539399526781282156544.f, 122143728775382565912576.f, 114000815325130245799936.f});    

    nd4j::ops::reduce_prod_bp op;
    auto result = op.execute({&input, &eps}, {}, {});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_BP_2) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f});
    auto eps = NDArrayFactory::create<double>(0.5f);
    //************************************//
//    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,0.5f});
    //************************************//
    auto exp = NDArrayFactory::create<double>('c', {3, 4});
    
    nd4j::ops::reduce_prod_bp op;
    nd4j::ops::reduce_prod op_exp;
    auto res = op_exp.execute({&input}, {}, {});
    auto result = op.execute({&input, &eps}, {}, {});
    exp.assign(res->at(0)->e<double>(0));
    exp /= input;
    exp *= eps.e<double>(0);
    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
    //z->printIndexedBuffer("Result is ");
    //exp.printIndexedBuffer("Expected");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
    delete res;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_BP_3) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f});
    auto eps = NDArrayFactory::create<double>('c', {1, 4}, {1.f, 2.f, 3.f, 4.f});
    //************************************//
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {45.f, 120.f, 231.f, 384.f, 9.f, 40.f, 99.f, 192.f, 5.f, 24.f, 63.f, 128.f});
    
    nd4j::ops::reduce_prod_bp op;
    //nd4j::ops::reduce_prod op_exp;
    auto result = op.execute({&input, &eps}, {1.f}, {0});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    exp.printIndexedBuffer("Expected");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_BP_03) {

    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f});
    auto eps = NDArrayFactory::create<double>('c', {1, 4}, {1.f, 2.f, 3.f, 4.f});
    //************************************//
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {45.f, 120.f, 231.f, 384.f, 9.f, 40.f, 99.f, 192.f, 5.f, 24.f, 63.f, 128.f});
    auto axis = NDArrayFactory::create<int>('c', {1}, 0);
    nd4j::ops::reduce_prod_bp op;
    //nd4j::ops::reduce_prod op_exp;
    auto result = op.execute({&input, &eps, &axis}, {}, {}, {true});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);
//    z->printIndexedBuffer("Result is ");
//    exp.printIndexedBuffer("Expected");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_BP_4) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    //************************************//
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {45.f, 120.f, 231.f, 384.f, 9.f, 40.f, 99.f, 192.f, 5.f, 24.f, 63.f, 128.f});
    
    nd4j::ops::reduce_prod_bp op;
    nd4j::ops::reduce_prod op_exp;
//    auto res = op_exp.execute({&input}, {}, {});
    auto result = op.execute({&input, &eps}, {0.f}, {0});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    exp.printIndexedBuffer("Expected");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
//    delete res;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Prod_BP_5) {
    
    auto input = NDArrayFactory::create<double>('c', {3, 4},   {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f});
    auto eps = NDArrayFactory::create<double>('c', {3}, {1.f, 2.f, 3.f});
    //************************************//
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {24.f, 12.f, 8.f, 6.f, 672.f, 560.f, 480.f, 420.f, 3960.f, 3564.f, 3240.f, 2970.f});
    
    nd4j::ops::reduce_prod_bp op;
    nd4j::ops::reduce_prod op_exp;
//    auto res = op_exp.execute({&input}, {}, {});
    auto result = op.execute({&input, &eps}, {0.f}, {1});

    ASSERT_EQ(Status::OK(), result->status());
    auto z = result->at(0);    
//    z->printIndexedBuffer("Result is ");
//    exp.printIndexedBuffer("Expected");
//    z->printShapeInfo();
    ASSERT_TRUE(exp.equalsTo(z));
    delete result;
//    delete res;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    exp.p(0, eps.e<double>(0));
    exp.p(1, eps.e<double>(1));
    exp.p(2, eps.e<double>(2));
    exp.p(3, eps.e<double>(3));
    x.linspace(1);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps}, {}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    exp.p(0, eps.e<double>(0));
    exp.p(1, eps.e<double>(1));
    exp.p(2, eps.e<double>(2));
    exp.p(3, eps.e<double>(3));
    x.linspace(1);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}
////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_02) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    exp.p(0, eps.e<double>(0));
    exp.p(1, eps.e<double>(1));
    exp.p(2, eps.e<double>(2));
    exp.p(3, eps.e<double>(3));
    auto axes = NDArrayFactory::create<int>({0,1});
    x.linspace(1);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps, &axes}, {}, {}, {true});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_3) {

    auto x = NDArrayFactory::create<double>('c', {3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1}, {0.5f});
    auto exp = NDArrayFactory::create<double>('c', {3, 4});
    x.linspace(1);
    x.p(2,2, -1.f);
    exp.p(2,2, 0.5f);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_4) {

    auto x = NDArrayFactory::create<double>('c', {3, 4});
    auto eps = NDArrayFactory::create<double>(0.5f);
    auto exp = NDArrayFactory::create<double>('c', {3, 4});
    x.linspace(1);
    x.p(2,2, -1.f);
    exp.p(2,2, 0.5f);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_5) {

    auto x = NDArrayFactory::create<double>('c', {4, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {4, 4});
    x.linspace(1);
    x.p(0,0, -1.f);
    x.p(1,1, -2.f);
    x.p(2,2, -3.f);
    x.p(3,3, -4.f);
    exp.p(0,0, 1.f);
    exp.p(1,1, 2.f);
    exp.p(2,2, 3.f);
    exp.p(3,3, 4.f);
//    exp(2,2) = 0.5f;
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps}, {}, {0});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Min_BP_6) {

    auto x = NDArrayFactory::create<double>('c', {4, 4});
    auto eps = NDArrayFactory::create<double>('c', {1,4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {4, 4});
    x.linspace(1);
    x.p(0,0, -1.f);
    x.p(1,1, -2.f);
    x.p(2,2, -3.f);
    x.p(3,3, -4.f);
    exp.p(0,0, 1.f);
    exp.p(1,1, 2.f);
    exp.p(2,2, 3.f);
    exp.p(3,3, 4.f);
//    exp(2,2) = 0.5f;
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_min_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {21.f, 22.f, 23.f, 24.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    exp.p(20, eps.e<double>(0));
    exp.p(21, eps.e<double>(1));
    exp.p(22, eps.e<double>(2));
    exp.p(23, eps.e<double>(3));
    x.linspace(1);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_max_bp op;
    auto result = op.execute({&x, &eps}, {}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_BP_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {21.f, 22.f, 23.f, 24.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    exp.p(20, eps.e<double>(0));
    exp.p(21, eps.e<double>(1));
    exp.p(22, eps.e<double>(2));
    exp.p(23, eps.e<double>(3));
    x.linspace(1);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_max_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0, 1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_BP_02) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {21.f, 22.f, 23.f, 24.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    exp.p(20, eps.e<double>(0));
    exp.p(21, eps.e<double>(1));
    exp.p(22, eps.e<double>(2));
    exp.p(23, eps.e<double>(3));
    auto axes = NDArrayFactory::create<int>({0, 1});
    x.linspace(1);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_max_bp op;
    auto result = op.execute({&x, &eps, &axes}, {}, {}, {true});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_BP_3) {

    auto x = NDArrayFactory::create<double>('c', {4, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {4, 4});
    x.linspace(1);
    x.p(0,0, 21.f);
    x.p(1,1, 22.f);
    x.p(2,2, 23.f);
    x.p(3,3, 24.f);
    exp.p(0,0, 1.f);
    exp.p(1,1, 2.f);
    exp.p(2,2, 3.f);
    exp.p(3,3, 4.f);
//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_max_bp op;
    auto result = op.execute({&x, &eps}, {}, {0});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Max_BP_4) {

    auto x = NDArrayFactory::create<double>('c', {4, 4});
    auto eps = NDArrayFactory::create<double>('c', {1,4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {4, 4});
    x.linspace(1);
    x.p(0,0, 21.f);
    x.p(1,1, 22.f);
    x.p(2,2, 23.f);
    x.p(3,3, 24.f);
    exp.p(0,0, 1.f);
    exp.p(1,1, 2.f);
    exp.p(2,2, 3.f);
    exp.p(3,3, 4.f);

//    x.printIndexedBuffer("Input is");
//    exp.printIndexedBuffer("Expected ");
    nd4j::ops::reduce_max_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>(5.f);
    x.linspace(1);
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.p(12, -2.f);
    x.p(20, -3.f);
    exp.assign(5.f);
    exp.p(12, -exp.e<double>(12));
    exp.p(20, -exp.e<double>(20));
    nd4j::ops::reduce_norm1_bp op;
    auto result = op.execute({&x, &eps}, {}, {});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_BP_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>({1.f, 2.f, 3.f, 4.f});
    x.linspace(1);
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4}, {1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f,1.f, 2.f, 3.f, 4.f,1.f, 2.f, 3.f, 4.f});
    nd4j::ops::reduce_norm1_bp op;
    auto result = op.execute({&x, &eps}, {}, {0,1});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());
    auto output = result->at(0);
    output->printIndexedBuffer("Result is");
    exp.printIndexedBuffer("Expect is");
    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_BP_02) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>({1.f, 2.f, 3.f, 4.f});
    x.linspace(1);
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4}, {1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f,1.f, 2.f, 3.f, 4.f,1.f, 2.f, 3.f, 4.f});
    auto axes = NDArrayFactory::create<int>({0,1});
    nd4j::ops::reduce_norm1_bp op;
    auto result = op.execute({&x, &eps, &axes}, {}, {}, {false});
    ASSERT_EQ(ND4J_STATUS_OK, result->status());
    auto output = result->at(0);
    output->printIndexedBuffer("Result is");
    exp.printIndexedBuffer("Expect is");
    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm1_BP_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {1.f, 2.f, 3.f, 4.f});
    x.linspace(1);
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4}, {1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f,1.f, 2.f, 3.f, 4.f,1.f, 2.f, 3.f, 4.f});
    nd4j::ops::reduce_norm1_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {31.7175f, 33.823071f, 35.97221f, 38.15757f});
    x.linspace(1);

    nd4j::ops::reduce_norm2_bp op;
    auto result = op.execute({&x, &eps}, {}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(x.isSameShape(output));
    ASSERT_TRUE(x.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_BP_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {31.7175f, 33.823071f, 35.97221f, 38.15757f});
    x.linspace(1);

    nd4j::ops::reduce_norm2_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(x.isSameShape(output));
    ASSERT_TRUE(x.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_BP_02) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 4}, {31.7175f, 33.823071f, 35.97221f, 38.15757f});
    auto axes = NDArrayFactory::create<int>({0, 1});
    x.linspace(1);

    nd4j::ops::reduce_norm2_bp op;
    auto result = op.execute({&x, &eps, &axes}, {}, {}, {true});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(x.isSameShape(output));
    ASSERT_TRUE(x.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_BP_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {3}, {29.597298f, 39.344631f, 49.759422f});
    x.linspace(1);

    nd4j::ops::reduce_norm2_bp op;
    auto result = op.execute({&x, &eps}, {}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(x.isSameShape(output));
    ASSERT_TRUE(x.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Norm2_BP_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1,3,1}, {29.597298f, 39.344631f, 49.759422f});
    x.linspace(1);

    nd4j::ops::reduce_norm2_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(x.isSameShape(output));
    ASSERT_TRUE(x.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4}, { 2.f,  8.f, 18.f, 32.f,
                                        10.f, 24.f, 42.f, 64.f, 
                                        18.f, 40.f, 66.f, 96.f, 
                                            26.f, 56.f, 90.f, 128.f, 
                                            34.f, 72.f, 114.f, 160.f, 
                                            42.f, 88.f, 138.f, 192.f});
    x.linspace(1);

    nd4j::ops::reduce_sqnorm_bp op;
    auto result = op.execute({&x, &eps}, {}, {0,1});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_SquaredNorm_BP_01) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4}, { 2.f,  8.f, 18.f, 32.f,
                                                                10.f, 24.f, 42.f, 64.f,
                                                                18.f, 40.f, 66.f, 96.f,
                                                                26.f, 56.f, 90.f, 128.f,
                                                                34.f, 72.f, 114.f, 160.f,
                                                                42.f, 88.f, 138.f, 192.f});
    auto axes = NDArrayFactory::create<int>({0, 1});
    x.linspace(1);

    nd4j::ops::reduce_sqnorm_bp op;
    auto result = op.execute({&x, &eps, &axes}, {}, {}, {false});

    ASSERT_EQ(ND4J_STATUS_OK, result->status());
    auto output = result->at(0);

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    exp.p(20, 1.f);
    exp.p(21, 2.f);
    exp.p(22, 3.f);
    exp.p(23, 4.f);

    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1,1,4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    exp.p(20, 1.f);
    exp.p(21, 2.f);
    exp.p(22, 3.f);
    exp.p(23, 4.f);

    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0,1});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_02) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1,1,4}, {1.f, 2.f, 3.f, 4.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto axes = NDArrayFactory::create<int>({0,1});
    x.linspace(1);
    exp.p(20, 1.f);
    exp.p(21, 2.f);
    exp.p(22, 3.f);
    exp.p(23, 4.f);

    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps, &axes}, {}, {}, {true});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}
////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_3) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {3}, {1.f, 2.f, 3.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);

    exp.p(15, 1.f);
    exp.p(19, 2.f);
    exp.p(23, 3.f);

    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {}, {0,2});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_4) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 3, 1}, {1.f, 2.f, 3.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    exp.p(15, 1.f);
    exp.p(19, 2.f);
    exp.p(23, 3.f);
    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {0,2});
    auto output = result->at(0);
//    output->printIndexedBuffer("Result is");
    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_5) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>(1.f);
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    exp.p(23, 1.f);
    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_6) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>(1.f);
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    exp.p(23, 1.f);
    
    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {}, {0, 1, 2});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_NormMax_BP_7) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto eps = NDArrayFactory::create<double>('c', {1, 1, 1}, {1.f});
    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    exp.p(23, 1.f);
    nd4j::ops::reduce_norm_max_bp op;
    auto result = op.execute({&x, &eps}, {1.f}, {});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Dot_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto y = NDArrayFactory::create<double>('c', {2, 3, 4});
    NDArray* z; // = NDArrayFactory::create<double>('c', {4});
    auto eps = NDArrayFactory::create<double>(1.f);
//    auto exp = NDArrayFactory::create<double>('c', {2, 3, 4});
    x.linspace(1);
    y.linspace(2);


    nd4j::ops::reduce_dot_bp op;
    auto result = op.execute({&x, &y, &eps}, {}, {});
    auto output = result->at(0);
    auto outputX = result->at(1);
    //tput->printIndexedBuffer("Result is");

//    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(x.equalsTo(outputX));
    ASSERT_TRUE(y.equalsTo(output));

    delete result;
//    delete z;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Dot_BP_2) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto y = NDArrayFactory::create<double>('c', {2, 3, 4});
//    auto z; // = NDArrayFactory::create<double>('c', {4});
    auto eps = NDArrayFactory::create<double>('c', {2, 4});
    auto expX = NDArrayFactory::create<double>('c', {2, 3, 4}, {2.f, 4.f, 6.f, 8.f, 2.f, 4.f, 6.f, 8.f, 2.f, 4.f, 6.f, 8.f,
                                        10.f, 12.f, 14.f, 16.f, 10.f, 12.f, 14.f, 16.f, 10.f, 12.f, 14.f, 16.f
                                        });
    auto expY = NDArrayFactory::create<double>('c', {2, 3, 4}, {1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f,
                                         5.f, 6.f, 7.f, 8.f, 5.f, 6.f, 7.f, 8.f, 5.f, 6.f, 7.f, 8.f
    });
    x.assign(1.f);
    eps.linspace(1);
    y.assign(2.f);
    nd4j::ops::reduce_dot_bp op;
    auto result = op.execute({&x, &y, &eps}, {}, {1});
    ASSERT_EQ(result->status(), ND4J_STATUS_OK);
    ASSERT_EQ(result->size(), 2);
    auto outputX = result->at(0);
    auto outputY = result->at(1);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(expX.equalsTo(outputX));
    ASSERT_TRUE(expY.equalsTo(outputY));

    delete result;
//    delete z;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Dot_BP_02) {

    auto x = NDArrayFactory::create<double>('c', {2, 3, 4});
    auto y = NDArrayFactory::create<double>('c', {2, 3, 4});
//    auto z; // = NDArrayFactory::create<double>('c', {4});
    auto eps = NDArrayFactory::create<double>('c', {2, 4});
    auto expX = NDArrayFactory::create<double>('c', {2, 3, 4}, {2.f, 4.f, 6.f, 8.f, 2.f, 4.f, 6.f, 8.f, 2.f, 4.f, 6.f, 8.f,
                                                                10.f, 12.f, 14.f, 16.f, 10.f, 12.f, 14.f, 16.f, 10.f, 12.f, 14.f, 16.f
    });
    auto expY = NDArrayFactory::create<double>('c', {2, 3, 4}, {1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f, 1.f, 2.f, 3.f, 4.f,
                                                                5.f, 6.f, 7.f, 8.f, 5.f, 6.f, 7.f, 8.f, 5.f, 6.f, 7.f, 8.f
    });
    auto axis = NDArrayFactory::create<int>('c', {1}, {1});
    x.assign(1.f);
    eps.linspace(1);
    y.assign(2.f);
    nd4j::ops::reduce_dot_bp op;
    auto result = op.execute({&x, &y, &eps, &axis}, {}, {}, {false});
    ASSERT_EQ(result->status(), ND4J_STATUS_OK);
    ASSERT_EQ(result->size(), 2);
    auto outputX = result->at(0);
    auto outputY = result->at(1);
    ASSERT_EQ(ND4J_STATUS_OK, result->status());

    ASSERT_TRUE(expX.equalsTo(outputX));
    ASSERT_TRUE(expY.equalsTo(outputY));

    delete result;
//    delete z;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_Dot_BP_3) {

    auto x = NDArrayFactory::create<double>('c', {3, 4});
    auto y = NDArrayFactory::create<double>('c', {3, 4});
    auto eps = NDArrayFactory::create<double>('c', {3});
    auto expX = NDArrayFactory::create<double>('c', {3, 4}, {2.f, 2.f, 2.f, 2.f, 4.f, 4.f, 4.f, 4.f, 6.f, 6.f, 6.f, 6.f});
    auto expY = NDArrayFactory::create<double>('c', {3, 4}, {1.f, 2.f, 3.f, 4.f, 10.f, 12.f, 14.f, 16.f, 27.f, 30.f, 33.f, 36.f});
    x.linspace(1);
    eps.linspace(1);
    y.assign(2.f);

    nd4j::ops::reduce_dot_bp op;
    auto result = op.execute({&x,&y, &eps}, {}, {1});
    auto outputX = result->at(0);
    auto outputY = result->at(1);

    ASSERT_EQ(ND4J_STATUS_OK, result->status());
    ASSERT_TRUE(expX.equalsTo(outputX));
    ASSERT_TRUE(expY.equalsTo(outputY));

    delete result;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_CumSum_BP_1) {

    auto x = NDArrayFactory::create<double>('c', {3, 4});
//    auto y = NDArrayFactory::create<double>('c', {3, 4});
//    auto z; // = NDArrayFactory::create<double>('c', {4});
    auto eps = NDArrayFactory::create<double>('c', {3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, {12.f, 11.f, 10.f, 9.f, 8.f, 7.f,
                                      6.f,  5.f,  4.f, 3.f, 2.f, 1.f});
    x.linspace(1);
    eps.assign(1.f);

//    z = x.applyReduce3<simdOps::Dot<float>>(&y, {0}, nullptr);
    nd4j::ops::cumsum_bp op;
    auto result = op.execute({&x, &eps}, {}, {0,0});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
//    output->printShapeInfo("Result shape is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;
//    delete z;
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_CumSum_BP_2) {
    auto x = NDArrayFactory::create<double>('c', {3, 4});
//    auto y = NDArrayFactory::create<double>('c', {3, 4});
//    auto z; // = NDArrayFactory::create<double>('c', {4});
    auto eps = NDArrayFactory::create<double>('c', {3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3, 4}, { 11.f, 10.f, 9.f, 8.f, 7.f, 6.f,
                                      5.f,  4.f, 3.f, 2.f, 1.f, 0.f});
    x.linspace(1);
//    exp.linspace(1);
    eps.assign(1.f);

//    z = x.applyReduce3<simdOps::Dot<float>>(&y, {0}, nullptr);
    nd4j::ops::cumsum_bp op;
    auto result = op.execute({&x, &eps}, {}, {1,0});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
//    output->printShapeInfo("Result shape is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;

}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests7, Test_Reduce_CumSum_BP_3) {
    auto x = NDArrayFactory::create<double>('c', {3, 4});
//    auto y = NDArrayFactory::create<double>('c', {3, 4});
//    auto z; // = NDArrayFactory::create<double>('c', {4});
    auto eps = NDArrayFactory::create<double>('c', {3, 4});
    auto exp = NDArrayFactory::create<double>('c', {3, 4});
                                    
    x.linspace(1);
    exp.linspace(0);
    eps.assign(1.f);

//    z = x.applyReduce3<simdOps::Dot<float>>(&y, {0}, nullptr);
    nd4j::ops::cumsum_bp op;
    auto result = op.execute({&x, &eps}, {}, {1,1});
    auto output = result->at(0);    
//    output->printIndexedBuffer("Result is");
//    output->printShapeInfo("Result shape is");

    ASSERT_EQ(ND4J_STATUS_OK, result->status());    

//    ASSERT_TRUE(exp.isSameShape(output));
    ASSERT_TRUE(exp.equalsTo(output));

    delete result;

}

