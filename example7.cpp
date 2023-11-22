#include <exception>
#include <vector>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
using std::cout;
using std::exception;
using std::string;
using std::vector;
using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::Throw;

class InvalidTimeWindowException : public exception
{
public:
    InvalidTimeWindowException(const char *messageParam) : message(messageParam) {}

    virtual const char *what() const throw()
    {
        return message;
    }

private:
    const char *const message{};
};

class ClientException : public exception
{
public:
    ClientException(const char *messageParam) : message(messageParam) {}

    virtual const char *what() const throw()
    {
        return message;
    }

private:
    const char *const message{};
};

class TimeWindow
{

public:
    TimeWindow() = delete;
    TimeWindow(int startParam, int endParam)
    {
        if (startParam > endParam)
            throw InvalidTimeWindowException("Start is later than end");
    }

    void setStart(int startParam)
    {
        if (startParam > end)
            throw InvalidTimeWindowException("Start is later than end");

        start = startParam;
    }

    void setEnd(int endParam)
    {
        if (endParam < start)
            throw InvalidTimeWindowException("End is earlier than start");

        end = endParam;
    }

    int getStart() const { return start; }
    int getEnd() const { return end; }

private:
    int start{};
    int end{};
};

class Client
{

public:
    Client() = delete;
    Client(long long idParam, const string &nameParam, int timeWindowStart, int timeWindowEnd, double demandParam)
        : timeWindow(new TimeWindow(timeWindowStart, timeWindowEnd))
    {
        if (id < 0)
            throw ClientException("Invalid negative param");

        id = idParam;

        setName(nameParam);
        setDemand(demandParam);
    }

    Client(const Client &other)
    {
        id = other.id;
        setName(other.getName());
        setDemand(other.getDemand());
        setTimeWindow(other.getTimeWindow()->getStart(), other.getTimeWindow()->getEnd());
    }

    virtual ~Client()
    {
        if (timeWindow != nullptr)
            delete timeWindow;
    }

    virtual long long getId() const { return id; }
    virtual string getName() const { return name; }
    virtual const TimeWindow *const getTimeWindow() const { return timeWindow; }
    virtual double getDemand() const { return demand; }

    virtual void setName(const string &nameParam)
    {
        if (nameParam.length() == 0)
            throw ClientException("The client name cannot be empty");

        name = nameParam;
    }

    virtual void setDemand(const double &paramDemand)
    {
        if (paramDemand < 0)
            throw ClientException("Invalid negative demand");

        demand = paramDemand;
    }

    virtual void setTimeWindow(int timeWindowStart, int timeWindowEnd)
    {
        if (timeWindow != nullptr)
            delete timeWindow;

        timeWindow = new TimeWindow(timeWindowStart, timeWindowEnd);
    }

private:
    long long id;
    string name;
    TimeWindow *timeWindow{};
    double demand;
};

class MockClient : public Client
{
public:
    MockClient(long long id, string name, int start, int end, double demand) : Client(id, name, start, end, demand) {}
    MOCK_METHOD(long long, getId, (), (const, override));
    MOCK_METHOD(string, getName, (), (const, override));
    MOCK_METHOD(const TimeWindow *const, getTimeWindow, (), (const, override));
    MOCK_METHOD(double, getDemand, (), (const, override));

    MOCK_METHOD(void, setName, (const string &), (override));
    MOCK_METHOD(void, setDemand, (const double &), (override));
    MOCK_METHOD(void, setTimeWindow, (int, int), (override));

    void settersExceptionWrapper(auto lambdaExpression)
    {
        try
        {
            lambdaExpression();
        }
        catch (exception &exception)
        {
        }
    }
};

TEST(ClientTest, testGetters)
{
    MockClient mock(1, "test", 1, 5, 10);

    EXPECT_CALL(mock, getName()).WillOnce(Return("test"));
    EXPECT_CALL(mock, getId()).WillOnce(Return(1));
    EXPECT_CALL(mock, getDemand()).WillOnce(Return(10));

    mock.getName();
    mock.getId();
    mock.getDemand();
}

// Action to mock an exception throw.
ACTION(ThrowTimeWindowException)
{
    throw InvalidTimeWindowException("Start is later than end");
}

TEST(ClientTest, testSetTimeWindowException)
{
    MockClient mock(1, "test", 1, 5, 10);

    EXPECT_CALL(mock, setTimeWindow(6, 4))
        .Times(1)
        .WillRepeatedly(ThrowTimeWindowException());
    EXPECT_CALL(mock, setTimeWindow(2, 4))
        .Times(1);
    EXPECT_CALL(mock, setTimeWindow(7, 4))
        .Times(1)
        .WillRepeatedly(ThrowTimeWindowException());

    // You can not call setTimeWindow function directly.
    mock.settersExceptionWrapper([&mock]
                                 { mock.setTimeWindow(6, 4); });
    mock.settersExceptionWrapper([&mock]
                                 { mock.setTimeWindow(2, 4); });
    mock.settersExceptionWrapper([&mock]
                                 { mock.setTimeWindow(7, 4); });
}

// Action to mock an exception throw.
ACTION(ThrowNegativeDemand)
{
    throw ClientException("Invalid negative demand");
}

TEST(ClientTest, testSetNegativeDemand)
{
    MockClient mock(1, "test", 1, 5, 10);

    EXPECT_CALL(mock, setDemand(-2))
        .Times(1)
        .WillRepeatedly(ThrowNegativeDemand());

    EXPECT_CALL(mock, setDemand(14))
        .Times(1);

    mock.settersExceptionWrapper([&mock]
                                 { mock.setDemand(-2); });
    mock.settersExceptionWrapper([&mock]
                                 { mock.setDemand(14); });
}

// Action to mock an exception throw.
ACTION(ThrowEmptyName)
{
    throw ClientException("The client name cannot be empty");
}

TEST(ClientTest, testSetEmptyName)
{
    MockClient mock(1, "test", 1, 5, 10);

    EXPECT_CALL(mock, setName(""))
        .Times(1)
        .WillRepeatedly(ThrowEmptyName());
    EXPECT_CALL(mock, setName("some name"))
        .Times(1);

    mock.settersExceptionWrapper([&mock]
                                 { mock.setName(""); });
    mock.settersExceptionWrapper([&mock]
                                 { mock.setName("some name"); });
}