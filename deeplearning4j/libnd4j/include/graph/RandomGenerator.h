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
//  @author raver119@protonmail.com
//

#ifndef LIBND4J_GRAPH_RNG_H
#define LIBND4J_GRAPH_RNG_H

#include <types/u64.h>
#include <pointercast.h>
#include <op_boilerplate.h>
#include <dll.h>
#include <chrono>
#include <array/DataTypeUtils.h>
#include <helpers/logger.h>

#ifdef __CUDACC__
#include <cuda.h>
#include <cuda_runtime.h>
#endif

namespace nd4j {
    namespace graph {
#ifdef __CUDACC__
        class ND4J_EXPORT CudaManagedRandomGenerator {
        private:

        protected:
            void *devHolder;

        public:
            void *operator new(size_t len) {
                void *ptr;
                cudaHostAlloc(&ptr, len, cudaHostAllocDefault);
                return ptr;
             }

            void operator delete(void *ptr) {
                cudaFreeHost(ptr);
            }
        };

        class ND4J_EXPORT RandomGenerator : public CudaManagedRandomGenerator {
#else
        class ND4J_EXPORT RandomGenerator {
#endif
        private:
#ifndef __CUDACC__
            void *placeHolder;
#endif
            // GRAPH-LEVEL STATE
            u64 _rootState;

            // NODE-LEVEL STATE
            u64 _nodeState;

            /**
             * Utility method, returns number of milliseconds since 1970
             */
            FORCEINLINE Nd4jLong currentMilliseconds();


            FORCEINLINE _CUDA_HD uint32_t xoroshiro32(Nd4jLong index);
            FORCEINLINE _CUDA_HD uint64_t xoroshiro64(Nd4jLong index);

            /**
             * This method returns integer value between 0 and MAX_UINT
             */
            //uint32_t relativeUInt32(Nd4jLong index);

        public:
            FORCEINLINE RandomGenerator(Nd4jLong rootSeed = 0, Nd4jLong nodeSeed = 0);

            /**
             * This method allows to change graph-level state in runtime.
             * PLEASE NOTE: this method will change state of node as well.
             */
            FORCEINLINE _CUDA_H void setStates(Nd4jLong rootSeed, Nd4jLong nodeState = 0);

            

            /**
             * This method returns T value between from and to
             */
            template <typename T>
            FORCEINLINE _CUDA_HD T relativeT(Nd4jLong index, T from, T to);

            /**
             * This method returns T value between 0 and MAX_T
             */
            template <typename T>
            FORCEINLINE _CUDA_HD T relativeT(Nd4jLong index);

            /**
             * These two methods are made for JVM
             * @param index
             * @return
             */
            FORCEINLINE _CUDA_HD int relativeInt(Nd4jLong index);
            FORCEINLINE _CUDA_HD Nd4jLong relativeLong(Nd4jLong index);

            FORCEINLINE _CUDA_HD void rewindH(Nd4jLong steps);

            /**
             * These methods set up only node states, with non-changed root ones
             */
            FORCEINLINE _CUDA_H void setSeed(int seed) {
                _nodeState._ulong = static_cast<uint64_t>(seed);
            }

            FORCEINLINE _CUDA_H void setSeed(uint64_t seed) {
                _nodeState._ulong = seed;
            }

            FORCEINLINE _CUDA_HD Nd4jLong rootState() {
                return _rootState._long;
            }

            FORCEINLINE _CUDA_HD Nd4jLong nodeState() {
                return _nodeState._long;
            }
        };


        FORCEINLINE RandomGenerator::RandomGenerator(Nd4jLong rootSeed, Nd4jLong nodeSeed) {
            // this seed is used graph-level state
            if (rootSeed == 0)
                rootSeed = currentMilliseconds();

            // graph-level state is just first seed
            _rootState._long = rootSeed;

            // used to build second, node state
            _nodeState._long = (nodeSeed != 0 ? nodeSeed: 1298567341LL);
        }

        FORCEINLINE void RandomGenerator::setStates(Nd4jLong rootSeed, Nd4jLong nodeSeed) {
            // this seed is used graph-level state
            if (rootSeed == 0)
                rootSeed = currentMilliseconds();

            // graph-level state is just first seed
            _rootState._long = rootSeed;

            // used to build second, node state
            _nodeState._long = (nodeSeed != 0 ? nodeSeed: 1298567341LL);
        }


        FORCEINLINE Nd4jLong RandomGenerator::currentMilliseconds() {
            auto s = std::chrono::system_clock::now().time_since_epoch();
            auto v = std::chrono::duration_cast<std::chrono::milliseconds>(s).count();
            return v;
        }

        template <>
        _CUDA_HD FORCEINLINE uint64_t RandomGenerator::relativeT<uint64_t>(Nd4jLong index) {
            return this->xoroshiro64(index);
        }

        template <>
        _CUDA_HD FORCEINLINE uint32_t RandomGenerator::relativeT<uint32_t>(Nd4jLong index) {
            return this->xoroshiro32(index);
        }

        template <>
        _CUDA_HD FORCEINLINE int RandomGenerator::relativeT<int>(Nd4jLong index) {
            auto x = this->relativeT<uint32_t>(index);
            auto r = static_cast<int>(x % DataTypeUtils::max<int>());
            return r;
        }

        template <>
        _CUDA_HD FORCEINLINE Nd4jLong RandomGenerator::relativeT<Nd4jLong>(Nd4jLong index) {
            auto x = this->relativeT<uint64_t>(index);
            auto r = static_cast<Nd4jLong>(x % DataTypeUtils::max<Nd4jLong>());
            return r;
        }

        template <typename T>
        _CUDA_HD FORCEINLINE T RandomGenerator::relativeT(Nd4jLong index, T from, T to) {
            auto t = this->relativeT<T>(index);
            auto z = from + (t * (to - from));
            return z;
        }

        template <typename T>
        _CUDA_HD FORCEINLINE T RandomGenerator::relativeT(Nd4jLong index) {
            // This is default implementation for floating point types
#ifdef __DOUBLE_RNG__
            auto i = static_cast<double>(this->relativeT<uint64_t>(index));
            auto r = i / static_cast<double>(DataTypeUtils::max<uint64_t>());
            return static_cast<T>(r);
#else
            auto i = static_cast<float>(this->relativeT<uint32_t>(index));
            auto r = i / static_cast<float>(DataTypeUtils::max<uint32_t>());
            return static_cast<T>(r);
#endif
        }


        _CUDA_HD FORCEINLINE int RandomGenerator::relativeInt(Nd4jLong index) {
            return relativeT<int>(index);
        }

        _CUDA_HD FORCEINLINE Nd4jLong RandomGenerator::relativeLong(Nd4jLong index) {
            return relativeT<Nd4jLong>(index);
        }

        //////
        static FORCEINLINE _CUDA_HD uint32_t rotl(const uint32_t x, int k) {
            return (x << k) | (x >> (32 - k));
        }

        static FORCEINLINE _CUDA_HD  uint64_t rotl(const uint64_t x, int k) {
            return (x << k) | (x >> (64 - k));
        }

        _CUDA_HD FORCEINLINE uint32_t RandomGenerator::xoroshiro32(Nd4jLong index) {
            auto s0 = _rootState._ulong;
            auto s1 = _nodeState._ulong;

            // xor by idx
            s0 |= ((index + 2) * (s1 + 24243287));
            s1 ^= ((index + 2) * (s0 + 723829));

            u64 v;

            v._ulong = s1 ^ s0;

            return rotl(v._du32._v0 * 0x9E3779BB, 5) * 5;
        }

        _CUDA_HD FORCEINLINE uint64_t RandomGenerator::xoroshiro64(Nd4jLong index) {
            auto s0 = _rootState._ulong;
            auto s1 = _nodeState._ulong;

            // xor by idx
            s0 |= ((index + 2) * (s1 + 24243287));
            s1 ^= ((index + 2) * (s0 + 723829));

            // since we're not modifying state - do rotl step right here
            s1 ^= s0;
            s0 = rotl(s0, 55) ^ s1 ^ (s1 << 14);
            s1 = rotl(s1, 36);

            return s0 + s1;
        }

        _CUDA_HD FORCEINLINE void RandomGenerator::rewindH(Nd4jLong steps) {
            auto s0 = _nodeState._du32._v0;
            auto s1 = _nodeState._du32._v1;

            s1 ^= s0;
            _nodeState._du32._v0 = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
            _nodeState._du32._v1 = rotl(s1, 13); // c

            _nodeState._long ^= (steps ^ 0xdeadbeef);
        }
    }
}

#endif