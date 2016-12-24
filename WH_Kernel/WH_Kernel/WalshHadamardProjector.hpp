//
//  WalshHadamardProjector.hpp
//  
//
//  Created by Ahmed Taha on 9/17/16.
//
//

#ifndef WalshHadamardProjector_hpp
#define WalshHadamardProjector_hpp

#include <iostream>
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
#include <ctime>

using namespace cv;
using namespace std;
class WalshHadamardProjector{
private:
    int patchWidth;
    Mat snakeOrder;
    Mat GCKs2D;
    vector<int> filtersY;
    int LastCbCrFilterIndex;
    
    void initKernels();
    vector<int> instersection(vector<int> &v1, vector<int> &v2);
    
public:
    /**
        WalshHadamardProjector constructor: takes the size of patch as parameter
        Patch size is assumed to be even number. Default value is 8
     **/
    WalshHadamardProjector(int width=8);
    
    /**
     get_WH_kernels: returns the WH kernels that you can use.
     ***** I don't remember using or testing this function. *****
     **/
    vector<Mat> get_WH_kernels();
    
    /**
     This functions computes the WH transform per patch.
     You should use it if you want to compute the WH transform to a patch on the fly.
     **/
    vector<int> WH_projections(Mat patch);
    
    /**
     This functions computes the WH transform per image.
     You should use it if you want to compute the WH transform to a whole image.
     The result is stored in the features matrix.
     The return value is the number of channels in the features matrox
     **/
    int applyWHKernels(const cv::Mat& img, cv::Mat& features);
};

#endif /* WalshHadamardProjector_hpp */
