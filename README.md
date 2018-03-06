# IntegralImage
## Описание программы ##
Программное обеспечение IntegralImage преденазначено для конвертации входного изображения \n
в интегральное изображение 
## Зависимости  ##

1. Boost версии 1.66.0 
* Библиотека program_option для разбора входных параметров

2. OpenCV: Open Source Computer Vision Library
3. OpenMP: Открытый стандарт для распараллеливания программ

### Инструкция по сборке boost 
1. [скачать boost версии 1.66.0]( http://www.boost.org/users/history/version_1_66_0.html)
2. Распаковать скаченный архив командой zcat "архив boost" | tar -xv
3. Перейти в корневой каталог boost и выполнить список команд:
* ./bootstrap.sh gcc
* ./b2 instal --prefix=/usr/lib/boost_1_66

### Инструкция по сборке OpenCV
1. [Клонировать репозиторий OpenCV](http://github.com/opencv/opencv.git)
2. [Выполнить инструкции разработчика библиотеки OpenCV](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)


