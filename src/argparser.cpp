#include <iostream>
#include <cstring>
#include <string>

#include "argparser.h"


/** @brief Конструктор c параметрами
*   @param argc - количество аргументов в массиве командной строки
*   @param argv - массив аргументов командной строки
*/
ArgParser::ArgParser(int argc, char** argv){

    po::options_description desc("The integral image build\n short description: ");

    /** Структура разбора аргументов командной строки*/
    desc.add_options()
           ("help,h","help \n")
           ("image,i",po::value<std::vector<std::string>>(),"Processed image. (loop options)")
           ("threads,t",po::value<uint16_t>()->default_value(COUNT_OF_THREAD),"Count of threads")
           ("verbose,v","Verbose option")
    ;

    po::variables_map vm;

    try{
          po::store(po::parse_command_line(argc, argv, desc), vm);
          po::notify(vm);
          this->_isInit = true;
    }
    catch(po::error& e){
         this->_isInit = false;
         std::cerr<<"*** ERROR *** "<<e.what()<<std::endl;
    }

    /** Вывод справочного сообщения */
    if (vm.count("help")){
          std::cout<<desc<<std::endl;
          this->_isInit = false;
    }

    /** Получение пути расположения файла изображения */
    if(vm.count("image")){
        this->_images = (vm["image"].as<std::vector<std::string>>());
    }

    /** Получение количества потоков переданных через командную строку */
    if(vm.count("threads")){
        this->_countThreads = vm["threads"].as<uint16_t>();
    }

    /** Получение флага подробного вывода информации о выполнении процесса */
    if(vm.count("verbose")){
        this->_verbose = true;
    }
    else this->_verbose = false;
}
