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

import breeze.linalg._
import monocle.Prism
import org.nd4j.linalg.api.buffer.DataType
import org.nd4j.linalg.api.ndarray.INDArray
import org.nd4j.linalg.factory.Nd4j
import org.nd4s.Implicits._
import org.scalacheck.{ Arbitrary, Gen, Prop }
import org.scalatest.FlatSpec
import org.scalatest.prop.Checkers

/**
  * Created by taisukeoe on 16/03/05.
  */
class BreezeCheck extends FlatSpec with Checkers {
  it should "work the same as NDArray slicing" in {
    check {
      Prop.forAll { (ndArray: INDArray) =>
        ndArray.setOrder('f')
        val shape = ndArray.shape().map(_.toInt)
        val Array(row, col) = shape
        Prop.forAll(Gen.choose(0, row - 1), Gen.choose(0, row - 1), Gen.choose(0, col - 1), Gen.choose(0, col - 1)) {
          (r1, r2, c1, c2) =>
            val rowRange = if (r1 > r2) r2 to r1 else r1 to r2
            val columnRange = if (c1 > c2) c2 to c1 else c1 to c2
            val slicedByND4S = ndArray(rowRange, columnRange)
            val slicedByBreeze = prism
              .getOption(ndArray)
              .map(dm => prism.reverseGet(dm(rowRange, columnRange)))
            slicedByBreeze.exists(_.shape() sameElements slicedByND4S.castTo(DataType.DOUBLE).shape())
        }
      }

    }
  }

  //This supports only real value since ND4J drops complex number support temporary.
  lazy val prism = Prism[INDArray, DenseMatrix[Double]] { ndArray =>
    //Breeze DenseMatrix doesn't support tensor nor C order matrix.
    if (ndArray.rank() > 2 || ndArray.ordering() == 'c')
      None
    else {
      val shape = ndArray.shape()
      val linear = ndArray.reshape(-1)
      val arr = (0 until ndArray.length().toInt).map(i => linear.getDouble(i.toLong)).toArray
      Some(DenseMatrix(arr).reshape(shape(0).toInt, shape(1).toInt))
    }
  } { dm =>
    val shape = Array(dm.rows, dm.cols)
    dm.toArray.mkNDArray(shape, NDOrdering.Fortran)
  }

  implicit def arbNDArray: Arbitrary[INDArray] = Arbitrary {
    for {
      rows <- Gen.choose(1, 100)
      columns <- Gen.choose(1, 100)
    } yield {
      val nd = Nd4j.rand(rows, columns)
      nd.setOrder('f')
      nd
    }
  }

}
