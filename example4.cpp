#include <iostream>
#include <string>
#include <exception>
#include <gtest/gtest.h>

class InvalidDate : public std::exception
{
public:
    InvalidDate(bool invalidDay, bool invalidMonth)
    {
        if (invalidDay && invalidMonth)
            message = "Invalid day and month";
        else if (!invalidDay)
            message = "Invalid day";
        else if (!invalidMonth)
            message = "Invalid month";
    }

    virtual const char *what() const throw()
    {
        return "Invalid month";
    }

private:
    std::string message;
};

namespace TimeUtils
{
    enum Season
    {
        SUMMER,
        WINTER,
        SPRING,
        FALL
    };

    bool isValidMonth(int month)
    {
        return month >= 1 && month <= 12;
    }

    int monthDays(int month)
    {
        if (!isValidMonth(month))
            throw InvalidDate(false, true);

        if (month == 2)
            return 28;
        if (month == 4 || month == 6 || month == 9 || month == 11)
            return 30;
        else
            return 31;
    }

    bool isValidDay(int day, int month)
    {
        return day > 0 && day <= monthDays(month);
    }

    // Does not validate latitude.
    Season getSeason(int day, int month, double latitude)
    {
        bool isInvalidMonth = !isValidMonth(month);
        bool isInvalidDay = !isValidDay(day, month);

        if (isInvalidMonth || isInvalidDay)
            throw InvalidDate(isInvalidDay, isInvalidMonth);

        if (month % 3 == 0)
            month = day < 23 ? month - 1 : (month + 1) % 12;

        switch (month)
        {
        case 1:
        case 2:
            return latitude > 0 ? Season::WINTER : Season::SUMMER;
        case 4:
        case 5:
            return latitude > 0 ? Season::SPRING : Season::FALL;
        case 7:
        case 8:
            return latitude > 0 ? Season::SUMMER : Season::WINTER;
        case 10:
        case 11:
            return latitude > 0 ? Season::FALL : Season::SPRING;
        default:
            // It could be marked [[unlikely]] on C++20
            throw InvalidDate(isInvalidDay, isInvalidMonth);
        }
    }

    bool isSummer(int day, int month, double latitude)
    {
        return getSeason(day, month, latitude) == Season::SUMMER;
    }
}

TEST(PredicateTests, ValidMonths)
{
    EXPECT_PRED1(TimeUtils::isValidMonth, 1);
    EXPECT_PRED1(TimeUtils::isValidMonth, 2);
    EXPECT_PRED1(TimeUtils::isValidMonth, 3);
    EXPECT_PRED1(TimeUtils::isValidMonth, 10);
    EXPECT_PRED1(TimeUtils::isValidMonth, 11);
    EXPECT_PRED1(TimeUtils::isValidMonth, 12);
}

TEST(PredicateTests, ValidDays)
{
    EXPECT_PRED2(TimeUtils::isValidDay, 20, 1);
    EXPECT_PRED2(TimeUtils::isValidDay, 28, 2);
    EXPECT_PRED2(TimeUtils::isValidDay, 31, 3);
    EXPECT_PRED2(TimeUtils::isValidDay, 31, 10);
    EXPECT_PRED2(TimeUtils::isValidDay, 30, 11);
    EXPECT_PRED2(TimeUtils::isValidDay, 31, 12);
}

TEST(PredicateTests, SummerDays)
{
    EXPECT_PRED3(TimeUtils::isSummer, 1, 1, -10);
    EXPECT_PRED3(TimeUtils::isSummer, 4, 2, -10);
    EXPECT_PRED3(TimeUtils::isSummer, 20, 3, -10);
    EXPECT_PRED3(TimeUtils::isSummer, 24, 6, 30);
    EXPECT_PRED3(TimeUtils::isSummer, 24, 7, 30);
    EXPECT_PRED3(TimeUtils::isSummer, 24, 8, 30);
    EXPECT_PRED3(TimeUtils::isSummer, 4, 9, 30);
}

TEST(AssertTests, InvalidMonths)
{
    ASSERT_FALSE(TimeUtils::isValidMonth(-1));
    ASSERT_FALSE(TimeUtils::isValidMonth(13));
    ASSERT_FALSE(TimeUtils::isValidMonth(0));
}

TEST(AssertTests, InvalidDays)
{
    ASSERT_FALSE(TimeUtils::isValidDay(-20, 1));
    ASSERT_FALSE(TimeUtils::isValidDay(29, 2));
    ASSERT_FALSE(TimeUtils::isValidDay(32, 3));
    ASSERT_FALSE(TimeUtils::isValidDay(0, 10));
    ASSERT_FALSE(TimeUtils::isValidDay(31, 11));
    ASSERT_FALSE(TimeUtils::isValidDay(32, 12));
}

TEST(AssertTests, IsNotSummer)
{
    ASSERT_FALSE(TimeUtils::isSummer(1, 1, 10));
    ASSERT_FALSE(TimeUtils::isSummer(4, 2, 10));
    ASSERT_FALSE(TimeUtils::isSummer(20, 3, 10));
    ASSERT_FALSE(TimeUtils::isSummer(24, 6, -30));
    ASSERT_FALSE(TimeUtils::isSummer(24, 7, -30));
    ASSERT_FALSE(TimeUtils::isSummer(24, 8, -30));
    ASSERT_FALSE(TimeUtils::isSummer(4, 9, -30));
}