#!bin/bash/

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
cd ./packages/cmake_3.7.1
./bootstrap
make -j2
make install
cmake --version

echo "--> Installing hwloc-1.8"
cd ./packages/hwloc-1.8
autoreconf -f -i
./configure
make -j2
make install

echo "--> Installing papi"
cd ./packages/papi-5.4.1
./configure
make -j2
make install

echo "--> Configuration for POCL"
cd ./pocl-0.11/
mkdir build
export PATH=$PATH:./packages/clang+llvm-3.6.0-armv7a-linux-gnueabihf/bin
cd ./pocl-0.11/build
cmake ..

echo "--> Building POCL" 
make

echo "--> Installing POCL"
make install

echo "--> Configuration for ICD loader"
echo /usr/local/lib/libpocl.so.1.4.0 > /etc/OpenCL/vendors/pocl.icd

exit