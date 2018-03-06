#include <iostream>
#include <cstring>
#include <signal.h>
#include <memory>
#include <locale>
#include <fstream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "omp.h"
#include "argparser.h"
#include "integralimage.h"
#include <thread>
#include <future>

IntegralImage::IntegralImage(Mat& src){

    if(!src.empty()){
        dst = Mat::ones(src.rows, src.cols, src.channels() == 3 ? CV_64FC3 : src.channels() == 2 ? CV_64FC2 : CV_64F);

        if(!dst.empty()){
            IntegrateAlgorithm(src);
        }
    }
}


void IntegralImage::IntegrateAlgorithm(Mat& src){

    uint16_t lx = src.rows;
    uint16_t ly = src.cols;
    uint16_t channels = src.channels();

    for( int x1 = 0; x1 < lx; x1++ ){
        for( int y1 = 0; y1 < ly ; y1++ ){

            auto calc = [&](int x , int y, int channel){

                if( x == 0 && y ==0){
                    uchar a = src.ptr<uchar>(x,y)[channel];
                    dst.ptr<double>(x,y)[channel] = a;
                }
                else if( x > 0 && y > 0){

                    uchar  a = src.ptr<uchar>(x,y)[channel];
                    double b = dst.ptr<double>(x-1,y-1)[channel];
                    double c = dst.ptr<double>(x,y-1)[channel];
                    double d = dst.ptr<double>(x-1,y)[channel];

                    dst.ptr<double>(x,y)[channel] = a - b + c + d;

                }
                else if( y == 0){
                    uchar a = src.ptr<uchar>(x,y)[channel];
                    double b = dst.ptr<double>(x-1,y)[channel];
                    dst.ptr<double>(x,y)[channel] = a + b;
                }
                else if( x == 0){

                    uchar a = src.ptr<uchar>(x,y)[channel];
                    double b = dst.ptr<double>(x,y-1)[channel];
                    dst.ptr<double>(x,y)[channel] = a + b;
                }
            };

            for(uint8_t t = 0; t < channels; t++){
                calc(x1,y1,t);
            }

            }
    }
}

/** Запись в файл нет никакого смысла паралеллить*/
std::ofstream& operator <<( std::ofstream& f, IntegralImage& i){

    Mat m = i.get();

    auto write = [&](int channel){

        uint16_t lx = m.rows;
        uint16_t ly = m.cols;

        for(uint16_t x = 0; x < lx ; x++){
            for(uint16_t y = 0; y < ly ; y++){
                f<<static_cast<double>(m.at<Vec3b>(x,y)[channel])<<", ";
            }
            f<<std::endl;
        }
    };

    for(uint16_t i = 0 ; i < m.channels(); i ++){
        write(i);
        f << std::endl;
    }

    return f;
}
