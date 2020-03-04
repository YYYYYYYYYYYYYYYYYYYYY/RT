#ifndef KERNEL_H
#define KERNEL_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <xmmintrin.h>
//#include <smmintrin.h>
#ifdef CUDABUILD
#include "device_launch_parameters.h"
#include "cuda_runtime.h"
#define CUDA_HD __host__ __device__
#define CUDA_D __device__
#define CUDA_H __host__
#define CUDA
/*
#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
	if (result) {
		std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
			file << ":" << line << " '" << func << "' \n" << cudaGetErrorString(result) << "\n";
		// Make sure we call CUDA Device Reset before exiting
		cudaDeviceReset();
		throw "Cuda got error";
	}
}
*/
#else
#define CUDA_HD
#define CUDA_D
#define CUDA_H
#endif

#define MXV 1e3

//#define MAX_DEPTH 10
#define MAX_OBJECTS 65536

#define TREE_DENSITY 55

#define DEFAULT_COST 1.0
#define INTERSECT_COST 3.0

#define EPS 1e-7

#endif