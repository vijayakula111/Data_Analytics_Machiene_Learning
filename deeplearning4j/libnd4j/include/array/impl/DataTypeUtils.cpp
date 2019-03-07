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

#include <array/DataType.h>
#include <array/DataTypeUtils.h>
#include <types/float16.h>

namespace nd4j {
    DataType DataTypeUtils::fromInt(int val) {
        return (DataType) val;
    }

    DataType DataTypeUtils::fromFlatDataType(nd4j::graph::DataType dtype) {
        return (DataType) dtype;
    }

    int DataTypeUtils::asInt(DataType type) {
        return (int) type;
    }
}