#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <chrono>

using namespace cv;

#define DEFAULT_ALGORITHM 1

/** @brief IntegralImage - Класс интегрального изображения
 * Предназначен для конвертации входного изображения в интегральное
*/
class IntegralImage
{
public:
    /** @brief Конструктор с параметрами
     *  @param [in] Mat& src - Матрица изображения с количество каналов n
     *  @warning Диапазон каналов подлежащих тестированию [1,3].
     * Поведение функции для матрицы содержащей свыше трёх каналов - не определено.
    */
    IntegralImage(Mat& src, uint8_t algorithm = DEFAULT_ALGORITHM );

    /** @brief Метод получения матрицы интегрального изображения
     *  @return Mat& - ссылка на объект матрицы интегрального изображения
    */
    Mat& get(){ return dst; }

    /** @brief Метод возвращает время вычисления интегрального изображения
     *  @return std::chrono::duration<double>& - время вычисления ии.
    */
    std::chrono::duration<double>& getTimeCalculate(){ return time ;}

    /** @brief Дружественная функция перегрузки оператора <<
     *  @param [in] IntegralImage& - Класс интегрального изображения
     *  @param [in] std::ofstream& - Открытый файловый поток
     *
    */
    friend std::ofstream& operator <<(std::ofstream& , IntegralImage&);

private:

    /** @brief Метод нахождения интегрального изображения
     * 	@param [in] Mat& - Исходная матрица
     *  @warning Полученное интегрально изображение хранится внитри класса
     *
     * Алгорит расчёта одного элемента интегральной матрицы представлен ниже:
     *
     *\f$ II(x,y) = I(x,y) - II(x-1,y-1) + II(x-1,y) + II(x,y-1)\f$
     *
    */
    void IntegrateAlgorithm_1(Mat& src);

    /** @brief Метод нахождения интегрального изображения
     * 	@param [in] Mat& - Исходная матрица
     *  @warning Полученное интегрально изображение хранится внитри класса
     *
     * Алгорит расчёта одного элемента интегральной матрицы представлен ниже:
     *
     *\f$ S(x,y) = I(x,y) + S(x,y-1)\f$ \n
     *\f$ II(x,y) = II(x-1,y) + S(x,y)\f$ \n
     *\f$ S(x,y)\f$ - Cумма в строке \n
     *\f$ I(x,y)\f$ - Исходное изображение \n
     *\f$ II(x,y)\f$ - Интегральное изображение \n
     *
    */
    void IntegrateAlgorithm_2(Mat& src);

private:
    /** Матрица интегрального изображения*/
    Mat dst ;

    /** */
    std::chrono::duration<double> time ;

public:

    /** @param Алгоритм вычисления интегрального изображения \n
     *\f$ II(x,y) = I(x,y) - II(x-1,y-1) + II(x-1,y) + II(x,y-1)\f$
    */
    static const uint8_t Algotithm1 = 1;

    /** @param Алгоритм вычисления интегрального изображения
     *\f$ S(x,y) = I(x,y) + S(x,y-1)\f$ \n
     *\f$ II(x,y) = II(x-1,y) + S(x,y)\f$ \n
     *\f$ S(x,y)\f$ - Cумма в строке \n
     *\f$ I(x,y)\f$ - Исходное изображение \n
     *\f$ II(x,y)\f$ - Интегральное изображение \n
     *
    */
    static const uint8_t Algotithm2 = 2;


};

#endif // INTEGRALIMAGE_H
