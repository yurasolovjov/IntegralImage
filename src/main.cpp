#include <iostream>
#include <cstring>
#include <signal.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include "omp.h"
#include "argparser.h"
#include "integralimage.h"

static bool isRun = true;

using namespace cv;

void myintegral(Mat& src, Mat& dst);

void signalHandler(int signo) {

    std::cerr << "Signal Handler get signal: ";

    switch( signo ) {
        case SIGABRT:
            std::cerr << "SIGABRT" << std::endl;
        break;

        case SIGTERM:
            std::cerr << "SIGTERM" << std::endl;
        break;

        case SIGINT:
            std::cerr << "SIGINT" << std::endl;
        break;
    };

    isRun = false;
}


using namespace std;

int main(int argc, char *argv[]) {


    ArgParser args(argc, argv);

    if(!args.isInit()){
        std::cerr<<"*** ERROR *** Arguments parser is not init"<<std::endl;
        return 0;
    }

    std::vector<std::string> images = args.getImages();
    std::string dumpPath 			= args.getDumpPath();
    uint8_t 	countThreads		= args.getThreads();
    bool		verbose				= args.isVerbose();
    uint16_t	max_threads			= omp_get_max_threads();

    if(verbose){

        std::cout<<" *** PARAMETR *** verbose: "<<(verbose ? "true" : "false")<<std::endl;
        std::cout<<" *** PARAMETR *** threads(max = "<<max_threads<<"): "<<countThreads<<std::endl;
        std::cout<<" *** PARAMETR *** dump of path: "<<dumpPath<<std::endl;

        for(auto image : images){
            std::cout<<" *** PARAMETER *** image: "<<image<<std::endl;
        }
    }

    if( (countThreads > 0) && (countThreads < max_threads) ){
        omp_set_num_threads( countThreads );
    }
    else {
        omp_set_num_threads( max_threads );
    }


    for(auto image: images){

        Mat integ;
        Mat src = imread( image.c_str() );
        Mat dst = Mat::ones(src.rows,src.cols, CV_64FC3);

        std::cout<<"channels src: "<<src.channels()<<std::endl;
        std::cout<<"channels dst: "<<dst.channels()<<std::endl;

        integral(src, integ);
        myintegral(src, dst);
//        std::cout<<src<<std::endl;

        std::cout<<dst<<std::endl;
        std::cerr << integ << std::endl;

//        imshow("asd",integ);
//        waitKey(0);
//        imshow("Display Image", dst);
//        waitKey(0);
    }

    return 0;
}


void myintegral(Mat& src, Mat& dst){


    uint16_t lx = src.rows;
    uint16_t ly = src.cols;
    uint16_t channels = src.channels();

    #pragma omp parallel shared(dst)
    {
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
                    dst.at<Vec3d>(x,y)[channel] =a + b;
                }
                else if( x == 0){

                    double a = src.at<Vec3b>(x,y)[channel];
                    double b = dst.at<Vec3d>(x,y-1)[channel];
                    dst.at<Vec3d>(x,y)[channel] =a + b;
                }
            };

            #pragma omp parallel for
            for(uint8_t t = 0; t < channels; t++){
                calc(x,y,t);
            }

            }
        }
    }
}
