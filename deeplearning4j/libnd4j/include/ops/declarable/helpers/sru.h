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
// @author Yurii Shyrma (iuriish@yahoo.com), created by on 06.04.2018
//

#ifndef LIBND4J_SRU_H
#define LIBND4J_SRU_H

#include <ops/declarable/helpers/helpers.h>

namespace nd4j    {
namespace ops     {
namespace helpers {


	void sruCell(const NDArray* x, const NDArray* c0, const NDArray* w, const NDArray* b, NDArray* h, NDArray* c);

	void sruTimeLoop(const NDArray* x, const NDArray* c0, const NDArray* w, const NDArray* b, NDArray* h, NDArray* c);

	void sruBI(NDArray* x, const NDArray* w, const NDArray* b, const NDArray* c0, const NDArray* mask, NDArray* ht, NDArray* ct);

	void sruBIBP(NDArray* x, const NDArray* w, const NDArray* b, const NDArray* c0, const NDArray* ct, const NDArray* inGradC0, const NDArray* inGradH, const NDArray* mask,    
                 NDArray* gradI, NDArray* gradWeights, NDArray* gradB, NDArray* gradC0);
}
}
}


#endif //LIBND4J_SRU_H


    