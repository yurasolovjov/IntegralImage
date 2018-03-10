#include <gtest/gtest.h>

TEST(AddTest, TwoAndTwo){

    ASSERT_EQ(5,5);

}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
