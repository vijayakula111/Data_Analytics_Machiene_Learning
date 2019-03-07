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

package jcuda.jcublas.ops;

import org.bytedeco.javacpp.FloatPointer;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.nd4j.jita.allocator.impl.AtomicAllocator;
import org.nd4j.jita.conf.Configuration;
import org.nd4j.jita.conf.CudaEnvironment;
import org.nd4j.linalg.api.buffer.DataBuffer;
import org.nd4j.linalg.api.buffer.util.DataTypeUtil;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.IndexAccumulation;
import org.nd4j.linalg.api.ops.impl.reduce.Sum;
import org.nd4j.linalg.api.ops.impl.reduce.distances.ManhattanDistance;
import org.nd4j.linalg.api.ops.impl.indexaccum.IMax;
import org.nd4j.linalg.api.ops.impl.transforms.ACos;
import org.nd4j.linalg.api.ops.impl.transforms.LogSoftMax;
import org.nd4j.linalg.api.ops.impl.transforms.OldSoftMax;
import org.nd4j.linalg.api.ops.impl.transforms.SoftMax;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.jcublas.context.CudaContext;
import org.nd4j.linalg.jcublas.ops.executioner.CudaGridExecutioner;
import org.nd4j.linalg.ops.transforms.Transforms;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.Arrays;

import static org.junit.Assert.*;

/**
 * Test suit for simple Half-precision execution
 *
 * @author raver119@gmail.com
 */
@Ignore
public class HalfOpsTests {
    private static Logger log = LoggerFactory.getLogger(HalfOpsTests.class);


    @Before
    public void setUp() {
        DataTypeUtil.setDTypeForContext(DataType.HALF);
        CudaEnvironment.getInstance().getConfiguration().enableDebug(true).setVerbose(true).setAllocationModel(Configuration.AllocationModel.CACHE_ALL);
    }

    @Test
    public void testScalarOp1() throws Exception {
        INDArray array = Nd4j.create(new float[]{1f, 2f, 3f, 4f, 5f});

        array.muli(2f);

        System.out.println("Array1: " + array);

        assertEquals(2f, array.getFloat(0), 0.1f);
        assertEquals(4f, array.getFloat(1), 0.1f);
        assertEquals(6f, array.getFloat(2), 0.1f);
        assertEquals(8f, array.getFloat(3), 0.1f);
        assertEquals(10f, array.getFloat(4), 0.1f);
    }


    @Test
    public void testHAxpy1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f});
        INDArray array2 = Nd4j.create(new float[]{1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f});

        long time1 = System.nanoTime();
        Nd4j.getBlasWrapper().axpy(new Float(0.75f), array1, array2);
        long time2 = System.nanoTime();
        System.out.println("AXPY execution time: [" + (time2 - time1) + "] ns");

        assertEquals(1.767578125, array2.getFloat(0), 0.00001);
        assertEquals(1.767578125, array2.getFloat(1), 0.00001);

        System.out.println("Array1: " + array2);

        assertEquals(1.01f, array1.getFloat(0), 0.001f);
        assertEquals(1.01f, array1.getFloat(1), 0.001f);

    }

    @Test
    public void testHDot1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f });
        INDArray array2 = Nd4j.create(new float[]{1.15f, 1.15f, 1.15f, 1.15f, 1.15f, 1.15f});


        double dotWrapped = 0;

        dotWrapped = Nd4j.getBlasWrapper().dot(array1, array2);

        assertEquals(6.968f, dotWrapped, 0.01f);
    }

    @Test
    public void testHGemm1() throws Exception {
        INDArray array1 = Nd4j.ones(10, 10);
        INDArray array2 = Nd4j.ones(10, 10);
        INDArray array3 = Nd4j.create(10, 10);

        array1.mmul(array2, array3);

        assertEquals(10.0f, array3.data().getFloat(0),0.001f);
        assertEquals(10.0f, array3.data().getFloat(1),0.001f);
        assertEquals(10.0f, array3.data().getFloat(10),0.001f);
        assertEquals(10.0f, array3.data().getFloat(11),0.001f);
        assertEquals(10.0f, array3.data().getFloat(20),0.001f);
        assertEquals(10.0f, array3.data().getFloat(21),0.001f);
    }

    @Test
    public void testHasum1() throws Exception {
        INDArray array1 = Nd4j.create(new float[] {1.0f, -1.0f, 1.0f, -1.0f, -2.0f, 2.0f, -2.0f});

        double sum = Nd4j.getBlasWrapper().asum(array1);

        assertEquals(10.0f, sum, 0.01f);
    }

    @Test
    public void testBroadcasts1() throws Exception {
        INDArray array1 = Nd4j.zeros(1500,150);
        INDArray array2 = Nd4j.linspace(1,150,150);

        AtomicAllocator.getInstance().getPointer(array1, (CudaContext) AtomicAllocator.getInstance().getDeviceContext().getContext());
        AtomicAllocator.getInstance().getPointer(array2, (CudaContext) AtomicAllocator.getInstance().getDeviceContext().getContext());

        long time1 = System.currentTimeMillis();
        array1.subiRowVector(array2);
        long time2 = System.currentTimeMillis();

        System.out.println("Execution time: " + (time2 - time1));

        //   System.out.println("Array1: " + array1);
//        System.out.println("Array2: " + array2);

        assertEquals(-1.0f, array1.getRow(0).getFloat(0), 0.01);
        assertEquals(-3.0f, array1.getRow(0).getFloat(2), 0.01);
        assertEquals(-10.0f, array1.getRow(0).getFloat(9), 0.01);
    }

    @Test
    public void testReduce1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{2.01f, 2.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f});

        Sum sum = new Sum(array1);
        Nd4j.getExecutioner().exec(sum, 1);

        Number resu = sum.getFinalResult();

        System.out.println("Result: " + resu);

        assertEquals(17.15f, resu.floatValue(), 0.01f);
    }

    @Test
    public void testCreation1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f});
    }

    @Test
    public void testIndexReduce1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f});

        int idx =  ((IndexAccumulation) Nd4j.getExecutioner().exec(new IMax(array1))).getFinalResult();

        assertEquals(3, idx);
    }

    @Test
    public void testTransform1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{0.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f});
        INDArray array2 = Nd4j.create(new float[]{1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f});

        Nd4j.getExecutioner().exec(new ACos(array1, array2));

        assertEquals(1.56f, array2.getFloat(0), 0.01);

        System.out.println("Array1: " + array1);
        System.out.println("Array2: " + array2);
    }

    @Test
    public void testSoftmax1()  throws Exception {
        INDArray array1 = Nd4j.zeros(15);
        array1.putScalar(0, 0.9f);

        Nd4j.getExecutioner().exec(new OldSoftMax(array1));

        System.out.println("Array1: " + array1);

        assertEquals(1.0f, array1.sumNumber().doubleValue(), 0.01f);
        assertEquals(0.14f, array1.getFloat(0), 0.01f);
    }

    @Test
    public void testLogSoftmax1()  throws Exception {
        INDArray array1 = Nd4j.zeros(15);
        array1.putScalar(0, 0.9f);

        Nd4j.getExecutioner().exec(new LogSoftMax(array1));

        System.out.println("Array1: " + Arrays.toString(array1.data().asFloat()));

        assertEquals(-41.12f, array1.sumNumber().doubleValue(), 0.01f);
        assertEquals(-1.9f, array1.getFloat(0), 0.1f);
        assertEquals(-2.8f, array1.getFloat(1), 0.1f);
    }

    @Test
    public void testLogSoftmax2() {
        DataTypeUtil.setDTypeForContext(DataType.FLOAT);
        INDArray array1 = Nd4j.create(new double[] {-3.9492188, -7.953125, -4.3476562, 2.1445312, -1.2080078, -2.640625, -5.984375, 13.125, -1.0908203, 4.140625, 1.7753906, -9.1875, -1.7216797, 0.02407837, -2.6074219, 5.1875, 2.8476562, -4.6796875, 2.3886719, -1.5478516, -5.65625, 7.6015625, 0.6513672, 0.36108398, -4.0234375, -0.73828125, 0.23620605, -2.5449219, 2.5097656, -3.1894531, 0.5698242, -2.8105469, 0.34570312, -1.8349609, -2.1777344, 3.7207031, 2.3554688, -4.71875, 1.9306641, -4.0234375, -5.8828125, 7.46875, -0.5571289, 1.3974609, -2.3925781, -0.7763672, -1.3125, 0.27685547, 0.7158203, -1.4169922, 0.25170898, -11.40625, 1.0244141, -6.375, 4.1914062, -0.9916992, 7.1132812, -2.1914062, -1.3115234, -0.54248047, -4.4726562, -5.78125, 1.3642578, 8.7890625, -3.0898438, -0.4284668, -3.8691406, -5.5429688, 2.8710938, -3.0449219, -5.09375, -6.9375, -5.6992188, -1.4013672, 6.578125, -0.8378906, -2.4882812, 3.6757812, 1.515625, 4.6015625, 3.2714844, -11.515625, 4.6953125, -6.7421875, -0.34765625, 0.55029297, 11.1328125, -7.9414062, -1.1298828, -4.3710938, -1.2958984, -5.7539062, -1.3154297, 2.9492188, -2.5507812, 4.7421875, 2.8300781, -3.859375, 1.6787109, -3.9316406, -6.703125, -3.1914062, -5.3554688, 0.25634766, 1.2685547, -0.09802246, -5.6445312, 2.8046875, 3.0644531, 5.1289062, 9.3828125, -14.03125, -0.8144531, -2.5136719, -4.921875, 2.0410156, 2.6503906, -5.5351562, 0.87109375, -2.7792969, -7.15625, -1.8056641, -3.6484375, 3.1542969, -1.9492188, -2.640625, -8.0, 10.625, 0.7675781, 3.5644531, 5.4726562, -7.3476562, 0.8720703, -0.8852539, -4.625, 2.2441406, 2.1074219, -5.71875, 1.4023438, -5.4960938, -1.453125, -6.4140625, 1.6884766, -3.4433594, -0.3112793, 1.3066406, 1.8271484, -2.0976562, 3.9550781, -2.6542969, -5.0195312, 4.9375, -0.4868164, 1.3183594, -1.5224609, -1.6308594, -2.6542969, 1.9677734, 0.052520752, -1.1816406, -3.3867188, -4.6484375, 0.74853516, 10.8203125, -6.4179688, 3.375, -2.6777344, -3.9882812, 2.0351562, -3.0859375, -1.0869141, -8.2421875, 10.796875, -3.9785156, 0.8901367, -3.7617188, 2.078125, -5.3085938, 2.4589844, -2.8769531, -4.7421875, -5.5664062, -5.4375, 0.62109375, 4.71875, 0.30004883, -2.7832031, 1.4794922, -0.5761719, 5.484375, 13.3125, -14.625, 1.0888672, -3.3847656, -8.4375, 5.0742188, 0.94140625, -2.7675781, 0.9082031, -3.5429688, 1.5888672, -10.6328125, -0.24804688, -4.0898438, -1.2021484, 4.9492188, -2.4238281, 1.1298828, 2.4082031, 2.0, 8.8671875, -10.4765625, -1.1660156, -0.11407471, -4.3789062, 2.3144531, 0.41967773, -3.1679688, -0.72753906, -3.1074219, -6.0390625, 8.140625, 0.29370117, 0.99658203, -3.6796875, -1.4394531, -1.0029297, -1.3320312, 2.0351562, -2.3378906, -4.7695312, 7.0078125, -0.40039062, 0.8964844, -2.7910156, -1.2470703, -1.9521484, -1.4960938, 2.3808594, -2.7539062, -0.15222168, -9.8125, 2.1445312, -0.9038086, -1.2236328, -1.1972656, 3.5898438, -1.1835938, 3.1035156, -3.9980469, 0.66845703, -0.89697266, -0.30273438, -2.4316406, -3.7617188, 4.6523438, -1.3779297, -1.2138672, 2.2128906, -2.5332031, -2.1542969, -5.3398438, -3.2363281, 2.7167969, -3.2265625, 6.3007812, -2.390625, -3.0449219, 2.5078125, -0.6972656, 4.7578125, -7.7304688, 1.2607422, -3.3457031, -2.0273438, 1.9853516, 7.6914062, -5.8320312, -3.1367188, -5.109375, -3.953125, 4.21875, 0.042419434, -0.08477783, -0.5551758, -1.9941406, -1.2919922, 0.009750366, 0.83740234, -2.0507812, 0.24902344, -7.9882812, 2.9492188, -5.5820312, 0.4975586, 2.7792969, 11.0625, -9.4453125, 1.8164062, -4.890625, -3.3496094, -3.7050781, 1.203125, 4.109375, -2.0839844, 3.3613281, 3.0800781, -7.6054688, 2.1679688, -3.5292969, -3.1816406, -3.4160156, -2.0175781, 2.4863281, -0.8803711, 5.2226562, 0.19165039, -4.2148438, 1.9824219, -0.796875, -5.25, -6.3671875, -3.1757812, -4.296875, 7.0820312, -1.8271484, -0.23388672, 1.3037109, 1.5605469, 1.9199219, -3.5234375, -9.4453125, 1.7578125, -0.16296387, -0.1965332, 0.5551758, 0.5957031, -4.0976562, 6.4570312, -0.10626221, -0.3630371, -7.1914062, -1.3144531, 0.83740234, -1.078125, 5.5390625, 1.2519531, -5.3046875, 2.6953125, -1.3242188, -5.7226562, -1.8388672, 0.26489258, 0.71435547, 0.60595703, 2.6523438, 5.6953125, -5.921875, 0.91552734, -3.7050781, -3.3242188, -13.0625, -3.1269531, -5.0625, 10.9453125, -0.06298828, 1.2373047, -2.6679688, 2.7070312, 2.3378906, -1.6533203, -1.8964844, 1.0283203, -2.3398438, -0.11425781, 1.1699219, 4.3203125, -3.5585938, 0.31958008, -1.5, 3.8417969, -10.9921875, -4.9648438, 2.0527344, -4.3515625, 8.3515625, -0.390625, -4.6210938, 1.4873047, -1.9785156, -3.7265625, -8.015625, -5.71875, -4.109375, 4.2617188, 2.3613281, -1.9804688, 1.5908203, 3.5742188, 4.765625, -7.1210938, -2.6601562, -5.4453125, 1.5078125, -1.3525391, -1.3378906, -7.2734375, 10.6484375, 0.95996094, 3.3320312, -3.9550781, -12.1015625, -4.546875, -3.0546875, 5.5625, -1.7119141, -3.9863281, 2.2265625, 2.4492188, 7.4101562, -0.32788086, -5.8476562, -2.0839844, 7.09375, -5.1210938, 5.5742188, -1.5576172, -3.984375, 1.0644531, -2.1660156, -5.4921875, 6.203125, 0.4140625, 0.47314453, -2.6699219, -2.9589844, -1.2353516, -1.3867188, 3.2324219, -2.7578125, -5.6367188, -4.5664062, 5.40625, 1.2607422, -5.0078125, -0.72265625, 2.5019531, 1.3212891, 0.31176758, -3.8769531, -1.8183594, -11.46875, -2.6640625, -5.1015625, 4.1054688, -2.3203125, -2.5683594, 5.2539062, 1.5615234, 6.0234375, 0.8071289, -9.6875, -4.4765625, -0.70947266, 0.4790039, 1.2851562, -1.5351562, 5.0351562, -2.1386719, 3.0097656, 5.9140625, -12.2265625, -0.30688477, -1.1142578, -2.5703125, 1.3789062, 0.33276367, -3.3554688, -0.29370117, -0.9404297, -3.0742188, -9.65625, -2.2441406, -3.9296875, 3.4335938, -0.56591797, -1.8828125, 2.9492188, 1.2861328, 5.2734375, -2.1230469, -7.0664062, -2.3515625, 7.5585938, -3.7910156, 5.3046875, -2.3789062, -4.5898438, 2.2421875, -1.8740234, -1.765625, -8.4296875, -6.9335938, -1.765625, 1.7792969, 0.16235352, -6.5898438, 10.4140625, -1.2558594, 6.625, -3.6503906, -6.5273438, 9.515625, -2.7675781, -3.1152344, -3.3027344, -0.5083008, -1.5878906, 3.2949219, -1.4951172, 13.3828125, -15.90625, 1.203125, -4.4804688, -8.1953125, 1.7871094, 2.1386719, -1.8378906, 0.3474121, -3.3046875, 2.0195312, -11.1953125, 2.1367188, -2.71875, -0.83447266, 3.1113281, 9.2734375, -11.3515625, 3.40625, -4.8398438, -1.6025391, -5.34375, -0.6191406, 1.8554688, -2.5683594, 6.0117188, 1.2353516, -3.6757812, 2.3320312, -3.078125, -4.40625, -9.875, -3.5390625, 0.0914917, 7.59375, 0.49438477, -1.5166016, -1.3427734, 0.4272461, 3.328125, -4.8984375, -4.671875, -1.4003906, 2.3769531, -2.4667969, -0.21350098, -0.1340332, -3.2675781, 5.4726562, -2.1191406, -2.6816406, -6.25, -2.8066406, -4.6601562, 5.8164062, 0.97998047, 0.48120117, -1.8466797, 2.2617188, 1.5771484, -2.8398438, -4.0, 2.3007812, -3.9453125, 0.4416504, 2.8554688, 8.046875, -7.6328125, 1.3613281, -3.5449219, -5.7617188, 5.1015625, -0.88427734, 1.6591797, -1.4179688, 0.27734375, -1.1875, 0.4296875, 0.20751953, -0.49414062, -5.5820312, -2.4042969, -2.5859375, -2.8398438, 1.4306641, -1.9990234, -1.5800781, 5.1523438, 2.5039062, 0.7314453, 1.9658203, -8.875, 3.6210938, -0.7963867, -4.109375, -0.092041016, 5.4335938, -2.5898438, 1.8925781, -4.53125, 1.3935547, -8.296875, 2.9355469, -5.53125, -1.7333984, 0.52490234, 10.3125, -10.0625, 4.3867188, -4.6171875, -5.9101562, 7.4609375, -0.6044922, 2.0, -2.4882812, -1.7333984, -2.5527344, 0.45996094, 0.83154297, -1.5947266, -7.8046875, -4.9375, -0.3491211, 7.2578125, 1.0126953, 0.95166016, -2.2128906, -2.8886719, 2.0136719, 1.1728516, -0.5097656, -11.59375, 3.0390625, -7.1289062, 3.140625, -1.2822266, 11.6953125, -3.7949219, 0.9658203, -3.9511719, -6.90625, -3.3691406, 2.4921875, 2.3710938, -1.125, -3.9628906, -3.0273438, 5.3632812, 0.5229492, 0.6230469, 8.890625, -12.296875, -2.953125, -1.6738281, -5.2695312, 6.0742188, -1.2392578, -2.0195312, 1.7099609, -1.015625, -1.8808594, -7.5, 0.55615234, 2.2148438, -2.4335938, 5.9609375, 3.625, -7.2421875, 2.796875, -3.7949219, -5.6875, 7.0703125, 0.17626953, 1.3525391, -2.8945312, -1.2324219, -1.1328125, 0.2076416, 0.41308594, -1.3964844, -1.65625, -3.9003906, 0.9082031, -1.8662109, 1.3046875, 0.61865234, 5.125, -6.8671875, 0.9135742, -3.0898438, -4.5625, -14.5078125, 0.015930176, -3.1972656, 8.953125, -1.4765625, 2.1523438, -0.3100586, 2.1386719, 2.1738281, -7.4140625, 7.296875, -0.43408203, 2.2675781, -4.3085938, -1.5605469, -2.7890625, 0.7661133, 1.8984375, -1.140625, 2.1992188, -9.0859375, 3.921875, -5.34375, -1.0742188, 0.68652344, 10.0, -6.6171875, -0.10107422, -4.0859375, -4.0078125, -9.9375, -3.1855469, -4.21875, 8.1171875, -2.7558594, 0.5078125, 0.42236328, 1.4384766, 1.9023438, 0.5966797, -9.3515625, -5.0546875, 0.7705078, -3.1660156, -1.4169922, -5.2539062, 12.875, -0.8510742, 2.9023438, -3.4355469, 4.5351562, -0.25170898, 0.24487305, -1.6630859, -0.47631836, -0.89160156, -0.034240723, 0.8989258, -0.7265625, -2.1035156, -4.1015625, 4.5, 0.5800781, -3.7675781, -2.3476562, -4.0117188, 0.625, 2.546875, -0.86621094, -4.3164062, 6.4179688, 0.14465332, 0.7241211, -3.3378906, -0.0074691772, -0.8852539, -0.8564453, 1.4521484, -1.6572266, -5.0117188, -5.0664062, -3.1757812, 0.20800781, 3.5859375, -0.4321289, -2.6386719, 1.3339844, -0.26904297, 5.453125, -0.9057617, -6.2539062, 2.1132812, -1.5947266, -0.34033203, 0.15539551, 8.71875, -7.6992188, 0.074279785, -4.4023438, -2.1289062, -9.25, -3.0136719, 3.0605469, -2.1152344, -2.046875, -6.1210938, 11.53125, -1.0195312, 3.9042969, -4.1289062, -6.3085938, -5.7460938, -0.2824707, -0.28051758, -0.24255371, -7.2734375, 10.921875, -0.15100098, 5.796875, 8.9609375, -13.53125, 0.016021729, -2.9667969, -3.890625, 3.9960938, 1.0693359, -2.9472656, 0.63916016, -2.7734375, 4.4335938, -10.84375, 5.984375, 5.6367188, -9.9921875, -1.7148438, 0.4831543, -1.2626953, 1.4619141, -6.6132812, 2.8847656, -10.34375, 2.8339844, -1.2333984, -3.4511719, 1.2685547, 1.8369141, -3.6035156, 3.1933594, -2.4277344, 0.3491211, -8.1328125, -4.5273438, 1.4755859, -1.1210938, 7.8125, 0.6621094, -4.6289062, 1.9970703, -0.14282227, -5.4765625, -5.8125, -2.1425781, -3.5058594, 3.5273438, -0.4975586, 0.8984375, 4.4296875, -0.5908203, 2.8964844, -4.21875, -9.4140625, -3.015625, -5.734375, 11.3515625, -2.2695312, 0.9716797, -0.7138672, 2.0722656, 2.3183594, -1.3671875, -8.453125, -1.0820312, 3.9921875, -4.8164062, 2.9804688, -2.5527344, 0.484375, 2.6015625, -0.22485352, -2.8417969, -6.8554688, 4.3554688, -6.25, 1.4355469, 2.0332031, 10.5546875, -10.53125, 2.5859375, -4.7460938, 0.8535156, -9.015625, -1.578125, 3.3457031, -2.2402344, 5.90625, -2.6230469, -4.8984375, 3.9707031, -2.8808594, -5.4257812, -5.5546875, -5.7382812, 0.5317383, 3.6269531, 2.0820312, -3.4121094, 0.8666992, 0.6401367, 6.2421875, 0.6777344, -7.921875, -1.5048828, 7.703125, -6.609375, 3.9121094, -3.015625, -3.4160156, 2.9121094, -4.1015625, -4.6132812, 5.296875, 0.89941406, 0.36865234, -1.4892578, -2.5703125, -1.5419922, -0.5732422, 1.1542969, -2.2695312, -5.25, -9.34375, -4.6484375, 1.1386719, 4.4609375, 1.5175781, -3.9589844, 1.0029297, 0.8779297, 6.6484375, -6.3125, 7.3164062, -1.40625, 1.3681641, -2.3339844, -0.2998047, -2.3105469, 0.08630371, 1.5976562, -1.0966797, 1.6259766, -7.2695312, -2.4863281, -0.91845703, -1.7949219, 3.6171875, 6.1914062, -8.53125, 0.026794434, -3.3789062, -3.9082031, -4.9335938, 9.390625, -0.30664062, -1.9775391, -2.6445312, -0.6386719, -3.6425781, 3.5996094, -3.4121094, -6.3476562, 5.609375, -0.6875, 2.2714844, -1.4580078, -1.7958984, -2.1601562, 1.9775391, -0.57666016, -0.3894043, -3.0410156, -4.0117188, 1.3916016, 6.8203125, -3.0039062, 0.40112305, -2.0097656, -2.7421875, 0.953125, -3.3945312, 1.2753906, -10.3203125, 3.3300781, -5.9179688, -0.20898438, -0.114746094, 10.265625, -5.9335938, 1.3808594, -5.1054688, 0.43286133, -7.5742188, -7.609375, -1.3525391, -1.046875, 2.6445312, -4.5117188, 9.578125, -0.17993164, 3.6777344, -3.0566406, -2.9921875, 0.85791016, -1.5742188, 1.3525391, 0.46557617, 3.8554688, -5.03125, 0.7138672, -1.671875, -1.6064453, -3.5117188, 0.6743164, 0.3972168, -1.5791016, 3.6015625, 2.9863281, -3.8574219, 2.1679688, -3.7148438, -4.4921875, -11.234375, 0.9291992, 5.2929688, -2.7070312, 0.6196289, -6.6992188, 1.4130859, 4.4921875, 0.53808594, -6.4804688, 7.578125, -0.9711914, 1.8388672, -2.7871094, -0.6245117, -1.6474609, -0.028076172, 1.6601562, -1.4501953, -6.5273438, 6.1015625, 0.36645508, 0.6503906, -0.47631836, -3.7050781, -1.5585938, -0.39892578, 1.5400391, -2.1992188, -5.3242188, -5.5820312, -1.1386719, 7.7070312, -5.8398438, 1.3330078, -5.8242188, 2.1933594, 2.7910156, 0.3317871, -4.2773438, -5.84375, -1.8154297, 7.375, -3.9296875, 1.8798828, -6.5117188, -0.79003906, 3.6953125, -0.33447266, -4.6171875, 4.9921875, 1.0126953, 0.49926758, -1.0654297, -3.5703125, -1.7978516, -0.7753906, 1.5996094, -3.0410156, -0.21472168, -7.1835938, 4.9765625, -0.6020508, 1.2802734, -1.8876953, 2.3300781, -5.359375, 0.6870117, -3.1914062, -3.9414062, -1.4169922, -1.4296875, -0.9970703, -5.390625, 0.515625, -3.7128906, -1.4501953, 8.3515625, -1.6914062, -1.1552734, -7.1054688, 5.0195312, -1.0087891, 3.0742188, -1.8681641, 2.8710938, -4.015625, -0.96240234, -1.6445312, -1.3134766, -14.6328125, 5.1992188, -7.28125, 6.59375, -3.7402344, 11.0625, -3.8730469, -0.59033203, -0.8720703, -6.3710938, -6.6640625, -5.78125, 2.7285156, 3.8046875, 1.6210938, -4.9492188, 1.5576172, 0.9472656, 5.9335938, -2.3652344, -5.0195312, 2.4023438, 1.4453125, -4.0234375, -2.3554688, -5.0625, 5.7851562, -0.094177246, 1.6894531, -4.9882812, -6.1367188, 0.5625, 6.4921875, -3.0117188, -0.27368164, -4.8945312, 2.1445312, 2.7753906, 0.41137695, -6.390625, -10.5625, -5.1835938, 0.75634766, 5.6875, -0.35205078, -4.5039062, 3.6367188, 0.4584961, 7.3203125, -3.8417969, -9.78125, -4.53125, -4.7304688, 9.28125, -1.3535156, 0.609375, 1.9599609, 2.1152344, 4.078125, -7.015625, -5.6210938, -4.1523438, -1.5693359, 7.8632812, -0.36376953, -0.49560547, -0.107543945, 0.93115234, 2.3046875, -4.6132812, -4.7773438, 14.34375, 1.8056641, -9.8046875, -1.8408203, 1.5429688, -6.1484375, 5.84375, -8.3046875, 10.4609375, -13.1796875, 2.0917969, -8.2421875, -4.8476562, 4.34375, 5.4960938, -7.859375, 2.0605469, -5.2539062, -3.1523438, -13.1484375, 2.421875, -5.96875, 7.2773438, -3.5996094, 5.4765625, -0.3918457, 0.34838867, 0.7138672, -4.9296875, 2.8085938, 6.328125, 1.2519531, -4.9179688, -1.9257812, 2.2773438, -1.8183594, 0.075927734, -4.8632812, -4.1210938, 4.7109375, -0.18847656, -0.0033340454, -0.9140625, -1.796875, -1.8095703, 0.2319336, 1.1972656, -2.1445312, -5.5273438, 6.7226562, -0.5678711, 1.2050781, -2.8398438, -1.7080078, -2.6015625, 1.6298828, 0.76416016, -1.3017578, -4.3554688, -8.53125, -1.4111328, 4.6640625, -2.765625, 1.6875, -5.8085938, 0.9165039, 2.5410156, 1.5732422});

        Nd4j.getExecutioner().exec(new LogSoftMax(array1));

        System.out.println("Array1: " + Arrays.toString(array1.data().asFloat()));
        DataTypeUtil.setDTypeForContext(DataType.HALF);
    }

    @Test
    public void testReduce3_1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{0.0f, 1.0f, 2.0f, 3.0f, 4.0f});
        INDArray array2 = Nd4j.create(new float[]{0.5f, 1.5f, 2.5f, 3.5f, 4.5f});


        double result = Nd4j.getExecutioner().execAndReturn(new ManhattanDistance(array1, array2)).getFinalResult().doubleValue();

        assertEquals(2.5, result, 0.01);
    }

    @Ignore
    @Test
    public void testReduce3_2() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{2.01f, 2.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f, 1.01f});
        INDArray array2 = Nd4j.create(new float[]{1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f});


        double similarity = Transforms.cosineSim(array1, array2);

        System.out.println("Cosine similarity: " + similarity);
        assertEquals(0.95f, similarity, 0.01f);
    }


    @Test
    public void testMetaOp1() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{1f, 1f, 1f, 1f, 1f, 1f, 1f});
        INDArray exp = Nd4j.create(new float[]{3f, 3f, 3f, 3f, 3f, 3f, 3f});

        INDArray res = array1.dup().addi(2f);

        assertEquals(exp, res);
    }

    @Test
    public void testMetaOp2() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{1f, 1f, 1f, 1f, 1f, 1f, 1f});
        INDArray exp = Nd4j.create(new float[]{2f, 2f, 2f, 2f, 2f, 2f, 2f});

        INDArray res = array1.dup().muli(2f);

        assertEquals(exp, res);
    }

    @Test
    public void testMetaOp3() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{1f, 1f, 1f, 1f, 1f, 1f, 1f});
        INDArray exp = Nd4j.create(new float[]{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f});

        INDArray res = array1.dup().divi(2f);

        assertEquals(exp, res);
    }

    @Test
    public void testMetaOp4() throws Exception {
        INDArray array1 = Nd4j.create(new float[]{1f, 1f, 1f, 1f, 1f, 1f, 1f});
        INDArray exp = Nd4j.create(new float[]{2f, 2f, 2f, 2f, 2f, 2f, 2f});

        INDArray res = array1.dup().divi(0.5f);

        assertEquals(exp, res);
    }

    @Test
    public void testMetaOp5() throws Exception {
        INDArray exp1 = Nd4j.create(500, 500).assign(3.0f);
        INDArray exp2 = Nd4j.create(500, 500).assign(6.0f);
        INDArray exp3 = Nd4j.create(500, 500).assign(2.0f);
        INDArray array = Nd4j.ones(500, 500);
        INDArray param = Nd4j.ones(500, 500);//.reshape('f',500, 500);

        INDArray am = param.mul(2);
        assertEquals(0, ((CudaGridExecutioner) Nd4j.getExecutioner()).getQueueLength());
        array.addi(am);

        array.divi(0.5f);
        Nd4j.getExecutioner().commit();
        Thread.sleep(1000);

        assertArrayEquals(exp3.data().asFloat(), am.data().asFloat(), 0.001f);

        assertArrayEquals(exp2.data().asFloat(), array.data().asFloat(), 0.001f);
    }


    @Test
    public void testHalfToFloat1() throws Exception {
        File tempFile = File.createTempFile("dsadasd","dsdfasd");
        tempFile.deleteOnExit();

        INDArray array = Nd4j.linspace(1, 100, 100);

        DataOutputStream stream = new DataOutputStream(new FileOutputStream(tempFile));

        Nd4j.write(array, stream);

        DataInputStream dis = new DataInputStream(new FileInputStream(tempFile));

        INDArray restoredFP16 = Nd4j.read(dis);

        //assertEquals(array, restoredFP16);


        DataTypeUtil.setDTypeForContext(DataType.FLOAT);
        assertEquals(DataType.FLOAT, Nd4j.dataType());
        log.error("--------------------");

        dis = new DataInputStream(new FileInputStream(tempFile));
        INDArray expFP32 = Nd4j.linspace(1, 100, 100);
        INDArray restoredFP32 = Nd4j.read(dis);

        CudaContext context = (CudaContext) AtomicAllocator.getInstance().getDeviceContext().getContext();

        assertTrue(AtomicAllocator.getInstance().getPointer(expFP32, context) instanceof FloatPointer);
        assertTrue(AtomicAllocator.getInstance().getPointer(restoredFP32, context) instanceof FloatPointer);

        assertEquals(DataType.FLOAT, expFP32.data().dataType());
        assertEquals(DataType.FLOAT, restoredFP32.data().dataType());

        assertEquals(expFP32, restoredFP32);

        DataTypeUtil.setDTypeForContext(DataType.HALF);
    }
}
