#! /bin/bash
#
# Copyright 2019 James E. King III
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#

#
# Build using a Boost Docker Development Environment container.
# The BDDE project at https://github.com/jeking3/bdde includes
# build containers for different operating systems and architectures
# using multiarch containers.  This allows for better continuous
# integration, for example you can build in a big-endian environment
# on Travis CI.
#
# In your .travis.yml file, set the BDDE_OS and BDDE_ARCH according
# to the instructions in the BDDE README.
#

set -ex

git clone https://github.com/jeking3/bdde.git ~/bdde
export PATH=~/bdde/bin/linux:$PATH

. $(dirname "${BASH_SOURCE[0]}")/enforce.sh

# this prepares the VM for multiarch docker
docker run --rm --privileged multiarch/qemu-user-static:register --reset
bdde "echo this just pulls the image"

# now we can bootstrap and build just like normal, but it is in the container
# and avoid any permissions issues
chmod -R 777 /home/travis/build
BOOST_STEM=boost bdde "./bootstrap.sh"
BOOST_STEM=boost bdde "b2 libs/$SELF toolset=$B2_TOOLSET cxxstd=$B2_CXXSTD $B2_CXXFLAGS $B2_DEFINES $B2_INCLUDE $B2_LINKFLAGS $B2_TESTFLAGS $B2_ADDRESS_MODEL $B2_LINK $B2_THREADING $B2_VARIANT -j${B2_JOBS} $*"
