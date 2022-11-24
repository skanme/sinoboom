#ifndef YOLO_H
#define YOLO_H
extern int cx,cy;
#pragma once
#pragma once

#include<iostream>
#include<math.h>
#include<opencv2/opencv.hpp>
#include <vector>

#define YOLO_P6 False
using namespace std;
using namespace cv;
struct Output {
    int id;//结果类别id
    float confidence;//结果置信度
    cv::Rect box;//矩形框
};
class Yolo
{
public:
    Yolo() {
    }
    ~Yolo() {}

private:
    //计算归一化函数
    float Sigmoid(float x) {
        return static_cast<float>(1.f / (1.f + exp(-x)));
    }
    //anchors
    const float netAnchors[3][6] = { { 10,13, 16,30, 33,23 },{ 30,61, 62,45, 59,119 },{116,90, 156,198, 373,326 } };
    //stride
    const float netStride[3] = { 8.0, 16.0, 32.0 };
    const int netWidth = 640; //网络模型输入大小
    const int netHeight = 640;
    float nmsThreshold = 0.5;
    float boxThreshold = 0.25;
    float classThreshold = 0.25;
    //类名
    std::vector<std::string> className = { "robot" };

public:
    float x,y,w,h;
    float ratio_h,ratio_w;
    char temp_1[20],temp_2[20];


public:
    bool readModel(cv::dnn::Net& net, std::string& netPath, bool isCuda);
    bool Detect(cv::Mat& SrcImg, cv::dnn::Net& net, std::vector<Output>& output);
    void drawPred(cv::Mat& img, std::vector<Output> result, std::vector<cv::Scalar> color);

};




#endif // YOLO_H
