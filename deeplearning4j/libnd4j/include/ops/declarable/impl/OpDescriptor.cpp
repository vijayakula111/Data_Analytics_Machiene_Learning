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
// Created by raver119 on 13.10.2017.
//

#include <ops/declarable/OpDescriptor.h>

namespace nd4j {
    namespace ops {

        OpDescriptor::OpDescriptor(const char * opName, bool isLogic) {
            _logic = isLogic;
            _opName = opName;
        }

        OpDescriptor::OpDescriptor(int numInputs, const char * opName, bool isScalar) {
            _numInputs = numInputs;
            _numOutputs = 1;

            _opName = opName;
            _hash = nd4j::ops::HashHelper::getInstance()->getLongHash(_opName);
            _opClass = nd4j::graph::OpClass_CONDITIONAL;

            _scalar = isScalar;
        }

        OpDescriptor::OpDescriptor(int numInputs, std::string opName, bool isScalar) {
            _numInputs = numInputs;
            _numOutputs = 1;

            _opName = opName;
            _hash = nd4j::ops::HashHelper::getInstance()->getLongHash(_opName);
            _opClass = nd4j::graph::OpClass_CONDITIONAL;

            _scalar = isScalar;
        }


        bool OpDescriptor::operator==(const OpDescriptor& other) const {
            if (_hash == -1 && other._hash == -1)
                return this->_opNum == other._opNum;
            else
                return this->_hash == other._hash;
        }

        OpDescriptor::OpDescriptor(int numInputs, int numOutputs, std::string opName, bool allowsInplace) : OpDescriptor::OpDescriptor(numInputs, numOutputs, opName.c_str(), allowsInplace) {
            //
        }

        void OpDescriptor::setHash(Nd4jLong hash) {
            _hash = hash;
        }

        // default constructor
        OpDescriptor::OpDescriptor(int numInputs, int numOutputs, const char *opName, bool allowsInplace) {
            _numInputs = numInputs;
            _numOutputs = numOutputs;

            std::string tmp(opName);
            _opName = tmp;
            _allowsInplace = allowsInplace;
            _hash = nd4j::ops::HashHelper::getInstance()->getLongHash(tmp);
            _divergent = false;

            // just default value
            _opClass = nd4j::graph::OpClass_TRANSFORM;
        }

        // constructor for configurable op
        OpDescriptor::OpDescriptor(int numInputs, int numOutputs, const char *opName, bool allowsInplace, int tArgs, int iArgs) : OpDescriptor::OpDescriptor(numInputs, numOutputs, opName, allowsInplace) {
            _tArgs = tArgs;
            _iArgs = iArgs;
        }

        // constructor for non-configurable divergent op
        OpDescriptor::OpDescriptor(int numInputs, int numOutputs, std::string opName, bool allowsInplace, bool divergent) : OpDescriptor::OpDescriptor(numInputs, numOutputs, opName.c_str(), allowsInplace, divergent) {

        }

        // constructor for non-configurable divergent op
        OpDescriptor::OpDescriptor(int numInputs, int numOutputs, const char *opName, bool allowsInplace, bool divergent) : OpDescriptor::OpDescriptor(numInputs, numOutputs, opName, allowsInplace) {
            _divergent = divergent;
        }

        // constructor for configurable divergent op
        OpDescriptor::OpDescriptor(int numInputs, int numOutputs, const char *opName, bool allowsInplace, bool divergent, int tArgs, int iArgs) : OpDescriptor(numInputs, numOutputs, opName, allowsInplace, tArgs, iArgs) {
            _divergent = divergent;
        }

        // default destructor
        OpDescriptor::~OpDescriptor() {
            //
        }

        int OpDescriptor::getNumberOfTArgs() {
            return _tArgs;
        }

        int OpDescriptor::getNumberOfIArgs() {
            return _iArgs;
        }

        int OpDescriptor::getNumberOfInputs() {
            return _numInputs;
        }

        Nd4jLong OpDescriptor::getHash() {
            return _hash;
        }

        int OpDescriptor::getNumberOfOutputs() {
            return _numOutputs;
        }

        std::string * OpDescriptor::getOpName() {
            return &_opName;
        }

        bool OpDescriptor::isDivergent() {
            return _divergent;
        }

        void OpDescriptor::setOpNum(int opNum) {
            _opNum = opNum;
        }

        bool OpDescriptor::allowsInplace() {
            return _allowsInplace;
        }

        int OpDescriptor::getOpNum() {
            return _opNum;
        }

        OpDescriptor* OpDescriptor::setInputType(const InputType type) {
            _inputType = type;
            return this;
        }

        InputType OpDescriptor::inputType() {
            return _inputType;
        }

        OpDescriptor* OpDescriptor::setAllowedInputTypes(const std::initializer_list<nd4j::DataType> &dtypes) {
            _allowedIns = dtypes;
            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedOutputTypes(const std::initializer_list<nd4j::DataType> &dtypes) {
            _allowedOuts = dtypes;
            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedInputTypes(const nd4j::DataType dtype) {
            _allowedIns.clear();
            _allowedIns.emplace_back(dtype);
            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedOutputTypes(const nd4j::DataType dtype) {
            _allowedOuts.clear();
            _allowedOuts.emplace_back(dtype);
            return this;
        }

        OpDescriptor* OpDescriptor::setInputType(const int idx, const nd4j::DataType dtype) {
            _inputTypes[idx] = { dtype };
            return this;
        }

        OpDescriptor* OpDescriptor::setOutputType(const int idx, const nd4j::DataType dtype) {
            _outputTypes[idx] = { dtype };
            return this;
        }

        OpDescriptor* OpDescriptor::setSameMode(const bool reallySame) {
            _sameMode = reallySame;
            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedInputTypes(int index, const std::vector<nd4j::DataType> &dtype) {
            _inputTypes[index] = dtype;
            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedOutputTypes(int index, const std::vector<nd4j::DataType> &dtype) {
            _outputTypes[index] = dtype;
            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedInputTypes(int index,  nd4j::DataType dtype) {
            if (_inputTypes.count(index) == 0)
                _inputTypes[index] = {dtype};
            else
                _inputTypes[index].emplace_back(dtype);

            return this;
        }

        OpDescriptor* OpDescriptor::setAllowedOutputTypes(int index, nd4j::DataType dtype) {
            if (_outputTypes.count(index) == 0)
                _outputTypes[index] = {dtype};
            else
                _outputTypes[index].emplace_back(dtype);

            return this;
        }

        bool OpDescriptor::checkDataTypesMatch(nd4j::DataType needle, std::vector<nd4j::DataType> &haystack) const {
            // if haystack is empty - INHERIT is occurs - any type is perfect?
            if (haystack.empty())
                return true;

            // first we're checking for direct input type match
            if (std::find(haystack.begin(), haystack.end(), needle) == haystack.end()) {

                // if direct input match failed - we're checking for ANY as allowed input
                if (std::find(haystack.begin(), haystack.end(), nd4j::DataType::ANY) == haystack.end())
                    return false;
                else
                    return true;
            } else {
                return true;
            }
        }

        bool OpDescriptor::checkInputMatch(int index, nd4j::DataType dataType) {
            // we check for per-input types first
            if (_inputTypes.empty() || _inputTypes.count(index) == 0) {
                // checking global input types
                return checkDataTypesMatch(dataType, _allowedIns);
            } else {
                // checking data type for specified input
                auto allowed = _inputTypes[index];
                return checkDataTypesMatch(dataType, allowed);
            }
            return true;
        }

        bool OpDescriptor::checkOutputMatch(int index, nd4j::DataType dataType) {
            // we check for per-output types first
            if (_outputTypes.empty() || _outputTypes.count(index) == 0) {

                // checking global output types
                return checkDataTypesMatch(dataType, _allowedOuts);
            } else {
                // checking data type for specified output
                auto allowed = _outputTypes[index];
                return checkDataTypesMatch(dataType, allowed);
            }
            return true;
        }

        bool OpDescriptor::isSameMode() {
            return _sameMode;
        }

        bool OpDescriptor::isInherit(int index) {
            if (std::find(_allowedOuts.begin(), _allowedOuts.end(), nd4j::DataType::INHERIT) != _allowedOuts.end())
                return true;
            if (_outputTypes.count(index) > 0) {
                auto vec = _outputTypes[index];

                if (std::find(vec.begin(), vec.end(), nd4j::DataType::INHERIT) != vec.end())
                    return true;
            }

            return false;
        }

        std::vector<nd4j::DataType> OpDescriptor::getOutputTypesForOutput(int index) {
            if (_outputTypes.count(index) > 0)
                return _outputTypes.at(index);
            else
                return std::vector<nd4j::DataType>();
        }
    }
}