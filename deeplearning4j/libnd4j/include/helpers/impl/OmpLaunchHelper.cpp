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
// @author raver119@gmail.com, created on 6/30/2018
// @author Yurii Shyrma (iuriish@yahoo.com)
//

#include <OmpLaunchHelper.h>
#include <Environment.h>
#include <templatemath.h>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace nd4j {


////////////////////////////////////////////////////////////////////////////////
OmpLaunchHelper::OmpLaunchHelper(const Nd4jLong N, float desiredNumThreads) {            

    auto maxItersPerThread = Environment::getInstance()->elementwiseThreshold();    
        
    if(N < maxItersPerThread)
        _numThreads = 1;
    else {
        #ifdef _OPENMP
            if(desiredNumThreads == -1)
                desiredNumThreads = omp_get_max_threads();
            else if(desiredNumThreads < 1) 
                desiredNumThreads = 1;
            else
                desiredNumThreads = nd4j::math::nd4j_min<int>(omp_get_max_threads(), desiredNumThreads);
        #else
            desiredNumThreads = 1;
        #endif
        _numThreads = nd4j::math::nd4j_min<int>(N / maxItersPerThread, desiredNumThreads);        
    }

    _itersPerThread = N / _numThreads;
    _remainder = N % _numThreads;  // last thread may contain bigger number of iterations    
}


Nd4jLong OmpLaunchHelper::betterSpan(Nd4jLong N) {
        return OmpLaunchHelper::betterSpan(N, OmpLaunchHelper::betterThreads(N));
    }

    Nd4jLong OmpLaunchHelper::betterSpan(Nd4jLong N, Nd4jLong numThreads) {
        auto r = N % numThreads;
        auto t = N / numThreads;
        
        if (r == 0)
            return t;
        else {
            // breaks alignment
            return t + 1;
        }
    }

    int OmpLaunchHelper::betterThreads(Nd4jLong N) {
        #ifdef _OPENMP
            return betterThreads(N, omp_get_max_threads());
        #else
            return 1;
        #endif
    }

    int OmpLaunchHelper::betterThreads(Nd4jLong N, int maxThreads) {
        auto t = Environment::getInstance()->elementwiseThreshold();
        if (N < t)
            return 1;
        else {
            return static_cast<int>(nd4j::math::nd4j_min<Nd4jLong>(N / t, maxThreads));
        }
    }

}
