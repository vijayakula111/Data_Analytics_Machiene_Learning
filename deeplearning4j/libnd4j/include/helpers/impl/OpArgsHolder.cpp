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

#include <OpArgsHolder.h>


namespace nd4j {

////////////////////////////////////////////////////////////////////////
OpArgsHolder OpArgsHolder::createArgsHolderForBP(const std::vector<NDArray*>& inGradArrs, const bool isInPlace) const {
	
	const int numInGradArrs = inGradArrs.size();

	OpArgsHolder result(std::vector<NDArray*>(_numInArrs + numInGradArrs, nullptr), _tArgs, _iArgs);
	
	if(isInPlace)
		result._isArrAlloc = std::vector<bool>(_numInArrs + numInGradArrs, false);

	for (int i = 0; i < _numInArrs; ++i) {
		
		if(isInPlace) {			
			result._inArrs[i] = new NDArray(*_inArrs[i]);		// make copy
			result._isArrAlloc[i] = true;
		}
		else 
			result._inArrs[i] = _inArrs[i];	
	}

	// input gradients 
	for (int i = 0; i < numInGradArrs; ++i)
		result._inArrs[_numInArrs + i] = inGradArrs[i];

	return result;
}

////////////////////////////////////////////////////////////////////////
// default destructor
OpArgsHolder::~OpArgsHolder() noexcept {
	
	for (int i = 0; i < _isArrAlloc.size(); ++i)
		if(_isArrAlloc[i])
			delete _inArrs[i];
        
}

}


