# Walsh Hadamard Transform
This C++ computes the Walsh Hadamard (WH) transform for image.
##Motivation
I want to measure the distance between image patches. One approach is to naively compare the pixel intensity values. With such approach, for an 8x8 rgb-patch you will make 8*8*3 (8*8*rgb) comparisons which is too much. Another problem is that it doesn't address the illumination changes in a proper way.
To get around these two problems, WH transform can be used to compute a descriptor for image patches. The descriptor length provided by this code is 23. So you only make 23 comparisons instead of 192. Also, the WH transform handles the illumination variations in better way than comparing pure RGB/LAB values.

##Setup
This is an Xcode project developing over XOS.
1. Download
2. Make sure to fix the OpenCV library linkage in the project build settings.
3. Run the code and the WH transform will be computed in feature mat

For help, please contact ahmdtaha [@] cs dot umd dot edu

##Contributor list

1. [Ahmed Taha](http://www.cs.umd.edu/~ahmdtaha/)

##Contribution guidelines
I am not an expert in Walsh-Hadamard projections. This implementation is guided by the approached presented in [Coherency Sensitive Hashing](http://www.eng.tau.ac.il/~avidan/papers/ICCV2011_CSH_korman_avidan.pdf). This way I computed by default 23 projections (transforms) for the image. But I am sure this code can be customized in a lot of different way. There is a function that provide the pure WH kernels which you can use in anyway you like. 

![enter image description here](http://www.cs.umd.edu/~ahmdtaha/pub_figs/8_8_WH_Kernels.png)

##License
Copyright (c) 2016, Ahmed Taha (ahmdtaha [@] cs dot umd dot edu)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Re distributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Re distributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
