#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;

class IntegralImage
{
public:
    IntegralImage(Mat& src);

    Mat& get(){ return dst; }

    friend std::ofstream& operator <<(std::ofstream& , IntegralImage&);
    void processing();
private:
    void processing(Mat& src);

private:
    Mat dst;

};

#endif // INTEGRALIMAGE_H
