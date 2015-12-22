//
//  Utilities.cpp
//  Assignment2
//
//  Created by Jasmine Talukder on 21/11/2015.
//  Copyright © 2015 Jasmine Enterprise. All rights reserved.
//

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include "Utilities.hpp"

using namespace std;
using namespace cv;

void writeText( Mat image, char* text, int row, int column, Scalar passed_colour, double scale, int thickness )
{
    Scalar colour( 0, 0, 255);
    Point location( column, row );
    putText( image, text, location, FONT_HERSHEY_SIMPLEX, scale, (passed_colour.val[0] == -1.0) ? colour : passed_colour, thickness );
}

Mat JoinImagesHorizontally( Mat& image1, char* name1, Mat& image2, char* name2, int spacing, Scalar passed_colour/*=-1.0*/ )
{
    Mat result( (image1.rows > image2.rows) ? image1.rows : image2.rows,
               image1.cols + image2.cols + spacing,
               image1.type() );
    result.setTo(Scalar(255,255,255));
    Mat imageROI;
    imageROI = result(cv::Rect(0,0,image1.cols,image1.rows));
    image1.copyTo(imageROI);
    if (spacing > 0)
    {
        imageROI = result(cv::Rect(image1.cols,0,spacing,image1.rows));
        imageROI.setTo(Scalar(255,255,255));
    }
    imageROI = result(cv::Rect(image1.cols+spacing,0,image2.cols,image2.rows));
    image2.copyTo(imageROI);
    writeText( result, name1, 13, 6, passed_colour );
    writeText( imageROI, name2, 13, 6, passed_colour );
    return result;
}

Mat JoinImagesVertically( Mat& image1, char* name1, Mat& image2, char* name2, int spacing, Scalar passed_colour/*=-1.0*/ )
{
    Mat result( image1.rows + image2.rows + spacing,
               (image1.cols > image2.cols) ? image1.cols : image2.cols,
               image1.type() );
    result.setTo(Scalar(255,255,255));
    Mat imageROI;
    imageROI = result(cv::Rect(0,0,image1.cols,image1.rows));
    image1.copyTo(imageROI);
    if (spacing > 0)
    {
        imageROI = result(cv::Rect(0,image1.rows,image1.cols,spacing));
        imageROI.setTo(Scalar(255,255,255));
    }
    imageROI = result(cv::Rect(0,image1.rows+spacing,image2.cols,image2.rows));
    image2.copyTo(imageROI);
    writeText( result, name1, 13, 6, passed_colour );
    writeText( imageROI, name2, 13, 6, passed_colour );
    return result;
}

Mat StretchImage( Mat& image )
{
    Mat result = image.clone();
    // Find max value
    int image_rows = image.rows;
    int image_channels = image.channels();
    int values_on_each_row = image.cols * image_channels;
    uchar max = 0;
    for (int row=0; row < image_rows; row++) {
        uchar* value = image.ptr<uchar>(row);
        for (int column=0; column < values_on_each_row; column++)
        {
            if (*value > max)
                max= *value;
            value++;
        }
    }
    // Stretch values using a lookup-table
    int entries(256);
    Mat lut(1,entries,CV_8U);
    for (int i=0; (i<256); i++)
        lut.at<uchar>(i) = (255*i)/max;
    LUT(image,lut,result);
    
    return result;
}