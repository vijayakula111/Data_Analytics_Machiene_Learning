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

package org.nd4j.linalg.api.ops.impl.layers.convolution.config;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.LinkedHashMap;
import java.util.Map;

@Builder
@Data
@AllArgsConstructor
@NoArgsConstructor
public class DeConv3DConfig extends BaseConvolutionConfig {
    public static final String NCDHW = "NCDHW";
    public static final String NDHWC = "NDHWC";

    @Builder.Default private long kD = -1L;
    @Builder.Default private long kH = -1L;
    @Builder.Default private long kW = -1L;
    @Builder.Default private long sD = 1L;
    @Builder.Default private long sH = 1L;
    @Builder.Default private long sW = 1L;
    @Builder.Default private long pD = 0;
    @Builder.Default private long pH = 0;
    @Builder.Default private long pW = 0;
    @Builder.Default private long dD = 1L;
    @Builder.Default private long dH = 1L;
    @Builder.Default private long dW = 1L;
    @Builder.Default private boolean isSameMode = false;
    @Builder.Default private String dataFormat = NCDHW;


    public Map<String, Object> toProperties() {
        Map<String, Object> ret = new LinkedHashMap<>();
        ret.put("kD", kD);
        ret.put("kH", kH);
        ret.put("kW", kW);
        ret.put("sD", sD);
        ret.put("sH", sH);
        ret.put("sW", sW);
        ret.put("pD", pD);
        ret.put("pH", pH);
        ret.put("pW", pW);
        ret.put("dD", dD);
        ret.put("dH", dH);
        ret.put("dW", dW);
        ret.put("isSameMode", isSameMode);
        ret.put("dataFormat", dataFormat);
        return ret;
    }
}
