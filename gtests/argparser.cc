#include <gtest/gtest.h>
#include "argparser.h"
#include "integralimage.h"
#include <opencv2/opencv.hpp>

/** @brief Тестовый класс IntegralImage*/
class TestArgParser : public ::testing::Test
{

protected:
    void SetUp()
    {
        char* argv[] = {"program name","--image", "000_img.jpg","--image", "000_img.jpg","--threads=3","--verbose"};
        int argc = 7;

        args = new ArgParser(argc,argv);
    }
    void TearDown()
    {
        delete args;

    }

public:

    ArgParser* args;
};

/** @brief Тест на проверку корректности работоспособности класса разбора
 * парамметров
*/
TEST_F(TestArgParser, testArgParser){

    EXPECT_TRUE(args->isInit() );

    /* Проверка корректности получения аргументов изображения*/
    for(auto v : args->getImages()){
        EXPECT_EQ("000_img.jpg", v);
    }

    /* Проверка корректности получения аргументов запуска потока*/
    EXPECT_EQ(3, args->getThreads() );

    /* Проверка корректности инициализации класса :w*/
    EXPECT_TRUE(args->isVerbose() );
}
