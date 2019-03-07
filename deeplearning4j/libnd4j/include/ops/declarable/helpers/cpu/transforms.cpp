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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 20.04.2018
//


#include <ops/declarable/helpers/transforms.h>
#include <array/ResultSet.h>
#include <helpers/ShapeUtils.h>
#include <numeric>
#include <NDArrayFactory.h>
#include <helpers/TAD.h>

namespace nd4j 	  {
namespace ops 	  {
namespace helpers {



//////////////////////////////////////////////////////////////////////////
void triu(const NDArray& input, NDArray& output, const int diagonal) {

    const int rank = input.rankOf();
    
    switch(rank) {

        case 1:
            for(int i = 0; i < output.sizeAt(0); ++i)
                output({i, i+1, 0,0}).assign(input);
            output.setValueInDiagMatrix(0., diagonal-1, 'l');    
            break;

        case 2:
            output.assign(input);
            output.setValueInDiagMatrix(0., diagonal-1, 'l');    
            break;

        default: 
            auto inTads  = input.allTensorsAlongDimension({rank-2, rank-1});
            auto outTads = output.allTensorsAlongDimension({rank-2, rank-1});

// #pragma omp parallel for schedule(guided) if(inTads->size() > Environment::getInstance()->elementwiseThreshold()) 
            for(int i = 0; i < inTads->size(); ++i) {
                auto inSubArr = inTads->at(i);
                auto outSubArr = outTads->at(i);
                outSubArr->assign(inSubArr);
                outSubArr->setValueInDiagMatrix(0., diagonal-1, 'l');
            }
            delete inTads;
            delete outTads;
    }
}


//////////////////////////////////////////////////////////////////////////
template <typename T>
static void triuBP_(const NDArray& input, const NDArray& gradO, NDArray& gradI, const int diagonal) {

    auto dOdI = NDArray(&gradO);                // dO/dI
    helpers::triu(input, dOdI, diagonal);

#pragma omp parallel for if(dOdI.lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(guided)     
    for(int i = 0; i < dOdI.lengthOf(); ++i) {
        if(dOdI.e<T>(i) != (T)0.f)
            dOdI.p(i,  T(1.f));
    }

    gradI.assign(dOdI * gradO);                          // chain rule: dLoss/dI = dO/dI * dLoss/dO 
}

    void triuBP(const NDArray& input, const NDArray& gradO, NDArray& gradI, const int diagonal) {
        BUILD_SINGLE_SELECTOR(gradO.dataType(), triuBP_, (input, gradO, gradI, diagonal), LIBND4J_TYPES);
    }


BUILD_SINGLE_TEMPLATE(template void triuBP_, (const NDArray& input, const NDArray& gradO, NDArray& gradI, const int diagonal), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template <typename T>
static void trace_(const NDArray& input, NDArray& output) {

    const int inRank = input.rankOf();

    auto setOfSubArrs = input.allTensorsAlongDimension({inRank-2, inRank-1});

#pragma omp parallel for if(setOfSubArrs->size() > Environment::getInstance()->elementwiseThreshold()) schedule(guided)     
    for(int i = 0; i < setOfSubArrs->size(); ++i)
        output.p(i, setOfSubArrs->at(i)->getTrace());

    delete setOfSubArrs;
}

    void trace(const NDArray& input, NDArray& output) {
        BUILD_SINGLE_SELECTOR(input.dataType(), trace_, (input, output), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void trace_, (const NDArray& input, NDArray& output), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template <typename T>
void randomShuffle_(NDArray& input, NDArray& output, nd4j::random::RandomBuffer& rng, const bool isInplace) {

    // check edge cases first
    int temp;
    const int firstDim = input.sizeAt(0);    
    if(input.lengthOf() == 1 || firstDim == 1) {
        
        if(!isInplace)
            output.assign(input);
    } 
    else if (input.isVector() || shape::isLikeVector(input.getShapeInfo(), temp)) {
                
        // apply Fisher-Yates shuffle 
        if(isInplace) {
#pragma omp parallel for if((firstDim-1) > Environment::getInstance()->elementwiseThreshold()) schedule(guided)       
            for(int i = firstDim-1; i > 0; --i) {
                int r = rng.nextInt(0, i);
                if(i == r)
                    continue;
                T _e0 = input.e<T>(i);
                T _e1 = input.e<T>(r);
                //math::nd4j_swap<T>(input(i), input(r));
                input.p<T>(i, _e1);
                input.p<T>(r, _e0);
            }        
        }
        else {        
            std::vector<int> indices(firstDim);        
            std::iota(indices.begin(), indices.end(), 0);        
            output.p<T>(Nd4jLong(0), input.e<T>(0));
#pragma omp parallel for if((firstDim-1) > Environment::getInstance()->elementwiseThreshold()) schedule(guided)       
            for(int i = firstDim-1; i > 0; --i) {
                int r = rng.nextInt(0, i);
                output.p(i, input.e<T>(indices[r]));
                if(i == r)
                    continue;

                output.p(r, input.e<T>(indices[i]));
                math::nd4j_swap<int>(indices[i], indices[r]);
            }           
            rng.rewindH(firstDim-1);
        }
    }
    else {
            
        // evaluate sub-arrays list of input array through all dimensions excluding first one
        std::vector<int> dimensions = ShapeUtils::evalDimsToExclude(input.rankOf(), {0});
        auto subArrsListIn = input.allTensorsAlongDimension(dimensions);

        // apply Fisher-Yates shuffle
        if(isInplace) {
#pragma omp parallel for if((firstDim-1) > Environment::getInstance()->elementwiseThreshold()) schedule(guided)        
            for(int i = firstDim-1; i > 0; --i) {
                int r = rng.nextInt(0, i);
                if(i == r)
                    continue;
                subArrsListIn->at(i)->swapUnsafe(*subArrsListIn->at(r));
            }        
        }
        else {
            // evaluate sub-arrays list of output array through all dimensions excluding first one        
            auto subArrsListOut = output.allTensorsAlongDimension(dimensions);
            std::vector<int> indices(firstDim);        
            std::iota(indices.begin(), indices.end(), 0);        
            bool isZeroShuffled = false;
#pragma omp parallel for if((firstDim-1) > Environment::getInstance()->elementwiseThreshold()) schedule(guided)       
            for(int i = firstDim-1; i > 0; --i) {
                int r = rng.nextInt(0, i);
                subArrsListOut->at(i)->assign(subArrsListIn->at(indices[r]));
                if(r == 0)
                    isZeroShuffled = true;
                if(i == r)
                    continue;
                subArrsListOut->at(r)->assign(subArrsListIn->at(indices[i]));
                math::nd4j_swap<int>(indices[i], indices[r]);
            }           
            if(!isZeroShuffled)
                subArrsListOut->at(0)->assign(subArrsListIn->at(0));
            delete subArrsListOut;
        }
        rng.rewindH(firstDim-1);
        delete subArrsListIn;
    }

}

    void randomShuffle(NDArray& input, NDArray& output, nd4j::random::RandomBuffer& rng, const bool isInplace) {
        BUILD_SINGLE_SELECTOR(input.dataType(), randomShuffle_, (input, output, rng, isInplace), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void randomShuffle_, (NDArray& input, NDArray& output, nd4j::random::RandomBuffer& rng, const bool isInplace), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
void pad_(const int mode, const NDArray& input, const NDArray& paddings, NDArray& output, NDArray const& padValue) {

    const int rank = output.rankOf();
    std::vector<int> dimsToExclude(rank);
    std::iota(dimsToExclude.begin(), dimsToExclude.end(), 0);             // fill with 0, 1, ... rank-1

    Nd4jLong numLeft    = paddings.e<Nd4jLong>(rank-1,0);
    Nd4jLong numRight   = paddings.e<Nd4jLong>(rank-1,1);
    Nd4jLong inDimSize  = input.sizeAt(rank-1);
    Nd4jLong outDimSize = output.sizeAt(rank-1);

    std::vector<std::vector<Nd4jLong>> outIdx = { std::vector<Nd4jLong>(2*rank), {numLeft, numLeft + inDimSize}, {0, numLeft}, {numLeft + inDimSize, outDimSize} };

    for(int i = 0; i < rank-1; ++i) {
        outIdx[0][2*i]     = paddings.e<Nd4jLong>(i, 0);
        outIdx[0][2*i + 1] = outIdx[0][2*i] + input.sizeAt(i);
    }
    outIdx[0][2*rank-1] = outIdx[0][2*rank-2] = 0;

    // ***** populate innermost sub-arrays firstly ***** //
    dimsToExclude.pop_back();

    Nd4jLong startL = mode == 1 ? 1 : 0;                            // REFLECT or SYMMETRIC
    Nd4jLong startR = mode == 1 ? inDimSize-2 : inDimSize-1;        // REFLECT or SYMMETRIC

    Nd4jLong numOfSubArrs = ShapeUtils::getNumOfSubArrs(input.getShapeInfo(), dimsToExclude);

    NDArray outSubArr0 = output(outIdx[0], true);

#pragma omp parallel for schedule(guided)
    for(Nd4jLong j = 0; j < numOfSubArrs; ++j) {

        NDArray outSubArr1   = outSubArr0(j, dimsToExclude);
        NDArray inSubArr     = input(j, dimsToExclude);
        NDArray outSubArrMid = outSubArr1(outIdx[1]);

        outSubArrMid.assign(inSubArr);      // assign middle

        if(mode == 0)  { // CONSTANT
            if(numLeft != 0) {
                NDArray temp = outSubArr1(outIdx[2]);
                temp.assign(padValue);                        // assign left
            }
            if(numRight != 0) {
                NDArray temp = outSubArr1(outIdx[3]);
                temp.assign(padValue);                        // assign right
            }
        }
        else {                                                              // REFLECT or SYMMETRIC

            for(Nd4jLong k = numLeft-1, e = startL; k >= 0; --k, ++e)     // fill left side
                outSubArr1.t<T>(k) = inSubArr.t<T>(e);

            for(Nd4jLong k = numLeft + inDimSize, e = startR; k < outDimSize; ++k, --e)     // fill right side
                outSubArr1.t<T>(k) = inSubArr.t<T>(e);
        }
    }

    // ***** fill rest of outer sub-arrays ***** //
    std::vector<Nd4jLong> outIdxInner(2, 0);
    std::vector<Nd4jLong> outIdxOuter(2, 0);

    for(int i = rank - 2; i >= 0; --i) {

        dimsToExclude.pop_back();

        outIdxInner.push_back(0), outIdxInner.push_back(0);
        outIdxOuter.push_back(0), outIdxOuter.push_back(0);

        Nd4jLong numLeft  = paddings.e<Nd4jLong>(i, 0);
        Nd4jLong numRight = paddings.e<Nd4jLong>(i, 1);

        if(numLeft == 0 && numRight == 0)
            continue;

        Nd4jLong inDimSize  = input.sizeAt(i);
        Nd4jLong outDimSize = output.sizeAt(i);

        if(mode == 0) {
            outIdxOuter[0] = 0;                   outIdxOuter[1] = numLeft;
            outIdxInner[0] = numLeft + inDimSize; outIdxInner[1] = outDimSize;
        }

        startL = mode == 1 ? numLeft + 1 : numLeft;                            // REFLECT or SYMMETRIC
        startR = mode == 1 ? numLeft + inDimSize - 2 : numLeft + inDimSize-1;      // REFLECT or SYMMETRIC

        numOfSubArrs = ShapeUtils::getNumOfSubArrs(output.getShapeInfo(), dimsToExclude);

#pragma omp parallel for schedule(guided) firstprivate(outIdxOuter, outIdxInner)
        for(Nd4jLong j = 0; j < numOfSubArrs; ++j) {

            NDArray outSubArr = output(j, dimsToExclude);

            if(mode == 0)  { // CONSTANT

                if(numLeft != 0) {
                    NDArray tempO = outSubArr(outIdxOuter);
                    tempO.assign(padValue);                              // assign left
                }

                if(numRight != 0) {
                    NDArray tempI = outSubArr(outIdxInner);
                    tempI.assign(padValue);                              // assign right
                }
            }
            else {                                                              // REFLECT or SYMMETRIC

                for(Nd4jLong k = numLeft-1, e = startL; k >= 0; --k, ++e) {    // fill left side
                    outIdxOuter[0] = k;
                    outIdxOuter[1] = k+1;
                    outIdxInner[0] = e;
                    outIdxInner[1] = e+1;
                    NDArray outSubArrInner = outSubArr(outIdxInner);
                    NDArray outSubArrOuter = outSubArr(outIdxOuter);
                    outSubArrOuter.assign(outSubArrInner);
                }

                for(Nd4jLong k = numLeft + inDimSize, e = startR; k < outDimSize; ++k, --e) {    // fill right side
                    outIdxOuter[0] = k;
                    outIdxOuter[1] = k+1;
                    outIdxInner[0] = e;
                    outIdxInner[1] = e+1;
                    NDArray outSubArrInner = outSubArr(outIdxInner);
                    NDArray outSubArrOuter = outSubArr(outIdxOuter);
                    outSubArrOuter.assign(outSubArrInner);
                }
            }
        }
    }
}

void pad(const int mode, const NDArray& input, const NDArray& paddings, NDArray& output, NDArray const& padValue) {
    BUILD_SINGLE_SELECTOR(input.dataType(), pad_, (mode, input, paddings, output, padValue), LIBND4J_TYPES);
}

BUILD_SINGLE_TEMPLATE(template void pad_, (const int mode, const NDArray& input, const NDArray& paddings, NDArray& output, NDArray const& padValue), LIBND4J_TYPES);

////////////////////////////////////////////////////////////////////////
/*// initial values of inIdx, outIdx, dim must be equal to zero
template<typename T>
static void recursiveLoopForPad_(const int mode, NDArray& input, const NDArray& paddings, NDArray& output, std::vector<int> dimensions, int dim, int inIdx, int outIdx, NDArray& padValue ) {
    
    int leftOffset;
    // dimensions are array of input dimensions, it is sorted in increasing order
    // every time at the beginning we erase first element from it (not good idea to use vector for this purpose, but luckily it is small enough)
    // then we use this array for tads building, every time while recursion the number of built tads becomes bigger 
    dimensions.erase(dimensions.begin());       
    // build tad basing on output array, also create auxiliary arrays pointing on required output array ranges
    shape::TAD tadOut(output.getShapeInfo(), dimensions.data(), dimensions.size());
    tadOut.createTadOnlyShapeInfo();
    tadOut.createOffsets();
    auto subArrOut = NDArray(output.getBuffer(), tadOut.tadOnlyShapeInfo, output.getWorkspace());
    auto subArr = NDArray(output.getBuffer(), tadOut.tadOnlyShapeInfo, output.getWorkspace());
    // build tad basing on input array, also create auxiliary array pointing on required input array range
    shape::TAD tadIn(input.getShapeInfo(), dimensions.data(), dimensions.size());
    tadIn.createTadOnlyShapeInfo();
    tadIn.createOffsets();
    auto subArrIn = NDArray(input.getBuffer(), tadIn.tadOnlyShapeInfo, output.getWorkspace());
    // these indices take into account recursion and always point to actual tads numbers
    if (input.rankOf() > 1 && output.rankOf() > 1) {// only for non-vector cases
        outIdx = outIdx * output.sizeAt(dim + 1);
        inIdx = inIdx * input.sizeAt(dim + 1);
    }
    // current input tad number, we add to it unity in a loop
    int k = -1;
    // loop through current dimension
    for(int i = 0; i < output.sizeAt(dim); ++i) {
        // corresponds to outer range (relevant indices are absent in input)                        
        leftOffset = paddings.e<int>(dim, 0);
        if(i < leftOffset || i >= (input.sizeAt(dim) + leftOffset))
            continue;

        // increase input tads number
        ++k;
        // recursion condition allows for the fact that tad can't reduce to scalar
        if(dim < input.rankOf() - 2)
            recursiveLoopForPad(mode, input, paddings, output, dimensions, dim + 1, inIdx + k, outIdx + i, padValue);
        else if (paddings.sizeAt(0) > dim + 1){
            leftOffset = paddings.e<int>(dim + 1, 0);
            // shift buffers pointers to actual element position
            if (output.rankOf() > 1) {
                subArrOut.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + i]);
                subArrIn.setBuffer(reinterpret_cast<T*>(input.getBuffer()) + tadIn.tadOffsets[inIdx + i - paddings.e<int>(dim, 0)]);
            }
            else {
                subArrOut.p(i, subArrIn.e<T>(i - leftOffset));
            }
            // most inner loop, corresponds to last dim = rank-1
            switch (mode) {
                case 0:             // CONSTANT mode                    
                    for(int j = 0; j < subArrOut.lengthOf(); ++j)                   
                            if(j < leftOffset || j >= (subArrIn.lengthOf() + leftOffset) )                  // firstly fill with zeros outer ranges
                                subArrOut.p(j, (T)0.f);
                            else
                                subArrOut.p(j, subArrIn.e<T>(j - leftOffset));   // fill middle with elements of input array
                    break;

                case 1:             // REFLECT mode                 
                    for(int j = 1;  j <= leftOffset; ++j)                                               // fill firstly left side 
                        subArrOut.p(leftOffset - j, subArrIn.e<T>(j));
                    for(int j = 0; j < subArrIn.lengthOf(); ++j)                                        // fill middle
                        subArrOut.p(leftOffset + j, subArrIn.e<T>(j));
                    for(int j = (subArrOut.lengthOf() - leftOffset); j < subArrOut.lengthOf(); ++j)     // fill right side
                        subArrOut.p(j, subArrIn.e<T>(subArrOut.lengthOf() - j - 1));
                    break;

                case 2:             // SYMMETRIC mode               
                    for(int j = 1;  j <= leftOffset; ++j)                                               // fill firstly left side 
                        subArrOut.p(leftOffset - j, subArrIn.e<T>(j-1));
                    for(int j = 0; j < subArrIn.lengthOf(); ++j)                                        // fill middle
                        subArrOut.p(leftOffset + j, subArrIn.e<T>(j));
                    for(int j = (subArrOut.lengthOf() - leftOffset); j < subArrOut.lengthOf(); ++j)     // fill right side
                        subArrOut.p(j, subArrIn.e<T>(subArrOut.lengthOf() - j));
                    break;
            }
        }
        else {

             if (mode == 0 && input.rankOf() < 2)
                 subArrOut.p(i, subArrIn.e<T>(i - leftOffset));   // fill middle with elements of input array
        }   
    }   
    // populate sub-array formed previously 
    leftOffset = paddings.e<int>(dim,0);
    switch (mode) {
        case 0:         // CONSTANT mode
            for(int j = 1;  j <= leftOffset; ++j) {
                // fill left side with padValue
                if (output.rankOf() > 1) {
                    subArrOut.setBuffer(
                            reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + leftOffset - j]);
                    subArrOut.assign(padValue);
                }
                else {
                    subArrOut.p(j - 1, padValue);
                }
            }
//            output.printIndexedBuffer("Output at");
            for(int j = (output.sizeAt(dim) - leftOffset); j < output.sizeAt(dim); ++j) {       // fill left side with zeros
                if (output.rankOf() > 1) {
                    subArrOut.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + j]);
                    subArrOut.assign(padValue);
                }
                else {
                    subArrOut.p(j, padValue);
                }
            }
            break;

        case 1:         // REFLECT mode 
            for(int j = 1;  j <= leftOffset; ++j) {                                                     // fill left side 
                subArr.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + leftOffset + j]);
                subArrOut.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + leftOffset - j]);
                subArrOut.assign(&subArr);
            }               
            for(int j = (output.sizeAt(dim) - leftOffset); j < output.sizeAt(dim); ++j) {       // fill right side
                subArr.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + output.sizeAt(dim) + leftOffset - 1 - j]);
                subArrOut.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + j]);
                subArrOut.assign(&subArr);              
            }   
            break;

        case 2:         // SYMMETRIC mode   
            for(int j = 1;  j <= leftOffset; ++j) {                                                     // fill left side
                subArr.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + leftOffset + j - 1]);
                subArrOut.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + leftOffset - j]);
                subArrOut.assign(&subArr);
            }           
            for(int j = (output.sizeAt(dim) - leftOffset); j < output.sizeAt(dim); ++j) {       // fill right side
                subArr.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + output.sizeAt(dim) + leftOffset - j]);
                subArrOut.setBuffer(reinterpret_cast<T*>(output.getBuffer()) + tadOut.tadOffsets[outIdx + j]);
                subArrOut.assign(&subArr);      
            }
            break;
    }
}
 */
/*
    void recursiveLoopForPad(const int mode, NDArray& input, const NDArray& paddings, NDArray& output, std::vector<int> dimensions, int dim, int inIdx, int outIdx, NDArray& padValue ) {
        BUILD_SINGLE_SELECTOR(input.dataType(), recursiveLoopForPad_, (mode, input, paddings, output, dimensions, dim, inIdx, outIdx, padValue), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void recursiveLoopForPad_, (const int mode, NDArray& input, const NDArray& paddings, NDArray& output, std::vector<int> dimensions, int dim, int inIdx, int outIdx, NDArray& padValue), LIBND4J_TYPES);

*/

////////////////////////////////////////////////////////////////////////
void invertPermutation(const NDArray& input, NDArray& output) {

    std::set<int> uniqueElems;
    const int length = input.lengthOf();    

// #pragma omp parallel for if(length > Environment::getInstance()->elementwiseThreshold()) schedule(static)         
    for(int i = 0; i < length; ++i) {
        
        int elem = input.e<int>(i);
 
        if(!uniqueElems.insert(elem).second)        // this operation forbids us to use #pragma omp
            throw std::runtime_error("helpers::invertPermutation function: input array contains duplicates !");
            
        if(elem < 0 || elem > length - 1)
            throw  std::runtime_error("helpers::invertPermutation function: element of input array is out of range (0, length-1) !");

        output.p<int>(elem, i);
    }
}

////////////////////////////////////////////////////////////////////////
template<typename T>
static void gatherND_(NDArray& input, NDArray& indices, NDArray& output) {

    if (input.ordering() != 'c') 
        input.streamline('c');

    if (indices.ordering() != 'c')
        indices.streamline('c');

    const int rankIn     = input.rankOf();
    const int rankInd    = indices.rankOf();
    const int lastIndDim = indices.sizeAt(-1);
    
    std::vector<int> tadDims(rankIn - lastIndDim);
    std::iota(tadDims.begin(), tadDims.end(), rankInd-1);
    auto innerMostOut = output.allTensorsAlongDimension(tadDims);

    auto innerMostInd = indices.allTensorsAlongDimension({rankInd-1});
    
    std::iota(tadDims.begin(), tadDims.end(), lastIndDim);
    auto innerMostIn = input.allTensorsAlongDimension(tadDims);

    Nd4jLong* outerShapeInfo = nullptr;
    ALLOCATE(outerShapeInfo, input.getWorkspace(), shape::shapeInfoLength(lastIndDim), Nd4jLong);
    outerShapeInfo[0] = lastIndDim;
    for(int i = 1; i <= lastIndDim; ++i)
        outerShapeInfo[i] = input.sizeAt(i-1);
    shape::updateStrides(outerShapeInfo, input.ordering());

    Nd4jLong idx[MAX_RANK];

    for(int i = 0; i < innerMostInd->size(); ++i) {
                
        auto idxSubArr = innerMostInd->at(i);
        
        for(int j = 0; j < lastIndDim; ++j) {
            if(idxSubArr->e<Nd4jLong>(j) >= input.sizeAt(j))
                throw std::runtime_error("helpers::gatherND function: indices array contains wrong elements, each element must be smaller than corresponding dimension of input array !");
            idx[j] = idxSubArr->e<Nd4jLong>(j);
        }
                
        auto currentInd0 = shape::getOffset(0, shape::shapeOf(outerShapeInfo), shape::stride(outerShapeInfo), idx, lastIndDim);

        if(rankIn != lastIndDim) {
            auto outSubArr = innerMostOut->at(i);
            outSubArr->assign(innerMostIn->at(currentInd0));
        }
        else
            output.p(i, input.e<T>(currentInd0));
    }

    delete innerMostInd;
    delete innerMostIn;
    delete innerMostOut;
    RELEASE(outerShapeInfo, input.getWorkspace());    
}

    void gatherND(NDArray& input, NDArray& indices, NDArray& output) {
        BUILD_SINGLE_SELECTOR(input.dataType(), gatherND_, (input, indices, output), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void gatherND_, (NDArray& input, NDArray& indices, NDArray& output), LIBND4J_TYPES);


////////////////////////////////////////////////////////////////////////
template<typename T>
static void gather_(NDArray* input, const NDArray* indices, NDArray* output, const std::vector<int>& intArgs) {

    int axis = intArgs.size() > 0 ? intArgs[0] : 0;
    const int inputRank = input->rankOf();
    if(axis < 0)
        axis += inputRank;

    const int numOfIntArgs = intArgs.size();

    if (indices != nullptr) {        

        for(int i = 0; i < indices->lengthOf(); ++i)
            if(indices->e<Nd4jLong>(i) >= input->sizeAt(axis))
                throw std::runtime_error("helpers::gather function: indices array contains wrong elements, each element must be smaller than corresponding dimension of input array !");
    
        // first case: indices consist of only one scalar
        if(indices->isScalar()) {
            if(input->rankOf() <= 1){
                //For scalar indices, rank 0 or 1 input: can't do tensor along dimension 0 as this is whole array... instead, we want to get a scalar
				auto idx = indices->e<Nd4jLong>(0);
				auto scalarNDArray = input->e(idx);
                output->assign(scalarNDArray);
            } else {
                std::vector<int> dimensions = ShapeUtils::evalDimsToExclude(input->rankOf(), {axis});
                shape::TAD tad;
                tad.init(input->getShapeInfo(), dimensions.data(), dimensions.size());
                tad.createTadOnlyShapeInfo();
                tad.createOffsets();
                auto tadArr = NDArray(reinterpret_cast<void *>(reinterpret_cast<T*>(input->getBuffer()) + tad.tadOffsets[indices->e<Nd4jLong>(0)]), tad.tadOnlyShapeInfo, output->getWorkspace());
                output->assign(&tadArr);
			}
        }
        else if (input->rankOf() == 1 && indices->isVector()) {
            // special case
#pragma omp parallel for if(indices->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(guided)     
            for (int e = 0; e < indices->lengthOf(); e++)
                output->p(e, input->e<T>(indices->e<Nd4jLong>(e)));
        }        
        else {   
            
            std::vector<int> dimsOut(indices->rankOf());            
            std::iota(dimsOut.begin(), dimsOut.end(), axis);   // fill with axis, axis+1, ... indices->rankOf()-1
            const Nd4jLong numOfSubArrs = ShapeUtils::getNumOfSubArrs(output->getShapeInfo(), dimsOut);            
#pragma omp parallel for if(numOfSubArrs > Environment::getInstance()->elementwiseThreshold()) schedule(guided)             
            for(int i = 0; i < numOfSubArrs; ++i) {
                NDArray subArrOut = (*output)(i, dimsOut);
                NDArray subArrIn  = (*input)(indices->e<Nd4jLong>(i), {axis});
                subArrOut.assign(subArrIn);
            }            
        }        
    } 
    else {
        
        for(int i = 1; i < numOfIntArgs; ++i)
            if(intArgs[i] >= input->sizeAt(axis))
                throw std::runtime_error("helpers::gather function: some of input indexes is larger than corresponding shape of input array !");

        // we only allow scalar/vector case here
        if (numOfIntArgs == 2) { // scalar case            
            output->assign((*input)(intArgs[1], {axis}));
        } 
        else { // vector case
            const Nd4jLong numOfSubArrs = ShapeUtils::getNumOfSubArrs(output->getShapeInfo(), {axis});
#pragma omp parallel for if(numOfSubArrs > Environment::getInstance()->elementwiseThreshold()) schedule(guided)             
            for(int i = 0; i < numOfSubArrs; ++i) {
                NDArray subArrOut = (*output)(i, {axis});
                NDArray subArrIn  = (*input)(intArgs[i+1], {axis});
                subArrOut.assign(subArrIn);
            }
        }
    }    
}

    void gather(NDArray* input, const NDArray* indices, NDArray* output, const std::vector<int>& intArgs) {
        BUILD_SINGLE_SELECTOR(input->dataType(), gather_, (input, indices, output, intArgs), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void gather_, (NDArray* input, const NDArray* indices, NDArray* output, const std::vector<int>& intArgs), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
void eye(NDArray& output) {

    const int rank = output.rankOf();
    auto arrs = output.allTensorsAlongDimension({rank-2, rank-1});

#pragma omp parallel for if(arrs->size() > Environment::getInstance()->elementwiseThreshold()) schedule(guided)
    for(int i = 0; i < arrs->size(); ++i)
        arrs->at(i)->setIdentity();
    
    delete arrs;
}

//////////////////////////////////////////////////////////////////////////
void scatterUpdate(NDArray& input, NDArray& updates, const std::vector<int>* intArgs) {

    int opCode = (*intArgs)[0];
    int dimSize = (*intArgs)[1];    
    Nd4jLong e;
    Nd4jLong limg = 2 + dimSize;
    std::vector<int> tadDimensions(dimSize);
    for (e = 2; e < limg; e++)
        tadDimensions[e-2] = (*intArgs)[e];

    std::vector<int> dimsToExclude = ShapeUtils::evalDimsToExclude(input.rankOf(), tadDimensions);

    // increasing counter to skip numIndices
    e++;
    std::vector<int> indices;        
    for (; e < intArgs->size(); e++)
        indices.push_back((*intArgs)[e]);

#pragma omp parallel for schedule(guided) proc_bind(close)
    for (Nd4jLong i = 0; i < indices.size(); ++i) {
                
        auto inSubArr  = input(indices[i], dimsToExclude, true);
        auto updSubArr = updates(i,        dimsToExclude, true);
        
        if (inSubArr.lengthOf() != updSubArr.lengthOf())
            continue;

        switch (opCode) {
            case 0:
                inSubArr.applyPairwiseTransform(pairwise::Add, &updSubArr, &inSubArr, nullptr);
                break;
            case 1:
                inSubArr.applyPairwiseTransform(pairwise::Subtract, &updSubArr, &inSubArr, nullptr);
                break;
            case 2:
                inSubArr.applyPairwiseTransform(pairwise::Multiply, &updSubArr, &inSubArr, nullptr);
                break;
            case 3:
                inSubArr.applyPairwiseTransform(pairwise::Divide, &updSubArr, &inSubArr, nullptr);
                break;
            case 4:
                inSubArr.applyPairwiseTransform(pairwise::ReverseSubtract, &updSubArr, &inSubArr, nullptr);
                break;
            case 5:
                inSubArr.applyPairwiseTransform(pairwise::ReverseDivide, &updSubArr, &inSubArr, nullptr);
                break;
            case 6:
                inSubArr.applyPairwiseTransform(pairwise::CopyPws, &updSubArr, &inSubArr, nullptr);
                break;
            default:
                continue;                 
        }
    }
}

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void mergeMaxIndex_(const std::vector<NDArray*>& inArrs, NDArray& output) {

    const Nd4jLong numArgs = inArrs.size();
    auto x = inArrs[0];

#pragma omp parallel for if(x->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(guided)
    for (Nd4jLong e = 0; e < x->lengthOf(); e++) {
        T max = -DataTypeUtils::max<T>();
        Nd4jLong idx = 0;
            
        for (int i = 0; i < numArgs; i++){
            
            T v = inArrs[i]->e<T>(e);
            if (v > max) {
                max = v;
                idx = i;
            }
        }
        output.p(e, idx);
    }
}
    void mergeMaxIndex(const std::vector<NDArray*>& inArrs, NDArray& output) {
        BUILD_SINGLE_SELECTOR(inArrs[0]->dataType(), mergeMaxIndex_, (inArrs, output), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void mergeMaxIndex_, (const std::vector<NDArray*>& inArrs, NDArray& output), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void mergeMax_(const std::vector<NDArray*>& inArrs, NDArray& output) {
    
    const Nd4jLong numArgs = inArrs.size();
    auto x = inArrs[0];

#pragma omp parallel for if(x->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(guided) proc_bind(close)
     for (Nd4jLong e = 0; e < x->lengthOf(); e++) {
        T max = -DataTypeUtils::max<T>();
        for (int i = 0; i < numArgs; i++) {
            T v = inArrs[i]->e<T>(e);
            if (v > max)
                max = v;
        }
        output.p(e, max);
    }
}
    void mergeMax(const std::vector<NDArray*>& inArrs, NDArray& output) {
        BUILD_SINGLE_SELECTOR(output.dataType(), mergeMax_, (inArrs, output), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void mergeMax_, (const std::vector<NDArray*>& inArrs, NDArray& output), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void mergeAvg_(const std::vector<NDArray*>& inArrs, NDArray& output) {
    
    const Nd4jLong numArgs = inArrs.size();
    const T factor = 1.f / numArgs;
    auto x = inArrs[0];
        
#pragma omp parallel for if(x->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(guided) proc_bind(close)
    for (Nd4jLong e = 0; e < x->lengthOf(); e++) {
        T sum = 0.;
        for (int i = 0; i < numArgs; i++) { 
            T v = inArrs[i]->e<T>(e);
            sum += v;
        }
        output.p<T>(e, sum * factor);
    }
}
    void mergeAvg(const std::vector<NDArray*>& inArrs, NDArray& output) {
        BUILD_SINGLE_SELECTOR(output.dataType(), mergeAvg_, (inArrs, output), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void mergeAvg_, (const std::vector<NDArray*>& inArrs, NDArray& output), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void mergeAdd_(const std::vector<NDArray*>& inArrs, NDArray& output) {
    
    const Nd4jLong numArgs = inArrs.size();
    auto x = inArrs[0];
        
#pragma omp parallel for if(x->lengthOf() > Environment::getInstance()->elementwiseThreshold()) schedule(guided) proc_bind(close)
    for (Nd4jLong e = 0; e < x->lengthOf(); e++) {
        
        T sum = (T) 0.f;
        
        for (int i = 0; i < numArgs; i++) 
            sum += inArrs[i]->e<T>(e);

        output.p(e, sum);
    }
}
    void mergeAdd(const std::vector<NDArray*>& inArrs, NDArray& output) {
        BUILD_SINGLE_SELECTOR(output.dataType(), mergeAdd_, (inArrs, output), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void mergeAdd_, (const std::vector<NDArray*>& inArrs, NDArray& output), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void clipByNorm_(NDArray& input, NDArray& output, const std::vector<int>& dimensions, const NDArray& clipNorm, const bool isInplace) {
        
    const int rank = input.rankOf();
   auto norm2 = input.reduceAlongDims(reduce::Norm2, dimensions);

    if (isInplace) {
        if(norm2.lengthOf() == 1) {

            if(norm2.e<T>(0) > clipNorm.e<T>(0))
                input *= (clipNorm.e<T>(0) / norm2.e<T>(0));
        }
        else {

            std::vector<int> dimsToExclude = ShapeUtils::evalDimsToExclude(rank, dimensions);
            const Nd4jLong numOfSubArrs = ShapeUtils::getNumOfSubArrs(input.getShapeInfo(), dimsToExclude);

#pragma omp parallel for schedule(guided) 
            for(Nd4jLong i = 0; i < numOfSubArrs; ++i) {
                if (norm2.e<T>(i) > clipNorm.e<T>(0)) {
                    
                    auto inputSubArr  = input(i, dimsToExclude);
                    inputSubArr *= (clipNorm.e<T>(0) / norm2.e<T>(i));
                }
            }
        }
    }
    else {
        
        if(norm2.lengthOf() == 1) {

            if(norm2.e<T>(0) > clipNorm.e<T>(0))
                output.assign( input * (clipNorm / norm2.e<T>(0)));
            else
                output.assign( input );
        }
        else {
            
            std::vector<int> dimsToExclude = ShapeUtils::evalDimsToExclude(rank, dimensions);
            const Nd4jLong numOfSubArrs = ShapeUtils::getNumOfSubArrs(input.getShapeInfo(), dimsToExclude);
            std::vector<Nd4jLong> idxRanges(rank * 2);

#pragma omp parallel for schedule(guided) firstprivate(idxRanges)
            for(Nd4jLong i = 0; i < numOfSubArrs; ++i) {

                ShapeUtils::evalIdxRangesForSubArr(i, input.getShapeInfo(), dimsToExclude, idxRanges.data());

                auto outputSubArr = output(idxRanges);
                auto inputSubArr  = input(idxRanges);
                outputSubArr.assign(inputSubArr);
                
                if (norm2.e<T>(i) > clipNorm.e<T>(0))
                    outputSubArr *= clipNorm / norm2.e<T>(i);
            }           
        }
    }
}

    void clipByNorm(NDArray& input, NDArray& output, const std::vector<int>& dimensions, const NDArray& clipNorm, const bool isInplace) {
        BUILD_SINGLE_SELECTOR(output.dataType(), clipByNorm_, (input, output, dimensions, clipNorm, isInplace), FLOAT_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void clipByNorm_, (NDArray& input, NDArray& output, const std::vector<int>& dimensions, const NDArray& clipNorm, const bool isInplace), FLOAT_TYPES);

    template <typename T>
    static void clipByGlobalNorm_(std::vector<NDArray*> const& inputs, double clipNorm, nd4j::memory::Workspace* workspace, std::vector<NDArray*>& outputs, bool isInplace) {
        NDArray globalNorm = NDArrayFactory::create<T>(0, workspace); //sqrt(sum([l2norm(t)**2 for t in t_list]))

        for (auto input: inputs) {
            auto l2norm = input->reduceNumber(reduce::Norm2);
            globalNorm += l2norm * l2norm;
        }

        globalNorm.applyTransform(transform::Sqrt, nullptr, nullptr);// = nd4j::math::nd4j_sqrt(globalNorm);
        outputs[inputs.size()]->p(0, globalNorm);

        const T factor = clipNorm / globalNorm.e<T>(0);

        for (size_t e = 0; e < inputs.size(); e++) {
            // all-reduce
            auto input = inputs[e];
            auto output = outputs[e];

            if (globalNorm.e<double>(0) <= clipNorm) {
                output->assign(input);
            }
            else {

                auto lambda = LAMBDA_T(_x, factor) { return _x * factor; };
                input->applyLambda<T>(lambda, output);
            }
        }
    }
    void clipByGlobalNorm(std::vector<NDArray*> const& inputs, double clipNorm, nd4j::memory::Workspace* workspace, std::vector<NDArray*>& outputs, bool isInplace) {
        BUILD_SINGLE_SELECTOR(outputs[0]->dataType(), clipByGlobalNorm_, (inputs, clipNorm, workspace, outputs, isInplace), FLOAT_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void clipByGlobalNorm_, (std::vector<NDArray*> const& inputs, double clipNorm, nd4j::memory::Workspace* workspace, std::vector<NDArray*>& outputs, bool isInplace), FLOAT_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void clipByNormBP_(const NDArray& input, const NDArray& gradO, NDArray& gradI /*output*/, const std::vector<int>& dimensions, const NDArray& clipNorm) {
    
    const int rank = input.rankOf();

    auto norm2 = input.reduceAlongDims(reduce::Norm2, dimensions);

    if(norm2.lengthOf() == 1) {        

        const T N = norm2.e<T>(0);

        auto cn = clipNorm.e<T>(0);
        
        if(N > cn) {

            const T sumOfProd = (input * gradO).reduceNumber(reduce::Sum).e<T>(0);    // reduce to scalar
            const T factor1 = static_cast<T>(1.f) / N;
            const T factor3 = factor1 / (N * N) ;                                            // 1 / (N*N*N)

            auto lambda = LAMBDA_TT(elem1, elem2, cn, sumOfProd, factor1, factor3) {
                return cn * (factor1 * elem2 - factor3 * elem1 * sumOfProd);
            };

            (const_cast<NDArray&>(input)).applyPairwiseLambda<T>(const_cast<NDArray*>(&gradO), lambda, &gradI);
        }
        else 
            gradI.assign(gradO);
    }
    else {
            
        std::vector<int> dimsToExclude = ShapeUtils::evalDimsToExclude(rank, dimensions);
        const Nd4jLong numOfSubArrs = ShapeUtils::getNumOfSubArrs(input.getShapeInfo(), dimsToExclude);
        std::vector<Nd4jLong> idxRanges(rank * 2);

#pragma omp parallel for schedule(guided) firstprivate(idxRanges)
        for(Nd4jLong i = 0; i < numOfSubArrs; ++i) {

            ShapeUtils::evalIdxRangesForSubArr(i, input.getShapeInfo(), dimsToExclude, idxRanges.data());
            T N = norm2.e<T>(i);

            auto gradOSubArr = gradO(idxRanges);
            auto gradISubArr = gradI(idxRanges);

            auto cn = clipNorm.e<T>(0);

            if (N > cn) {
                
                auto inputSubArr = input(idxRanges);
                
                const T sumOfProd = (inputSubArr * gradOSubArr).reduceNumber(reduce::Sum).e<T>(0);    // reduce to scalar
                const T factor1 = static_cast<T>(1.f) / N;
                const T factor3 = factor1 / (N * N) ;                                            // 1 / (N*N*N)

                auto lambda = LAMBDA_TT(elem1, elem2, cn, sumOfProd, factor1, factor3) {
                    return cn * (factor1 * elem2 - factor3 * elem1 * sumOfProd);
                };
                inputSubArr.applyPairwiseLambda<T>(&gradOSubArr, lambda, &gradISubArr);
            }
            else
                gradISubArr.assign(gradOSubArr);
        }           
    }
}

    void clipByNormBP(const NDArray& input, const NDArray& gradO, NDArray& gradI /*output*/, const std::vector<int>& dimensions, const NDArray& clipNorm) {
        BUILD_SINGLE_SELECTOR(gradI.dataType(), clipByNormBP_, (input, gradO, gradI, dimensions, clipNorm), FLOAT_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void clipByNormBP_, (const NDArray& input, const NDArray& gradO, NDArray& gradI /*output*/, const std::vector<int>& dimensions, const NDArray& clipNorm), FLOAT_TYPES);


//////////////////////////////////////////////////////////////////////////
template<typename T>
static void clipByAveraged_(NDArray& input, NDArray& output, const std::vector<int>& dimensions, const NDArray& clipNorm, const bool isInplace) {

    auto cn = clipNorm.e<T>(0);
    if (dimensions.size() == 0) {
        // all-reduce
        T n2 = input.reduceNumber(reduce::Norm2).e<T>(0) / input.lengthOf();
        if (n2 <= cn) {
            if (!isInplace)
                output.assign(input);
        } 
        else {
            const T factor = cn / n2;
            auto lambda = LAMBDA_T(_x, factor) { return _x * factor; };
            input.applyLambda<T>(lambda, &output);
        }
    } 
    else {
        // along dimension
        auto norm2 = input.reduceAlongDims(reduce::Norm2, dimensions, false);
        if (!isInplace)
                output.assign(input);
        auto tads = output.allTensorsAlongDimension(dimensions);
        // TODO: make this CUDA-compliant somehow
        for (int e = 0; e < tads->size(); e++) {
            T n2 = norm2.e<T>(e) / tads->at(e)->lengthOf();
            const T factor = cn / n2;
            if (n2 > cn) {
                auto lambda = LAMBDA_T(_x, factor) {return _x * factor;};
                tads->at(e)->applyLambda<T>(lambda, &output);
            }
        }
        delete tads;
    }
}

    void clipByAveraged(NDArray& input, NDArray& output, const std::vector<int>& dimensions, const NDArray& clipNorm, const bool isInplace) {
        BUILD_SINGLE_SELECTOR(input.dataType(), clipByAveraged_, (input, output, dimensions, clipNorm, isInplace), FLOAT_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void clipByAveraged_, (NDArray& input, NDArray& output, const std::vector<int>& dimensions, const NDArray& clipNorm, const bool isInplace), FLOAT_TYPES);

/*
    if (d1 > params[1])
    return params[1];
    else if (d1 < params[0])
    return params[0];
    else return d1;
*/

    template <typename T>
    static void clipByValue_(NDArray& input, double leftBound, double rightBound, NDArray& output) {
        auto routine = LAMBDA_T(_x, leftBound, rightBound) {
            if (_x > rightBound) return rightBound;
            if (_x < leftBound)  return leftBound;
            return _x;
        };

        input.applyLambda<T>(routine, &output);
    }

    void clipByValue(NDArray& input, double leftBound, double rightBound, NDArray& output) {
        BUILD_SINGLE_SELECTOR(input.dataType(), clipByValue_, (input, leftBound, rightBound, output), FLOAT_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void clipByValue_, (NDArray& input, double leftBound, double rightBound, NDArray& output);, FLOAT_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void mirrorPad_(const NDArray& input, const NDArray& paddings, NDArray& output, const int mode) {
    
    // mode:  0 - REFLECT, else - SYMMETRIC
    const int reflBorder = (bool)mode ? 1 : 0;
    const int rank        = input.rankOf();
    const Nd4jLong outLen = output.lengthOf();

    if(rank <= 1) {

        const Nd4jLong inLen         = input.lengthOf();
        const auto leftSide          = paddings.e<Nd4jLong>(0);
        const auto leftSideCorrected = leftSide - reflBorder;
        const Nd4jLong len           = 2*(inLen-1) + leftSide + reflBorder;        

        for(int i = 0; i < outLen; ++i) {
            
            if (i < leftSide)                                   // left side 
                output.p(i, input.e<T>(leftSideCorrected - i));            
            
            else if(i >= leftSide && i < leftSide + inLen)      // middle
                output.p(i, input.e<T>(i - leftSide));
            
            else                                                // right side
                output.p(i, input.e<T>(len - i));
        }
    }
    else {

        std::vector<Nd4jLong> inIdx(rank), outIdx(rank);
#pragma omp parallel for if(outLen > Environment::getInstance()->elementwiseThreshold()) schedule(guided) firstprivate(inIdx, outIdx)
        for(int i = 0; i < outLen; ++i) {

            shape::ind2subC(rank, output.shapeOf(), i, outIdx.data());

            for(int j = 0; j < rank; ++j) {
            
                const Nd4jLong inLen         = input.sizeAt(j);
                const auto leftSide          = paddings.e<T>(j, 0);
                const auto leftSideCorrected = leftSide - reflBorder;
                const Nd4jLong len           = 2*(inLen-1) + leftSide + reflBorder;

                if(outIdx[j] < leftSide)                                        // left side
                    inIdx[j] = leftSideCorrected - outIdx[j];

                else if(outIdx[j] >= leftSide && outIdx[j] < leftSide + inLen)  // middle
                    inIdx[j] = outIdx[j] - leftSide;

                else                                                            // right side
                    inIdx[j] = len - outIdx[j];
            }
    
            auto outOffset = shape::getOffset(0, output.shapeOf(), output.stridesOf(), outIdx.data(), rank);
            auto inOffset  = shape::getOffset(0, input.shapeOf(),  input.stridesOf(),  inIdx.data(),  rank);
            reinterpret_cast<T*>(output.buffer())[outOffset] = reinterpret_cast<T*>(input.getBuffer())[inOffset];
        }
    }
}

    void mirrorPad(const NDArray& input, const NDArray& paddings, NDArray& output, const int mode) {
        BUILD_SINGLE_SELECTOR(input.dataType(), mirrorPad_, (input, paddings, output, mode), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void mirrorPad_, (const NDArray& input, const NDArray& paddings, NDArray& output, const int mode), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template<typename T>
static void concat_(const std::vector<NDArray*>& inArrs, NDArray& output, const int axis) {

    const int numOfArrs = inArrs.size();
    bool allC = true;
    bool allScalar = true;
    bool allVectors = true;
    
    const Nd4jLong lenOfFirstArr = inArrs[0]->lengthOf();

    //detect whether all arrays are c ordered or not
    //Also detect whether they are all scalars
    for(int i = 0; i < numOfArrs; i++) {
        allC &= (inArrs[i]->ordering() == 'c');
        allScalar &= (inArrs[i]->isScalar());
        allVectors &= (inArrs[i]->isRowVector() && inArrs[i]->lengthOf() == lenOfFirstArr);
    }

    T* outBuff = output.bufferAsT<T>();

    //we are merging all scalars
    if(allScalar) {
        for(int i = 0; i < numOfArrs; i++)
            outBuff[i] = inArrs[i]->bufferAsT<T>()[0];
        return;
    }

    if(allC && axis == 0 && allVectors && output.ordering() == 'c') {
        
        if (numOfArrs >= 8) {

#pragma omp parallel for schedule(guided)
            for (int r = 0; r < numOfArrs; r++) {

                T *z = outBuff + r * lenOfFirstArr;
                T *x = inArrs[r]->bufferAsT<T>();

#pragma omp simd
                for (Nd4jLong e = 0; e < lenOfFirstArr; e++)
                    z[e] = x[e];
            }
        } 
        else {
            int currBuffer = 0;
            int currBufferOffset = 0;
            for (int i = 0; i < output.lengthOf(); i++) {
                outBuff[i] = inArrs[currBuffer]->bufferAsT<T>()[currBufferOffset++];
                if (currBufferOffset >= inArrs[currBuffer]->lengthOf()) {
                    currBuffer++;
                    currBufferOffset = 0;
                }
            }
        }
        return;
    }
    
    const int rank  = inArrs[0]->rankOf();
    const int rank2 = 2*rank;
    std::vector<std::vector<Nd4jLong>> indices(numOfArrs, std::vector<Nd4jLong>(rank2,0));

    // take into account indices for first array
    indices[0][2 * axis + 1] = inArrs[0]->sizeAt(axis);

    // loop through the rest of input arrays
    for(int i = 1; i < numOfArrs; ++i) {
        indices[i][2 * axis]     = indices[i-1][2 * axis + 1];                                // index start from
        indices[i][2 * axis + 1] = indices[i-1][2 * axis + 1] + inArrs[i]->sizeAt(axis);      // index end with (excluding)
    }

    for(int i = 0; i < numOfArrs; ++i) {
        auto temp = output(indices[i], true);
        temp.assign(inArrs[i]);
    }
}

    void concat(const std::vector<NDArray*>& inArrs, NDArray& output, const int axis) {
        BUILD_SINGLE_SELECTOR(output.dataType(), concat_,(inArrs, output, axis), LIBND4J_TYPES);
    }

    BUILD_SINGLE_TEMPLATE(template void concat_, (const std::vector<NDArray*>& inArrs, NDArray& output, const int axis), LIBND4J_TYPES);

//////////////////////////////////////////////////////////////////////////
template <typename T>
static void tileBP_(const NDArray& gradO /*input*/, NDArray& gradI /*output*/, const std::vector<Nd4jLong> reps) {

    T* gradIBuff      = reinterpret_cast<T*>(gradI.getBuffer());
    const T* gradOBuff      = reinterpret_cast<T*>(gradO.getBuffer());
    const Nd4jLong gradILen = gradI.lengthOf();
    const Nd4jLong gradOLen = gradO.lengthOf();  // gradOLen >= gradILen
    const Nd4jLong gradIEWS = nd4j::math::nd4j_abs<Nd4jLong>(gradI.ews());
    const Nd4jLong gradOEWS = gradO.ews();

    // initial zeroing of gradI content
    if(gradIEWS == 1)
        memset(gradIBuff, 0, gradILen * sizeof(T));
    else
#pragma omp parallel for schedule(static) proc_bind(close)
        for (int i = 0; i < gradILen * gradIEWS; i += gradIEWS)
            gradIBuff[i] = static_cast<T>(0.f);


    if(gradO.ordering() == 'c' && gradOEWS == 1) {
#pragma omp parallel for simd if(gradOLen > Environment::getInstance()->elementwiseThreshold()) schedule(guided)
        for(Nd4jLong i=0;  i<gradOLen; ++i) {
            auto idx = shape::subArrayIndex(i, gradO.getShapeInfo(), gradI.getShapeInfo());
            gradI.p(idx, gradI.e<T>(idx) + gradOBuff[i]);
        }
    }
    else if(gradO.ordering() == 'c' && gradOEWS > 1) {
#pragma omp parallel for simd if(gradOLen > Environment::getInstance()->elementwiseThreshold()) schedule(guided)
        for(Nd4jLong i=0;  i<gradOLen; ++i) {
            auto idx = shape::subArrayIndex(i, gradO.getShapeInfo(), gradI.getShapeInfo());
            gradI.p(idx, gradI.e<T>(idx) + gradOBuff[i * gradOEWS]);
        }
    }
    else {
        
#pragma omp parallel for simd if(gradOLen > Environment::getInstance()->elementwiseThreshold()) schedule(guided)
        for(Nd4jLong i=0;  i<gradOLen; ++i) {

            auto fidx = shape::subArrayIndex(i, gradO.getShapeInfo(), gradI.getShapeInfo());
            gradI.p(fidx, gradI.e<T>(fidx) + gradOBuff[shape::getIndexOffset(i, gradO.getShapeInfo(), gradOLen)]);
        }
    }
}

    void tileBP(const NDArray& gradO /*input*/, NDArray& gradI /*output*/, const std::vector<Nd4jLong> reps) {
        BUILD_SINGLE_SELECTOR(gradI.dataType(), tileBP_, (gradO, gradI, reps), FLOAT_TYPES);
    }


    BUILD_SINGLE_TEMPLATE(template void tileBP_, (const NDArray& gradO /*input*/, NDArray& gradI /*output*/, const std::vector<Nd4jLong> reps), FLOAT_TYPES);

}
}
}
