
//  Creator: Aljosa Klajderic 
// Creation Date: 3.2 2019
//

#pragma once

#include "./device_module.h"
#include "./host_device_functions.h"

#include "./pfc_bitmap_3.h"
#include "FileSystemHelper.h"
#include <cstdint>
#include <cmath>

#include <sstream>
#include <vector>
#include<iostream>
#include <fstream>
#include <string>



int main () {
	std::vector<std::string> Folders;
	Folders.emplace_back("cuda_data");


#if DELETE_FILES
	std::cout << "removing the files ..." << std::endl;
	CleanTheFolders(Folders);
#endif 

#if ENABLE_STREAM
	
	std::size_t const size = BURST;
#else
	std::size_t const size = 1;
#endif
	const int Width = WIDTH;
    const int Height = HEIGHT;	

#if CUDA_INFO
	Cuda_Info();
#endif

 


#if ENABLE_STREAM
   pfc::BGR_4_t* d_p[size];
   pfc::BGR_4_t* h_p[size];
   cudaStream_t streams[size]; 
#else
   pfc::BGR_4_t* d_p;
   pfc::BGR_4_t* h_p;
#endif
   cudaSetDevice(0);
   std::chrono::nanoseconds d_cuda(0);
   int const count_cycle = NUMBER_OF_PICTURES/size;

   
#if ENABLE_STREAM
   for (int i = 0; i < size; i++) {
	   Errchk(cudaMalloc((void**)&d_p[i]    , sizeof(pix_t)*(HEIGHT*WIDTH)));
	   Errchk(cudaMallocHost((void**)&h_p[i], sizeof(pix_t)*(HEIGHT*WIDTH)));  // allocate pinned memory
	   Errchk(cudaStreamCreate(&streams[i]));  // allocate pinned memory
   }
#else
   Errchk(cudaMalloc(&d_p, sizeof(pix_t)*(HEIGHT*WIDTH)));

	#if PINNED_MEM
		#if WRTIE_COMBINED
			Errchk(cudaMallocHost(&h_p, sizeof(pix_t)*(HEIGHT*WIDTH), cudaHostAllocWriteCombined));  // allocate pinned memory with no cache protocol 
		#else
			Errchk(cudaMallocHost(&h_p, sizeof(pix_t)*(HEIGHT*WIDTH)));  // allocate pinned memory
		#endif
	#else
	   Errchk(cudaMalloc(&h_p, sizeof(pix_t)*(HEIGHT*WIDTH)));  // allocate pagged memory 
	#endif
#endif


#if SEC_APPROACH
   std::ifstream reader("ZoomValues.txt");
   if (!reader) {
	   std::cout << "Error: cannot open input file" << std::endl;
	   return -1;
   }

   size_t i = 0;
   float ZoomValues[NUMBER_OF_PICTURES] = { 0 };
   std::string item[NUMBER_OF_PICTURES];
   while (!reader.eof() && i < NUMBER_OF_PICTURES) {
		  
		   std::getline(reader, item[i], '\n');
		   ZoomValues[i] = (float)::atof(item[i].c_str());
		   //std::cout << ZoomValues[i] << std::endl;
		   i++;
   }
   reader.close();
#else
	float zoomLevel = 0.01f;
#endif



   for (int x = 0; x< count_cycle; x++) {
	   size_t start = size * x;

#if  SEC_APPROACH
#if  ENABLE_STREAM
	   d_cuda += Calculate_GPU_Time(&h_p[start], &d_p[start], streams, size, &ZoomValues[start]);
#else
	   d_cuda += Calculate_GPU_Time(h_p, d_p, size, ZoomValues[x]);
#endif
#else
	   d_cuda += Calculate_GPU_Time(h_p, d_p, size, zoomLevel);
	   zoomLevel += 0.063f;


#endif

	   
#if  SAVE_IMAGES
	   WriteImagetoFile(Folders[0], h_p, Width, Height, size, start);
#endif
   }


   std::cout << "Cuda Time:\t\t\t\t"; GetTimeInfo(std::cout, d_cuda);
   //std::cout << "Calc MB per msec:\t\t\t"<< GetMBpPerSec(d_cuda, (21*1024))<<" MB/ms"<< std::endl; // 21 GB --> 21*1024
   getLastCUDAError();   
   std::cout << "finished" << std::endl;


   //clean up
   cudaFree(d_p);
#if PINNED_MEM
   cudaFreeHost(h_p);
#else
   cudaFree(h_p);
#endif


}
