#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;


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
    IntegralImage(Mat& src);

    /** @brief Метод получения матрицы интегрального изображения
     *  @return Mat& - ссылка на объект матрицы интегрального изображения
    */
    Mat& get(){ return dst; }

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
     *\f$ II(x_i,y_i) = I(x_i,y_i) - I(x_{i-1},y_{i-1}) + I(x_{i-1},y_i) + I(x_i,y_{i-1}) \f$
     *
    */
    void IntegrateAlgorithm(Mat& src);

private:
    /** Матрица интегрального изображения*/
    Mat dst ;
};

#endif // INTEGRALIMAGE_H
