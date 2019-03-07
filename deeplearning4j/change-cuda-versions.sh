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

VALID_VERSIONS=( 8.0 9.0 9.1 9.2 10.0 )
CUDA_80_VERSION="8\.0"
CUDA_90_VERSION="9\.0"
CUDA_91_VERSION="9\.1"
CUDA_92_VERSION="9\.2"
CUDA_100_VERSION="10\.0"
CUDNN_60_VERSION="6\.0"
CUDNN_70_VERSION="7\.0"
CUDNN_71_VERSION="7\.1"
CUDNN_74_VERSION="7\.4"
JAVACPP_141_VERSION="1\.4\.1"
JAVACPP_142_VERSION="1\.4\.2"
JAVACPP_144_VERSION="1\.4\.4"

usage() {
  echo "Usage: $(basename $0) [-h|--help] <cuda version to be used>
where :
  -h| --help Display this help text
  valid cuda version values : ${VALID_VERSIONS[*]}
" 1>&2
  exit 1
}

if [[ ($# -ne 1) || ( $1 == "--help") ||  $1 == "-h" ]]; then
  usage
fi

VERSION=$1

check_cuda_version() {
  for i in ${VALID_VERSIONS[*]}; do [ $i = "$1" ] && return 0; done
  echo "Invalid CUDA version: $1. Valid versions: ${VALID_VERSIONS[*]}" 1>&2
  exit 1
}


check_cuda_version "$VERSION"

case $VERSION in
  10.0)
    VERSION=$CUDA_100_VERSION
    VERSION2=$CUDNN_74_VERSION
    VERSION3=$JAVACPP_144_VERSION
    ;;
  9.2)
    VERSION=$CUDA_92_VERSION
    VERSION2=$CUDNN_71_VERSION
    VERSION3=$JAVACPP_142_VERSION
    ;;
  9.1)
    VERSION=$CUDA_91_VERSION
    VERSION2=$CUDNN_71_VERSION
    VERSION3=$JAVACPP_141_VERSION
    ;;
  9.0)
    VERSION=$CUDA_90_VERSION
    VERSION2=$CUDNN_70_VERSION
    VERSION3=$JAVACPP_141_VERSION
    ;;
  8.0)
    VERSION=$CUDA_80_VERSION
    VERSION2=$CUDNN_60_VERSION
    VERSION3=$JAVACPP_141_VERSION
    ;;
esac

sed_i() {
  sed -e "$1" "$2" > "$2.tmp" && mv "$2.tmp" "$2"
}

export -f sed_i

echo "Updating CUDA versions in pom.xml files to CUDA $1";

BASEDIR=$(dirname $0)

#Artifact ids, ending with "-8.0", "-9.0", etc. nd4j-cuda, etc.
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(artifactId>nd4j-cuda-\)[0-9.]*<\/artifactId>/\1'$VERSION'<\/artifactId>/g' {}" \;

#Artifact ids, ending with "-8.0-platform", "-9.0-platform", etc. nd4j-cuda-platform, etc.
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(artifactId>nd4j-cuda-\)[0-9.]*-platform<\/artifactId>/\1'$VERSION'-platform<\/artifactId>/g' {}" \;

#Profiles ids, ending with "-8.0", "-9.0", etc. test-nd4j-cuda, etc.
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(test-nd4j-cuda-\)[0-9.]*</\1'$VERSION'</g' {}" \;

#Artifact ids, ending with "-8.0", "-9.0", etc. deeplearning4j-cuda, etc.
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(artifactId>deeplearning4j-cuda-\)[0-9.]*<\/artifactId>/\1'$VERSION'<\/artifactId>/g' {}" \;

#Artifact ids, ending with "-8.0-platform", "-9.0-platform", etc. deeplearning4j-cuda-platform, etc.
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(artifactId>deeplearning4j-cuda-\)[0-9.]*-platform<\/artifactId>/\1'$VERSION'-platform<\/artifactId>/g' {}" \;

#CUDA versions, like <cuda.version>9.1</cuda.version>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(cuda.version>\)[0-9.]*<\/cuda.version>/\1'$VERSION'<\/cuda.version>/g' {}" \;

#cuDNN versions, like <cudnn.version>7.0</cudnn.version>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(cudnn.version>\)[0-9.]*<\/cudnn.version>/\1'$VERSION2'<\/cudnn.version>/g' {}" \;

#JavaCPP versions, like <javacpp-presets.cuda.version>1.4</javacpp-presets.cuda.version>
find "$BASEDIR" -name 'pom.xml' -not -path '*target*' \
  -exec bash -c "sed_i 's/\(javacpp-presets.cuda.version>\).*<\/javacpp-presets.cuda.version>/\1'$VERSION3'<\/javacpp-presets.cuda.version>/g' {}" \;

echo "Done updating CUDA versions.";
