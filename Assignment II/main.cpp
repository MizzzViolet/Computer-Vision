//
//  main.cpp
//  Assignment2
//
//  Created by Jasmine Talukder on 12/11/2015.
//  Copyright © 2015 Jasmine Enterprise. All rights reserved.
//
#include <iostream>
#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "Utilities.hpp"
#include <string.h>
#define SAMPLE_INDEX 38

Mat geometric(Mat image, Mat masked){
    // Perspective transformation
    Point2f source_points[4], destination_points[4];
    int top = 0;
    int right = 0;
    int left = 0;
    for(int i = 0; i <  image.rows; i++){
        /*Go through the column of the image*/
        for(int j = 0; j < image.cols;j++){
            if ((image.at<uchar>(i,j) != 0) && (top == 0)){
                source_points[0] = Point2f( j, i );
                top = 1;
                right = j;
            }
            if((image.at<uchar>(i,j) != 0) && (j > right)){
                source_points[1] = Point2f( j, i );
                right = j;
                left = j;
            }
            if((image.at<uchar>(i,j) != 0) && (j < left)){
                source_points[2] = Point2f( j, i );
                left = j;
            }
            if (image.at<uchar>(i,j)  != 0) {
                source_points[3] = Point2f( j, i );
            }
        }
    }
    Mat perspective_matrix( 3, 3, CV_32FC1 ), perspective_warped_image;
    perspective_warped_image = Mat::zeros( 300, 220, CV_8UC3);
    resize(perspective_warped_image, perspective_warped_image, Size(380,572)); // to half size or
    destination_points[0] = Point2f( 0.0, 0.0 );
    destination_points[1] = Point2f( 380.0, 0.0 );
    destination_points[2] = Point2f( 0.0, 572.0 );
    destination_points[3] = Point2f( 380.0, 572.0 );
    perspective_matrix = getPerspectiveTransform( source_points, destination_points );
    warpPerspective( masked, perspective_warped_image, perspective_matrix, perspective_warped_image.size() );
//        for(int i = 0; i <  4; i++){
//              cout << source_points[i]<<endl;
//            circle(image, source_points[0], 6, Scalar(255,0,0));
//            circle(image, source_points[1], 6, Scalar(255,0,0));
//            circle(image, source_points[2], 6, Scalar(255,0,0));
//            circle(image, source_points[3], 6, Scalar(255,0,0));
//        }
//        imshow("Edges Found", image );
    return perspective_warped_image;
}
Mat mask(Mat mask,Mat sampleImg){
    Mat backPoj = HistogramsDemos(mask, sampleImg);
    dilate(backPoj, backPoj, Mat());
    dilate(backPoj, backPoj, Mat());
    dilate(backPoj, backPoj, Mat());
    dilate(backPoj, backPoj, Mat());
    return backPoj;
}
Mat splitImage(Mat image, Mat sampleImg){
    vector<Mat> channels;
    split(image, channels);
    threshold(channels[0], channels[0], 140, 255, THRESH_BINARY);
    dilate(channels[0], channels[0], Mat());
    dilate(channels[0], channels[0], Mat());
    dilate(channels[0], channels[0], Mat());
    dilate(channels[0], channels[0], Mat());
    erode(channels[0], channels[0], Mat());
    erode(channels[0], channels[0], Mat());
    erode(channels[0], channels[0], Mat());
    erode(channels[0], channels[0], Mat());
    Mat mask;
    image.copyTo(mask, channels[0]);
    return mask;
}

double MatchingMethod(Mat templates, Mat img)
{
    Mat cropedImage = templates(Rect(15,13,380,572));
    Mat crop = img(Rect(12,14,360,556));
    Mat canny_image_display,canny_image,output1, output2;
    Mat templ, templ_edge_image, res;
    int res_cols, res_rows, match_method;
    double minVal, maxVal;
    Point minLoc, maxLoc;
    resize(crop, crop, Size(380,572));
    Mat temp;
    Canny(crop,canny_image,80,150);
    cvtColor(canny_image, canny_image_display, CV_GRAY2BGR);
    resize(cropedImage, cropedImage, Size(380,572));
    Canny(cropedImage,templ_edge_image,80,150);
    cvtColor(templ_edge_image, templ, CV_GRAY2BGR);
    res_cols = canny_image_display.cols - templ.cols + 1;
    res_rows = canny_image_display.rows - templ.rows + 1;
    res.create( res_cols, res_rows, CV_32FC1 );
    matchTemplate( canny_image_display, templ, res, CV_TM_CCORR);

    minMaxLoc( res, &minVal, &maxVal);
    cout << "min val : 1" << minVal << "\nmax val: " << maxVal << endl;
    output1 = JoinImagesHorizontally( canny_image_display, "Canny Edge Image", templ, "Canny Edge TEMPLATE Image", 2 );
    output2 = JoinImagesHorizontally( crop, "Page Recognised", cropedImage, "Page Matched", 2 );
    imshow("Canny Images", output1);
    imshow("Recognised Image", output2);
    return minVal;
}

int matchImage(Mat image[], Mat geo){
    int found = 0;
    int match = 0;
    double tempMatching = 0.0;
    double m = 0.0;
    for(int i = 25; i < 38; i++ ){
        tempMatching = MatchingMethod(image[i], geo);
        if(found == 0)
        {
            match = i;
            m = tempMatching;
            found = 1;
        } else if((found != 0) &&(m < tempMatching)){
            match = i;
            m = tempMatching;
        }
    }
    MatchingMethod(image[match], geo);
    return match;
}
void showImage(Mat image, Mat masked, Mat backProject, Mat geo){
    imshow("Back project", backProject);
    resize(image, image, Size(image.rows/2, image.cols/2));
    resize(masked, masked, Size(masked.rows/2, masked.cols/2));
    resize(backProject, backProject, Size(backProject.rows/2, backProject.cols/2));
    Mat output1 = JoinImagesHorizontally( image, "Original Image", masked, "Masked Image", 2 );
    imshow("Image Found", output1);
    imshow("Geometric Transfromation", geo);
}
Mat masked, backProject, geo;
void methodsUsed(Mat image, Mat samplePix){
    masked = splitImage(image, samplePix);
    backProject = mask(masked,samplePix);
    geo = geometric(backProject,masked);
    showImage(image, masked, backProject, geo);
}
int main(int argc, const char * argv[])
{
    char* file_location = "/Users/JasmineTalukder/Documents/College /4th Year/Vision/Assignment2/Assignment2/Assignment2/Books/";
    string image_files[39];
    string name = "BookView0";
    for(int i = 0; i < 25; i++){
        if(i<9){
            name = "BookView0";
        }else{
            name = "BookView";
        }
        name += to_string(i+1) + ".JPG";
        image_files[i] = name;
    }
    int j=0;
    for(int i = 25 ; i < 38; i++){
        if(i<34){
            name = "Page0";
        }else{
            name = "Page";
        }
        name += to_string(j+1) + ".jpg";
        image_files[i] = name;
        j++;
    }
    image_files[38] = "samp.jpg";

    int number_of_images = sizeof(image_files)/sizeof(image_files[0]);
    Mat* image = new Mat[number_of_images];
    for (int file_no=0; (file_no < number_of_images); file_no++)
    {
        string filename(file_location);
        filename.append(image_files[file_no]);
       
        image[file_no] = imread(filename, -1);
        if (image[file_no].empty())
        {
            cout << "Could not open " << image[file_no] << endl;
            return -1;
        }
    }
    Mat default_image = image[0];
    int choice;
    imshow("Welcome", default_image);
    do
    {
        choice = cvWaitKey();
        cvDestroyAllWindows();
        int matchedImg;
        switch (choice)
        {
            case '0':
                methodsUsed(image[0],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '1':
                methodsUsed(image[1],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '2':
                methodsUsed(image[2],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '3':
                methodsUsed(image[3],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '4':
                methodsUsed(image[4],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '5':
                methodsUsed(image[5],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '6':
                methodsUsed(image[6],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '7':
                methodsUsed(image[7],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '8':
                methodsUsed(image[8],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case '9':
                methodsUsed(image[9],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case 'a': /*10*/
                methodsUsed(image[10],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case 'b':/*11*/
                methodsUsed(image[11],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case 'c':/*12*/
                methodsUsed(image[12],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                cout << "Match " << matchedImg <<endl;
                break;
            case 'd':/*13*/
                methodsUsed(image[13],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'e':/*14*/
                methodsUsed(image[14],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'f':/*15*/
                methodsUsed(image[15],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'g':/*16*/
                methodsUsed(image[16],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'h':/*17*/
                methodsUsed(image[17],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'i':/*18*/
                methodsUsed(image[18],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'j':/*19*/
                methodsUsed(image[19],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'k':/*20*/
                methodsUsed(image[20],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'l':/*21*/
                methodsUsed(image[21],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'm':/*22*/
                methodsUsed(image[22],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'n':/*23*/
                methodsUsed(image[23],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            case 'o':/*24*/
                methodsUsed(image[24],image[SAMPLE_INDEX]);
                matchedImg = matchImage(image, geo);
                break;
            default:
                break;
        }
    } while ((choice != 'x') && (choice != 'X'));

    return 0;
}
