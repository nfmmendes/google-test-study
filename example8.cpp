#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
using std::move;
using std::vector;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

template <class T>
class ExtendedVector
{

public:
    ExtendedVector() = delete;
    ExtendedVector(const vector<T> &input)
    {
        for (auto &item : input)
        {
            container.push_back(item);
        }
    }

    template <class... Ts>
    ExtendedVector(Ts... elements)
    {
        (container.push_back(elements), ...);
    }

    template <class... Ts>
    void insert(Ts... elements)
    {
        (container.push_back(elements), ...);
    }

    virtual size_t count() const
    {
        return container.size();
    }

    virtual int binarySearch(int start, int end, T &&value) const
    {
        if (start == end)
            return container[start] == value ? start : -1;

        int middle = (start + end) / 2;

        if (container[middle] == value)
            return middle;
        if (container[middle] > value)
            return binarySearch(start, middle, move(value));

        return binarySearch(middle + 1, end, move(value));
    }

    void removeFirstFound(T &&value)
    {
        int index = binarySearch(0, container.size() - 1, value);

        if (index < 0)
            return;

        container.erase(begin(container) + index);
    }

private:
    vector<T> container;
};

template <class T>
class MockExtendedVector : public ExtendedVector<T>
{
public:
    MockExtendedVector(const vector<T> &input) : ExtendedVector<T>(input) {}

    template <class... Ts>
    MockExtendedVector(Ts... elements) : ExtendedVector<T>(elements...) {}

    MOCK_METHOD(int, binarySearch, (int, int, T &&), (const override));
    MOCK_METHOD(size_t, count, (), (const override));
    MOCK_METHOD(void, removeFirstFound, (T &&), (const override));

    template <class... Ts>
    void mockInsert(Ts... values)
    {
        this->ExtendedVector<T>::insert(values...);
    }
};

TEST(ExtendedVectorTest, binarySearch)
{
    MockExtendedVector<int> mock(1, 2, 3, 4, 5);

    // This case must be studied better. If we repleace the lambda expression by Return(1) second EXPECT_CALL fails.
    // If the mock parameter qualifier does not match the real function parameter qualifiers, first EXPECT_CALL works,
    // but not the second.
    EXPECT_CALL(mock, binarySearch(0, 4, 2)).Times(1).WillOnce([&mock](int start, int end, int &&value)
                                                               { return mock.ExtendedVector::binarySearch(start, end, move(value)); });

    EXPECT_CALL(mock, binarySearch(0, 2, 2)).Times(1);

    mock.binarySearch(0, 4, 2);
}

TEST(ExtendedVectorTest, testAddRemoveAndCount)
{
    MockExtendedVector<int> mock(1, 2, 3, 4, 5);

    EXPECT_CALL(mock, count()).Times(5).WillOnce(Return(5)).WillOnce(Return(8)).WillRepeatedly(Return(6));
    EXPECT_CALL(mock, removeFirstFound(7));
    EXPECT_CALL(mock, removeFirstFound(6));
    EXPECT_CALL(mock, removeFirstFound(8));

    mock.count(); // Return 1 already satisfied.
    mock.removeFirstFound(7);
    mock.count(); // Does not affect WillRepeatedly(Return(6)).
    mock.mockInsert(6, 7, 8);
    mock.count(); // Return 8 satisfied.
    mock.removeFirstFound(6);
    mock.removeFirstFound(8);
    mock.count();
    mock.mockInsert();
    mock.count();
}