#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// ===== Интерфейс стратегии скидок =====
class IDiscount
{
public:
    virtual double calculate(double price) const = 0;
    virtual ~IDiscount() {}
};

// ===== Реализация стратегии: без скидки =====
class NoDiscount : public IDiscount
{
public:
    double calculate(double price) const override
    {
        return price;
    }
};

// ===== Реализация стратегии: процентная скидка =====
class PercentageDiscount : public IDiscount
{
private:
    double percent;

public:
    PercentageDiscount(double p) : percent(p) {}

    double calculate(double price) const override
    {
        return price * (1 - percent / 100.0);
    }
};

// ===== Типы комнат =====
enum class RoomType
{
    SINGLE = 1,
    DOUBLE,
    LUX
};

string roomTypeToString(RoomType t)
{
    switch (t)
    {
    case RoomType::SINGLE:
        return "Single";
    case RoomType::DOUBLE:
        return "Double";
    case RoomType::LUX:
        return "Lux";
    }
    return "Unknown";
}

// ===== Класс Room =====
class Room
{
private:
    int id;
    double price;
    RoomType type;
    IDiscount *roomDiscount = nullptr; // скидка на конкретную комнату
    IDiscount *typeDiscount = nullptr; // скидка на тип комнаты

public:
    Room(int id, double price, RoomType type, IDiscount *disc)
        : id(id), price(price), type(type) {}

    int getId() const { return id; }
    RoomType getType() const { return type; }

    // Установка скидки на конкретную комнату
    void setRoomDiscount(IDiscount *d) { roomDiscount = d; }

    // Установка скидки на тип комнаты
    void setTypeDiscount(IDiscount *d) { typeDiscount = d; }

    double getPrice() const
    {
        double finalPrice = price;

        if (typeDiscount)
            finalPrice = typeDiscount->calculate(finalPrice);

        if (roomDiscount)
            finalPrice = roomDiscount->calculate(finalPrice);

        return finalPrice;
    }

    void show() const
    {
        cout << "Комната " << id
             << " — тип: " << roomTypeToString(type)
             << " — базовая цена: " << price
             << " — цена со скидкой: " << getPrice()
             << "\n";
    }
};

// ===== Класс Hotel =====
class Hotel
{
private:
    vector<Room> rooms;

public:
    bool addRoom(const Room &r)
    {
        for (const auto &room : rooms)
        {
            if (room.getId() == r.getId())
            {
                cout << "Ошибка: номер с таким ID уже существует.\n";
                return false;
            }
        }
        rooms.push_back(r);
        return true;
    }

    void showRooms() const
    {
        cout << "\nВсе номера:\n";
        for (const auto &r : rooms)
            r.show();
    }

    double averagePrice() const
    {
        if (rooms.empty())
            return 0;
        double sum = 0;
        for (const auto &r : rooms)
            sum += r.getPrice();
        return sum / rooms.size();
    }

    bool setDiscountForRoom(int id, IDiscount *disc)
    {
        for (auto &room : rooms)
        {
            if (room.getId() == id)
            {
                room.setRoomDiscount(disc);
                return true;
            }
        }
        return false;
    }

    void setDiscountForType(RoomType type, IDiscount *disc)
    {
        for (auto &room : rooms)
            if (room.getType() == type)
                room.setTypeDiscount(disc);
    }
};

// ===== Класс Menu =====
class Menu
{
private:
    Hotel hotel;
    NoDiscount noDisc;
    PercentageDiscount disc10{10};

public:
    void run()
    {
        while (true)
        {
            cout << "\n===== МЕНЮ =====\n"
                 << "1. Добавить номер\n"
                 << "2. Показать все номера\n"
                 << "3. Средняя стоимость\n"
                 << "4. Управление скидками\n"
                 << "5. Выход\n"
                 << "Ваш выбор: ";

            int choice;
            string input;

            while (true)
            {
                getline(cin, input);

                try
                {
                    size_t pos;
                    choice = stoi(input, &pos);
                    if (pos != input.size() || choice < 1 || choice > 5)
                        throw invalid_argument("неверный ввод");
                    break;
                }
                catch (...)
                {
                    cout << "Ошибка! Введите целое число от 1 до 5.\n";
                }
            }

            if (choice == 1)
                addRoomMenu();
            else if (choice == 2)
                hotel.showRooms();
            else if (choice == 3)
                cout << "Средняя цена: " << hotel.averagePrice() << endl;
            else if (choice == 4)
                manageDiscountMenu();
            else if (choice == 5)
                break;
        }
    }

private:
    void addRoomMenu()
    {
        int id, type;
        double price;
        int disc;
        string input;

        while (true)
        {
            cout << "Введите номер комнаты (1–999): ";

            getline(cin, input);

            try
            {
                size_t pos;
                id = stoi(input, &pos);
                if (pos != input.size() || id < 1 || id > 999)
                    throw invalid_argument("неверный ввод");
                break;
            }
            catch (...)
            {
                cout << "Ошибка! Введите целое число от 1 до 999.\n";
            }
        }

        while (true)
        {
            cout << "Введите цену: ";
            if (cin >> price && price < 100000 && price >= 1)
                break;
            cout << "Ошибка! Введите верное значение:\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (true)
        {
            cout << "Тип комнаты (1 — Single, 2 — Double, 3 — Lux): ";

            getline(cin, input);

            try
            {
                size_t pos;
                type = stoi(input, &pos);
                if (pos != input.size() || type < 1 || type > 3)
                    throw invalid_argument("неверный ввод");
                break;
            }
            catch (...)
            {
                cout << "Ошибка! Введите тип.\n";
            }
        }

        try
        {
            Room r(id, price, (RoomType)type, &noDisc);
            if (hotel.addRoom(r))
                cout << "Комната добавлена.\n";
        }
        catch (exception &e)
        {
            cout << "Ошибка: " << e.what() << endl;
        }
    }

    void manageDiscountMenu()
    {
        while (true)
        {
            cout << "\n=== УПРАВЛЕНИЕ СКИДКАМИ ===\n"
                 << "1. Назначить скидку конкретной комнате\n"
                 << "2. Назначить скидку типу комнат\n"
                 << "3. Убрать все скидки\n"
                 << "4. Назад\n"
                 << "Ваш выбор: ";

            string input;
            int ch;

            getline(cin, input);
            try
            {
                size_t pos;
                ch = stoi(input, &pos);
                if (pos != input.size() || ch < 1 || ch > 4)
                    throw invalid_argument("err");
            }
            catch (...)
            {
                cout << "Ошибка выбора.\n";
                continue;
            }

            if (ch == 4)
                return;

            double p = 0;
            IDiscount *strategy = nullptr;

            if (ch != 3)
            {
                while (true)
                {
                    cout << "Введите скидку (от 0 до 90) ";
                    if (cin >> p && p <= 90 && p >= 0)
                        break;
                    cout << "Ошибка! Введите верное значение:\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                strategy = (p == 0 ? (IDiscount *)&noDisc : new PercentageDiscount(p));
            }

            if (ch == 1)
            {
                int id;
                while (true)
                {
                    cout << "Введите номер комнаты (1–999): ";

                    getline(cin, input);

                    try
                    {
                        size_t pos;
                        id = stoi(input, &pos);
                        if (pos != input.size() || id < 1 || id > 999)
                            throw invalid_argument("неверный ввод");
                        break;
                    }
                    catch (...)
                    {
                        cout << "Ошибка! Введите целое число от 1 до 999.\n";
                    }
                }

                if (hotel.setDiscountForRoom(id, strategy))
                    cout << "Скидка применена.\n";
                else
                    cout << "Комната не найдена.\n";
            }
            else if (ch == 2)
            {
                int t;
                while (true)
                {
                    cout << "Тип комнаты (1 — Single, 2 — Double, 3 — Lux): ";

                    getline(cin, input);

                    try
                    {
                        size_t pos;
                        t = stoi(input, &pos);
                        if (pos != input.size() || t < 1 || t > 3)
                            throw invalid_argument("неверный ввод");
                        break;
                    }
                    catch (...)
                    {
                        cout << "Ошибка! Введите тип.\n";
                    }
                }

                if (t < 1 || t > 3)
                {
                    cout << "Неверный тип.\n";
                    continue;
                }

                hotel.setDiscountForType((RoomType)t, strategy);
                cout << "Скидка применена для всех комнат этого типа.\n";
            }
            else if (ch == 3)
            {
                hotel.setDiscountForType(RoomType::SINGLE, &noDisc);
                hotel.setDiscountForType(RoomType::DOUBLE, &noDisc);
                hotel.setDiscountForType(RoomType::LUX, &noDisc);
                cout << "Все скидки убраны.\n";
            }
        }
    }
};

// ===== main =====
int main()
{
    setlocale(LC_ALL, "");
    Menu m;
    m.run();
    return 0;
}
