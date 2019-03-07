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

package org.nd4j.linalg.api.buffer;


import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.indexer.Indexer;
import org.nd4j.linalg.api.memory.MemoryWorkspace;

import java.nio.ByteBuffer;

/**
 * Data buffer for floats
 *
 * @author Adam Gibson
 */
public class Int16Buffer extends BaseDataBuffer {

    /**
     * Meant for creating another view of a buffer
     *
     * @param pointer the underlying buffer to create a view from
     * @param indexer the indexer for the pointer
     * @param length  the length of the view
     */
    public Int16Buffer(Pointer pointer, Indexer indexer, long length) {
        super(pointer, indexer, length);
    }

    /**
     * Create a float buffer with the given length
     * @param length the float buffer with the given length
     */
    public Int16Buffer(long length) {
        super(length);

    }

    public Int16Buffer(long length, boolean initialize) {
        super(length, initialize);
    }

    public Int16Buffer(long length, boolean initialize, MemoryWorkspace workspace) {
        super(length, initialize, workspace);
    }

    public Int16Buffer(int length, int elementSize) {
        super(length, elementSize);
    }

    public Int16Buffer(int length, int elementSize, long offset) {
        super(length, elementSize, offset);
    }

    /**
     * Initialize the opType of this buffer
     */
    @Override
    protected void initTypeAndSize() {
        type = DataType.SHORT;
        elementSize = 2;
    }

    public Int16Buffer(DataBuffer underlyingBuffer, long length, long offset) {
        super(underlyingBuffer, length, offset);
    }

    public Int16Buffer(float[] data) {
        this(data, true);
    }

    public Int16Buffer(float[] data, MemoryWorkspace workspace) {
        this(data, true, workspace);
    }

    public Int16Buffer(int[] data) {
        this(data, true);
    }

    public Int16Buffer(double[] data) {
        this(data, true);
    }

    public Int16Buffer(int[] data, boolean copyOnOps) {
        super(data, copyOnOps);
    }

    public Int16Buffer(int[] data, boolean copy, long offset) {
        super(data, copy, offset);
    }

    public Int16Buffer(double[] data, boolean copyOnOps) {
        super(data, copyOnOps);
    }

    public Int16Buffer(double[] data, boolean copy, long offset) {
        super(data, copy, offset);
    }

    public Int16Buffer(ByteBuffer buffer, int length) {
        super(buffer, length);
    }

    public Int16Buffer(ByteBuffer buffer, int length, long offset) {
        super(buffer, length, offset);
    }

    public Int16Buffer(byte[] data, int length) {
        super(data, length);
    }

    public Int16Buffer(float[] floats, boolean copy) {
        super(floats, copy);
    }

    public Int16Buffer(float[] floats, boolean copy, MemoryWorkspace workspace) {
        super(floats, copy, workspace);
    }

    public Int16Buffer(float[] data, boolean copy, long offset) {
        super(data, copy, offset);
    }

    public Int16Buffer(float[] data, boolean copy, long offset, MemoryWorkspace workspace) {
        super(data, copy, offset, workspace);
    }

    @Override
    protected DataBuffer create(long length) {
        return new Int16Buffer(length);
    }


    @Override
    public DataBuffer create(double[] data) {
        return new Int16Buffer(data);
    }

    @Override
    public DataBuffer create(float[] data) {
        return new Int16Buffer(data);
    }

    @Override
    public DataBuffer create(int[] data) {
        return new Int16Buffer(data);
    }


}
