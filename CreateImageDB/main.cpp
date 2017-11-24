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
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// 配列の初期化
void initRGB(long *R, long *G, long *B);
// 出力データの整形
std::string getFormatData(long *ary, bool end);
// ディレクトリ一覧の取得
std::vector<std::string> getDirs();
// 各ディレクトリ内の画像一覧を取得
std::vector<std::string> getImages(const char *path);
// ヒストグラムデータの出力
void createHistogram(std::vector<std::string> dirs);
// ヒストグラムの作成
std::string getHistgram(std::string input_path);

void initRGB(long *R, long *G, long *B) {
    for (int i = 0; i < 16; i++) {
        R[i] = 0;
        G[i] = 0;
        B[i] = 0;
    }
}

std::string getFormatData(long *ary, bool end) {
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

std::vector<std::string> getDirs() {
    std::vector<std::string> dirs;
    const char* path = "./";
    DIR *dp;
    dirent* entry;
    dp = opendir(path);
    if (dp==NULL) exit(1);
    // .と..をスキップ
    readdir(dp);
    readdir(dp);
    
    do {
        entry = readdir(dp);
        if (entry != NULL && entry->d_type == DT_DIR) {
            dirs.push_back(entry->d_name);
        }
    } while (entry != NULL);
    return dirs;
}

std::vector<std::string> getImages(const char *path) {
    std::vector<std::string> imgs;
    DIR *dp;
    dirent* entry;
    dp = opendir(path);
    if (dp==NULL) exit(1);
    
    do {
        entry = readdir(dp);
        if (entry != NULL && entry->d_type == DT_REG) {
            imgs.push_back(entry->d_name);
        }
    } while (entry != NULL);
    return imgs;
}

void createHistogram(std::vector<std::string> dirs) {
    std::ofstream outputfile("histgram.txt");
    for(std::string dir : dirs) {
        std::vector<std::string> imgs = getImages(dir.c_str());
        for(std::string img : imgs) {
            std::string path = dir + "/" + img;
            outputfile << getHistgram(path);
        }
    }
    outputfile.close();
}

std::string getHistgram(std::string input_path) {
    long R[16], G[16], B[16];
    initRGB(R, G, B);
    cv::Mat input_img = cv::imread(input_path);
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
    std::string str = getFormatData(R, false);
    str += getFormatData(G, false);
    str += getFormatData(B, true);
    str += input_path + "\n";
    return str;
}

int main(int argc, const char * argv[]) {
//    char dir[255];
//    getcwd(dir,255);
    std::vector<std::string> dirs = getDirs();
    createHistogram(dirs);
    return 0;
}

