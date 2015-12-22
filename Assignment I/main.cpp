/* Student No: 12307269 */

#include <iostream>
#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

/* We count the number of black and white pixels in each split image*/
void countPixels(Mat img, int n){
    int black = 0;
    int white = 0;
    
     /*Go through the row of the image*/
    for(int i = 0; i < img.rows; i++){
        /*Go through the column of the image*/
        for(int j = 0; j < img.cols;j++){
            if (img.at<uchar>(i,j) == 0){ //if black pixel
                black ++;
            }
            else{
                white ++;
            }
        }
    }
    if(((double)black / (double)white) < 0.14){
        cout << "Glue " << n << " doesn't have a label" <<endl;
    }
}

void analyse(Mat img, int width)
{
    Mat img0, img1, img2, img3, img4, img5 = img;
    imshow("Original Image", img);
    /* Apply Gray Scale to image */
    cvtColor(img, img, COLOR_BGR2GRAY);
    imshow("Gray Scale Image", img);
    /* Apply adaptive threshold */
    adaptiveThreshold(img,img,255.0,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY, 11, 11 );
    imshow("Adaptive Threshold Image", img);

    std::vector<Mat> channels;
    split(img, channels);
    
    /* Get rid of the top half of the image. */
    Rect rec = Rect(0, img.rows/2, width, (img.rows / 2) - 1 );
    img0 = img(rec);
    imshow("Split Image", img0);
    
    /* Split the image to 5 separate images so we can count the number of black pixels */
    Rect rect = Rect(0, img.rows/2, width/5, (img.rows / 2) - 1 );
    img1 = img(rect);
    countPixels(img1, 1);
    imshow("Glue 1", img1);
    
    Rect rect_2 = Rect(width/5, img.rows/2, width/5, (img.rows / 2) - 1 );
    img2 = img(rect_2);
    countPixels(img2, 2);
    imshow("Glue 2", img2);
    
    Rect rect_3 = Rect((width/5)*2, img.rows/2, width/5, (img.rows / 2) - 1 );
    img3 = img(rect_3);
    countPixels(img3, 3);
    imshow("Glue 3", img3);
    
    Rect rect_4 = Rect((width/5)*3, img.rows/2, width/5, (img.rows / 2) - 1 );
    img4 = img(rect_4);
    countPixels(img4, 4);
    imshow("Glue 4", img4);
    
    Rect rect_5 = Rect((width/5)*4, img.rows/2, width/5, (img.rows / 2) - 1 );
    img5 = img(rect_5);
    countPixels(img5, 5);
    imshow("Glue 5", img5);
}

int main(int argc, const char** argv)
{
    /*Might need to change the location if you want to ran it*/
	char* file_location = "/Users/JasmineTalukder/Documents/College /4th Year/Vision/IntroLab/IntroLab/Media/";
	char* image_files[] = {
        "Glue1.jpg",
        "Glue2.jpg",
        "Glue3.jpg",
        "Glue4.jpg",
        "Glue5.jpg",
        "Glue6.jpg",
        "Church.jpg"
    };

	/* Load images */
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
 
	int line_step = 13;
	Point location( 7, 13 );
	Scalar colour( 0, 0, 0);
    Mat default_image = image[6];
	putText( default_image, "ASSIGNMENT I", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
	location.y += line_step*3/2;
	putText( default_image, "1. Image 1", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
    location.y += line_step;
    putText( default_image, "2. Image 2", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
    location.y += line_step;
    putText( default_image, "3. Image 3", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
	location.y += line_step;
    putText( default_image, "4. Image 4", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
    location.y += line_step;
    putText( default_image, "5. Image 5", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
    location.y += line_step;
    putText( default_image, "6. Image 6", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );
    location.y += line_step;
	putText( default_image, "X. eXit", location, FONT_HERSHEY_SIMPLEX, 0.4, colour );

	int choice;
    imshow("Welcome", default_image);
	do
	{
		choice = cvWaitKey();
		cvDestroyAllWindows();
		switch (choice)
		{
        /*Parameters: Image and width*/
		case '1':
            cout << "Image 1:" << endl;
            analyse(image[0], 630);
			break;
        case '2':
            cout << "\nImage 2:" << endl;
            analyse(image[1], 616);
            break;
        case '3':
            cout << "\nImage 3:" << endl;
            analyse(image[2], 607);
            break;
        case '4':
            cout << "\nImage 4:" << endl;
            analyse(image[3], 623);
            break;
        case '5':
            cout << "\nImage 5:" << endl;
            analyse(image[4], 604);
            break;
        case '6':
            cout << "\nImage 6:" << endl;
            analyse(image[5], 642);
            break;
		default:
			break;
		}
	} while ((choice != 'x') && (choice != 'X'));
}
