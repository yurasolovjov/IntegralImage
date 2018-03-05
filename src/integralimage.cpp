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
        Mat d = Mat::ones(src.rows, src.cols, src.channels() == 3 ? CV_64FC3 : src.channels() == 2 ? CV_64FC2 : CV_64FC1);
        dst = d;
        processing(src);
//        processing();
    }
}


void IntegralImage::processing(Mat& src){


    auto start_time = std::chrono::steady_clock::now();

    uint16_t lx = src.rows;
    uint16_t ly = src.cols;
    uint16_t channels = src.channels();

//    #pragma omp parallel
  //  {
        for( int x = 0; x < lx; x++ ){
            for( int y = 0; y < ly ; y++ ){

            auto calc = [&](int x , int y, int channel){

                if( x == 0 && y ==0){
                    uchar a = src.at<Vec3b>(x,y)[channel];
                    dst.at<Vec3d>(x,y)[channel] = a;
                }
                else if( x > 0 && y > 0){

                    uchar a = src.at<Vec3b>(x,y)[channel];
                    double b = dst.at<Vec3d>(x-1,y-1)[channel];
                    double c = dst.at<Vec3d>(x,y-1)[channel];
                    double d = dst.at<Vec3d>(x-1,y)[channel];

                    dst.at<Vec3d>(x,y)[channel] = a - b + c + d;

                }
                else if( y == 0){
                    double a = src.at<Vec3b>(x,y)[channel];
                    double b = dst.at<Vec3d>(x-1,y)[channel];
                    dst.at<Vec3d>(x,y)[channel] = a + b;
                }
                else if( x == 0){

                    double a = src.at<Vec3b>(x,y)[channel];
                    double b = dst.at<Vec3d>(x,y-1)[channel];
                    dst.at<Vec3d>(x,y)[channel] =a + b;
                }
            };

//            #pragma omp parallel for shared(dst)
            for(uint8_t t = 0; t < channels; t++){
                calc(x,y,t);
            }

            }
        }
    //}

    auto end_time = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
    std::cout<<"Calculate time: "<<elapsed.count()<<std::endl;
}

/** Запись в файл нет никакого смысла паралеллить*/
//std::ofstream& operator <<( std::ofstream& f, Mat& m){
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
