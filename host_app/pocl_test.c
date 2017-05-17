#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "papi.h"
 
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define DATA_SIZE       (1024*16)

////////////////////////////////////////////////////////////////////////////////
// Function to extract cl kernel for execution
static char* load_program_source(const char *filename)
{
    struct stat statbuf;
    FILE        *fh;
    char        *source;
    int err;

    fh = fopen(filename, "r");
    if (fh == 0)
        return 0;

    stat(filename, &statbuf);
    source = (char *) malloc(statbuf.st_size + 1);
    err = fread(source, statbuf.st_size, 1, fh);
    source[statbuf.st_size] = '\0';
    fclose(fh);

    return source;
}
////////////////////////////////////////////////////////////////////////////////

int main()  {

    int err;                            // error code returned from api calls
    uint data[DATA_SIZE];              // original data set given to device
    uint results[DATA_SIZE];            // results returned from device
    uint check_results[DATA_SIZE];      // results calculated by the host
    unsigned int correct;                // number of correct results returned
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    cl_mem input;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    int i = 0;
    unsigned int count = DATA_SIZE;

    long long timer1 = 0;
    cl_event event;
    long long timer2 = 0;
    long long ptimer[10];
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    char p_vendor[1024];                   //this string will hold a platforms vendor
    char d_vendor[1024];                   //this string will hold a device vendor
    char deviceName[1024];              //this string will hold the devices name
    cl_uint numberOfCores;              //this variable holds the number of cores of on a device
    cl_bool available;              //this variable holds if the device is available
    
    int temp = atoi(getenv("DATA_SIZE"));
    count = 1024 * temp;
    for(i = 0; i < count; i++) {
        data[i] = i;
    }

    /* Get Platform and Device Info */
    err = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to get platform info! %d\n", err);
        exit(1);
    }

    err = clGetPlatformInfo (platform_id, CL_PLATFORM_VENDOR, sizeof(p_vendor), p_vendor, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve platform info! %d\n", err);
        exit(1);
    }

    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1, &device_id, &ret_num_devices);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve device info! %d\n", err);
        exit(1);
    }

    clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
    clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(d_vendor), d_vendor, NULL);
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(numberOfCores), &numberOfCores, NULL);
    clGetDeviceInfo(device_id, CL_DEVICE_AVAILABLE, sizeof(available), &available, NULL);

    /* Create OpenCL context */
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
     
    /* Create Command Queue */
    command_queue = clCreateCommandQueue(context, device_id,  CL_QUEUE_PROFILING_ENABLE, &err);

    //Use function and load the kernel source from .cl files in the same folder
    char *KernelSource = load_program_source("poly.cl");

    /* Timer start */   
    ptimer[0] = PAPI_get_virt_usec();

    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    ptimer[1] = PAPI_get_virt_usec();

    // Build the program executable
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    ptimer[2] = PAPI_get_virt_usec();

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "poly", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel! - %d\n",err);
        exit(1);
    }

    ptimer[3] = PAPI_get_virt_usec();

    // Create the input and output arrays in device memory for our calculation
    //
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * count, NULL, NULL);
    if (!input || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }  

    ptimer[4] = PAPI_get_virt_usec();

    // Write our data set into the input arrays in device memory 
    err = clEnqueueWriteBuffer(command_queue, input, CL_TRUE, 0, sizeof(int) * count, data, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    ptimer[5] = PAPI_get_virt_usec();
    
    // Set the arguments to our compute kernel
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    ptimer[6] = PAPI_get_virt_usec();

    // Get the maximum work group size for executing the kernel on the devices
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    ptimer[7] = PAPI_get_virt_usec();

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    global = count;
    err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global, NULL, 0, NULL, &event);
    if (err)
    {
        printf("Error: Failed to execute kernel!-%d\n",err);
        return EXIT_FAILURE;
    }

    ptimer[8] = PAPI_get_virt_usec();

    // Read back the results from the device to verify the output
    err = clEnqueueReadBuffer( command_queue, output, CL_TRUE, 0, sizeof(int) * count, results, 0, NULL, NULL );  
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    ptimer[9] = PAPI_get_virt_usec();

    // Validate our results
    correct = 0;
    for(i = 0; i < count; i++)
    {
        check_results[i] = data[i]+data[i];            
    }
    for(i = 0; i < count; i++)
    {        
        if(results[i] == check_results[i])//(2 * data1[i] * 3 * data2[i]) + 1)
        {
            // printf("Host Value %f\n", check_results[i]);
            // printf("Device Value %f\n", results[i]);
            correct++;
        }
    }
    // Print a brief summary detailing the results
    printf("\nPlatform:\t%u\n\n", ret_num_platforms);
    printf("\tPlatform Vendor:\t\t%s\n", p_vendor);
    printf("\tNo of Devices: \t\t\t%u\n", ret_num_devices);
    printf("\t\tName:\t\t\t\t%s\n", deviceName);
    printf("\t\tDevice Vendor:\t\t\t%s\n", d_vendor);
    printf("\t\tAvailable:\t\t\t%s\n", available ? "Yes" : "No");
    printf("\t\tCompute Units:\t\t\t%u\n", numberOfCores);
    printf("\tComputed '%d/%d' correct values!\n", correct, count);
    // printf("\tTime elapsed is (PAPI) %llu usec \n",(ptimer2-ptimer1));

    /* Finalization */
    err = clFlush(command_queue);
    err = clFinish(command_queue);
    err = clReleaseMemObject(input);
    err = clReleaseMemObject(output);
    err = clReleaseCommandQueue(command_queue);
    err = clReleaseContext(context);

    /* Data logging */
    FILE *fp;
    char str[80];
    long long timer_diff;    
    fp=fopen("report.txt", "a");
    
    sprintf(str, "%u, ", count);
    fputs(str, fp);     
    for(i = 1; i < 10 ; i++){
            timer_diff = ptimer[i]-ptimer[i-1];
            sprintf(str, "%llu, ", timer_diff);
            fputs(str, fp);
    }
    fputc('\n',fp);
    fclose(fp);
    return 0;

}
