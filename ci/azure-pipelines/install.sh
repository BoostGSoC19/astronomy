#! /bin/bash
#
# Copyright 2017 - 2019 James E. King III
# Copyright 2019 Mateusz Loskot <mateusz at loskot dot net>
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Executes the install phase for Azure Pipelines (AzP)
#
# This was ported from ci/travis/install.sh following
# this official guide:
# https://docs.microsoft.com/en-us/azure/devops/pipelines/migrate/from-travis
#
# If your repository has additional directories beyond
# "example", "examples", "tools", and "test" then you
# can add them in the environment variable DEPINST.
# i.e. - DEPINST="--include dirname1 --include dirname2"
#
# AzP requires to run special task in order to export
# SELF and BOOST_ROOT as job-scoped variable from a script.
# Follow invocation of this install.sh in .azure-pipelines.yaml
# with these two lines:
#
#  set +x
#  echo "##vso[task.setvariable variable=SELF]$SELF"
#  echo "##vso[task.setvariable variable=BOOST_ROOT]$BOOST_ROOT"
#  set -x
#
# NOTE: The set +x is required! See the troubleshooting guide:
# https://docs.microsoft.com/en-us/azure/devops/pipelines/troubleshooting#variables-having--single-quote-appended
#
set -ex

if [ "$AGENT_OS" == "Darwin" ]; then
    unset -f cd
fi

. $(dirname "${BASH_SOURCE[0]}")/enforce.sh

function show_bootstrap_log
{
    cat bootstrap.log
}

# AzP does not clone into folder named after repository,
# but into one named something like /home/vsts/work/1/s
# SELF needs to be derived from the name of the repository
# that this build is configured for.
export SELF=`basename $BUILD_REPOSITORY_NAME`

# e.g. BUILD_SOURCESDIRECTORY=/home/vsts/work/1/s
# change from /home/vsts/work/1/s to /home/vsts/work/1
cd ..
if [ "$SELF" == "interval" ]; then
    export SELF=numeric/interval
fi

# CI builds set BUILD_SOURCEBRANCHNAME
# Pull request builds set SYSTEM_PULLREQUEST_TARGETBRANCH.
# We want to build PRs against develop branch anyway.
if [ "$BUILD_SOURCEBRANCHNAME" == "master" ]; then
    export BOOST_BRANCH="master"
else
    export BOOST_BRANCH="develop"
fi
# e.g. clone into /home/vsts/work/1/boost-root
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update -q --init libs/headers
git submodule update -q --init tools/boost_install
git submodule update -q --init tools/boostdep
git submodule update -q --init tools/build
mkdir -p libs/$SELF
cp -r $BUILD_SOURCESDIRECTORY/* libs/$SELF
# e.g. move /home/vsts/work/1/boost-root
# back inside BUILD_SOURCESDIRECTORY=/home/vsts/work/1/s
cd ..
mv boost-root $BUILD_SOURCESDIRECTORY/
cd $BUILD_SOURCESDIRECTORY/boost-root
# NOTE: AzP images come with predefined BOOST_ROOT that can not be overwritten
# https://gitlab.kitware.com/cmake/cmake/issues/19056
# Set custom variable here, then export as 
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
    # (it is on Linux images) then boost build silently ignores B2_TOOLSET and
    # uses CXX instead
    if [ "${CXX}" != "clang"* ]; then
        echo "CXX is set to ${CXX} in this environment which would override"
        echo "the setting of B2_TOOLSET=clang, therefore we clear CXX here."
        export CXX=
    fi
fi

trap show_bootstrap_log ERR
./bootstrap.sh --with-toolset=${B2_TOOLSET%%-*}
trap - ERR
./b2 headers

# AzP official images of Ubuntu 16.04 behave differently to Travis CI.
# The gcc and clang variants are being installed somewhat weirdly
# and, unlike on linux images on Travis CI, b2 fails with:
#
# /home/vsts/work/1/s/boost-root/tools/build/src/tools/gcc.jam:230: in gcc.init from module gcc
# error: toolset gcc initialization:
# error: no command provided, default command 'g++' not found
# error: initialized from ../../project-config.jam:12
#
# Hence, we work around this issue with user-config.jam
if ! command -v ${CXX}; then
    echo "WARNING: Compiler ${CXX} was not installed properly"
    #exit 1
fi
echo "using ${B2_TOOLSET} : : $(which ${CXX}) : ${B2_CXXFLAGS} ;" > ${HOME}/user-config.jam
