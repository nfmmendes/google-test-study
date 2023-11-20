#include <iostream>
#include <exception>
#include <gtest/gtest.h>

class InvalidMonthException : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Invalid month";
    }
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

    // Does not validate day and latitude.
    Season getSeason(int day, int month, double latitude)
    {
        if (month % 3 == 0 && month <= 12)
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
            throw InvalidMonthException();
        }
    }
}

TEST(FunctionsWithMultipleParameters, testSummerAndWinterSeasons)
{
    using Season = TimeUtils::Season;
    EXPECT_EQ(TimeUtils::getSeason(1, 1, -10), Season::SUMMER);
    EXPECT_EQ(TimeUtils::getSeason(1, 1, 30), Season::WINTER);
    EXPECT_EQ(TimeUtils::getSeason(4, 2, -10), Season::SUMMER);
    EXPECT_EQ(TimeUtils::getSeason(4, 2, 30), Season::WINTER);
    EXPECT_EQ(TimeUtils::getSeason(20, 3, -10), Season::SUMMER);
    EXPECT_EQ(TimeUtils::getSeason(20, 3, 30), Season::WINTER);
}

TEST(FunctionsWithMultipleParameters, testFallAndSpringSeasons)
{
    using Season = TimeUtils::Season;
    EXPECT_EQ(TimeUtils::getSeason(24, 3, -10), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(24, 3, 30), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(24, 4, -10), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(24, 4, 30), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(24, 5, -10), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(24, 5, 30), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(4, 6, -10), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(4, 6, 30), Season::SPRING);
}

TEST(FunctionsWithMultipleParameters, testWinterAndSummerSeasons)
{
    using Season = TimeUtils::Season;
    EXPECT_EQ(TimeUtils::getSeason(24, 6, -10), Season::WINTER);
    EXPECT_EQ(TimeUtils::getSeason(24, 6, 30), Season::SUMMER);
    EXPECT_EQ(TimeUtils::getSeason(24, 7, -10), Season::WINTER);
    EXPECT_EQ(TimeUtils::getSeason(24, 7, 30), Season::SUMMER);
    EXPECT_EQ(TimeUtils::getSeason(24, 8, -10), Season::WINTER);
    EXPECT_EQ(TimeUtils::getSeason(24, 8, 30), Season::SUMMER);
    EXPECT_EQ(TimeUtils::getSeason(4, 9, -10), Season::WINTER);
    EXPECT_EQ(TimeUtils::getSeason(4, 9, 30), Season::SUMMER);
}

TEST(FunctionsWithMultipleParameters, testSpringAndFallSeasons)
{
    using Season = TimeUtils::Season;
    EXPECT_EQ(TimeUtils::getSeason(24, 9, -10), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(24, 9, 30), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(24, 10, -10), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(24, 10, 30), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(24, 11, -10), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(24, 11, 30), Season::FALL);
    EXPECT_EQ(TimeUtils::getSeason(4, 12, -10), Season::SPRING);
    EXPECT_EQ(TimeUtils::getSeason(4, 12, 30), Season::FALL);
}