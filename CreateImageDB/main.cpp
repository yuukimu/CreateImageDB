//
//  main.cpp
//  CreateImageDB
//
//  Created by 室川優希 on 2017/11/24.
//  Copyright © 2017年 室川優希. All rights reserved.
//

#include <iostream>
#include<fstream>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void initRGB(long *R, long *G, long *B) {
    for (int i = 0; i < 16; i++) {
        R[i] = 0;
        G[i] = 0;
        B[i] = 0;
    }
}

std::string outputEachData(long *ary, bool end) {
    std::string str = "";
    for (int i = 0; i < 16; i++) {
        if (end && i == 15) {
            str += std::to_string(ary[i]) + ":";
        } else {
            str += std::to_string(ary[i]) + " ";
        }
    }
    return str;
}

void writeToFile(long *R, long *G, long *B) {
    std::string str = "";
    str += outputEachData(R, false);
    str += outputEachData(G, false);
    str += outputEachData(B, true);
    std::cout << str << std::endl;
    std::ofstream outputfile("result.txt");
    outputfile << str;
    outputfile << "watch/image_0014.ppm\n";
    outputfile.close();
}

int main(int argc, const char * argv[]) {
    char dir[255];
    getcwd(dir,255);
    long R[16], G[16], B[16];
    initRGB(R, G, B);
    cv::Mat input_img = cv::imread("watch/image_0014.ppm");
    int width = input_img.cols;
    int height = input_img.rows;
    
    for (int y = 0; y < height; y++) {
        cv::Vec3b* ptr = input_img.ptr<cv::Vec3b>(y);
        for (int x = 0; x < width; x++) {
            cv::Vec3b bgr = ptr[x];
            R[bgr[2] / 16] += 1;
            G[bgr[1] / 16] += 1;
            B[bgr[0] / 16] += 1;
        }
    }
    writeToFile(R, G, B);
    return 0;
}

