#include <string>
#include <gtest/gtest.h>
using std::string;

int returnOne()
{
    return 1;
}

string returnHello()
{
    return "Hello";
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
    // Expect equality.
    EXPECT_EQ(returnOne(), 1);
    EXPECT_EQ(returnHello(), "Hello");
}