#! /bin/bash
#
# Copyright 2018 - 2019 James E. King III
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Bash script to install Intel ICC.
#

#
# Environment Variables
#
# INTEL_ICC_SERIAL_NUMBER           - the Intel ICC serial number to use
# SELF                              - the boost libs directory name
# B2_TOOLSET                        - the toolset to use (intel-linux)

set -ex

. $(dirname "${BASH_SOURCE[0]}")/enforce.sh

if [ -z "$INTEL_ICC_SERIAL_NUMBER" ]; then
    echo "ERROR: you did not set the INTEL_ICC_SERIAL_NUMBER environment variable"
    exit 1
fi

function finish {
  rm -rf /tmp/parallel_studio_xe_2019_update3_professional_edition_for_cpp_online/silent.cfg
}

pushd /tmp
wget --quiet http://registrationcenter-download.intel.com/akdlm/irc_nas/tec/15270/parallel_studio_xe_2019_update3_professional_edition_for_cpp_online.tgz
tar xzf parallel_studio_xe_2019_update3_professional_edition_for_cpp_online.tgz
cd parallel_studio_xe_2019_update3_professional_edition_for_cpp_online/
cp $(dirname "${BASH_SOURCE[0]}")/intelicc.cfg silent.cfg
trap finish EXIT
sed -i "s/ACTIVATION_SERIAL_NUMBER=.*/ACTIVATION_SERIAL_NUMBER=$INTEL_ICC_SERIAL_NUMBER/g" silent.cfg
sudo ./install.sh -s silent.cfg
rm -f silent.cfg
export PATH=/opt/intel/bin:$PATH
popd
cd ../..
./bootstrap.sh --with-toolset=$B2_TOOLSET
cd libs/$SELF
ci/travis/build.sh
