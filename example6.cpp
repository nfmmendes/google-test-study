#include <vector>
#include <exception>
#include <algorithm>
using std::begin;
using std::exception;
using std::reverse;
using std::vector;
#include <gtest/gtest.h>

class InvalidFirstIndexException : public exception
{
    virtual const char *what() const throw()
    {
        return "Invalid first index exception. This index must reference an element before both the modified interval (that has minimum size 2) and the second index";
    }
};

class InvalidSecondIndexException : public exception
{
    virtual const char *what() const throw()
    {
        return "Invalid second index exception. This index must reference an element after both the modified interval (that has minimum size 2) and the first index";
    }
};

class TooShortRouteException : public exception
{
    virtual const char *what() const throw()
    {
        return "The route must have a size greater than 3 to apply a two-opt.";
    }
};

class TooShortTwoOptRangeException : public exception
{
    virtual const char *what() const throw()
    {
        return "The two opt range must have a minimum size greater than one.";
    }
};

void twoOpt(vector<int> &route, int firstIndex, int secondIndex)
{
    /**
     * This function alters the route R, inverting the subsequence delimited by <<firstIndex>> and
     * <<secondIndex>>. After this invertion the element referenced by <<firstIndex>> will be succeded
     * by the one that preceeds that referenced by <<secondIndex>>, and element referenced by
     * <<secondIndex>> will be preceeded by the one the succeded the one referenced by <<firstIndex>>.
     * Ex:
     *    Original route R = A -- B -- C -- D -- E -- F, firstIndex = 1, secondIndex = 4
     *                        firstIndex    secondIndex
     *    Final route    R = A -- B -- D -- C -- E -- F
     */

    if (secondIndex < firstIndex)
        std::swap(secondIndex, firstIndex);

    // Out of range exceptions will not be managed here

    if (route.size() < 4)
        throw TooShortRouteException();

    if (firstIndex >= route.size() - 2)
        throw InvalidFirstIndexException();
    if (secondIndex < 3)
        throw InvalidSecondIndexException();

    // It must have at least two elements between the two-opt extremities
    if (secondIndex <= firstIndex + 2)
        throw TooShortTwoOptRangeException();

    std::reverse(begin(route) + firstIndex + 1, begin(route) + secondIndex);
}

TEST(TwoOptTests, tooShortRoute)
{
    vector<int> route{1, 2, 3};
    const int firstIndex{1};
    const int secondIndex{3};

    EXPECT_THROW(twoOpt(route, 1, 3), TooShortRouteException);
}

TEST(TwoOptTests, tooShortTwoOptExceptions)
{
    vector<int> route{1, 2, 3, 4, 5};
    int firstIndex{1};
    int secondIndex{3};

    EXPECT_THROW(twoOpt(route, firstIndex, secondIndex), TooShortTwoOptRangeException);

    firstIndex = 2;
    secondIndex = 4;

    EXPECT_THROW(twoOpt(route, firstIndex, secondIndex), TooShortTwoOptRangeException);
}

TEST(TwoOptTests, invalidFirstIndex)
{
    vector<int> route{1, 2, 3, 4, 5};
    const int firstIndex{3};
    const int secondIndex{4};

    EXPECT_THROW(twoOpt(route, firstIndex, secondIndex), InvalidFirstIndexException);
}

TEST(TwoOptTests, invalidSecondIndex)
{
    vector<int> route{1, 2, 3, 4, 5};
    const int firstIndex{1};
    const int secondIndex{2};

    EXPECT_THROW(twoOpt(route, firstIndex, secondIndex), InvalidSecondIndexException);
}

TEST(TwoOptTests, simpleTwoOpt)
{
    vector<int> route{1, 2, 3, 4};
    vector<int> control{route};

    EXPECT_NO_THROW(twoOpt(route, 0, 3));
    EXPECT_EQ(route[0], control[0]);
    EXPECT_EQ(route[1], control[2]);
    EXPECT_EQ(route[2], control[1]);
    EXPECT_EQ(route[3], control[3]);
}

TEST(TwoOptTests, largeTwoOptEvenRange)
{
    vector<int> route{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> control{route};

    EXPECT_NO_THROW(twoOpt(route, 2, 8));
    EXPECT_EQ(route[0], control[0]);
    EXPECT_EQ(route[1], control[1]);
    EXPECT_EQ(route[2], control[2]);
    EXPECT_EQ(route[3], control[7]);
    EXPECT_EQ(route[4], control[6]);
    EXPECT_EQ(route[5], control[5]);
    EXPECT_EQ(route[8], control[8]);
    EXPECT_EQ(route[9], control[9]);
}

TEST(TwoOptTests, largeTwoOptOddRange)
{
    vector<int> route{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> control{route};

    EXPECT_NO_THROW(twoOpt(route, 2, 9));
    EXPECT_EQ(route[0], control[0]);
    EXPECT_EQ(route[1], control[1]);
    EXPECT_EQ(route[2], control[2]);
    EXPECT_EQ(route[3], control[8]);
    EXPECT_EQ(route[4], control[7]);
    EXPECT_EQ(route[5], control[6]);
    EXPECT_EQ(route[9], control[9]);
}