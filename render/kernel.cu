//#ifdef CUDABUILD
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
//#endif
#include "Vector3.h"
#include "Quaternion.h"
#include <stdio.h>
#include <iostream>
 cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);

 /*
int main()
{
	
    return 0;
}*/