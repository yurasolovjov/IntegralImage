#ifndef ARG_PASRSER_H
#define ARG_PASRSER_H

#include <boost/program_options.hpp>
#include <vector>

#define COUNT_OF_THREAD 0
#define DUMP_OF_PATH "./"

namespace po = boost::program_options;

/**
 * @brief ArgParser - Класс разбора параметров командной строки
 * @brief Входные аргументы командной строки:
 * @param \--image(-i) - Входное изображение
 * @param \--threads(-t) - Количество запускаемых потоков
 * @param \--verbose(-v) - Подробный вывод процесса
 * @param \--help(-h)    - Отображение справки программы
 *
*/
class ArgParser{

    public:

    /** @brief Конструктор c параметрами
     *  @param argc - количество аргументов в массиве командной строки
     *  @param argv - массив аргументов командной строки
    */
    ArgParser(int argc, char** argv);

    /** @brief Деструктор класса*/
    ~ArgParser(){};
  

    public:
        /** @brief Метод получения количества входных изображений
         * 	переданных через командную строку
         *  @return std::vector<std::string> - Вектор строк хранящий расположение файлов
        */
        std::vector<std::string> getImages(){ return _images; }

        /** @brief Метод получения количество потоков на исполнение
         * 	переданных через командную строку
         *  @return uint16_t - Количество потоков на исполнение.
         *
        */
        uint16_t getThreads(){ return _countThreads;}

        /** @brief Метод определения корректной инициализации класса
         *  @return bool - True в случае успешного разбора входных параметров.
         *  @return bool - False в противном случае.
        */
        bool isInit(){ return _isInit; }

        /** @brief Метод получения флага режима отладки
         *  @return bool - True если режим подробного вывода информации включён.
         *  @return bool - False в противном случае.
        */
        bool isVerbose(){ return _verbose;}


    private:
        std::vector<std::string> _images;
        bool  	  _isInit;
        bool 	  _verbose;
        uint16_t  _countThreads = 0;
};
		
#endif // ARG_PASRSER_H
