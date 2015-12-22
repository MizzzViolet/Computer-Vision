//
//  Histogram.cpp
//  Assignment2
//
//  Created by Jasmine Talukder on 20/11/2015.
//  Copyright © 2015 Jasmine Enterprise. All rights reserved.
//

#include "Utilities.hpp"

class Histogram
{
protected:
    Mat mImage;
    int mNumberChannels;
    int* mChannelNumbers;
    int* mNumberBins;
    float mChannelRange[2];
public:
    Histogram( Mat image, int number_of_bins )
    {
        mImage = image;
        mNumberChannels = mImage.channels();
        mChannelNumbers = new int[mNumberChannels];
        mNumberBins = new int[mNumberChannels];
        mChannelRange[0] = 0.0;
        mChannelRange[1] = 255.0;
        for (int count=0; count<mNumberChannels; count++)
        {
            mChannelNumbers[count] = count;
            mNumberBins[count] = number_of_bins;
        }
    }
};

class ColourHistogram : public Histogram
{
private:
    MatND mHistogram;
public:
    ColourHistogram( Mat image, int number_of_bins ) :
    Histogram( image, number_of_bins )
    {
        ComputeHistogram();
    }
    void ComputeHistogram()
    {
        const float* channel_ranges[] = { mChannelRange, mChannelRange, mChannelRange };
        calcHist(&mImage, 1, mChannelNumbers, Mat(), mHistogram, mNumberChannels, mNumberBins, channel_ranges);
    }
    void NormaliseHistogram()
    {
        normalize(mHistogram,mHistogram,1.0);
    }
    Mat BackProject( Mat& image )
    {
        Mat result = image.clone();
        const float* channel_ranges[] = { mChannelRange, mChannelRange, mChannelRange };
        calcBackProject(&image,1,mChannelNumbers,mHistogram,result,channel_ranges,255.0);
        return result;
    }
    MatND getHistogram()
    {
        return mHistogram;
    }
};
Mat HistogramsDemos(Mat page_image, Mat sample)
{
    Mat hls_image;
    cvtColor(sample, hls_image, CV_BGR2HLS);
    ColourHistogram histogram3D(hls_image,4);
    histogram3D.NormaliseHistogram();
    cvtColor(page_image, hls_image, CV_BGR2HLS);
    Mat back_projection_probabilities = histogram3D.BackProject(hls_image);
    back_projection_probabilities = StretchImage( back_projection_probabilities );
    Mat back_projection_probabilities_display;
    cvtColor(back_projection_probabilities, back_projection_probabilities_display, CV_GRAY2BGR);
    threshold(back_projection_probabilities , back_projection_probabilities , 15, 255, THRESH_BINARY);
    return back_projection_probabilities;

}