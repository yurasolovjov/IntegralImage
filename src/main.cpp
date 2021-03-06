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


#define FIRST_ELEMENT(e) e & 0x01

using namespace cv;

static const std::string postfixName = ".integral";

/** @brief Программный компонент конвертации матрицы изображения в интегральную матрицу *
 *  @author Соловьёв Ю.И.
 *  @version 1.0
 *  @date Март 2017 года
 *
 * @param [in] int argc - количество аргументов передаваемых через командную строку
 * @param [in] char** argv - массив аргументов передаваемых через командную строку
*/
int main(int argc, char *argv[]) {

    /** Класс разбора параметров */
    ArgParser args(argc, argv);

    if(!args.isInit()){
        std::cerr<<"*** ERROR *** Arguments parser is not init"<<std::endl;
        return 0;
    }

    std::vector<std::string> images = args.getImages();
    uint16_t 	countThreads		= args.getThreads();
    bool		verbose				= args.isVerbose();
    uint16_t	max_threads			= omp_get_max_threads();

    if(verbose){

        std::cout<<"=======================================================================\n"<<std::endl;
        std::cout<<" *** PARAMETR *** verbose: "<<(verbose ? "true" : "false")<<std::endl;
        std::cout<<" *** PARAMETR *** threads(max = "<<max_threads<<"): "<<countThreads<<std::endl;

        for(auto image : images){
            std::cout<<" *** PARAMETER *** image: "<<image<<std::endl;
        }
        std::cout<<"=======================================================================\n"<<std::endl;
    }

    /** Функции OpenMP Получающие информациию о возможно-запускаемом количестве потоков
     * и соответственно устанавливающие их
    */
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


    /** Дириктива OpenMP сообщающая о необходимости распараллеливания.
     *  Поддерживается всеми известными компиляторами*/
    #pragma omp parallel for if( images.size() > 1)
    for(int i = 0; i < images.size(); i++){

        std::string fileIntegralSave = images[i] + postfixName;

        Mat src = imread( images[i].c_str() );

        if( src.empty() ){
            std::cout<<" *** WARNING *** File "<<images[i].c_str()<<" can not be found "<<std::endl;
            continue;
        }

        /** В случае если количество изображений больше одного,
         *  то будёт премененён стандартный алгоритм, а изображения будут вычисляться параллельно*/
        uint8_t algorithm = images.size() > 1 ? IntegralImage::Algotithm1 :
                                                IntegralImage::Algotithm2;
        IntegralImage img(src,algorithm);

        if(verbose){
            std::chrono::duration<double> time = img.getTimeCalculate();
            std::cout<<" *** PROCESSED *** Image "<<images[i]<<" conversion time: "<<time.count()<<" ms"<<std::endl;
        }

        auto start_time = std::chrono::steady_clock::now();

        std::ofstream ofile(fileIntegralSave, std::ios_base::out | std::ios_base::app);

        if(ofile.is_open()){
            ofile << img <<std::endl;
        }

        ofile.close();

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);

        if( verbose ){
            std::cout<<"*** PROCESSED *** Image "<<images[i]
                     <<" has been converted. Save file: "<<fileIntegralSave
                     <<" Spent time: "<<elapsed.count()<<" ms"<<std::endl;
        }
    }

    return 0;
}
