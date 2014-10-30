//
//  segment.h
//  OtsuSegment
//
//  Created by simicah on 10/25/14.
//  Copyright (c) 2014 Yu. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;


#ifndef OtsuSegment_segment_h
#define OtsuSegment_segment_h
// display matrix
void disp_mat(Mat disp_mat){
    
    int rnum = disp_mat.rows;
    int cnum = disp_mat.cols;
    cout << "rows: " << rnum <<"cols: "<< cnum<< endl;
    
    for (int i = 0; i < rnum; i++) {
        for (int j = 0; j < cnum; j++) {
            cout << "|" << (int)disp_mat.at<uchar>(i,j);
        }
        cout << endl;
    }
    
}
// find the best value that maximize between-class variance
int findThreshold(Mat hist, double N, int histSize){
    
    double mut = 0;
    for (int i = 0; i < histSize; i++) {
        mut = mut + i * hist.at<float>(i,0)/N;
    }
    int threshold = 0, sigma = -1;
    for (int k = 0; k < histSize; k++){
        
        float omega0 = 0, omega1, mu0 = 0, mu1 = 0;
        for (int j = 0; j < k+1; j++) {
            omega0 = omega0 + hist.at<float>(j,0)/N;
            mu0 = mu0 + j * hist.at<float>(j,0)/N;
            
        }
        mu1 = mut - mu0;
        omega1 = 1 - omega0;
        //cout << N << endl;
        mu0 = mu0/omega0;
        mu1 = mu1/omega1;
        float sigmaB = omega0*omega1*(mu1-mu0)*(mu1-mu0);
        if (sigmaB > sigma) {
            sigma = sigmaB;
            threshold = k;
        }
       
        
    }
    return threshold;

}
// Otsu algorithm
Mat Otsu(Mat src){
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( src, bgr_planes );
    
    /// Establish the number of bins
    int histSize = 256;
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    Mat b_hist, g_hist, r_hist;
    
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // find the best threshold
    // that maximize the between classes variance
    float N = src.rows * src.cols;
    int th1 = findThreshold(b_hist, N, histSize);
    int th2 = findThreshold(g_hist, N, histSize);
    int th3 = findThreshold(r_hist, N, histSize);

    cout << th1 << endl;
    cout << th2 << endl;
    cout << th3 << endl;
    
    Mat bimage(src.rows, src.cols, CV_8UC1);
    for (int i = 0; i < bimage.rows; i++) {
        for (int j = 0; j < bimage.cols; j++) {
            if (((int)bgr_planes[0].at<uchar>(i,j) >= th1) &&\
                ((int)bgr_planes[1].at<uchar>(i,j) >= th2) &&\
                ((int)bgr_planes[2].at<uchar>(i,j) >= th3) ) {
                bimage.at<uchar>(i, j) = 255;
                //bimage.at<uchar>(i, j) = 0;
                //src.at<Vec3b>(i,j) = Vec3b(0,0,0);
            }else{
                bimage.at<uchar>(i, j) = 0;
                src.at<Vec3b>(i,j) = Vec3b(0,0,0);
                //bimage.at<uchar>(i, j) = 255;
            }
            
        }
    }
    try {
        imwrite("image_mask.png", bimage);
        imwrite("image_overlay.png", src);
    }
    catch (runtime_error& ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
    }
    return bimage;
}
// check if one pixel is of true contour
bool isConnEdge(Mat block){
    // block is 3 by 3
    int flag = 0;
    if (block.at<uchar>(1,1) == 255) {
        flag++;
    }
    Scalar mean = cv::mean(block);
    if (mean[0] >= 56 && mean[0] < 255) {
        flag++;
    }
    return flag==2;
}
// extract contour from binary mask
Mat findContour(Mat src){
    Mat contour(src.rows, src.cols, CV_8UC1, Scalar(0));
    
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++) {
            //cout << i << "::" << j << endl;
            Mat block = src(Rect(j-1,i-1,3,3));
            //disp_mat(block);
            if (src.at<uchar>(i,j) == 255 && isConnEdge(block)) {
                contour.at<uchar>(i,j) = 255;
            }
        
        }
    }
    
    return contour;
}



#endif
