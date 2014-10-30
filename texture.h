//
//  texture.h
//  OtsuSegment
//
//  Created by simicah on 10/26/14.
//  Copyright (c) 2014 Yu. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;

#ifndef OtsuSegment_texture_h
#define OtsuSegment_texture_h

// calculate variance in a matrix
double calcVariance(Mat block){
    
    Scalar mean, stdev;
    meanStdDev(block, mean, stdev);
    
    return stdev[0]*stdev[0];
    
}
// generating texture channels --> normalize --> combine three channels 
Mat createTextureImage(Mat src){

    
    Mat var_layer1(src.rows, src.cols, CV_64FC1, Scalar(0));
    Mat var_layer2(src.rows, src.cols, CV_64FC1, Scalar(0));
    Mat var_layer3(src.rows, src.cols, CV_64FC1, Scalar(0));
    
    Mat channel1(src.rows, src.cols, CV_8UC1, Scalar(0));
    Mat channel2(src.rows, src.cols, CV_8UC1, Scalar(0));
    Mat channel3(src.rows, src.cols, CV_8UC1, Scalar(0));
    
    for (int i = 3; i < src.rows-3; i++) {
        for (int j = 3; j < src.cols-3; j++) {
            Mat block3 = src(Rect(j-1,i-1,3,3));
            Mat block5 = src(Rect(j-2,i-2,5,5));
            Mat block7 = src(Rect(j-3,i-3,7,7));
            
            // calc variance for different block size
            var_layer1.at<double>(i,j) = calcVariance(block3);
            var_layer2.at<double>(i,j) = calcVariance(block5);
            var_layer3.at<double>(i,j) = calcVariance(block7);
            
        }
    }
    double l1min, l1max, l2min, l2max, l3min, l3max;
    minMaxLoc(var_layer1, &l1min, &l1max);
    minMaxLoc(var_layer2, &l2min, &l2max);
    minMaxLoc(var_layer3, &l3min, &l3max);
    // normalization 
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            //cout << var_layer1.at<double>(i,j) << endl;
            channel1.at<uchar>(i,j) = (uchar)round(var_layer1.at<double>(i,j)/l1max*255);
            channel2.at<uchar>(i,j) = (uchar)round(var_layer2.at<double>(i,j)/l2max*255);
            channel3.at<uchar>(i,j) = (uchar)round(var_layer3.at<double>(i,j)/l3max*255);
        }
    }

    vector<Mat> channels;
    Mat output;
    channels.push_back(channel1);
    channels.push_back(channel2);
    channels.push_back(channel3);
    
    merge(channels, output);
    
    return output;
}

#endif
