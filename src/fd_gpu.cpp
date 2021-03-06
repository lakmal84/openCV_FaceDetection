//
//  fd_gpu.cpp
//  TestOpenCV
//
//  Created by Lakmal on 20/02/15.
//  Copyright (c) 2015 NTU. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>

#include "face_detect.hpp"

//#define FIND_BIGGEST_FACE

using namespace std;
using namespace cv;
using namespace cv::gpu;

void gpuFaceDetect(string classi_val, int big_face)
{
    const int show_biggest_faec = big_face;
    const string face_cascade_name = classi_val;
    const string window_name = "Capture - Face detection";
    const string bf_window_name = "Biggest Face";

    const float size_factor = 1.1;
    const size_t num_buffers = 2;
    const Size face_size(30, 30);

    CascadeClassifier_GPU face_cascade;
    Mat src_frame;
    GpuMat gpu_frame;
    timeval t1, t2;
    double elapsedTime;

    VideoCapture capture(0);

    if (!capture.isOpened())
    {
        printf("No Camera input found\n");
    }



    if (!face_cascade.load(face_cascade_name))
    {
        printf("--(!)Error loading haarcascade_frontalface_alt.xml\n");
    }


    while (1)
    {
        capture >> src_frame;

        flip(src_frame, src_frame, 1);

        gpu_frame.upload(src_frame);

        if (!gpu_frame.empty())
        {
            gettimeofday(&t1, NULL); /* mark start time */

            GpuMat gfaces, gframe_gray;
            Mat dst_frame, gray, crop, res;
            string text;
            stringstream sstm;

            gpu::cvtColor(gpu_frame, gframe_gray, COLOR_BGR2GRAY);
            gpu::equalizeHist(gframe_gray, gframe_gray);


            if (show_biggest_faec == 1)
            {
                unsigned int detect_num = face_cascade.detectMultiScale(gframe_gray, gfaces, size_factor, num_buffers, face_size);

                gfaces.colRange(0, detect_num).download(dst_frame);  // retrieve results from GPU

                Rect* cfaces = dst_frame.ptr<Rect>();  // results are now in "dst_frame"

                Rect area_c; // Area of interest current
                Rect area_b; // Area of interest biggest

                size_t index_current = 0;
                size_t index_biggest = 0;

                int area_current = 0;
                int area_biggest = 0;

                for (index_current = 0; index_current < detect_num; index_current++)
                {
                    area_c.x = cfaces[index_current].x;
                    area_c.y = cfaces[index_current].y;
                    area_c.width = (cfaces[index_current].width);
                    area_c.height = (cfaces[index_current].height);

                    area_current = area_c.width * area_c.height;

                    area_b.x = cfaces[index_biggest].x;
                    area_b.y = cfaces[index_biggest].y;
                    area_b.width = (cfaces[index_biggest].width);
                    area_b.height = (cfaces[index_biggest].height);

                    area_biggest = area_b.width * area_b.height;

                    if (area_current > area_biggest)
                    {
                        index_biggest = index_current;
                        area_b.x = cfaces[index_biggest].x;
                        area_b.y = cfaces[index_biggest].y;
                        area_b.width = (cfaces[index_biggest].width);
                        area_b.height = (cfaces[index_biggest].height);
                    }

                    crop = src_frame(area_b);
                    resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR);
                    cvtColor(crop, gray, CV_BGR2GRAY);

                    Point pt1(cfaces[index_current].x, cfaces[index_current].y);
                    Point pt2((cfaces[index_current].x + cfaces[index_current].height), (cfaces[index_current].y + cfaces[index_current].width));
                    rectangle(src_frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
                }

                sstm << "Biggest face size in the image: " << area_b.width << "x" << area_b.height;
                text = sstm.str();

                putText(src_frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
                imshow(window_name, src_frame);

                if (!crop.empty())
                {
                    imshow(bf_window_name, crop);
                }
                else
                {
                    destroyWindow(bf_window_name);
                }
            }
            else // if no need to show biggest face
            {

                int detect_num = face_cascade.detectMultiScale(gframe_gray, gfaces, size_factor, num_buffers, face_size);

                gfaces.colRange(0, detect_num).download(dst_frame);  // retrieve results from GPU

                Rect* cfaces = dst_frame.ptr<Rect>();  // results are now in "dst_frame"

                for(int i=0; i < detect_num; i++)
                {
                    Point pt1 = cfaces[i].tl();
                    Size sz = cfaces[i].size();
                    Point pt2(pt1.x+sz.width, pt1.y+sz.height);
                    rectangle(src_frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
                }

                imshow(window_name, src_frame);
            }

            gettimeofday(&t2, NULL); /* mark end time */
            elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;     // sec to ms
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;  // us to ms
            cout << elapsedTime << " ms.\n";
        }
        else
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }

        int c = waitKey(10);

        if (27 == char(c))
        {
            break;
        }
    }

}
