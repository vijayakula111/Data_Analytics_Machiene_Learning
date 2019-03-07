################################################################################
# Copyright (c) 2015-2018 Skymind, Inc.
#
# This program and the accompanying materials are made available under the
# terms of the Apache License, Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
#
# SPDX-License-Identifier: Apache-2.0
################################################################################


import jnius_config
import os
import warnings
import pydl4j

pydl4j.validate_nd4j_jars()


# -------------JVM starts here-------------

from jnius import autoclass

Nd4j = autoclass('org.nd4j.linalg.factory.Nd4j')
INDArray = autoclass('org.nd4j.linalg.api.ndarray.INDArray')
Transforms = autoclass('org.nd4j.linalg.ops.transforms.Transforms')
NDArrayIndex = autoclass('org.nd4j.linalg.indexing.NDArrayIndex')
DataBuffer = autoclass('org.nd4j.linalg.api.buffer.DataBuffer')
DataType = autoclass('org.nd4j.linalg.api.buffer.DataType')
System = autoclass('java.lang.System')
Integer = autoclass('java.lang.Integer')
Long = autoclass('java.lang.Long')
Float = autoclass('java.lang.Float')
Double = autoclass('java.lang.Double')
Shape = autoclass('org.nd4j.linalg.api.shape.Shape')
BinarySerde = autoclass('org.nd4j.serde.binary.BinarySerde')
NativeOpsHolder = autoclass('org.nd4j.nativeblas.NativeOpsHolder')

DoublePointer = autoclass('org.bytedeco.javacpp.DoublePointer')
FloatPointer = autoclass('org.bytedeco.javacpp.FloatPointer')
HalfPointer = autoclass('org.bytedeco.javacpp.ShortPointer')
LongPointer = autoclass('org.bytedeco.javacpp.LongPointer')
IntPointer = autoclass('org.bytedeco.javacpp.IntPointer')
ShortPointer = autoclass('org.bytedeco.javacpp.ShortPointer')
BoolPointer = autoclass('org.bytedeco.javacpp.BoolPointer')


DataTypeUtil = autoclass('org.nd4j.linalg.api.buffer.util.DataTypeUtil')
MemoryManager = autoclass('org.nd4j.linalg.memory.MemoryManager')
SameDiff = autoclass('org.nd4j.autodiff.samediff.SameDiff')
TFGraphMapper = autoclass('org.nd4j.imports.graphmapper.tf.TFGraphMapper')
JDataset = autoclass('org.nd4j.linalg.dataset.DataSet')
ArrayList = autoclass('java.util.ArrayList')


def KerasModelImport():
    return autoclass('org.deeplearning4j.nn.modelimport.keras.KerasModelImport')


def ArrayDescriptor():
    return autoclass('org.deeplearning4j.spark.parameterserver.python.ArrayDescriptor')


def DatasetDescriptor():
    return autoclass('org.deeplearning4j.spark.parameterserver.python.DataSetDescriptor')


def spark_utils():
    return autoclass('org.deeplearning4j.spark.parameterserver.python.Utils')
