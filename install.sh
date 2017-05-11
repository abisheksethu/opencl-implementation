#bin/bash

POCL_DIR="$1"

echo "--> Checking POCL_DIRECTORY"

if [ $# -lt 1 ]
then
        echo "	*** Please enter a POCL_DIRECTORY location as a argument"
        exit 3

elif [ ! -e "$POCL_DIR" ]
then
        echo "	*** POCL_DIRECTORY does not exist!"
        exit 4

elif [ ! -d "$POCL_DIR" ]
then
        echo "	*** Not a POCL_DIRECTORY"
        exit 5
else
        echo "	*** Path is valid"

fi

echo "--> Checking superuser permission"

if [ $(id -u) -ne 0 ]
  then echo "	*** Please run the script as superuser"
  echo "	"
  exit 6
fi

echo "--> Checking Internet Connection"

if [ $(nm-tool | grep "State: connected" | wc -l) -ne 1 ]
  then echo "	*** Please Connect to internet"
  	   echo "	"
  exit 7
fi

echo "--> Cloning OpenCL-implementation repository"
cd $1
git clone https://github.com/abisheksethu/opencl-implementation

echo "--> apt-get dependencies for POCL"
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install git gcc-4.9 g++-4.9 libz-dev libffi-dev autoconf libtool ruby1.8-dev libtinfo-dev
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

echo "--> Installing ocl-icd-2.2.10"
cd $1/packages/ocl-icd-2.2.10
autoreconf -f -i
./configure
make -j2
make install

echo "--> Installing clang+llvm-3.6.0-armv7a-linux-gnueabihf"
cd $1/packages/clang+llvm-3.6.0-armv7a-linux-gnueabihf

echo "--> Configuration for POCL"
echo /usr/local/lib/libpocl.so.1.7.0 > /etc/OpenCL/vendors/pocl.icd
mkdir $POCL_DIR/pocl-0.11/build
export PATH+=:$1/packages/clang+llvm-3.6.0-armv7a-linux-gnueabihf/bin
cd $1/build
cmake ..

echo "--> Building POCL" 
make

echo "--> Installing POCL"
make install

exit 8
