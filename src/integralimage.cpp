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

IntegralImage::IntegralImage(Mat& src, uint8_t algorithm){

    if(!src.empty()){
        dst = Mat::ones(src.rows, src.cols, src.channels() == 3 ? CV_64FC3 : src.channels() == 2 ? CV_64FC2 : CV_64F);

        if(!dst.empty()){
            switch (algorithm) {
                case Algotithm2:
                    IntegrateAlgorithm_2(src);
                break;

                default:
                    IntegrateAlgorithm_1(src);
                break;
            }
        }
    }
}

/** @brief Метод нахождения интегрального изображения
* 	@param [in] Mat& - Исходная матрица
*   @warning Полученное интегрально изображение хранится внитри класса
*
* Алгорит расчёта одного элемента интегральной матрицы представлен ниже:
*
*\f$ II(x_i,y_i) = I(x_i,y_i) - I(x_{i-1},y_{i-1}) + I(x_{i-1},y_i) + I(x_i,y_{i-1}) \f$
*
*/
void IntegralImage::IntegrateAlgorithm_1(Mat& src){

    auto start_time = std::chrono::steady_clock::now();
    uint16_t lx = src.rows;
    uint16_t ly = src.cols;
    uint16_t channels = src.channels();

    for( int x = 0; x < lx; x++ ){
        for( int y = 0; y < ly ; y++ ){

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
                calc(x,y,t);
            }

            }
    }

    auto end_time = std::chrono::steady_clock::now();

    /* Рассчёт затраченного времени на вычисление ИИ*/
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
}

/** @brief Метод нахождения интегрального изображения
* 	@param [in] Mat& - Исходная матрица
*   @warning Полученное интегрально изображение хранится внитри класса
*
* Алгорит расчёта одного элемента интегральной матрицы представлен ниже:
*
*\f$ S(x,y) = I(x,y) + S(x,y-1)\f$ \n
*\f$ II(x,y) = II(x-1,y) + S(x,y)\f$ \n
*\f$ S(x,y)\f$ - Cумма в строке \n
*\f$ I(x,y)\f$ - Исходное изображение \n
*\f$ II(x,y)\f$ - Интегральное изображение \n
*
*/
void IntegralImage::IntegrateAlgorithm_2(Mat& src){

    auto start_time = std::chrono::steady_clock::now();
    Mat 	 sum = Mat::ones(src.rows,
                             src.cols,
                             src.channels() == 3 ? CV_64FC3 :
                             src.channels() == 2 ? CV_64FC2 : CV_64F);
    uint16_t lx  = src.rows;
    uint16_t ly  = src.cols;
    uint16_t channels = src.channels();

    #pragma omp parallel for
    for(uint8_t i = 0; i < channels; i++){
        for( int x = 0; x < lx; x ++){
            for( int y = 0; y < ly; y++){

                auto calc = [&](int x , int y, int channel){

                    if( x == 0 && y ==0){
                        sum.ptr<double>(x,y)[channel] = src.ptr<uchar>(x,y)[channel];
                        dst.ptr<double>(x,y)[channel] = sum.ptr<double>(x,y)[channel];
                    }

                    else if( x > 0 && y > 0){
                        sum.ptr<double>(x,y)[channel] = src.ptr<uchar>(x,y)[channel] + sum.ptr<double>(x,y-1)[channel];
                        dst.ptr<double>(x,y)[channel] = dst.ptr<double>(x-1,y)[channel] + sum.ptr<double>(x,y)[channel];
                    }
                    else if( y == 0){
                        sum.ptr<double>(x,y)[channel] = src.ptr<uchar>(x,y)[channel] ;
                        dst.ptr<double>(x,y)[channel] = dst.ptr<double>(x-1,y)[channel] + sum.ptr<double>(x,y)[channel];
                    }
                    else if( x == 0){
                        sum.ptr<double>(x,y)[channel] = src.ptr<uchar>(x,y)[channel] + sum.ptr<double>(x,y-1)[channel];
                        dst.ptr<double>(x,y)[channel] = sum.ptr<double>(x,y)[channel];
                    }
                };

                calc(x,y,i);
            }
        }
    }

    /* Рассчёт затраченного времени на вычисление ИИ*/
    auto end_time = std::chrono::steady_clock::now();
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
}

/** Запись в файл нет никакого смысла паралеллить*/
std::ofstream& operator <<( std::ofstream& f, IntegralImage& i){

    Mat m = i.get();

    auto write = [&](int channel){

        uint16_t lx = m.rows;
        uint16_t ly = m.cols;

        for(uint16_t x = 0; x < lx ; x++){
            for(uint16_t y = 0; y < ly ; y++){
                f<<static_cast<double>(m.ptr<double>(x,y)[channel])<<", ";
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
