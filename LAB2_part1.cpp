#include <iostream>
#include <string>
#include <map>
#include <vector>

class Order
{ // хранит данные заказ
public:
    std::string dishName; // какое блюдо заказано
    int tableId;          // от какого стола заказ

    Order(std::string dish, int table)
        : dishName(dish), tableId(table) {}
};

class Kitchen
{ // содержит ингредиенты и готовит блюда
private:
    std::map<std::string, int> ingredients; // ингредиенты на складе

    std::map<std::string, int> dishes; // блюда и время приготовления

    std::map<std::string, std::map<std::string, int>> recipe; // блюда и необходимые ингредиенты

public:
    Kitchen()
    {
        // Инициализируем ингредиенты
        ingredients["meat"] = 5;
        ingredients["salt"] = 10;
        ingredients["potato"] = 8;

        // Время приготовления
        dishes["Steak"] = 15;
        dishes["Fries"] = 10;

        // Рецепты
        recipe["Steak"] = {{"meat", 1}, {"salt", 1}};
        recipe["Fries"] = {{"potato", 2}, {"salt", 1}};
    }

    bool canCook(const std::string &dish)
    { // Проверяем, можно ли приготовить заказ
        if (!recipe.count(dish))
            return false;
        for (auto &ing : recipe[dish])
        {
            if (ingredients[ing.first] < ing.second)
                return false;
        }
        return true;
    }

    int cook(const std::string &dish)
    { // Готовим блюдо: уменьшаем ингредиенты и возвращаем время
        for (auto &ing : recipe[dish])
        {
            ingredients[ing.first] -= ing.second;
        }
        return dishes[dish];
    }
};

class Restaurant
{ // Singleton, управляет заказами и кухней
private:
    static Restaurant *instance; // статическое поле Singleton
    Kitchen kitchen;             // кухня — контейнеризируемый класс

    Restaurant() {} // приватный конструктор

public:
    static Restaurant *getInstance()
    {
        if (!instance)
            instance = new Restaurant();
        return instance;
    }

    void processOrder(const Order &order)
    { // Метод обработки заказа
        std::cout << "Стол #" << order.tableId
                  << " заказал: " << order.dishName << "\n";

        if (!kitchen.canCook(order.dishName))
        {
            std::cout << "Блюдо нельзя приготовить, не хватает ингредиентов\n";
            return;
        }

        int time = kitchen.cook(order.dishName);
        std::cout << "Блюдо будет готово через " << time << " минут\n";
    }
};

Restaurant *Restaurant::instance = nullptr;

int main()
{
    Restaurant *r = Restaurant::getInstance();

    Order o1("Steak", 3);
    Order o2("Fries", 1);
    Order o3("Steak", 2);
    Order o4("Steak", 5);
    Order o5("Steak", 6);
    Order o6("Steak", 7);
    Order o7("Steak", 7); // на него уже не хватит мяса

    r->processOrder(o1);
    r->processOrder(o2);
    r->processOrder(o3);
    r->processOrder(o4);
    r->processOrder(o5);
    r->processOrder(o6);
    r->processOrder(o7);

    return 0;
}
