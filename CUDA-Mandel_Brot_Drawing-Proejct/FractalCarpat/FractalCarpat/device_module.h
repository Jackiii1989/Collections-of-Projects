//           $Id: device_module.h 37993 2018-10-28 09:05:09Z p20068 $
//          $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/vocational/teaching/ESD/SPS3/2016-WS/ILV/src/file-structure/src/device_module.h $
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

#include <device_launch_parameters.h>
#include "pfc_types.h"
#include "cuda_runtime.h"
#include<cstddef>
#include <stdio.h>
#include "device_functions.h"
#include "./device_module.h"
#include "Macros.h"


 //void call_kernel(pfc::BGR_4_t * bmp, std::size_t height, std::size_t width, float rmax, float rmin, float imax, float imin, size_t maxIter);
void Fractal_Carpat_GPU(pfc::BGR_4_t * bmp, float ZoomLevel);
void  Fractal_Carpat_GPU(pix_t * bmp, cudaStream_t stream, float zoomLevel);


