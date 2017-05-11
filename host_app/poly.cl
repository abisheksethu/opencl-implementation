__kernel void poly(__global uint* input,__global uint* output)                                                  
{                                                                               
   int i = get_global_id(0);                                                    
   output[i] = (input[i] + input[i]);
}