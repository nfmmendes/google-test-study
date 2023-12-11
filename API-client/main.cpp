#include "HttpClientInterface.h"
#include "AlRightAPI.h"
#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
using std::make_shared;
using ::testing::_;
using ::testing::Return;

#define RESPONSE_PARAM HttpResponse &
#define COMMON_PARAM_TYPE_LIST const std::string &, const Header &

class MockHttpClient : public HttpClientInterface
{
public:
    MOCK_METHOD(const bool, Head, (COMMON_PARAM_TYPE_LIST, RESPONSE_PARAM), (const override));
    MOCK_METHOD(const bool, Get, (COMMON_PARAM_TYPE_LIST, RESPONSE_PARAM), (const override));
    MOCK_METHOD(const bool, Del, (COMMON_PARAM_TYPE_LIST, RESPONSE_PARAM), (const override));
    MOCK_METHOD(const bool, Post, (COMMON_PARAM_TYPE_LIST, const std::string &, RESPONSE_PARAM), (const override));
    MOCK_METHOD(const bool, Put, (COMMON_PARAM_TYPE_LIST, const std::string &, RESPONSE_PARAM), (const override));
    MOCK_METHOD(const bool, Put, (COMMON_PARAM_TYPE_LIST, const ByteBuffer &, RESPONSE_PARAM), (const override));

    void activateErrorX(int errorCode)
    {
        ON_CALL(*this, Get).WillByDefault([this, errorCode](const string &url, const Header &header, HttpResponse &response)
                                          { response.code = errorCode; return false; });
    }
};

TEST(HttpClient, getCalls)
{
    MockHttpClient mock{};
    AlrightAPIClient api(&mock);

    EXPECT_CALL(mock, Get(_, _, _)).Times(6);

    api.getMenu();
    api.getTomorrowMenu();
    api.getYesterdayMenu();
    api.getEntries();
    api.getMainCourses();
    api.getSideDishes();
}

TEST(HttpClient, getMenuCalls)
{
    MockHttpClient mock{};
    AlrightAPIClient api(&mock);

    EXPECT_CALL(mock, Get("menu/date/" + Date::today().toString(), _, _)).Times(1);
    EXPECT_CALL(mock, Get("menu/date/" + Date::tomorrow().toString(), _, _)).Times(1);
    EXPECT_CALL(mock, Get("menu/date/" + Date::yesterday().toString(), _, _)).Times(1);

    api.getMenu();
    api.getTomorrowMenu();
    api.getYesterdayMenu();
}

TEST(HttpClient, getDishesByCategory)
{
    MockHttpClient mock{};
    AlrightAPIClient api(&mock);

    EXPECT_CALL(mock, Get("menu/date/" + Date::today().toString() + "/dishes/entries", _, _)).Times(1);
    EXPECT_CALL(mock, Get("menu/date/" + Date::today().toString() + "/dishes/maincourses", _, _)).Times(1);
    EXPECT_CALL(mock, Get("menu/date/" + Date::tomorrow().toString() + "/dishes/maincourses", _, _)).Times(1);
    EXPECT_CALL(mock, Get("menu/date/" + Date::today().toString() + "/dishes/sidedishes", _, _)).Times(1);

    api.getEntries();
    api.getMainCourses(Date::today());
    api.getMainCourses(Date::tomorrow());
    api.getSideDishes();
}

TEST(HttpClient, getAlergenics)
{
    MockHttpClient mock{};
    AlrightAPIClient api(&mock);
    DishDTO dish{"bread", DishCategory::ENTRY, "A big bread", "xyzBread", "local/bread.png"};

    EXPECT_CALL(mock, Get("dishes/id/" + dish.id + "/alergenics", _, _)).Times(1);

    api.getAlergenics(dish.id);
}

TEST(HttpClient, errorCodes)
{
    MockHttpClient mock{};
    mock.activateErrorX(404);
    AlrightAPIClient api(&mock);

    EXPECT_CALL(mock, Get(_, _, _)).Times(3).WillRepeatedly(Return(false));

    auto menu = api.getMenu();
    api.getEntries();
    api.getSideDishes();

    EXPECT_EQ(menu.dishes.size(), 0);
}