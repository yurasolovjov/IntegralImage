#include <iostream>
#include <cstring>
#include <signal.h>
#include <memory>
#include <locale>
#include <fstream>
#include <future>
#include <opencv2/opencv.hpp>
#include "omp.h"
#include "argparser.h"
#include "integralimage.h"

using namespace cv;

int main(int argc, char *argv[]) {


    ArgParser args(argc, argv);

    if(!args.isInit()){
        std::cerr<<"*** ERROR *** Arguments parser is not init"<<std::endl;
        return 0;
    }

    std::vector<std::string> images = args.getImages();
    std::string dumpPath 			= args.getDumpPath();
    uint16_t 	countThreads		= args.getThreads();
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

    if( (countThreads > 0) && (countThreads <= max_threads) ){
        omp_set_num_threads( countThreads );
    }
    else if( countThreads == 0){
        omp_set_num_threads( max_threads );
    }
    else {
        std::cout<<" *** ERROR *** Parameter --threads (-t) is not valid. Maximum number of threads: "<<max_threads<<std::endl;
        return 0;
    }

    auto start_time = std::chrono::steady_clock::now();

    #pragma omp parallel for
    for(int i = 0; i < images.size(); i++){

        std::string asd = images[i] + ".integral";
        std::ofstream ofile(asd,std::ios_base::out | std::ios_base::app);

        Mat integ;
        Mat dst ;
        Mat src = imread( images[i].c_str() );

        if(src.empty()){
            std::cout<<" *** WARNING *** File "<<images[i].c_str()<<" can not be found "<<std::endl;
            continue;
        }

        IntegralImage img(src);

        if(ofile.is_open()){

            ofile << img <<std::endl;
        }

        ofile.close();

 //       cvtColor(src,src,CV_BGR2GRAY);

 //       Mat src2 = Mat::ones(10,10,CV_8UC3);

//      integral(src, integ);


    }

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
    std::cout<<"Program execution time:"<<elapsed.count()<<std::endl;

    return 0;
}