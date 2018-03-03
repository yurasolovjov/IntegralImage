#ifndef ARG_PASRSER_H
#define ARG_PASRSER_H

#include <boost/program_options.hpp>
#include <vector>

#define COUNT_OF_THREAD 0
#define DUMP_OF_PATH "./"

namespace po = boost::program_options;

class ArgParser{

    public:

    /** Конструктор */
    ArgParser(int argc, char** argv);

    /* Деструктор */
    ~ArgParser();
  

    public:

        std::vector<std::string> getImages(){ return _images; }
        std::string getDumpPath(){ return _dumpImage; }
        uint16_t getThreads(){ return _countThreads;}
        bool isInit(){ return _isInit; }
        bool isVerbose(){ return _verbose;}


    private:
        std::vector<std::string> _images;
        std::string _dumpImage;
        bool        _isInit;
        bool        _verbose;
        uint16_t		_countThreads = 0;
};
		
#endif // ARG_PASRSER_H
