#pragma once

//macro definitions
#define INFO				0
#define CUDA_INFO			1
#define DEBUG_GPU			0
#define DEBUG_CPU		    0


#define DELETE_FILES		1
#define SAVE_IMAGES			1

#define SEC_APPROACH		1

#define PINNED_MEM			1
#define WRTIE_COMBINED		0

#define ENABLE_STREAM		0
#define BURST				10

#define NUMBER_OF_PICTURES  20


#define	PERIODICITY_CHECKING 0 
#define INTRISIC			 0
#define LOOPUNROLLING		 0

#define ALIGN			     0

#define NMBLOOPS2UNROLL		 128
#define MAXITER				 250
#define WIDTH				 8192
#define HEIGHT				 4608 


#define IMAX				 1.23807502f
#define IMIN				 -1.01192498f
#define RMAX				 1.25470996f
#define RMIN				 -2.74529004f
#define ICENTER				 0.113075003f
#define RCENTER				 -0.745289981f

#define IMG_SPACE_I				(IMAX-IMIN)
#define FORLOOP				 1

#define THREADSPERBLOCK		32  // sqrt(1024) = 32
#define BlocksX			    WIDTH/THREADSPERBLOCK 
#define BlocksY				HEIGHT/THREADSPERBLOCK
 

#if defined(__CUDACC__) // NVCC
#define MY_ALIGN(n) __align__(n)
#elif defined(__GNUC__) // GCC
#define MY_ALIGN(n) __attribute__((aligned(n)))
#elif defined(_MSC_VER) // MSVC
#define MY_ALIGN(n) __declspec(align(n))
#else
#error "Please provide a definition for MY_ALIGN macro for your host compiler!"
#endif

//typedef MY_ALIGN(16) pfc::BGR_4_t pix_t;
#ifdef ALIGN
	typedef MY_ALIGN(16) pfc::BGR_4_t pix_t;
#else
	typedef pfc::BGR_4_t pix_t;
#endif // ALIGN

