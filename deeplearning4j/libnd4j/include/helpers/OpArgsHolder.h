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
// @author Yurii Shyrma (iuriish@yahoo.com), created on 15.07.2018
//

#ifndef LIBND4J_OPARGSHOLDER_H
#define LIBND4J_OPARGSHOLDER_H


#include <NDArray.h>
#include <dll.h>

namespace nd4j {
 
class ND4J_EXPORT OpArgsHolder {

private: 
	std::vector<NDArray*> _inArrs = std::vector<NDArray*>();
    std::vector<double>           _tArgs  = std::vector<double>();
    std::vector<Nd4jLong>    _iArgs  = std::vector<Nd4jLong>();
    std::vector<bool>        _bArgs  = std::vector<bool>();

    int _numInArrs = _inArrs.size();
    int _numTArgs  = _tArgs.size();
    int _numIArgs  = _iArgs.size();
    int _numBArgs  = _bArgs.size();
    std::vector<bool> _isArrAlloc = std::vector<bool>();

public:

	OpArgsHolder() = delete;

    OpArgsHolder(const std::vector<NDArray*>& inArrs, const std::vector<double>& tArgs = std::vector<double>(), const std::vector<Nd4jLong>& iArgs = std::vector<Nd4jLong>(), const std::vector<bool>& bArgs = std::vector<bool>())
    			: _inArrs(inArrs), _tArgs(tArgs), _iArgs(iArgs), _bArgs(bArgs) { }

    const std::vector<NDArray*>& getInArrs() const
    {return _inArrs; }

    const std::vector<double>& getTArgs() const
    {return _tArgs; }

    const std::vector<Nd4jLong>& getIArgs() const
    {return _iArgs; }

    const std::vector<bool>& getBArgs() const
    {return _bArgs; }

    const std::vector<bool>& getAllocInfo() const
    {return _isArrAlloc; }

    int getNumInArrs() const
    {return _numInArrs; }

    int getNumTArgs() const
    {return _numTArgs; }

    int getNumIArgs() const
    {return _numIArgs; }

    int getNumBArgs() const
    {return _numBArgs; }

    OpArgsHolder createArgsHolderForBP(const std::vector<NDArray*>& inGradArrs, const bool isInPlace = false) const;

    ~OpArgsHolder() noexcept; 
    
};





}

#endif //LIBND4J_OPARGSHOLDER_H
