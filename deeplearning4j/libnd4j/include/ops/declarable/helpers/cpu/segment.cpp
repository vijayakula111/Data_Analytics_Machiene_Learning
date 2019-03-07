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

#include <ops/declarable/helpers/segment.h>

namespace nd4j {
namespace ops {
namespace helpers {

    // segment max
    template <typename T>
    static void segmentMaxFunctor_(NDArray* input, NDArray* indices, NDArray* output) {
        //int numClasses = output->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        Nd4jLong idx = indices->e<Nd4jLong>(0);
        if (input->isVector()) {
            T val = input->e<T>(0);
//#pragma omp parallel for default(shared) schedule(guided)
            for (Nd4jLong e = 1; e < indices->lengthOf(); e++) {
                if (idx == indices->e<Nd4jLong>(e)) {
                   // max
                   val = nd4j::math::nd4j_max<T>(val, input->t<T>(e));
                }
                else {
                    idx = indices->e<Nd4jLong>(e);
                    val = input->t<T>(e);
                }
                output->t<T>(idx) = val;
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            auto listOfTensors = input->allTensorsAlongDimension(restDims);
            auto listOfOutTensors = output->allTensorsAlongDimension(restDims);

            auto numOfClasses = output->sizeAt(0); // number of classes
            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
            auto maxT = listOfOutTensors->at(idx);

            //int pos = 0;
            maxT->assign(listOfTensors->at(0));

//#pragma omp parallel for schedule(static)
            for (Nd4jLong i = 1; i < indices->lengthOf(); i++) {
                if (indices->e<int>(i) == idx) {
//#pragma omp parallel for schedule(static)
                    for (Nd4jLong e = 0; e < maxT->lengthOf(); e++) {
                       maxT->t<T>(e) = nd4j::math::nd4j_max(maxT->t<T>(e), listOfTensors->at(i)->t<T>(e));
                    }
                }
                else {
                    idx = indices->e<Nd4jLong>(i);
                    maxT = listOfOutTensors->at(idx);
                    maxT->assign(listOfTensors->at(i));
                }

            }
            delete listOfTensors;
            delete listOfOutTensors;
        }
    }

    // segmen min 
    template <typename T>
    static void segmentMinFunctor_(NDArray* input, NDArray* indices, NDArray* output) {
        //int numClasses = output->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        Nd4jLong idx = indices->e<Nd4jLong>(0);
        if (input->isVector()) {
            T val = input->e<T>(0);
//#pragma omp parallel for default(shared) schedule(guided)
            for (int e = 1; e < indices->lengthOf(); e++) {
                if (idx == indices->e<Nd4jLong>(e)) {
                   // min 
                   val = nd4j::math::nd4j_min<T>(val, input->t<T>(e));
                }
                else {
                    idx = indices->e<int>(e);
                    val = input->t<T>(e);
                }
                output->t<T>(idx) = val;
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors( input->allTensorsAlongDimension(restDims) );
            std::unique_ptr<ResultSet> listOfOutTensors( output->allTensorsAlongDimension(restDims) );

            int numOfClasses = output->sizeAt(0); // number of classes
            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
            auto minT = listOfOutTensors->at(idx);

            int pos = 0;
            minT->assign(listOfTensors->at(0));
//#pragma omp parallel for schedule(dynamic)
            for (Nd4jLong i = 1; i < indices->lengthOf(); i++) {
                if (indices->e<T>(i) == idx) {
//#pragma omp parallel for schedule(static)
                    for (int e = 0; e < minT->lengthOf(); e++) {
                       minT->p(e, nd4j::math::nd4j_min(minT->e<T>(e), listOfTensors->at(i)->e<T>(e)));
                    }
                }
                else {
                    idx = indices->e<T>(i);
                    minT = listOfOutTensors->at(idx);
                    minT->assign(listOfTensors->at(i));
                }
            }
        }
    }

    // segmen mean
    template <typename T>
    static void segmentMeanFunctor_(NDArray* input, NDArray* indices, NDArray* output) {
        int numClasses = output->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        int idx = indices->e<int>(0);
        if (input->isVector()) {
            T val = T(0.f);
            int count = 0;
//#pragma omp parallel for schedule(static)
            for (int e = 0; e < indices->lengthOf(); e++) {
                if (idx == indices->e<int>(e)) {
                   // mean 
                   val += input->e<T>(e);
                   count++;
                }
                else {
                   output->p<T>(idx, val / count);
                    idx = indices->e<int>(e);
                    val = input->e<T>(e);
                    count = 1;
                }
                output->p<T>(idx, val / count);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            auto listOfTensors = input->allTensorsAlongDimension(restDims);
            auto listOfOutTensors = output->allTensorsAlongDimension(restDims);

            int numOfClasses = output->sizeAt(0); // number of classes
            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
            auto meanT = listOfOutTensors->at(idx);
            int count = 1;
            auto meanV = meanT->dup();
            meanV->assign(listOfTensors->at(0));
//#pragma omp parallel for schedule(static)
            for (int i = 1; i < indices->lengthOf(); i++) {
                if (indices->e<int>(i) == idx) {
#pragma omp parallel for schedule(static)
                    for (int e = 0; e < meanT->lengthOf(); e++) {
                       meanV->p<T>(e, meanV->e<T>(e) + listOfTensors->at(i)->e<T>(e));
                    }
                    count++;
                }
                else {
                    //meanT->assign(meanV);
                    meanV->applyScalar(scalar::Divide, count, meanT, nullptr);
                    idx = indices->e<int>(i);
                    meanT = listOfOutTensors->at(idx);
                    meanV->assign(listOfTensors->at(i));
                    count = 1;
                }
                meanV->applyScalar(scalar::Divide, count, meanT, nullptr);
            }
            delete meanV;
            delete listOfTensors;
            delete listOfOutTensors;
        }
    }

    template <typename T>
    static void segmentSumFunctor_(NDArray* input, NDArray* indices, NDArray* output) {
        int numClasses = output->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        int idx = indices->e<int>(0);
        if (input->isVector()) {
            T val = T(0.f);
            int count = 0;
            for (int e = 0; e < indices->lengthOf(); e++) {
                if (idx == indices->e<int>(e)) {
                   // sum 
                   val += input->t<T>(e);
                }
                else {
                    idx = indices->e<int>(e);
                    val = input->t<T>(e);
                }
                output->p(idx, val);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            auto listOfTensors = input->allTensorsAlongDimension(restDims);
            auto listOfOutTensors = output->allTensorsAlongDimension(restDims);

            int numOfClasses = output->sizeAt(0); // number of classes
            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
            auto sumT = listOfOutTensors->at(idx);

            for (int i = 0; i < indices->lengthOf(); i++) {
                if (indices->e<int>(i) == idx) {
#pragma omp parallel for schedule(static)
                    for (int e = 0; e < sumT->lengthOf(); e++) {
                       sumT->p(e, sumT->e<T>(e) + listOfTensors->at(i)->e<T>(e));
                    }
                }
                else {
                    idx = indices->e<int>(i);
                    sumT = listOfOutTensors->at(idx);
                    sumT->assign(listOfTensors->at(i));
                }
            }
            delete listOfTensors;
            delete listOfOutTensors;
        }
    }

    template <typename T>
    static void segmentProdFunctor_(NDArray* input, NDArray* indices, NDArray* output) {
        //int numClasses = output->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        int idx = indices->e<int>(0);
        output->assign(1.f);
        if (input->isVector()) {
            T val = input->e<T>(0);
            int count = 0;
//#pragma omp parallel for //reduction(* : val) schedule(static)
            for (int e = 1; e < indices->lengthOf(); e++) {
                if (idx == indices->e<int>(e)) {
                   // sum 
                   val *= input->e<T>(e);
                }
                else {
                    idx = indices->e<int>(e);
                    val = input->e<T>(e);
                }
                output->p(idx, val);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            auto listOfTensors = input->allTensorsAlongDimension(restDims);
            auto listOfOutTensors = output->allTensorsAlongDimension(restDims);

            int numOfClasses = output->sizeAt(0); // number of classes
            auto sumT = listOfOutTensors->at(idx);
            sumT->assign(listOfTensors->at(0));
            for (int i = 1; i < indices->lengthOf(); i++) {
                if (indices->e<int>(i)  == idx) {
#pragma omp parallel for schedule(static)
                    for (int e = 0; e < sumT->lengthOf(); e++) {
                       sumT->p(e, sumT->e<T>(e) * listOfTensors->at(i)->e<T>(e));
                    }
                }
                else {
                    idx = indices->e<int>(i);
                    sumT = listOfOutTensors->at(idx);
                    sumT->assign(listOfTensors->at(i));
                }
            }
            delete listOfTensors;
            delete listOfOutTensors;
        }
    }

//    template <typename T>
//    static bool segmentIndicesValidate_(NDArray* indices, NDArray& aexpected, NDArray& anOutput) {
//      }

    void segmentMaxFunctor(NDArray* input, NDArray* indices, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), segmentMaxFunctor_, (input, indices, output), LIBND4J_TYPES);
    }

    void segmentMinFunctor(NDArray* input, NDArray* indices, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), segmentMinFunctor_, (input, indices, output), LIBND4J_TYPES);
    }

    void segmentMeanFunctor(NDArray* input, NDArray* indices, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), segmentMeanFunctor_, (input, indices, output), LIBND4J_TYPES);
    }

    void segmentSumFunctor(NDArray* input, NDArray* indices, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), segmentSumFunctor_, (input, indices, output), LIBND4J_TYPES);
    }

    void segmentProdFunctor(NDArray* input, NDArray* indices, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), segmentProdFunctor_, (input, indices, output), LIBND4J_TYPES);
    }

    bool segmentIndicesValidate(NDArray* indices, NDArray& expected, NDArray& output) {
        auto val = indices->e(0);
        for (int e = 1; e < indices->lengthOf(); e++) {
            output = indices->e(e);
            if (val.e<Nd4jLong>(0) > output.e<Nd4jLong>(0))
                return false;
            val = indices->e(e);
        }

        return true;
    }

    //BUILD_SINGLE_TEMPLATE(template bool segmentIndicesValidate_, (NDArray*, NDArray&, NDArray&), LIBND4J_TYPES);
    BUILD_SINGLE_TEMPLATE(template void segmentProdFunctor_, (NDArray* input, NDArray* indices, NDArray* output), LIBND4J_TYPES);
    BUILD_SINGLE_TEMPLATE(template void segmentSumFunctor_, (NDArray* input, NDArray* indices, NDArray* output), LIBND4J_TYPES);
    BUILD_SINGLE_TEMPLATE(template void segmentMeanFunctor_, (NDArray* input, NDArray* indices, NDArray* output), LIBND4J_TYPES);
    BUILD_SINGLE_TEMPLATE(template void segmentMinFunctor_, (NDArray* input, NDArray* indices, NDArray* output), LIBND4J_TYPES);
    BUILD_SINGLE_TEMPLATE(template void segmentMaxFunctor_, (NDArray* input, NDArray* indices, NDArray* output), LIBND4J_TYPES);
    // -------------------------------------------------------------------------------------------------------------- //
    // Unsorted segment ops
    // -------------------------------------------------------------------------------------------------------------- //

    bool unsortedSegmentIndicesValidate(NDArray* indices, Nd4jLong expected, Nd4jLong& output) {
        Nd4jLong val = indices->e<Nd4jLong>(0);

        Nd4jLong maxInd = indices->argMax();
        if (indices->e<Nd4jLong>(maxInd) >= expected) {
                output = val;
                return false;
        }
        output = expected;
        return true;
    }

    template <typename T>
    static void unsortedSegmentMaxFunctor_(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {

        // if input is a vector: (as if in doc sample)
        //int idx = static_cast<int>((*indices)(0.));
        std::map<Nd4jLong, std::vector<Nd4jLong>> idxs;//(indices->lengthOf());
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e)
            idxs[indices->e<Nd4jLong>(e)].push_back(e);

        //std::sort(idxs.begin(), idxs.end());

        if (input->isVector()) { // 1D case
            T maxVal = DataTypeUtils::max<T>();
            output->assign(-maxVal);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                T val = input->e<T>(fi->second.at(0));
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    val = nd4j::math::nd4j_max(val, input->e<T>(fi->second.at(idx)));
                }
                output->p(fi->first, val);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
            Nd4jLong idx = idxs[0][0];
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

//            int numOfClasses = output->sizeAt(0); // number of classes
//            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
//            NDArray* maxT = listOfOutTensors->at(idx);
            T maxVal = DataTypeUtils::max<T>();
            output->assign(-maxVal);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                auto outputT = listOfOutTensors->at(fi->first);
                outputT->assign(listOfTensors->at(fi->second.at(0)));
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    auto maxT = listOfTensors->at(fi->second.at(idx));
                    for (Nd4jLong e = 0; e < outputT->lengthOf(); ++e) {
                        T val = nd4j::math::nd4j_max(maxT->e<T>(e), outputT->e<T>(e));

                        outputT->p(e, val);
                    }
                }
                //outputT->assign(maxT);
            }
        }
    }
    void unsortedSegmentMaxFunctor(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), unsortedSegmentMaxFunctor_, (input, indices, numOfClasses, output), NUMERIC_TYPES);
    }
    BUILD_SINGLE_TEMPLATE(template void unsortedSegmentMaxFunctor_, (NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output), NUMERIC_TYPES);

    template <typename T>
    static void unsortedSegmentMinFunctor_(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        // if input is a vector: (as if in doc sample)
        //int idx = static_cast<int>((*indices)(0.));
        std::map<Nd4jLong, std::vector<Nd4jLong>> idxs;//(indices->lengthOf());
//#pragma omp parallel for schedule(static)
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e)
            idxs[indices->e<Nd4jLong>(e)].push_back(e);

        //std::sort(idxs.begin(), idxs.end());

        if (input->isVector()) { // 1D case
            T maxVal = DataTypeUtils::max<T>();
            output->assign(maxVal);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                T val = input->t<T>(fi->second.at(0));
//#pragma omp parallel for schedule(static)
                for (size_t idx = 1; idx < fi->second.size(); ++idx) {
                    val = nd4j::math::nd4j_min(val, input->t<T>(fi->second.at(idx)));
                }
                output->t<T>(fi->first) = val;
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
            Nd4jLong idx = idxs[0][0];
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

//            int numOfClasses = output->sizeAt(0); // number of classes
//            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
//            NDArray* maxT = listOfOutTensors->at(idx);
            T maxVal = DataTypeUtils::max<T>();
            output->assign(maxVal);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                auto outputT = listOfOutTensors->at(fi->first);
                outputT->assign(listOfTensors->at(fi->second.at(0)));
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    auto minT = listOfTensors->at(fi->second.at(idx));
//#pragma omp parallel for schedule(guided)
                    for (Nd4jLong e = 0; e < outputT->lengthOf(); ++e) {
                        outputT->t<T>(e) = nd4j::math::nd4j_min(minT->t<T>(e), outputT->t<T>(e));
                    }
                }
                //outputT->assign(maxT);
            }
        }

    }
    void unsortedSegmentMinFunctor(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), unsortedSegmentMinFunctor_, (input, indices, numOfClasses, output),
                              NUMERIC_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void unsortedSegmentMinFunctor_, (NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output), NUMERIC_TYPES);

    void unsortedSegmentMeanFunctor(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        std::map<Nd4jLong, std::vector<Nd4jLong>> idxs;//(indices->lengthOf());
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e)
            idxs[indices->e<Nd4jLong>(e)].push_back(e);

        //std::sort(idxs.begin(), idxs.end());

        if (input->isVector()) { // 1D case
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                double sumValue = input->e<double>(fi->second.at(0));
#pragma omp parallel for reduction(+:sumValue) schedule(static)
                for (size_t idx = 1; idx < fi->second.size(); ++idx) {
                    sumValue += input->e<double>(fi->second.at(idx));
                }
                output->p(fi->first, sumValue / fi->second.size());
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

//            int numOfClasses = output->sizeAt(0); // number of classes
//            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
//            NDArray* maxT = listOfOutTensors->at(idx);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                auto outputT = listOfOutTensors->at(fi->first);
                outputT->assign(listOfTensors->at(fi->second.at(0)));
#pragma omp parallel for schedule(static)
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    auto current = listOfTensors->at(fi->second.at(idx));
                    *outputT += *current;
                }
                //outputT->assign(maxT);
                (*outputT) /= double(fi->second.size());
            }
        }
    }

    void unsortedSegmentSumFunctor(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        std::map<Nd4jLong, std::vector<Nd4jLong>> idxs;//(indices->lengthOf());
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e)
            idxs[indices->e<Nd4jLong>(e)].push_back(e);

        //std::sort(idxs.begin(), idxs.end());

        if (input->isVector()) { // 1D case
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                double sumValue = input->e<double>(fi->second.at(0));
#pragma omp parallel for reduction(+:sumValue) schedule(static)
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    sumValue += input->e<double>(fi->second.at(idx));
                }
                output->p(fi->first, sumValue);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

//            int numOfClasses = output->sizeAt(0); // number of classes
//            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
//            NDArray* maxT = listOfOutTensors->at(idx);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                auto outputT = listOfOutTensors->at(fi->first);
                outputT->assign(listOfTensors->at(fi->second.at(0)));
#pragma omp parallel for schedule(static)
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    auto current = listOfTensors->at(fi->second.at(idx));
                    *(outputT) += *current;
                }
                //outputT->assign(maxT);
            }
        }
    }

    template <typename T>
    void unsortedSegmentProdFunctor_(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        std::map<Nd4jLong, std::vector<Nd4jLong>> idxs;//(indices->lengthOf());
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e)
            idxs[indices->e<Nd4jLong>(e)].push_back(e);

        //std::sort(idxs.begin(), idxs.end());

        output->assign(1.f);

        if (input->isVector()) { // 1D case
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                T prodValue = input->e<T>(fi->second.at(0));
                for (size_t idx = 1; idx < fi->second.size(); ++idx) {
                    prodValue *= input->e<T>(fi->second.at(idx));
                }
                output->p(fi->first, prodValue);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

//            int numOfClasses = output->sizeAt(0); // number of classes
//            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
//            NDArray* maxT = listOfOutTensors->at(idx);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                auto outputT = listOfOutTensors->at(fi->first);
                outputT->assign(listOfTensors->at(fi->second.at(0)));
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    auto current = listOfTensors->at(fi->second.at(idx));

                    *outputT *= *current;
                }
            }
        }
    }

    void unsortedSegmentProdFunctor(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        BUILD_SINGLE_SELECTOR(input->dataType(), unsortedSegmentProdFunctor_, (input, indices, numOfClasses, output), NUMERIC_TYPES);
    }
    BUILD_SINGLE_TEMPLATE(template void unsortedSegmentProdFunctor_, (NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output), NUMERIC_TYPES);

    void unsortedSegmentSqrtNFunctor(NDArray* input, NDArray* indices, Nd4jLong numOfClasses, NDArray* output) {
        std::map<Nd4jLong, std::vector<Nd4jLong>> idxs;//(indices->lengthOf());
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e)
            idxs[indices->e<Nd4jLong>(e)].push_back(e);

        //std::sort(idxs.begin(), idxs.end());

        if (input->isVector()) { // 1D case
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                double sumValue = input->e<double>(fi->second.at(0));
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    sumValue += input->e<double>(fi->second.at(idx));
                }
                output->p(fi->first, sumValue / nd4j::math::nd4j_sqrt<Nd4jLong, double>(fi->second.size()));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

//            int numOfClasses = output->sizeAt(0); // number of classes
//            std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
//            NDArray* maxT = listOfOutTensors->at(idx);
//#pragma omp parallel for schedule(static)
            for (auto fi = idxs.begin(); fi != idxs.end(); ++fi) {
                auto outputT = listOfOutTensors->at(fi->first);
                outputT->assign(listOfTensors->at(fi->second.at(0)));
                for (Nd4jLong idx = 1; idx < fi->second.size(); ++idx) {
                    auto current = listOfTensors->at(fi->second.at(idx));
                    *outputT += *current;
                }
                //outputT->assign(maxT);
                (*outputT) /= nd4j::math::nd4j_sqrt<size_t, double>(fi->second.size());
            }
        }
    }

    // -------------------------------------------------------------------------------------------------------------- //
    // Backpropagate ops helpers
    // -------------------------------------------------------------------------------------------------------------- //
    // Sorted backpropagate ops
    //

    // segment max
    template <typename T>
    int segmentMaxFunctorBP_(NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output) {
        //int numOfClasses = gradOut->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        auto tempRes = gradOut->dup();
        segmentMaxFunctor_<T>(input, indices, tempRes);
        if (input->isVector()) {
#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < input->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                if (nd4j::math::nd4j_abs(tempRes->e<T>(classNum) -input->e<T>(e) <= T(1.e-6)))
                    output->p(e, gradOut->e<T>(classNum));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfBPTensors(tempRes->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (Nd4jLong i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);

                for (Nd4jLong e = 0; e < current->lengthOf(); e++) {
                    if (nd4j::math::nd4j_abs(listOfBPTensors->at(classNum)->e<T>(e) - current->e<T>(e)) <= T(1.e-6))
                        currentOut->p(e, currentGradOut->e<T>(e));
                }
            }
        }
        delete tempRes;
        return ND4J_STATUS_OK;
    }

    int segmentMaxFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output) {
        BUILD_SINGLE_SELECTOR(output->dataType(), return segmentMaxFunctorBP_, (input, indices, gradOut, output), NUMERIC_TYPES);
    }
    BUILD_SINGLE_TEMPLATE(template int segmentMaxFunctorBP_, (NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output), NUMERIC_TYPES);

    // segmen min
    int segmentMinFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output) {
        std::unique_ptr<NDArray> tempRes(gradOut->dup());
        segmentMinFunctor(input, indices, tempRes.get());
        if (input->isVector()) {
#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < input->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                if (nd4j::math::nd4j_abs(tempRes->e<double>(classNum) - input->e<double>(e)) < 1.e-5)
                    output->p(e, gradOut->e<double>(classNum));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfBPTensors(tempRes->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);
            output->assign(0.);
            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);
                for (int e = 0; e < current->lengthOf(); e++) {
                    if (nd4j::math::nd4j_abs(listOfBPTensors->at(classNum)->e<double>(e) - current->e<double>(e)) < 1.e-5)
                        currentOut->p(e, currentGradOut->e<double>(e));
                }
            }
        }
        return ND4J_STATUS_OK;
    }

    // segmen mean
    int segmentMeanFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output) {
        int numClasses = output->sizeAt(0);
        std::map<Nd4jLong, Nd4jLong> classCount;//(numClasses);

        for (Nd4jLong count = 0; count < numClasses; ++count) {
            classCount[count] = 0;
        }

        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
            classCount[indices->e<Nd4jLong>(e)] ++;
        }

        // if input is a vector: (as if in doc sample)
        if (input->isVector()) {
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p(e, gradOut->e<double>(classNum) / classCount[classNum]);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);

                for (int e = 0; e < current->lengthOf(); e++) {
                    currentOut->p(e, currentGradOut->e<double>(e) / classCount[classNum]);
                }
            }
        }
        return ND4J_STATUS_OK;
    }

    int segmentSumFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output) {
//        int numClasses = output->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        Nd4jLong idx = indices->e<Nd4jLong>(0);
        if (input->isVector()) {
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p(e, gradOut->e<double>(classNum));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);
                currentOut->assign(currentGradOut);
            }
        }
        return ND4J_STATUS_OK;
    }

    int segmentProdFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, NDArray* output) {
        auto tempRes = gradOut->dup();
        segmentProdFunctor(input, indices, tempRes);
        if (input->isVector()) {
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p(e, gradOut->e<double>(classNum) * tempRes->e<double>(classNum)/ input->e<double>(e));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfBPTensors(tempRes->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);
                NDArray* currentFFOut = listOfBPTensors->at(classNum);

                currentOut->assign((*currentFFOut) * (*currentGradOut) / (*current));
            }
        }
        delete tempRes;
        return ND4J_STATUS_OK;
    }

    // -------------------------------------------------------------------------------------------------------------- //
    // Unsorted backpropagate segment ops
    // -------------------------------------------------------------------------------------------------------------- //

    template <typename T>
    static int unsortedSegmentMaxFunctorBP_(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {
//        int numOfClasses = gradOut->sizeAt(0);
        // if input is a vector: (as if in doc sample)
        auto tempRes = gradOut->dup();
        unsortedSegmentMaxFunctor(input, indices, numOfClasses, tempRes);
        if (input->isVector()) {
//#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < input->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                if (nd4j::math::nd4j_abs(tempRes->e<double>(classNum) - input->e<double>(e)) < 1.e-5)
                    output->p(e, gradOut->e<T>(classNum));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
//#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfBPTensors(tempRes->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
//#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);
                for (int e = 0; e < current->lengthOf(); e++) {
                    if (nd4j::math::nd4j_abs(listOfBPTensors->at(classNum)->e<double>(e) - current->e<double>(e)) < 1.e-5)
                        currentOut->p(e, currentGradOut->e<T>(e));
                }
            }
        }
        delete tempRes;
        return ND4J_STATUS_OK;
    }

    int unsortedSegmentMaxFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {
        BUILD_SINGLE_SELECTOR(output->dataType(), return unsortedSegmentMaxFunctorBP_, (input, indices, gradOut, numOfClasses, output), NUMERIC_TYPES);
    }
    BUILD_SINGLE_TEMPLATE(template int unsortedSegmentMaxFunctorBP_, (NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output), NUMERIC_TYPES);

    template <typename T>
    static int unsortedSegmentMinFunctorBP_(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {
        auto tempRes = gradOut->dup();
        unsortedSegmentMinFunctor(input, indices, numOfClasses, tempRes);
        if (input->isVector()) {
#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < input->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                if (nd4j::math::nd4j_abs(tempRes->t<T>(classNum) - input->t<T>(e)) < 1.e-6)
                    output->t<T>(e) = gradOut->t<T>(classNum);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfBPTensors(tempRes->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);

                for (int e = 0; e < current->lengthOf(); e++) {
                    if (nd4j::math::nd4j_abs(listOfBPTensors->at(classNum)->t<T>(e) - current->t<T>(e)) < 1.e-6)
                        currentOut->t<T>(e) = currentGradOut->t<T>(e);
                }
            }
        }
        delete tempRes;
        return ND4J_STATUS_OK;
    }

    int unsortedSegmentMinFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {
        BUILD_SINGLE_SELECTOR(output->dataType(), return unsortedSegmentMinFunctorBP_, (input, indices, gradOut, numOfClasses, output), NUMERIC_TYPES);
    }
    BUILD_SINGLE_TEMPLATE(template int unsortedSegmentMinFunctorBP_, (NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output), NUMERIC_TYPES);

    int unsortedSegmentMeanFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {

        std::map<Nd4jLong, Nd4jLong> classCount;//(numClasses);

//#pragma omp parallel for schedule(static)
        for (Nd4jLong count = 0; count < numOfClasses; ++count) {
            classCount[count] = 0;
        }

//#pragma omp parallel for schedule(static)
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
            classCount[indices->e<Nd4jLong>(e)]++;
        }

        // if input is a vector: (as if in doc sample)
        if (input->isVector()) {
//#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p(e, gradOut->e<double>(classNum) / classCount[classNum]);
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
//#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
//#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);
                currentOut->assign(*currentGradOut / double(classCount[classNum]));
            }
        }
        return ND4J_STATUS_OK;
    }

    int unsortedSegmentSumFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {

        // if input is a vector: (as if in doc sample)
        Nd4jLong idx = indices->e<Nd4jLong>(0);
        if (input->isVector()) {
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p(e, gradOut->e<double>(classNum));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                //NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);

                currentOut->assign(currentGradOut);
            }
        }
        return ND4J_STATUS_OK;
    }

    int unsortedSegmentProdFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {
        auto tempRes = gradOut->dup();

        unsortedSegmentProdFunctor(input, indices, numOfClasses, tempRes);
        if (input->isVector()) {
#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p<double>(e, gradOut->e<double>(classNum) * tempRes->e<double>(classNum)/ input->e<double>(e));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfBPTensors(tempRes->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);
                auto currentFFOut = listOfBPTensors->at(classNum);

                currentOut->assign((*currentFFOut) * (*currentGradOut) / (*current));
            }
        }
        delete tempRes;
        return ND4J_STATUS_OK;
    }

//    template <typename T>
    int unsortedSegmentSqrtNFunctorBP(NDArray* input, NDArray* indices, NDArray* gradOut, Nd4jLong numOfClasses, NDArray* output) {
        std::map<Nd4jLong, Nd4jLong> classCount;//(numClasses);

//#pragma omp parallel for schedule(static)
        for (Nd4jLong count = 0; count < numOfClasses; ++count) {
            classCount[count] = 0;
        }

//#pragma omp parallel for schedule(static)
        for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
            classCount[indices->e<Nd4jLong>(e)]++;
        }

        // if input is a vector: (as if in doc sample)
        if (input->isVector()) {
#pragma omp parallel for schedule(static)
            for (Nd4jLong e = 0; e < indices->lengthOf(); ++e) {
                Nd4jLong classNum = indices->e<Nd4jLong>(e);
                output->p(e, gradOut->e<double>(classNum) / nd4j::math::nd4j_sqrt<double,double>(classCount[classNum]));
            }
        }
        else {
            std::vector<int> restDims(input->rankOf() - 1);
#pragma omp parallel for schedule(static)
            for (int e = 1; e < input->rankOf(); e++)
                restDims[e - 1] = e;

            std::unique_ptr<ResultSet> listOfGradOuts(gradOut->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfTensors(input->allTensorsAlongDimension(restDims));
            std::unique_ptr<ResultSet> listOfOutTensors(output->allTensorsAlongDimension(restDims));

            //int numOfClasses = tempRes->sizeAt(0); // number of classes
            //std::vector<std::pair<NDArray*, int>> outputs(numOfClasses);

            int pos = 0;
#pragma omp parallel for schedule(static)
            for (int i = 0; i < indices->lengthOf(); i++) {
                Nd4jLong classNum = indices->e<Nd4jLong>(i);
                NDArray* current = listOfTensors->at(i);
                NDArray* currentOut = listOfOutTensors->at(i);
                NDArray* currentGradOut = listOfGradOuts->at(classNum);

                for (int e = 0; e < current->lengthOf(); e++) {
                    currentOut->p(e, currentGradOut->e<double>(e) / nd4j::math::nd4j_sqrt<double,double>(classCount[classNum]));
                }
            }
        }
        return ND4J_STATUS_OK;
    }

}
}
}