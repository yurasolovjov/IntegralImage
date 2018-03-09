## Описание программы ##
Программное обеспечение integral_image преденазначено для конвертации входного изображения \n
в интегральное изображение каждый элемент которого рассчитывается по формуле:
\f$ I(x_i,y_i) = I(x_i,y_i) - I(x_{i-1},y_{i-1}) + I(x_{i-1},y_i) + I(x_i,y_{i-1}) \f$

----------------------------------------------------------------------------------------
## Зависимости  ##

1. Boost версии 1.66.0 
  * Библиотека program_option для разбора входных параметров

2. OpenCV: Open Source Computer Vision Library
3. OpenMP: Открытый стандарт для распараллеливания программ

### Инструкция по сборке boost ###
1. [скачать boost версии 1.66.0]( http://www.boost.org/users/history/version_1_66_0.html)
2. Распаковать скаченный архив командой zcat "архив boost" | tar -xv
3. Перейти в корневой каталог boost и выполнить список команд:
@code{.bash}
 ./bootstrap.sh gcc
 ./b2 instal --prefix=/usr/lib/boost_1_66
@endcode

### Инструкция по сборке OpenCV ###
1. [Клонировать репозиторий OpenCV](http://github.com/opencv/opencv.git)
2. [Выполнить инструкции разработчика библиотеки OpenCV](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)


------------------------------------------------------------------------------------------
## Описание запуска программного обеспечения ##

### Программный компонент integral_image запускается с нижеперечисленными аргументами:###
* --image(-i)   - Входное изображение(циклический параметр. Может повторяться n раз)
* --threads(-t) - Количество запускаемых потоков
* --verbose(-v) - Подробный вывод процесса
* --help(-h)    - Отображение справки программы


------------------------------------------------------------------------------------------
# Пример запуска integral_image #

1. 
@code{.bash}
./integral_image --image 001.jpg --image 002.jpg --threads 2 --verbose
@endcode

2. 
@code{.bash}
./integral_image -i 001.jpg -i 002.jpg -i 003.jpg -t 3
@endcode


------------------------------------------------------------------------------------------

# Информационные сообщения integral_image #

В процессе выполнения программного компонента integral_image при выставленном ключе --verbose(см. п. Описание запуска программного обеспечения) могут пояляться информационные сообщения описаные в таблице 1.

### Таблица 1 "Информационные сообщения integral_image"

Тип сообщения | Описание | 
---|---|
| PARAMETER| Информация о загруженных аргументах программы|
| PROCESSED | Выполнение операции завершено. |
| WARNING | Предупреждение о невыполнении операции |
| ERROR | Ошибка программного компонента. Завершение работы программы |


------------------------------------------------------------------------------------------