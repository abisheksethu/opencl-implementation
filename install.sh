#!/bin/bash

POCL_DIR="$1"

echo "--> Checking opencl-implementation Project"

if [ $# -lt 1 ]
then
        echo "  *** Enter a opencl-implementation location as a argument"
        exit 3

elif [ ! -e "$POCL_DIR" ]
then
        echo "  *** opencl-implementation does not exist!"
        exit 4

elif [ ! -d "$POCL_DIR" ]
then
        echo "  *** Not a opencl-implementation"
        exit 5
else
        echo "  *** Path is valid"

fi

echo "--> Checking superuser permission"

if [ $(id -u) -ne 0 ]
  then echo "	*** run the script as superuser"
  echo "	"
  exit 6
fi

echo "--> Checking Internet Connection"

if [ $(nm-tool | grep "State: connected" | wc -l) -ne 1 ]
  then echo "	*** Connect to internet"
  	   echo "	"
  exit 7
fi

echo "--> apt-get dependencies for POCL"
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install gcc-4.9 g++-4.9 libz-dev libffi-dev autoconf libtool ruby1.8-dev libtinfo-dev
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9

echo "--> Installing cmake_3.7.1"
cd $1/packages/cmake_3.7.1
./bootstrap
make -j2
make install
cmake --version

echo "--> Installing hwloc-1.8"
cd $1/packages/hwloc-1.8
autoreconf -f -i
./configure
make -j2
make install

echo "--> Installing papi"
cd $1/packages/papi-5.4.1/src
./configure
make -j2
make install

echo "--> Configuration for POCL"
cd $1/pocl-0.11/
mkdir build
export PATH=$PATH:$1/packages/clang+llvm-3.6.0-armv7a-linux-gnueabihf/bin
cd $1/pocl-0.11/build
cmake ..

echo "--> Building POCL" 
make

echo "--> Installing POCL"
make install

echo "--> Configuration for ICD loader"
echo /usr/local/lib/libpocl.so.1.4.0 > /etc/OpenCL/vendors/pocl.icd

exit