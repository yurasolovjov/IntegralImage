# IntegralImage
## Описание программы ##
## Зависимости  ##

1. Boost версии 1.66.0 
* Библиотека program_option для разбора входных параметров

2. OpenCV: Open Source Computer Vision Library

### Инструкция по сборке boost 
1. [скачать boost версии 1.66.0]( http://www.boost.org/users/history/version_1_66_0.html)
2. Распаковать скаченный архив командой zcat "архив boost" | tar -xv
3. Перейти в корневой каталог boost и выполнить список команд:
* ./bootstrap.sh gcc
* ./b2 instal --prefix=/usr/lib/boost_1_66

### Инструкция по сборке OpenCV
1. [Клонировать репозиторий OpenCV](http://github.com/opencv/opencv.git)
2. Создать директорию сборки

    Для примера: 

    @code{.bash}
    cd ~/opencv
    mkdir build
    cd build
    @endcode

3.  Конфигурация. Запустите cmake с ключём -D CMAKE_BUILD_TYPE=Release

    For example
    @code{.bash}
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
    @endcode

