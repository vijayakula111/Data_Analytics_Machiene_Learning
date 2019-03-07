#!/usr/bin/env bash

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

# This shell script is adapted from Apache Flink (in turn, adapted from Apache Spark) some modifications.

set -e

VALID_VERSIONS=( 2.10 2.11 )
SCALA_210_VERSION=$(grep -F -m 1 'scala210.version' pom.xml); SCALA_210_VERSION="${SCALA_210_VERSION#*>}"; SCALA_210_VERSION="${SCALA_210_VERSION%<*}";
SCALA_211_VERSION=$(grep -F -m 1 'scala211.version' pom.xml); SCALA_211_VERSION="${SCALA_211_VERSION#*>}"; SCALA_211_VERSION="${SCALA_211_VERSION%<*}";

usage() {
  echo "Usage: $(basename $0) [-h|--help] <scala version to be used>
where :
  -h| --help Display this help text
  valid scala version values : ${VALID_VERSIONS[*]}
" 1>&2
  exit 1
}

if [[ ($# -ne 1) || ( $1 == "--help") ||  $1 == "-h" ]]; then
  usage
fi

TO_VERSION=$1

check_scala_version() {
  for i in ${VALID_VERSIONS[*]}; do [ $i = "$1" ] && return 0; done
  echo "Invalid Scala version: $1. Valid versions: ${VALID_VERSIONS[*]}" 1>&2
  exit 1
}


check_scala_version "$TO_VERSION"

if [ $TO_VERSION = "2.11" ]; then
  FROM_BINARY="_2\.10"
  TO_BINARY="_2\.11"
  FROM_VERSION=$SCALA_210_VERSION
  TO_VERSION=$SCALA_211_VERSION
else
  FROM_BINARY="_2\.11"
  TO_BINARY="_2\.10"
  FROM_VERSION=$SCALA_211_VERSION
  TO_VERSION=$SCALA_210_VERSION
fi

sed_i() {
  sed -e "$1" "$2" > "$2.tmp" && mv "$2.tmp" "$2"
}

export -f sed_i

echo "Updating Scala versions in pom.xml files to Scala $1, from $FROM_VERSION to $TO_VERSION";

BASEDIR=$(dirname $0)

#Artifact ids, ending with "_2.10" or "_2.11". Spark, spark-mllib, kafka, etc.
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(artifactId>.*\)'$FROM_BINARY'<\/artifactId>/\1'$TO_BINARY'<\/artifactId>/g' {}" \;

#Scala versions, like <scala.version>2.10</scala.version>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(scala.version>\)'$FROM_VERSION'<\/scala.version>/\1'$TO_VERSION'<\/scala.version>/g' {}" \;

#Scala binary versions, like <scala.binary.version>2.10</scala.binary.version>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(scala.binary.version>\)'${FROM_BINARY#_}'<\/scala.binary.version>/\1'${TO_BINARY#_}'<\/scala.binary.version>/g' {}" \;

#Scala versions, like <artifactId>scala-library</artifactId> <version>2.10.6</version>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(version>\)'$FROM_VERSION'<\/version>/\1'$TO_VERSION'<\/version>/g' {}" \;

#Scala maven plugin, <scalaVersion>2.10</scalaVersion>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(scalaVersion>\)'$FROM_VERSION'<\/scalaVersion>/\1'$TO_VERSION'<\/scalaVersion>/g' {}" \;
  
#Edge case for Korean NLP artifact not following conventions:	https://github.com/deeplearning4j/deeplearning4j/issues/6306
#https://github.com/deeplearning4j/deeplearning4j/issues/6306
if [[ $TO_VERSION == 2.11* ]]; then
  sed_i 's/<artifactId>korean-text-scala-2.10<\/artifactId>/<artifactId>korean-text<\/artifactId>/g' deeplearning4j/deeplearning4j-nlp-parent/deeplearning4j-nlp-korean/pom.xml
  sed_i 's/<version>4.2.0<\/version>/<version>4.4<\/version>/g' deeplearning4j/deeplearning4j-nlp-parent/deeplearning4j-nlp-korean/pom.xml
else
  sed_i 's/<artifactId>korean-text<\/artifactId>/<artifactId>korean-text-scala-2.10<\/artifactId>/g' deeplearning4j/deeplearning4j-nlp-parent/deeplearning4j-nlp-korean/pom.xml
  sed_i 's/<version>4.4<\/version>/<version>4.2.0<\/version>/g' deeplearning4j/deeplearning4j-nlp-parent/deeplearning4j-nlp-korean/pom.xml
fi


echo "Done updating Scala versions.";
