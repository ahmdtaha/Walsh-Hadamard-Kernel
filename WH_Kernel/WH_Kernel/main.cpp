//
//  main.cpp
//  WH_Kernel
//
//  Created by Ahmed Taha on 12/24/16.
//  Copyright (c) 2016 Ahmed Taha. All rights reserved.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "WalshHadamardProjector.hpp"

using namespace std;
using namespace cv;


int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, OpenCV!\n";
    
    WalshHadamardProjector WHProjector;
    Mat img = imread("sm_img.png");
    cvtColor(img, img, CV_BGR2RGB);
    
    Mat features = Mat(img.rows, img.cols, CV_32SC(23));
    //vector<Mat> kernels = WHProjector.get_WH_kernels();
    WHProjector.applyWHKernels(img,features);

    
    cout << "Done !\n";
    return 0;
}
