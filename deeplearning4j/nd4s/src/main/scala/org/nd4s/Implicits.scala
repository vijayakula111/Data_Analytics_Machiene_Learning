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
package org.nd4s

import org.nd4j.linalg.api.ndarray.INDArray
import org.nd4j.linalg.factory.Nd4j
import org.nd4j.linalg.indexing.{ INDArrayIndex, NDArrayIndex }
import scala.collection.breakOut
object Implicits {

  implicit class RichINDArray[A <: INDArray](val underlying: A)
      extends SliceableNDArray[A]
      with OperatableNDArray[A]
      with CollectionLikeNDArray[A]

  implicit def rowProjection2NDArray(row: RowProjectedNDArray): INDArray =
    row.array

  implicit def columnProjection2NDArray(column: ColumnProjectedNDArray): INDArray = column.array

  implicit def sliceProjection2NDArray(sliced: SliceProjectedNDArray): INDArray = sliced.array

  /*
   Avoid using Numeric[T].toDouble(t:T) for sequence transformation in XXColl2INDArray to minimize memory consumption.
   */

  implicit def floatArray2INDArray(s: Array[Float]): FloatArray2INDArray =
    new FloatArray2INDArray(s)
  implicit def floatColl2INDArray(s: Seq[Float]): FloatArray2INDArray =
    new FloatArray2INDArray(s.toArray)
  implicit def jfloatColl2INDArray(s: Seq[java.lang.Float]): FloatArray2INDArray =
    new FloatArray2INDArray(s.map(x => x: Float)(breakOut))
  class FloatArray2INDArray(val underlying: Array[Float]) extends AnyVal {
    def mkNDArray(shape: Array[Int], ord: NDOrdering = NDOrdering(Nd4j.order()), offset: Int = 0): INDArray =
      Nd4j.create(underlying, shape, ord.value, offset)

    def asNDArray(shape: Int*): INDArray =
      Nd4j.create(underlying, shape.toArray)

    def toNDArray: INDArray = Nd4j.create(underlying)
  }

  implicit def doubleArray2INDArray(s: Array[Double]): DoubleArray2INDArray =
    new DoubleArray2INDArray(s)
  implicit def doubleArray2CollArray(s: Seq[Double]): DoubleArray2INDArray =
    new DoubleArray2INDArray(s.toArray)
  implicit def jdoubleColl2INDArray(s: Seq[java.lang.Double]): DoubleArray2INDArray =
    new DoubleArray2INDArray(s.map(x => x: Double)(breakOut))
  class DoubleArray2INDArray(val underlying: Array[Double]) extends AnyVal {
    def mkNDArray(shape: Array[Int], ord: NDOrdering = NDOrdering(Nd4j.order()), offset: Int = 0): INDArray =
      Nd4j.create(underlying, shape, offset, ord.value)

    def asNDArray(shape: Int*): INDArray =
      Nd4j.create(underlying, shape.toArray)

    def toNDArray: INDArray = Nd4j.create(underlying)
  }

  implicit def intColl2INDArray(s: Seq[Int]): IntArray2INDArray =
    new IntArray2INDArray(s.toArray)
  implicit def intArray2INDArray(s: Array[Int]): IntArray2INDArray =
    new IntArray2INDArray(s)
  implicit def jintColl2INDArray(s: Seq[java.lang.Integer]): IntArray2INDArray =
    new IntArray2INDArray(s.map(x => x: Int)(breakOut))
  class IntArray2INDArray(val underlying: Array[Int]) extends AnyVal {
    def mkNDArray(shape: Array[Int], ord: NDOrdering = NDOrdering(Nd4j.order()), offset: Int = 0): INDArray =
      Nd4j.create(underlying.map(_.toFloat), shape, ord.value, offset)

    def asNDArray(shape: Int*): INDArray =
      Nd4j.create(underlying.map(_.toFloat), shape.toArray)

    def toNDArray: INDArray = Nd4j.create(underlying.map(_.toFloat).toArray)
  }

  implicit class FloatMtrix2INDArray(val underlying: Seq[Seq[Float]]) extends AnyVal {
    def mkNDArray(ord: NDOrdering): INDArray =
      Nd4j.create(underlying.map(_.toArray).toArray, ord.value)
    def toNDArray: INDArray = Nd4j.create(underlying.map(_.toArray).toArray)
  }

  implicit class FloatArrayMtrix2INDArray(val underlying: Array[Array[Float]]) extends AnyVal {
    def mkNDArray(ord: NDOrdering): INDArray =
      Nd4j.create(underlying, ord.value)
    def toNDArray: INDArray = Nd4j.create(underlying)
  }

  implicit class DoubleMtrix2INDArray(val underlying: Seq[Seq[Double]]) extends AnyVal {
    def mkNDArray(ord: NDOrdering): INDArray =
      Nd4j.create(underlying.map(_.toArray).toArray, ord.value)
    def toNDArray: INDArray = Nd4j.create(underlying.map(_.toArray).toArray)
  }

  implicit class DoubleArrayMtrix2INDArray(val underlying: Array[Array[Double]]) extends AnyVal {
    def mkNDArray(ord: NDOrdering): INDArray =
      Nd4j.create(underlying, ord.value)
    def toNDArray: INDArray = Nd4j.create(underlying)
  }

  implicit class IntMtrix2INDArray(val underlying: Seq[Seq[Int]]) extends AnyVal {
    def mkNDArray(ord: NDOrdering): INDArray =
      Nd4j.create(underlying.map(_.map(_.toFloat).toArray).toArray, ord.value)
    def toNDArray: INDArray =
      Nd4j.create(underlying.map(_.map(_.toFloat).toArray).toArray)
  }

  implicit class IntArrayMtrix2INDArray(val underlying: Array[Array[Int]]) extends AnyVal {
    def mkNDArray(ord: NDOrdering): INDArray =
      Nd4j.create(underlying.map(_.map(_.toFloat)), ord.value)
    def toNDArray: INDArray = Nd4j.create(underlying.map(_.map(_.toFloat)))
  }

  implicit class Num2Scalar[T](val underlying: T)(implicit ev: Numeric[T]) {
    def toScalar: INDArray = Nd4j.scalar(ev.toDouble(underlying))
  }

  implicit def intArray2IndexRangeArray(arr: Array[Int]): Array[IndexRange] =
    arr.map(new IntRange(_))

  case object -> extends IndexRange {
    override def hasNegative: Boolean = false
  }

  case object ---> extends IndexRange {
    override def hasNegative: Boolean = false
  }

  implicit class IntRange(val underlying: Int) extends IndexNumberRange {
    protected[nd4s] override def asRange(max: => Int): DRange =
      DRange(underlying, underlying, true, 1, max)

    override protected[nd4s] def asNDArrayIndex(max: => Int): INDArrayIndex =
      NDArrayIndex.point(underlying)

    override def hasNegative: Boolean = false

    override def toString: String = s"$underlying"
  }

  implicit class TupleRange(val underlying: _root_.scala.Tuple2[Int, Int]) extends IndexNumberRange {
    protected[nd4s] override def asRange(max: => Int): DRange =
      DRange(underlying._1, underlying._2, false, 1, max)

    override protected[nd4s] def asNDArrayIndex(max: => Int): INDArrayIndex =
      IndexNumberRange.toNDArrayIndex(underlying._1, underlying._2, false, 1, max)

    override def toString: String = s"${underlying._1}->${underlying._2}"

    override def hasNegative: Boolean = underlying._1 < 0 || underlying._2 < 0

    def by(i: Int) =
      new IndexRangeWrapper(underlying._1 until underlying._2 by i)
  }

  implicit class IntRangeFromGen(val underlying: Int) extends AnyVal {
    def -> = IntRangeFrom(underlying)
  }

  implicit class IndexRangeWrapper(val underlying: Range) extends IndexNumberRange {
    protected[nd4s] override def asRange(max: => Int): DRange =
      DRange.from(underlying, max)

    override protected[nd4s] def asNDArrayIndex(max: => Int): INDArrayIndex =
      IndexNumberRange.toNDArrayIndex(underlying.start, underlying.end, underlying.isInclusive, underlying.step, max)

    override def toString: String =
      s"${underlying.start}->${underlying.end} by ${underlying.step}"

    override def hasNegative: Boolean =
      underlying.start < 0 || underlying.end < 0 || underlying.step < 0
  }

  implicit class NDArrayIndexWrapper(val underlying: INDArrayIndex) extends IndexNumberRange {
    protected[nd4s] override def asRange(max: => Int): DRange =
      DRange(underlying.current().asInstanceOf[Int],
             underlying.end().asInstanceOf[Int],
             false,
             underlying.stride().asInstanceOf[Int],
             max)

    override protected[nd4s] def asNDArrayIndex(max: => Int): INDArrayIndex =
      underlying

    override def toString: String =
      s"${underlying.current}->${underlying.end} by ${underlying.stride}"

    override def hasNegative: Boolean = false
  }

  lazy val NDOrdering = org.nd4s.NDOrdering

  lazy val i = new ImaginaryNumber[Integer](1)

  implicit class Pair2Tuple[T, U](a: org.nd4j.linalg.primitives.Pair[T, U]) {
    def asScala: (T, U) = (a.getFirst, a.getSecond)
  }

  implicit class Triple2Tuple[T, U, V](a: org.nd4j.linalg.primitives.Triple[T, U, V]) {
    def asScala: (T, U, V) = (a.getFirst, a.getSecond, a.getThird)
  }

  implicit class Tuple2Pair[T, U](a: (T, U)) {
    def toPair: org.nd4j.linalg.primitives.Pair[T, U] =
      new org.nd4j.linalg.primitives.Pair(a._1, a._2)
  }

  implicit class Tuple2Triple[T, U, V](a: (T, U, V)) {
    def toTriple: org.nd4j.linalg.primitives.Triple[T, U, V] =
      new org.nd4j.linalg.primitives.Triple(a._1, a._2, a._3)
  }
}

private[nd4s] class ImaginaryNumber[T <: Number](val value: T) extends AnyVal {
  override def toString: String = s"${value}i"
}

sealed trait IndexNumberRange extends IndexRange {
  protected[nd4s] def asRange(max: => Int): DRange
  protected[nd4s] def asNDArrayIndex(max: => Int): INDArrayIndex
}
object IndexNumberRange {
  def toNDArrayIndex(startR: Int, endR: Int, isInclusive: Boolean, step: Int, max: Int): INDArrayIndex = {
    val (start, end) = {
      val start = if (startR >= 0) startR else max + startR
      val diff = if (isInclusive) 1 else 0
      val endExclusive = if (endR >= 0) endR + diff else max + endR + diff
      (start, endExclusive)
    }

    NDArrayIndex.interval(start, step, end, false)
  }
}

sealed trait IndexRange {
  def hasNegative: Boolean
}

case class IntRangeFrom(underlying: Int) extends IndexRange {
  def apply[T](a: T): (Int, T) = (underlying, a)

  override def toString: String = s"$underlying->"

  override def hasNegative: Boolean = false
}

private[nd4s] case class DRange(startR: Int, endR: Int, isInclusive: Boolean, step: Int, max: Int) {
  lazy val (start, end) = {
    val start = if (startR >= 0) startR else max + startR
    val diff = if (isInclusive) 0 else if (step >= 0) -1 else +1
    val endInclusive = if (endR >= 0) endR + diff else max + endR + diff
    (start, endInclusive)
  }
  lazy val length = (end - start) / step + 1

  def toList: List[Int] = List.iterate(start, length)(_ + step)

  override def toString: String = s"[$start to $end by $step len:$length]"
}

private[nd4s] object DRange extends {
  def from(r: Range, max: => Int): DRange =
    DRange(r.start, r.end, r.isInclusive, r.step, max)

  def apply(startR: Int, endR: Int, step: Int): DRange =
    DRange(startR, endR, false, step, Int.MinValue)
}
