#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <gtest/gtest.h>
using std::string;
using std::vector;

class SimpleStringStack
{

public:
    SimpleStringStack() = default;
    void push(const string newValue)
    {
        stack.push_back(newValue);
    };

    /// It is not really a pop, neither a top as defined in std::stack type.
    string pop()
    {
        if (empty())
            return "";

        string top = stack[stack.size() - 1];
        stack.erase(stack.end() - 1);

        return top;
    };

    void clear()
    {
        stack.clear();
    }

    bool empty()
    {
        return stack.empty();
    }

    int count()
    {
        return stack.size();
    }

private:
    vector<string> stack;
};

class SimpleStringStackTest : public testing::Test
{
protected:
    void SetUp() override
    {
        stack.push("first");
        stack.push("second");
        stack.push("third");
    };

    void TearDown() override
    {
        stack.clear();
    }

    SimpleStringStack stack;
};

TEST_F(SimpleStringStackTest, testPop)
{
    EXPECT_EQ(stack.count(), 3);
    EXPECT_EQ(stack.pop(), "third");
    EXPECT_EQ(stack.count(), 2);
    EXPECT_EQ(stack.pop(), "second");
    EXPECT_EQ(stack.count(), 1);
    EXPECT_EQ(stack.pop(), "first");
    EXPECT_EQ(stack.count(), 0);
    ASSERT_TRUE(stack.empty());
}

TEST_F(SimpleStringStackTest, testPush)
{
    EXPECT_EQ(stack.count(), 3);
    stack.push("fourth");
    EXPECT_EQ(stack.count(), 4);

    stack.pop();
    stack.pop();
    stack.pop();
    stack.pop();
    ASSERT_TRUE(stack.empty());

    stack.push("some");
    ASSERT_FALSE(stack.empty());
}