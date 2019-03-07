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
package org.deeplearning4j.scalnet.layers.recurrent

import org.scalatest.{ Matchers, WordSpec }

class BidirectionalTest extends WordSpec with Matchers {

  "A Bidirectional wrapper layer" should {

    "compile to a DL4J Bidirectional wrapper layer with a LSTM" in {
      val bidirectionalLSTM = Bidirectional(LSTM(10, 100))
      val compiledLayer = bidirectionalLSTM.compile
      compiledLayer.isInstanceOf[org.deeplearning4j.nn.conf.layers.recurrent.Bidirectional] shouldBe true
    }

    "compile to a DL4J Bidirectional wrapper layer with a GravesLSTM" in {
      val bidirectionalLSTM = Bidirectional(GravesLSTM(10, 100))
      val compiledLayer = bidirectionalLSTM.compile
      compiledLayer.isInstanceOf[org.deeplearning4j.nn.conf.layers.recurrent.Bidirectional] shouldBe true
    }

  }
}
