//
//  main.cpp
//  OtsuSegment
//
//  Created by simicah on 10/25/14.
//  Copyright (c) 2014 Yu. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "segment.h"
#include "texture.h"

using namespace std;
using namespace cv;



/**
 * @function main
 */
int main( int argc, char** argv )
{
    Mat src, dst;
    
    /// Load image
    src = imread("pic1.jpg", 1); // use 2 for texture image
    
    if (src.empty()) {
        cout<<"Error loading the image.." << endl;
    }
    imshow("test", src);
    
    Mat texture = createTextureImage(src);
    Mat bimage = Otsu(texture); // Otsu(texture)
    Mat ctimage = findContour(bimage);
    
    try {
        imwrite("image_contour.png", ctimage);
    }
    catch (runtime_error& ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
    }
    
    /* Just for testing: */
//    double tmin, tmax;
//    Mat temp = (Mat_<uchar>(3,3)<< 10,11,12,2,3,4,6,7,8);
//    disp_mat(temp);
//    minMaxLoc(temp, &tmin, &tmax);
    
    
//    Mat test = temp(Rect(1,1,3,3));
//    disp_mat(test);
    
    
    waitKey(0);
    
    return 0;
}