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
// @author Yurii Shyrma, created on 16.04.2017
//

#ifndef LIBND4J_RNN_H
#define LIBND4J_RNN_H

#include <ops/declarable/helpers/helpers.h>

namespace nd4j    {
namespace ops     {
namespace helpers {


	void rnnCell(const NDArray* xt, const NDArray* Wx, const NDArray* Wh, const NDArray* b, const NDArray* ht_1, NDArray* ht);

	void rnnTimeLoop(const NDArray* x, const NDArray* Wx, const NDArray* Wh, const NDArray* b, const NDArray* h0, const NDArray* maxTimeStep, NDArray* h, NDArray* hFinal);

}
}
}


#endif //LIBND4J_RNN_H
