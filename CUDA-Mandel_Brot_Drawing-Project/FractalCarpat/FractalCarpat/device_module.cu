
#include "device_module.h"
//#include "host_device_functions.h"
#include<math.h>
#include "Macros.h"



__device__  float MapToImaginary1(int y, float imax, float imin) {
	float range = imax - imin;
	return y * (range / HEIGHT) + imin;
}

#define LIMIT 4.0
__device__  int findMandelBrot1(float cr, float ci) {

	size_t i = 0;
	float zr = 0.0, zi = 0.0;

#if LOOPUNROLLING
#pragma unroll (MAXITER)
	for (; i < MAXITER && zr*zr + zi * zi < LIMIT; i++) {
		//if (zr*zr + zi * zi < LIMIT) { break; }
		float temp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		zi = 2.0f * zr * zi + ci;
		zr = temp; //now it is okay
		
	}
#else
	while (zr*zr + zi * zi < LIMIT && i < MAXITER) {
		float temp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		zi = 2.0f * zr * zi + ci;
		zr = temp; //now it is okay
		//if (zr*zr + zi * zi < LIMIT) { break; }
		i++;
	}
#endif
	return static_cast<int>(i);
}


//__global__  void call_kernel(pfc::BGR_4_t * bmp, ImageMetaData* md) {
__global__  void call_kernel(pix_t * bmp, float zoomLevel) {

	auto const x{ blockIdx.x * blockDim.x + threadIdx.x };
	auto const y{ blockIdx.y * blockDim.y + threadIdx.y };



	
	//pfc::byte_t k; // Iteration counter 
	//ci = MapToImaginary1(y, IMAX, IMIN); // map y to the imaginary parf of value
	//cr = MapToReal1(x, RMAX, RMIN); // map x to real part of the value
#if  SEC_APPROACH
#if INTRISIC
	float rmax = RCENTER - (__fdividef(RMIN, zoomLevel));
	float rmin = RCENTER + (__fdividef(RMIN, zoomLevel));
	float imax = ICENTER - (__fdividef(RMIN, zoomLevel));
	float imin = ICENTER + (__fdividef(RMIN, zoomLevel));
#else
	
	float rmax = RCENTER - (RMIN / zoomLevel);
	float rmin = RCENTER + (RMIN / zoomLevel);
	float imax = ICENTER - (RMIN / zoomLevel);
	float imin = ICENTER + (RMIN / zoomLevel);
	
#endif
#else
	float rmax = RCENTER - (RMIN / exp2f(zoomLevel));
	float rmin = RCENTER + (RMIN / exp2f(zoomLevel));
	float imax = ICENTER - (RMIN / exp2f(zoomLevel));
	float imin = ICENTER + (RMIN / exp2f(zoomLevel));
#endif
	float cr=  x * ((rmax - rmin) / WIDTH) + rmin;
	float ci = y * ((imax - imin) / HEIGHT) + imin;

	//float cr = x * ((RMAX - RMIN) / WIDTH) + RMIN;
	//float ci = y * ((IMAX - IMIN) / HEIGHT) + IMIN;
	//float cr = x * ((md->rmax - md->rmin) / WIDTH) + md->rmin;
	//float ci = y * ((md->imax - md->imin) / HEIGHT) + md->imin;
# if DEBUG_GPU
	int point = y * WIDTH + x;

	if (point == 0) {
#if  SEC_APPROACH
		printf("Thread %i, ,zoom=%f \n", point, zoomLevel);
#else
		printf("Thread %i, ,zoom=%f \n", point, exp2f(zoomLevel));		
#endif
		//printf("Thread %i ,threadIdx.y=%i,block x=%i, block y=%i,width=%i, height=%i, imax=%i ,imin=%f, rmax=%f ,rmin=%f, \n", point, threadIdx.y, blockDim.x, blockDim.y, WIDTH, HEIGHT, (int)md->imax, md->imin, md->rmax, md->rmin);
	}
#endif
	pfc::byte_t  i = 0;
	float zr = 0.0, zi = 0.0;

#if LOOPUNROLLING
	#pragma unroll (NMBLOOPS2UNROLL)
#endif
#if FORLOOP
	for (; i < MAXITER && zr*zr + zi * zi < LIMIT; i++) {
#if PERIODICITY_CHECKING
		float rtemp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		float itemp = 2.0f * zr * zi + ci;
		if (zi == itemp && zr == rtemp) {
			i = MAXITER;
			break;
		}
		zi = itemp;
		zr = rtemp; //now it is okay

#else
		//float zi2 = zi * zi;				 // Instruction Parallelisem 
		float temp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		zi = 2.0f * zr * zi + ci;
		zr = temp; //now it is okay
#endif
	}
#else
	while (zr*zr + zi * zi < LIMIT && i < MAXITER) {
#if PERIODICITY_CHECKING
		float rtemp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		float itemp = 2.0f * zr * zi + ci;
		if (zi == itemp && zr == rtemp) {
			i = MAXITER;
			break;
		}
		zi = itemp;
		zr = rtemp; //now it is okay
		i++

#else
		float temp = zr * zr - zi * zi + cr; // need save the value in temp in order to not overwrite the old value
		zi = 2.0f * zr * zi + ci;
		zr = temp; //now it is okay
		i++;
#endif
	}
#endif
	// compute  pixel color and write it to file 
	bmp[y * WIDTH + x] = {
		static_cast<pfc::byte_t>(((2 * i))),
		static_cast<pfc::byte_t>(((3 * i))),
		static_cast<pfc::byte_t>((3 * i))
	};
}


 void  Fractal_Carpat_GPU(pfc::BGR_4_t * bmp, float zoomLevel) {

	dim3 ThreadsPerBlock(THREADSPERBLOCK, THREADSPERBLOCK);
	dim3 blocksperGrid(WIDTH /ThreadsPerBlock.x, HEIGHT /ThreadsPerBlock.y);
	call_kernel << <blocksperGrid, ThreadsPerBlock >> > (bmp, zoomLevel);

}

 void  Fractal_Carpat_GPU(pix_t * bmp, cudaStream_t stream, float zoomLevel) {

  dim3 ThreadsPerBlock(THREADSPERBLOCK, THREADSPERBLOCK);
  dim3 blocksperGrid(WIDTH / ThreadsPerBlock.x, HEIGHT / ThreadsPerBlock.y);
  call_kernel << <blocksperGrid, ThreadsPerBlock, 0, stream >> > (bmp, zoomLevel);

 }


