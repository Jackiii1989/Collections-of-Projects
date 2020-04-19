//           $Id: host_device_functions.h 37993 2018-10-28 09:05:09Z p20068 $
//          $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/vocational/teaching/ESD/SPS3/2016-WS/ILV/src/file-structure/src/host_device_functions.h $
//     $Revision: 37993 $
//         $Date: 2018-10-28 10:05:09 +0100 (So., 28 Okt 2018) $
//       $Author: p20068 $
//
//       Creator: Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
// Creation Date:
//     Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//
//       License: This document contains proprietary information belonging to
//                University of Applied Sciences Upper Austria, Campus
//                Hagenberg. It is distributed under the Boost Software License,
//                Version 1.0 (see http://www.boost.org/LICENSE_1_0.txt).
//
//    Annotation: This file is part of the code snippets handed out during one
//                of my HPC lessons held at the University of Applied Sciences
//                Upper Austria, Campus Hagenberg.

#pragma once

#include <stdio.h>
#include <chrono>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "./pfc_bitmap_3.h"

#include "device_functions.h"
#include "./device_module.h"
#include "thread.h"
#include "time_func.h"
#include "Macros.h"


#if defined __CUDACC__
   #define GPU_ENABLED __device__ __forceinline__
#else
   #define GPU_ENABLED inline
#endif



GPU_ENABLED void getLastCUDAError() {
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "[GPU assert]: %s\n", cudaGetErrorString(cudaStatus));
	}

}

#define Errchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
GPU_ENABLED void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
#if _DEBUG // works only if we run in the debug mode
	if (code != cudaSuccess)
	{
		fprintf(stderr, "[GPU assert]: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
#endif
}


GPU_ENABLED void Cuda_Info(void)
{
	int count{};
	cudaGetDeviceCount(&count);
	cudaSetDevice(0);
	cudaDeviceProp prop{};  cudaGetDeviceProperties(&prop, 0);
	std::cout << "------------------------------------------------------------" << std::endl;
	std::cout << "Device:\t\t\t" << prop.name << std::endl;
	std::cout << "Total global memory:\t" << ((float)prop.totalGlobalMem / 1024 / 1024) << " MB" << std::endl;
	std::cout << "Total shared memory:\t" << ((float)prop.sharedMemPerBlock / 1024) << " KB" << std::endl;
	std::cout << "size of a warp:\t\t" << prop.warpSize << " threads" << std::endl;
	std::cout << "stream enabled:\t\t" << (ENABLE_STREAM ? "yes" : "no") << std::endl;
	std::cout << "loop unrolling on GPU:\t" << (LOOPUNROLLING ? "yes" : "no") << std::endl;
	std::cout << "------------------------------------------------------------" << std::endl;
	getLastCUDAError();

}



GPU_ENABLED float MapToReal(int x,pfc::long_t ImageWidth,float rmax, float rmin) {

	float range = rmax - rmin;
	return x * (range / ImageWidth) + rmin;

}

GPU_ENABLED float MapToImaginary(int y, pfc::long_t ImageHeight, float imax, float imin) {
	float range = imax - imin;
	return y *(range / ImageHeight) + imin;
}

GPU_ENABLED int findMandelBrot(float cr, float ci, size_t maxIter) {

	size_t i = 0;
	float zr = 0.0, zi = 0.0;
	while (i < maxIter && zr*zr + zi * zi < 4.0) {
		float temp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		zi = 2.0f * zr * zi + ci;
		zr = temp; //now it is okay
		i++;
	}
	return static_cast<int>(i);
}


GPU_ENABLED void Fractal_Carpat_CPU(pfc::BGR_4_t* p_CPU ,float rmax, float rmin, float imax, float imin, size_t maxIter)
{

	int height = HEIGHT;
	int width = WIDTH;
	int x, y; // Pixel counters 
	float cr, ci; // constants for real and imanginary
	pfc::byte_t k; // Iteration counter 

	for (y = 0; y < height; y++) { // rows
		ci = MapToImaginary(y, height, imax, imin); // map y to the imaginary parf of value
		for (x = 0; x < width; x++) { // pixels in row
			cr = MapToReal(x, width, rmax, rmin); // map x to real part of the value

			k = findMandelBrot(cr, ci, maxIter); // here the main calculation is going on
#if DEBUG_CPU
			if (((y*width) + x) % 1000 == 0) {
				//printf("CPU: t = %i/%i, k = %i, xr = %f, ci = %f, x = %i, y = %i, iter = %i \n", ((y*width) + x), width*height, k, cr, ci, x, y, (int)maxIter);
				printf("Thread %i /size=%i, x=%d,y=%d, k=%i, \n", (y*width) + x, height*width, x, y, k);
			}
#endif DEBUG
			// compute  pixel color and write it to file 
			p_CPU[(y*width) + x] = {
			   static_cast<pfc::byte_t>((int)(k*cosf(k)) % 128),
			   static_cast<pfc::byte_t>((int)(k*sinf(k)) % 128),
			   static_cast<pfc::byte_t>((int)(k*cosf(k)*sinf(k)) % 128)
			};
		}
	}
}

GPU_ENABLED std::chrono::steady_clock::duration Calculate_CPU_Time(pfc::BGR_4_t* p_CPU, float rtmax, float rtmin, float itmax, float itmin,
												size_t maxIter, float rcenter, float icenter, float divider, float inv_divider)
{

	std::cout << "task started" << std::endl;
	size_t size = 1;
	auto d_task = timed_run(1, [&]() {

		thread_group task;
		for (unsigned int n = 0; n < size; n++)
		{
		
			task.add([&]() {
				Fractal_Carpat_CPU(p_CPU, rtmax, rtmin, itmax, itmin, maxIter);
			});

		}
	});
	std::cout << "task finished" << std::endl;
	return d_task;
}


GPU_ENABLED std::chrono::steady_clock::duration Calculate_GPU_Time(pfc::BGR_4_t *h_p, pfc::BGR_4_t *d_p, size_t size, float ZoomLevel)
{
#if INFO
	std::cout << "number of Iterations: " << size << std::endl;
	std::cout << "cuda calculation started... " << std::endl;
# endif


	auto d_cuda = timed_run(1, [&]() {
		//calculating
		for (unsigned int n = 0; n < size; n++)
		{

			Fractal_Carpat_GPU(d_p, ZoomLevel);

#if INFO
			std::cout << " [cudaMemcpy] copied form CUDA to Host: Image " << n << std::endl;
#endif


			Errchk(cudaMemcpy(h_p, d_p, (HEIGHT*WIDTH) * sizeof(pfc::BGR_4_t), cudaMemcpyDeviceToHost));
		}

	});
	return d_cuda;
}

GPU_ENABLED std::chrono::steady_clock::duration Calculate_GPU_Time(pix_t *h_p[], pix_t *d_p[], cudaStream_t streams[], size_t size, float* ZoomLevel)
{
#if INFO
	std::cout << "cuda calculation started... number of Iterations: " << size << std::endl;
# endif

	auto d_cuda = timed_run(1, [&]() {
		//calculating
		for (unsigned int n = 1; n < size/2; n=n+2)
		{
			Fractal_Carpat_GPU(d_p[n], streams[n], ZoomLevel[n]);
		//}
		//for (size_t i = 0; i < size; i++)
		//{
#if INFO
			std::cout << " [cudaMemcpy] coping form CUDA to Host: Image " << n << std::endl;
#endif
			Errchk(cudaMemcpyAsync(&h_p[n], &d_p[n], (HEIGHT*WIDTH) * sizeof(pix_t), cudaMemcpyDeviceToHost, streams[n+1]));

			//cudaStreamSynchronize(streams[i]);

		}
		//Errchk(cudaDeviceSynchronize()); // wait until of the streams have been completed
	});
#if INFO
	std::cout << "cuda finished" << std::endl;
#endif
	return d_cuda;
}


#define ONE_MBYTE (1024*1024)

void HowMuchMemoryDoIHave() {
    
	size_t free_byte;
	size_t total_byte;
	Errchk(cudaMemGetInfo(&free_byte, &total_byte));
	double free_db = (double)free_byte;
	double total_db = (double)total_byte;
	double used_db = total_db - free_db;

	printf("[GPU memory usage]: used = %.2f MB, free = %.2f MB, total = %.2f MB\n", used_db / ONE_MBYTE, free_db / ONE_MBYTE, total_db / ONE_MBYTE);
}
