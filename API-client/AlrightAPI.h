#include "HttpClientInterface.h"
#include <memory>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <string>
#include <map>
#include <chrono>
#include <utility>
#include <algorithm>
using std::array;
using std::map;
using std::ostream;
using std::pair;
using std::remove_if;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;
using std::chrono::days;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::year_month_day;

enum DishCategory
{
    ENTRY,
    MAIN,
    SIDE
};

namespace
{
    static const vector<string> DISH_CATEGORY_TO_STRING = {"Primo", "Secondo", "Contorno"};
    static const map<string, DishCategory> DISH_CATEGORY_FROM_STRING = {{"Primo", ENTRY}, {"Secondo", MAIN}, {"Contorno", SIDE}};
}

enum OrderStatus
{
    PENDING,
    CANCELED,
    FINISHED
};

static const vector<string> ORDER_STATUS_TO_STRING = {"PENDING", "CANCELED", "FINISHED"};

struct Date
{
    unsigned day;
    unsigned month;
    int year;

    static const Date today()
    {
        using namespace std::literals;
        const time_point now{system_clock::now()};
        const year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
        return Date{static_cast<unsigned>(ymd.day()), static_cast<unsigned>(ymd.month()), static_cast<int>(ymd.year())};
    };

    static const Date yesterday()
    {
        using namespace std::literals;
        const time_point yesterday{system_clock::now() - 24h};
        const year_month_day ymd{std::chrono::floor<std::chrono::days>(yesterday)};
        return Date{static_cast<unsigned>(ymd.day()), static_cast<unsigned>(ymd.month()), static_cast<int>(ymd.year())};
    }

    static const Date tomorrow()
    {
        using namespace std::literals;
        const time_point tomorrow{system_clock::now() + 24h};
        const year_month_day ymd{std::chrono::floor<std::chrono::days>(tomorrow)};
        return Date{static_cast<unsigned>(ymd.day()), static_cast<unsigned>(ymd.month()), static_cast<int>(ymd.year())};
    };

    string toString() const
    {
        return std::to_string(day) + "-" + std::to_string(month) + "-" + std::to_string(year);
    }

    bool operator==(const Date &other) const
    {
        return day == other.day && month == other.month && year == other.year;
    }

    static Date fromString(string dateString)
    {
        string accumulator;
        int values[3];
        int position = 0;

        for (auto i = 0; i < dateString.length(); i++)
        {
            if (dateString[i] != '-')
                accumulator += dateString[i];
            else
            {
                accumulator.erase(remove(begin(accumulator), end(accumulator), '\"'), end(accumulator));
                values[position] = stoi(accumulator);
                accumulator = "";
                position++;
                if (position > 2)
                    break;
            }
        }

        if (!accumulator.empty() && position < 3)
            values[position] = stoi(accumulator);

        return Date(values[0], values[1], values[2]);
    }

    friend ostream &operator<<(ostream &os, const Date &other)
    {
        os << other.toString();
        return os;
    }
};

struct DishDTO
{
    string name;
    DishCategory dishCategory;
    string description;
    string id;
    string pictureUrl;

    static DishDTO fromJson(string jsonData)
    {
        bool isReadingKey = true;
        string key;
        string value;
        map<string, string> result;

        for (auto i = 0; i < jsonData.length(); i++)
        {
            if (jsonData[i] == ':')
            {
                isReadingKey = false;
                continue;
            }

            if (jsonData[i] == ' ')
                continue;
            if (jsonData[i] == ',')
            {
                key.erase(remove(begin(key), end(key), '\"'), end(key));
                value.erase(remove(begin(value), end(value), '\"'), end(value));
                result[key] = value;
                key = "";
                value = "";
                isReadingKey = true;
                continue;
            }

            if (isReadingKey)
                key += jsonData[i];
            else
                value += jsonData[i];
        }

        if (result.contains("id") && result.contains("name") && result.contains("category"))
        {
            DishDTO returnObject;
            returnObject.id = result["id"];
            returnObject.name = result["name"];
            returnObject.dishCategory = DISH_CATEGORY_FROM_STRING.at(result["category"]);
            returnObject.description = result["description"];
            returnObject.pictureUrl = result["pictureURL"];

            return returnObject;
        }

        return DishDTO();
    }
};

struct MenuDTO
{
    Date date;
    vector<DishDTO> dishes;

    static MenuDTO fromJson(string jsonData)
    {
        if (jsonData.empty())
            return MenuDTO();

        // Optimistic approach. For demo purposes only.
        string date{};
        for (int i = 7; i < jsonData.length(); i++)
        {
            if (jsonData[i] == '[')
                break;
            date += jsonData[i];
        }

        vector<string> entries;
        string newEntry = "";
        // The first caracter is '[' (not useful).
        for (auto i = date.length() + 1; i < jsonData.length(); i++)
        {
            if (jsonData[i] == '{')
                continue;
            if (jsonData[i] != '}')
                newEntry += jsonData[i];
            else
            {
                entries.push_back(newEntry);
                newEntry = "";
            }
        }

        vector<DishDTO> dishes;
        for (auto item : entries)
            dishes.push_back(DishDTO::fromJson(item));

        return MenuDTO(Date::fromString(date), dishes);
    }
};

struct ConsumerDTO
{
    string name;
    string id;
    string hasPriority;
};

struct Order
{
    string id;
    OrderStatus status;
    string consumerId;
    vector<string> dishIds;
};

class AlrightAPIClient
{

public:
    AlrightAPIClient(HttpClientInterface *concreteHttpClient) : httpClient(concreteHttpClient)
    {
    }

    MenuDTO getMenu(const Date &date = Date::today())
    {
        MenuDTO menu;
        Header header;
        HttpResponse response;
        httpClient->Get("menu/date/" + date.toString(), header, response);

        if (response.code == 200)
        {
            menu = MenuDTO::fromJson(response.strBody);
        }

        return menu;
    }

    MenuDTO getTomorrowMenu()
    {
        return getMenu(Date::tomorrow());
    }

    MenuDTO getYesterdayMenu()
    {
        return getMenu(Date::yesterday());
    }

    vector<DishDTO> getEntries(const Date &date = Date::today())
    {
        vector<DishDTO> dishes;
        Header header;
        HttpResponse response;
        httpClient->Get("menu/date/" + date.toString() + "/dishes/entries", header, response);
        return dishes;
    }

    vector<DishDTO> getMainCourses(const Date &date = Date::today())
    {
        vector<DishDTO> dishes;
        Header header;
        HttpResponse response;
        httpClient->Get("menu/date/" + date.toString() + "/dishes/maincourses", header, response);
        return dishes;
    }

    vector<DishDTO> getSideDishes(const Date &date = Date::today())
    {
        vector<DishDTO> dishes;
        Header header;
        HttpResponse response;
        httpClient->Get("menu/date/" + date.toString() + "/dishes/sidedishes", header, response);
        return dishes;
    }

    DishDTO getDish(string id)
    {
        DishDTO dish;
        Header header;
        HttpResponse response;
        httpClient->Get("dishes/id/" + id, header, response);
        return dish;
    }

    vector<string> getAlergenics(string dishId)
    {
        vector<string> alergenics{};
        Header header;
        HttpResponse response;
        httpClient->Get("dishes/id/" + dishId + "/alergenics", header, response);
        return alergenics;
    }

    // Clients orders requests
    void orderDishes(string consumerId, const vector<string> &disheIds)
    {
        Header header;
        HttpResponse response;

        string concatenedString = "";
        std::for_each(begin(disheIds), end(disheIds), [&concatenedString](string str)
                      { concatenedString += (str + ","); });
        concatenedString.erase(concatenedString.length() - 1, 1);
        string data = "[" + concatenedString + "]";

        httpClient->Post("order/consumer/id/" + consumerId + "/dishes", header, data, response);
    }

    // Self-service
    vector<Order> getOrders(Date date = Date::today())
    {
        Header header;
        HttpResponse response;

        httpClient->Get("order/date/" + date.toString(), header, response);

        return vector<Order>();
    }

    Order getOrder(string orderId)
    {
        Header header;
        HttpResponse response;

        httpClient->Get("order/id/" + orderId, header, response);

        return Order();
    }

    vector<Order> getPendingOrders()
    {
        Header header;
        HttpResponse response;

        httpClient->Get("order/date/" + Date::today().toString() + "/status/" + ORDER_STATUS_TO_STRING[OrderStatus::PENDING], header, response);

        return vector<Order>();
    }

private:
    HttpClientInterface *httpClient{};
};