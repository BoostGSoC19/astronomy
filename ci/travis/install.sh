#! /bin/bash
#
# Copyright 2017 - 2019 James E. King III
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Executes the install phase for travis
#
# If your repository has additional directories beyond
# "example", "examples", "tools", and "test" then you
# can add them in the environment variable DEPINST.
# i.e. - DEPINST="--include dirname1 --include dirname2"
#

set -ex

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
    unset -f cd
fi

. $(dirname "${BASH_SOURCE[0]}")/enforce.sh

function show_bootstrap_log
{
    cat bootstrap.log
}

export SELF=`basename $TRAVIS_BUILD_DIR`
cd ..
if [ "$SELF" == "interval" ]; then
    export SELF=numeric/interval
fi
if [ "$TRAVIS_BRANCH" == "master" ]; then
    export BOOST_BRANCH="master"
else
    export BOOST_BRANCH="develop"
fi
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update -q --init libs/headers
git submodule update -q --init tools/boost_install
git submodule update -q --init tools/boostdep
git submodule update -q --init tools/build
mkdir -p libs/$SELF
cp -r $TRAVIS_BUILD_DIR/* libs/$SELF
export BOOST_ROOT="`pwd`"
export PATH="`pwd`":$PATH
python tools/boostdep/depinst/depinst.py --include benchmark --include example --include examples --include tools $DEPINST $SELF

# If clang was installed from LLVM APT it will not have a /usr/bin/clang++
# so we need to add the correctly versioned llvm bin path to the PATH
if [ "${B2_TOOLSET%%-*}" == "clang" ]; then
    ver="${B2_TOOLSET#*-}"
    export PATH=/usr/lib/llvm-${ver}/bin:$PATH
    ls -ls /usr/lib/llvm-${ver}/bin || true
    hash -r || true
    which clang || true
    which clang++ || true

    # Additionally, if B2_TOOLSET is clang variant but CXX is set to g++
    # (it is on Travis CI) then boost build silently ignores B2_TOOLSET and
    # uses CXX instead
    if [ "${CXX}" != "clang"* ]; then
        echo "CXX is set to ${CXX} in this environment which would override"
        echo "the setting of B2_TOOLSET=clang, therefore we clear CXX here."
        export CXX=
    fi
fi

trap show_bootstrap_log ERR
if [[ "${B2_TOOLSET}" == "gcc-"* ]] || [[ "${B2_TOOLSET}" == "clang-"* ]]; then
    ./bootstrap.sh --with-toolset=${B2_TOOLSET%%-*}
else
    ./bootstrap.sh --with-toolset=${B2_TOOLSET}
fi
trap - ERR
./b2 headers
