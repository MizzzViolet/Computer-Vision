//
//  Utilities.hpp
//  Assignment2
//
//  Created by Jasmine Talukder on 21/11/2015.
//  Copyright © 2015 Jasmine Enterprise. All rights reserved.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace cv;

void writeText( Mat image, char* text, int row, int column, Scalar colour=-1.0, double scale=0.4, int thickness=1 );
Mat JoinImagesHorizontally( Mat& image1, char* name1, Mat& image2, char* name2, int spacing=0, Scalar colour=-1.0 );
Mat JoinImagesVertically( Mat& image1, char* name1, Mat& image2, char* name2, int spacing, Scalar passed_colour=-1.0 );
Mat StretchImage( Mat& image );
void matchImage(Mat img, Mat templ);
Mat HistogramsDemos(Mat people_image, Mat sample);
#endif /* Utilities_hpp */
