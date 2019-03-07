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
//  @author raver119@gmail.com
//

#include <ops/declarable/helpers/adjust_hue.h>

namespace nd4j {
namespace ops {
namespace helpers {

    template <typename T>
    static void _adjust_hue_single(NDArray *array, NDArray *output, float delta, bool isNHWC) {
        // we're 100% sure it's 3
        const int numChannels = 3;
        int tuples = array->lengthOf() /  numChannels;
        auto bIn = reinterpret_cast<T *>(array->buffer());
        auto bOut = reinterpret_cast<T *>(output->buffer());
        static const int kChannelRange = 6;

        int stridesDim = isNHWC ? 2 : 0;
        if (isNHWC) {
            // for NHWC our rgb values are stored one by one
            #pragma omp parallel for simd
            for (int e = 0; e < tuples; e++) {
                auto i = bIn + e * numChannels;
                auto o = bOut + e * numChannels;

                T h, v_min, v_max;
                helpers::rgb_to_hv(i[0], i[1], i[2], &h, &v_min, &v_max);

                h += delta * kChannelRange;
                while (h < (T) 0.)
                    h += (T) kChannelRange;
              
                while (h >= (T) kChannelRange)
                    h -= (T) kChannelRange;

                helpers::hv_to_rgb(h, v_min, v_max, o, o + 1, o + 2);
            }
        } else {
            auto tadsChannelsIn  = array->allTensorsAlongDimension({0});
            auto tadsChannelsOut = output->allTensorsAlongDimension( {0});

            auto bufferR = reinterpret_cast<T *>(tadsChannelsIn->at(0)->buffer());
            auto bufferG = reinterpret_cast<T *>(tadsChannelsIn->at(1)->buffer());
            auto bufferB = reinterpret_cast<T *>(tadsChannelsIn->at(2)->buffer());

            auto outputR = reinterpret_cast<T *>(tadsChannelsOut->at(0)->buffer());
            auto outputG = reinterpret_cast<T *>(tadsChannelsOut->at(1)->buffer());
            auto outputB = reinterpret_cast<T *>(tadsChannelsOut->at(2)->buffer());

            #pragma omp parallel for simd 
            for (int e = 0; e < tuples; e++) {
                auto _ri = bufferR + e;
                auto _gi = bufferG + e;
                auto _bi = bufferB + e;

                auto _ro = outputR + e;
                auto _go = outputG + e;
                auto _bo = outputB + e;

                T h, v_min, v_max;
                helpers::rgb_to_hv(_ri[0], _gi[0], _bi[0], &h, &v_min, &v_max);

                h += delta * kChannelRange;
                while (h < (T) 0)
                    h += (T) kChannelRange;
              
                while (h >= (T) kChannelRange)
                    h -= (T) kChannelRange;

                helpers::hv_to_rgb(h, v_min, v_max, _ro, _go, _bo);
            }

            delete tadsChannelsIn;
            delete tadsChannelsOut;
        }
    }

    void _adjust_hue(NDArray *array, NDArray *output, NDArray* delta, bool isNHWC) {
        auto xType = array->dataType();

        float d = delta->e<float>(0);
        if (array->rankOf() == 4) {
            auto tadsIn = array->allTensorsAlongDimension({0});
            auto tadsOut = output->allTensorsAlongDimension({0});

            // FIXME: template selector should be moved out of loop
#pragma omp parallel for
            for (int e = 0; e < tadsIn->size(); e++) {
                BUILD_SINGLE_SELECTOR(xType, _adjust_hue_single, (tadsIn->at(e), tadsOut->at(e), d, isNHWC);, FLOAT_TYPES);
            }
            

            delete tadsIn;
            delete tadsOut;
        } else {
            BUILD_SINGLE_SELECTOR(xType, _adjust_hue_single, (array, output, d, isNHWC);, FLOAT_TYPES);
        }
    }

    BUILD_SINGLE_TEMPLATE(template void _adjust_hue_single, (NDArray *array, NDArray *output, float delta, bool isNHWC);, FLOAT_TYPES);

}
}
}