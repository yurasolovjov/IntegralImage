#include <iostream>
#include <cstring>
#include <string>

#include "argparser.h"


ArgParser::ArgParser(int argc, char** argv){

    std::cout<<"Parameters: "<<std::endl;
    po::options_description desc("The integral image build\n short description: ");

    desc.add_options()
           ("help,h","help \n")
           ("image,i",po::value<std::string>(),"Processed image")
           ("threads,t",po::value<uint16_t>()->default_value(COUNT_OF_THREAD),"Count of threads")
           ("dump,d",po::value<std::string>()->default_value(DUMP_OF_PATH),"Dump of builded the integral image")
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

    if (vm.count("help")){
          std::cout<<desc<<std::endl;
          this->_isInit = false;
    }

    if(vm.count("image")){
        this->_images.push_back(vm["image"].as<std::string>());
    }

    if(vm.count("threads")){
        this->_countThreads = vm["threads"].as<uint16_t>();
    }

    if(vm.count("dump")){
        this->_dumpImage = vm["dump"].as<std::string>();
    }

    if(vm.count("verbose")){
        this->_verbose = true;
    }
    else this->_verbose = false;
}

ArgParser::~ArgParser(){
}




