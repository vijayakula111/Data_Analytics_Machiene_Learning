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
// This class implements Workspace functionality in c++
//
//
// @author raver119@gmail.com
//

#ifndef LIBND4J_WORKSPACE_H
#define LIBND4J_WORKSPACE_H

#include <atomic>
#include <vector>
#include <mutex>
#include <dll.h>
#include <pointercast.h>
#include <types/float16.h>
#include <memory/ExternalWorkspace.h>

namespace nd4j {
    namespace memory {

//        void ping();

        enum MemoryType {
            HOST,
            DEVICE,
        };

        class ND4J_EXPORT Workspace {
        protected:
            char* _ptrHost = nullptr;
            char* _ptrDevice = nullptr;

            bool _allocatedHost = false;
            bool _allocatedDevice = false;

            std::atomic<Nd4jLong> _offset;

            Nd4jLong _initialSize = 0L;
            Nd4jLong _currentSize = 0L;

            std::mutex _mutexAllocation;
            std::mutex _mutexSpills;

            bool _externalized = false;

            std::vector<void*> _spills;

            std::atomic<Nd4jLong> _spillsSize;
            std::atomic<Nd4jLong> _cycleAllocations;

            void init(Nd4jLong bytes);
            void freeSpills();
        public:
            explicit Workspace(ExternalWorkspace *external);
            explicit Workspace(Nd4jLong initialSize = 0);
            ~Workspace();

            Nd4jLong getAllocatedSize();
            Nd4jLong getCurrentSize();
            Nd4jLong getCurrentOffset();
            Nd4jLong getSpilledSize();
            Nd4jLong getUsedSize();

            void expandBy(Nd4jLong numBytes);
            void expandTo(Nd4jLong numBytes);

//            bool resizeSupported();

            void* allocateBytes(Nd4jLong numBytes);
            void* allocateBytes(MemoryType type, Nd4jLong numBytes);

            void scopeIn();
            void scopeOut();

            /*
             * This method creates NEW workspace of the same memory size and returns pointer to it
             */
            Workspace* clone();
        };
    }
}

#endif //LIBND4J_WORKSPACE_H
