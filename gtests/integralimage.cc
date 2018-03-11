#include <gtest/gtest.h>
#include "argparser.h"
#include "integralimage.h"
#include <opencv2/opencv.hpp>

/** @brief Тестовый класс IntegralImage*/
class TestIntegralImage : public ::testing::Test
{

protected:
    void SetUp()
    {

        src = cv::Mat::ones(10,10, CV_8UC3);
        img = new IntegralImage(src);
        img_dst = img->get();

    }
    void TearDown()
    {
        delete img;
    }

public:
    IntegralImage* img;
    Mat src;
    Mat img_dst;
};

/** @brief Тест на проверку размера выходной матрицы
 *  Ожидание: Размеры выходной матрицы совпадает с размером выходной матрицы
*/
TEST_F(TestIntegralImage, testCounterElements){

    EXPECT_EQ( src.rows, img_dst.rows );
    EXPECT_EQ( src.cols, img_dst.cols );
}


/** @brief Тест на проверку корректности алгоритма вычисления.
 *  @warning Предполагается что алгоритм разработа разными людьми
*/
TEST_F(TestIntegralImage, testAlgorithm){

    Mat dst = Mat::ones(src.rows, src.cols, src.channels() == 3 ? CV_64FC3 : src.channels() == 2 ? CV_64FC2 : CV_64F);
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

                EXPECT_EQ(dst.ptr<double>(x,y)[channel], img_dst.ptr<double>(x,y)[channel]);
            };

            for(uint8_t t = 0; t < channels; t++){
                calc(x,y,t);
            }

            }
    }
}
