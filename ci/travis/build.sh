#! /bin/bash
#
# Copyright 2017 - 2019 James E. King III
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Bash script to run in travis to perform a bjam build
# cwd should be $BOOST_ROOT/libs/$SELF before running
#

set -ex

. $(dirname "${BASH_SOURCE[0]}")/enforce.sh

$BOOST_ROOT/b2 . toolset=$B2_TOOLSET cxxstd=$B2_CXXSTD $B2_CXXFLAGS $B2_DEFINES $B2_INCLUDE $B2_LINKFLAGS $B2_TESTFLAGS $B2_ADDRESS_MODEL $B2_LINK $B2_THREADING $B2_VARIANT -j${B2_JOBS} $*
