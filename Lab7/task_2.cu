#include <stdio.h>
#include <math.h>
#include "utils/utils.h"

// TODO 6: Write the code to add the two arrays element by element and 
// store the result in another array
__global__ void add_arrays(const float *a, const float *b, float *c, int N) {
	unsigned int i = threadIdx.x + blockDim.x * blockIdx.x;

	if (i < N)
		c[i] = a[i] + b[i];    
}

int main(void) {
    cudaSetDevice(0);
    int N = 1 << 20;

    float *host_array_a = 0;
    float *host_array_b = 0;
    float *host_array_c = 0;

    float *device_array_a = 0;
    float *device_array_b = 0;
    float *device_array_c = 0;

    // TODO 1: Allocate the host's arrays
    host_array_a = (float *)malloc(N * sizeof(float));
    host_array_b = (float *)malloc(N * sizeof(float));
    host_array_c = (float *)malloc(N * sizeof(float));

    // TODO 2: Allocate the device's arrays
    cudaMalloc((void **) &device_array_a, N * sizeof(float));	
    cudaMalloc((void **) &device_array_b, N * sizeof(float));
    cudaMalloc((void **) &device_array_c, N * sizeof(float));
    
    // TODO 3: Check for allocation errors
    if (host_array_a == 0 || host_array_b == 0 || host_array_c == 0 ||
	device_array_a == 0 || device_array_b == 0 || device_array_c == 0 ) {
    	printf("Memory allocation failed\n");
	return 1;
    }
    // TODO 4: Fill array with values; use fill_array_float to fill
    // host_array_a and fill_array_random to fill host_array_b. Each
    // function has the signature (float *a, int n), where n = number of elements.
    fill_array_float(host_array_a, N);
    fill_array_random(host_array_b, N);    

    // TODO 5: Copy the host's arrays to device
    int num_bytes = N * sizeof(float);

    cudaMemcpy(device_array_a, host_array_a, num_bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(device_array_b, host_array_b, num_bytes, cudaMemcpyHostToDevice);

    // TODO 6: Execute the kernel, calculating first the grid size
    // and the amount of threads in each block from the grid
    // Hint: For this execise the block_size can have any value lower than the
    //      API's maximum value (it's recommended to be close to the maximum
    //      value).
    int block_size = 256;
    size_t blocks_no = N / block_size;

    if (N % block_size)
	++blocks_no;

    add_arrays<<<blocks_no, block_size>>>(device_array_a, device_array_b, device_array_c, N);


    // TODO 7: Copy back the results and then uncomment the checking function

    cudaMemcpy(host_array_c, device_array_c, num_bytes, cudaMemcpyDeviceToHost);
    check_task_2(host_array_a, host_array_b, host_array_c, N);

    // TODO 8: Free the memory
    free (host_array_a); 
    free (host_array_b); 
    free (host_array_c);

    cudaFree(device_array_a); 
    cudaFree(device_array_b); 
    cudaFree(device_array_c); 
    return 0;
}
