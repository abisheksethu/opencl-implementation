# opencl-implementation
This project describes the implementation of OpenCL standard for Zedboard using Portable Computing Language, [POCL](http://portablecl.org). This implementation supports both CPU and FPGA as a OpenCL device. OpenCL Implementation for FPGA includes supporting OpenCL-C APIs using xillybus project. 

## Table of Contents
+ [General information](#info) 
    + [OpenCL](#info-opencl)
    + [POCL](#info-pocl)
    + [Xilinux Distribution](#info-xilinux)
+ [Pre-Requisites](#prereq) 
    + [Zedboard Pre-Requisites](#prereq-zb)
	+ [POCL Pre-Requisites](#prereq-pocl)
+ [Adding new device in POCL](#pocldev) 
+ [OpenCL Host Application Example](#hostapp) 

  
## <a name="info"></a> 1) General Information
### <a name="info-opencl"></a> 1.1: OpenCL
Open Computing Language, OpenCL is a Open royalty-free standard for general purpose parallel programming across CPUs, GPUs and accelerators. It enables software developers to take full advantage of heterogeneous processing platforms.

### <a name="info-pocl"></a> 1.2: POCL
Portable Computing Language, POCL is a MIT-licensed open source implementation of OpenCL standard. It can be easily adapted for new devices. OpenCL Compiler option are supported by using 
* Clang as an OpenCL C Frontend
* LLVM for kernel compiler implementation
* Easily targeted for a device with LLVM backend

### <a name="info-xilinux"></a> 1.3: Xilinux Distribution
This project is implemented and tested using Xilinux distribution. Xilinux Distribution is a demo kit with ubuntu 12.04 for Processing System (PS) and xillybus IP core for Programmable Logic (PL). The PS can send and receive data to Programmable Logic part using linux drivers.

## <a name="prereq"></a> 2) Pre-Requisites
### <a name="prereq-zb"></a> 2.1: Zedboard Pre-Requisites

* **Booting Xilinux on zedboard:**
In order to install the xilinux on zedboard with the base xillybus bitstream please follow the instructions given in the "Getting Started" guide provided in the <a href="http://xillybus.com/xillinux" target="_blank">xillybus website</a>

* **Xillybus bitstream:** With the xillybus IP core kit, Any application logic can be connected to plain FIFO's xillybus in PL. 

### <a name="prereq-pocl"></a> 2.2: POCL Pre-Requisites
In this project, we use pocl-0.11 version with llvm-3.6. To support llvm-3.6, host c++ toolchain version should be greater than gcc-3.7. Following are the required dependencies need to be installed before pocl.

* LLVM, Clang and compiler-rt(pre-compiled binaries available)
* Host compiler toolchain version: gcc-4.9 g++-4.9
* libhwloc-dev 1.8 (available only when compiled from source)
* ocl-icd 2.2.10 (available only when compiled from source)
* To build software package: cmake-3.7
* other dependencies like libz-dev, libffi-dev, autoconf, libtool, ruby1.8-dev, libtinfo-dev

Reference wiki page: <a href="https://github.com/umaurmi/OPENCL_EXAMPLES_ZEDBOARD/wiki/Installing-POCL-dependencies-on-Ubuntu-linux-based-targets" target="_blank">Installing-POCL-dependencies-on-Ubuntu-linux-based-targets</a>

All dependency packages are stored in this repository, we also developed a script file to install necessary packages with pocl-0.11. The script file configures the default host toolchain as gcc-4.9 and g++-4.9

## <a name="pocldev"></a> 3) Adding new device in POCL

**ACCELERATOR as a device**: 

* The POCL software architecture has all its device layer implementation in pocl-0.11/lib/CL/devices. The CPU device implementation can be found in pthread. Similarly, POCL has provided a basic device implementation to add our custom device, which can be customized in pocl-0.11/lib/CL/devices/basic/basic.c. 
* We use xillybus linux drivers to develop basic device layer in POCL.
* Currently, we support two OpenCL APIs clEnqueueWriteBuffer, clEnqueueReadBuffer. These APIs are independent of OpenCL Compiler implementation.
* In POCL's basic layer, The above two API's corresponding hardware definition can be found in pocl_basic_read() and pocl_basic_write().
* We configured the device name as "xillybus" and device type as CL_DEVICE_TYPE_ACCELERATOR.
* After compiling POCL using host toolchain, we get the implementation as a shared object in /usr/local/lib/libpocl.so, which supports both CPU and ACCELERATOR as a device.

## <a name="hostapp"></a> 4) OpenCL C Application Example
The customized OpenCL C APIs are tested using host_app/pocl_test.c, a OpenCL C application. The given application can be compiled using pocl library. These APIs are profiled using performance API (PAPI). This can be executed on CPU and FPGA device by exporting respective device name for POCL_DEVICES variable. 

The example uses a OpenCL Kernel that adds the input to itself. The respective xillybus bitstream is available in /xillybus directory.

* **CPU as a device** : POCL_DEVICES=pthread 

* **FPGA as a device** : export POCL_DEVICES=xillybus

The script hostapp/run.sh automates the test case for variable input length of 1K to 16K. The comparision of two APIs for pthread and xillybus are shown below. 

**Timing graph for clEnqueueWriteBuffer**
![alt tag](https://github.com/abisheksethu/opencl-implementation/blob/master/host_app/clEnqueueWriteBuffer.jpg?raw=true)

**Timing graph for clEnqueueReadBuffer**
![alt tag](https://github.com/abisheksethu/opencl-implementation/blob/master/host_app/clEnqueueReadBuffer.jpg?raw=true)