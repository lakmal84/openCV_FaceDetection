//
//  face_detect.hpp
//  OpenCV-FaceDetection
//
//  Created by Lakmal on 20/02/15.
//  Copyright (c) 2015 NTU. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

void cpuFaceDetect(string classi_val, int big_face);
void gpuFaceDetect(string classi_val, int big_face);
