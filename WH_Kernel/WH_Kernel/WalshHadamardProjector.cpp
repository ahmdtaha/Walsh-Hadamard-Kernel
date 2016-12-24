
//
//  WalshHadamardProjector.cpp
//  
//
//  Created by Ahmed Taha on 9/17/16.
//
//

#include "WalshHadamardProjector.hpp"

#include <iostream>

using namespace std;
using namespace cv;

vector<int> WalshHadamardProjector::instersection(vector<int> &v1, vector<int> &v2)
{
    
    vector<int> v3;
    
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    
    set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));
    
    return v3;
}


WalshHadamardProjector::WalshHadamardProjector(int width)
{
    this->patchWidth = width;
    
    
    initKernels();
    
    int nSequencyOrder16u[]   = {1,  4,10,  2,3,  13,17,  7,18,21,16,14,15,5,6,11,12, 8,9,22,23,19,20};
    int nSequencyLevels16u[]  = {1, 3,  5,  7,  17,  23};
    //% these are all the filters we will use in the algorithm
    //filters = sort([1,4,10,  2,3,13,25,  7,28,46,22,16,19,5,6,11,12,  8,9,49,73,31,43]);
    
    int filters[] = {1,4,10,  2,3,13,25,  7,28,46,22,16,19,5,6,11,12,  8,9,49,73,31,43};
    vector<int> filter_vector(filters,filters+23);
    sort (filter_vector.begin(), filter_vector.end());
    //% this is in ordered form:
    //% index:    1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
    //% filters:  1  2  3  4  5  6  7  8  9  10  11  12  13  16  19  22  25  28  31  43  46  49  73
    
    //TODOD:: filtersY = intersect(filters,1:3:80); //% [1,4,7,10,13,16,19,22,25,28,31,43,46,49,73]
    
    vector<int> tmp;
    for (int i=1;i<80;i+=3)
        tmp.push_back(i);
    
    filtersY = instersection(filter_vector, tmp);
    int procFilterIndToUse[] =  {0,1, 4, 7, 10, 13, 14, 15, 16, 17, 18, -1,  -1,  -1,  14,  20,  21, -1, -1,  -1, -1, -1,  -1, -1,  18};
    //% the SNAKE INDEX from which we come FOR THE COMPUTATION
    int procSnakeIndToUse[] =   {0,1 ,2, 3,  4,  5,  6,  7,  8,  9, 10, -1,  -1,  -1,   6 , 15,  16 ,-1, -1 , -1 ,-1 ,-1 , -1, -1,  10};
    LastCbCrFilterIndex = 12;
    
}

vector<Mat> WalshHadamardProjector::get_WH_kernels()
{
    vector<Mat> WH_kernels;
    Mat DC_Kernel = Mat::ones(this->patchWidth,this->patchWidth,CV_8UC1);
    for(int i=0;i<3;i++)
        WH_kernels.push_back(DC_Kernel);
    
    int maxKernels = 75; // TODO: I Need to figure the equation for this variable
    
    int currFilter = 4;
    int channelInd = 0;
    int yOnly,snakeInd,addition;
    
    for (int iteration = 1 ; iteration< maxKernels;iteration++)
    {
        channelInd = channelInd + 1;
        if (channelInd > 3 && iteration < maxKernels)
        {
            
            channelInd = 1;
            vector<int>::iterator it = find(filtersY.begin(),filtersY.end(),iteration);
            if (it==filtersY.end())
                continue;
            yOnly = (iteration > LastCbCrFilterIndex);
            addition = 2*(1-yOnly);
            snakeInd = 1 + floor(iteration/3); //% the index of the filter that we are going source_to
            
            int row = static_cast<int>(snakeOrder.at<int>(0,snakeInd-1))-1;//(1,snakeInd);
            int col = static_cast<int>(snakeOrder.at<int>(1,snakeInd-1))-1;//(2,snakeInd);
            
            int rangeStart = col*this->patchWidth + row * (this->patchWidth * this->patchWidth);
            int rangeEnd = rangeStart+ this->patchWidth;
            Mat filter = GCKs2D(Range(rangeStart,rangeEnd),Range::all());
            for(int filtChan = 1 ;filtChan <= 1 + addition;filtChan++)
            {
                WH_kernels.push_back(filter);
            }
            currFilter = currFilter + 1 + addition;
            
        }
    }
    return WH_kernels;
}
vector<int> WalshHadamardProjector::WH_projections(Mat patch){
    vector<int> result;
    Mat conv_kernel = Mat(this->patchWidth,this->patchWidth, CV_32SC1);
    Mat DC_Kernel = Mat::ones(this->patchWidth,this->patchWidth,CV_32SC1);
    
    Mat patchS;
    vector<Mat> channels(3);
    patch.convertTo(patchS,CV_32SC3);
    split(patchS, channels);
    int ChannelColors = 3;
    
    
    int maxKernels = 75; // TODO: I Need to figure the equation for this variable
    

    for (int i = 0; i<ChannelColors;i++){
        conv_kernel = DC_Kernel.mul(channels[i]);
        
        int value = cv::sum(cv::sum(conv_kernel))[0];
        result.push_back(value);
    }
    
    int currFilter = 4;
    int channelInd = 0;
    int yOnly,snakeInd,addition;
    
    for (int iteration = 1 ; iteration< maxKernels;iteration++)
    {
        channelInd = channelInd + 1;
        if (channelInd > 3 && iteration < maxKernels)
        {
            
            channelInd = 1;
            //auto it = filtersY.find(iteration);
            vector<int>::iterator it = find(filtersY.begin(),filtersY.end(),iteration);
            if (it==filtersY.end())
                continue;
            yOnly = (iteration > LastCbCrFilterIndex);
            addition = 2*(1-yOnly);
            snakeInd = 1 + floor(iteration/3); //% the index of the filter that we are going source_to
            
            int row = static_cast<int>(snakeOrder.at<int>(0,snakeInd-1))-1;//(1,snakeInd);
            int col = static_cast<int>(snakeOrder.at<int>(1,snakeInd-1))-1;//(2,snakeInd);
            
            int rangeStart = col*this->patchWidth + row * (this->patchWidth * this->patchWidth);
            int rangeEnd = rangeStart+ this->patchWidth;
            Mat filter = GCKs2D(Range(rangeStart,rangeEnd),Range::all());
            
            for(int filtChan = 1 ;filtChan <= 1 + addition;filtChan++)
            {
                conv_kernel =filter.mul(channels[filtChan-1]);
                result.push_back(cv::sum(cv::sum(conv_kernel))[0]);
            }
            currFilter = currFilter + 1 + addition;
            
        }
    }
    return result;
}

int WalshHadamardProjector::applyWHKernels(const cv::Mat& img, cv::Mat& features)
{
    
    Mat conv_kernel = Mat(this->patchWidth,this->patchWidth, CV_32SC1);
    Mat DC_Kernel = Mat::ones(this->patchWidth,this->patchWidth,CV_32SC1);
    
    
    //Mat conuolution_result;
    Point anchor = Point( -1, -1 );
    int delta = 0;
    int ddepth = -1;
    
    vector<Mat> channels(3);
    Mat imgS;
    img.convertTo(imgS,CV_32SC3);
    split(imgS, channels);
    
    int ChannelColors = 3;
    
    int from_to[] = {0, 0};
    int maxKernels = 75; // TODO: I Need to figure the equation for this variable
    
    Mat conuolution_result = Mat(img.size().height, img.size().width, CV_32SC1);
    for (int i = 0; i<ChannelColors;i++){
        filter2D(channels[i], conuolution_result, ddepth , DC_Kernel, anchor, delta, BORDER_CONSTANT );
        mixChannels(&conuolution_result, 1, &features, 1, from_to, 1);from_to[1]++;
        
        
    }
    
    int currFilter = 4;
    int channelInd = 0;
    int yOnly,snakeInd,addition;
    
    for (int iteration = 1 ; iteration< maxKernels;iteration++)
    {
        channelInd = channelInd + 1;
        if (channelInd > 3 && iteration < maxKernels)
        {
            
            channelInd = 1;
            //auto it = filtersY.find(iteration);
            vector<int>::iterator it = find(filtersY.begin(),filtersY.end(),iteration);
            if (it==filtersY.end())
                continue;
            yOnly = (iteration > LastCbCrFilterIndex);
            addition = 2*(1-yOnly);
            snakeInd = 1 + floor(iteration/3); //% the index of the filter that we are going source_to
            
            int row = static_cast<int>(snakeOrder.at<int>(0,snakeInd-1))-1;//(1,snakeInd);
            int col = static_cast<int>(snakeOrder.at<int>(1,snakeInd-1))-1;//(2,snakeInd);
            
            int rangeStart = col*this->patchWidth + row * (this->patchWidth * this->patchWidth);
            int rangeEnd = rangeStart+ this->patchWidth;
            Mat filter = GCKs2D(Range(rangeStart,rangeEnd),Range::all());
            for(int filtChan = 1 ;filtChan <= 1 + addition;filtChan++)
            {
                Mat tmp;
                
                filter2D(channels[filtChan-1], conuolution_result, ddepth , filter, anchor, delta, BORDER_CONSTANT );
                mixChannels(&conuolution_result, 1, &features, 1, from_to, 1);from_to[1]++;
            }
            currFilter = currFilter + 1 + addition;
        }
    }
    
    return from_to[1];
}
void WalshHadamardProjector::initKernels(){
    
    cv::Mat* tree = new cv::Mat[2*this->patchWidth-1];

    Mat t1 = (Mat_<int>(1,1) << 1);
    Mat t2 = (Mat_<int>(1,2) << 1,1);
    Mat t3 = (Mat_<int>(1,2) << 1,-1);
    Mat t2n_1 = (Mat_<int>(1,1) << 1);
    
    Mat mult;
    Mat v = (Mat_<int>(1,2) <<-1,1);
    tree[0] = t1;
    tree[1] = t2;
    tree[2] = t3;
    tree[14] = t2n_1;
    int m;
    for( int parent=2;parent<this->patchWidth;parent++)
    {
        int sign = 2*(parent%2)-1;
        mult = sign*v;
        cv::Mat v1;
        cv::Mat A = tree[parent-1];
        m = mult.at<int>(0,0);
        cv::Mat B = m*(tree[parent-1]);
        hconcat(A,B,tree[2*parent-1]);
        m = mult.at<int>(0,1);
        B = m*(tree[parent-1]);
        hconcat(A,B,tree[2*parent]);
        
    }
    
    
    Mat res1D(this->patchWidth,this->patchWidth,CV_64FC1,Scalar::all(0));
    for(int i=this->patchWidth;i<=2*this->patchWidth-1;i++)
        tree[i-1].copyTo(res1D.row(i-this->patchWidth));
        
    
    snakeOrder = Mat(2, 0, CV_32SC1);
    
    for( int i = 1 ;i<= this->patchWidth;i++)
    {
        Mat is = i* Mat::ones(1, i, CV_32SC1);
        Mat one2i = Mat::ones(1, i, CV_32SC1);
        for (int j=0;j<i;j++)
            one2i.at<int>(0,j) = j+1;
        
        Mat vec = Mat(2, is.cols, CV_32SC1);
        is.row(0).copyTo(vec.row(0));
        one2i.row(0).copyTo(vec.row(1));
        
        Mat swappedVec;
        flip(vec(Range::all(),Range(0,vec.cols-1)),swappedVec,-1);
        
        Mat order = Mat::zeros(2, vec.cols*2-1, CV_32SC1);
        vec.copyTo(order(Range::all(),Range(0,vec.cols)));
        
        if(swappedVec.cols>0)
            swappedVec.copyTo(order(Range::all(),Range(vec.cols,order.cols)));
        
        if(i % 2==1)
            flip(order,order,1);
        
        hconcat(snakeOrder, order, snakeOrder);
    }
    
    
    
    GCKs2D.create(this->patchWidth*this->patchWidth*this->patchWidth, this->patchWidth, CV_32SC1);
    Mat kernel = Mat::zeros(this->patchWidth, this->patchWidth, CV_64FC1);
    
    
    int widthSquare = this->patchWidth*this->patchWidth;
    int rangeStart,rangeEnd;
    //int index = 1;
    for( int row = 0; row< this->patchWidth;row++)
    {
        for(int col = 0 ;col < this->patchWidth;col++)
        {
            Mat A1 = res1D.row(row).t();
            Mat B1 = res1D.row(col);
            kernel =  A1 * B1;
            
            
            rangeStart = col*this->patchWidth + row * widthSquare;
            rangeEnd = rangeStart+ this->patchWidth;
            kernel.copyTo(GCKs2D(Range(rangeStart,rangeEnd),Range::all()));
            kernel.convertTo(kernel, CV_8U);
            //imwrite("kernel_"+to_string(index++)+".png", kernel*255);
        }
    }
}

