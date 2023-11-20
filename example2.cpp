#include <iostream>
#include <string>
#include <exception>
#include <gtest/gtest.h>

class InvalidMonthException : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Invalid month";
    }
};

bool isBissextile(int year)
{
    return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}

int monthDays(int month)
{
    if (month < 1 || month > 12)
        throw InvalidMonthException();

    if (month == 2)
        return 28;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else
        return 31;
}

// Demonstrate EXPECT_* (TRUE or FALSE).
TEST(BissextileYearTest, BasicAssertions)
{
    EXPECT_FALSE(isBissextile(1900));
    EXPECT_TRUE(isBissextile(1996));
    EXPECT_FALSE(isBissextile(1999));
    EXPECT_TRUE(isBissextile(2000));
    EXPECT_FALSE(isBissextile(2002));
}

// Demonstrate EXPECT_ and EXPECT_ANY_THROW
TEST(MonthDaysTest, EqualsAndThrowAssertions)
{
    EXPECT_EQ(monthDays(4), 30);
    EXPECT_EQ(monthDays(2), 28);
    EXPECT_EQ(monthDays(1), 31);
    EXPECT_EQ(monthDays(7), 31);
    EXPECT_EQ(monthDays(11), 30);
    EXPECT_ANY_THROW(monthDays(13));
    EXPECT_THROW(monthDays(-1), InvalidMonthException);
}