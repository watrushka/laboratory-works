#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iostream>
using namespace std;

enum class RoomType
{
    SINGLE = 1,
    DOUBLE,
    LUX
};

class Room
{
private:
    int id;
    double price;
    RoomType type;
    bool occupied;

public:
    Room(int id, double price, RoomType type)
        : id(id), price(price), type(type), occupied(false) {}

    ~Room()
    {
        // не обязателен, но нужен по заданию
        // cout << "Room " << id << " destroyed\n";
    }

    int getId() const { return id; }
    double getPrice() const { return price; }
    bool isOccupied() const { return occupied; }
    RoomType getType() const { return type; }

    bool occupy()
    {
        if (occupied)
            return false;
        occupied = true;
        return true;
    }
};

class Client
{
private:
    string surname;
    int roomId;
    int days;

public:
    Client(string s, int roomId, int days)
        : surname(s), roomId(roomId), days(days) {}

    ~Client() {}

    string getSurname() const { return surname; }
    int getRoomId() const { return roomId; }
    int getDays() const { return days; }

    double calcCost(double price) const
    {
        return price * days;
    }
};

class Hotel
{
private:
    vector<Room> rooms;
    vector<Client> clients;

public:
    Hotel() {}
    ~Hotel() {}

    bool addRoom(const Room &r)
    {
        for (const auto &room : rooms)
        {
            if (room.getId() == r.getId())
            {
                cout << "Ошибка: комната с таким номером уже существует.\n";
                return false;
            }
        }
        rooms.push_back(r);
        return true;
    }

    bool registerClient(const string &surname, int roomId, int days)
    {
        for (auto &r : rooms)
        {
            if (r.getId() == roomId)
            {
                if (!r.isOccupied())
                {
                    r.occupy();
                    clients.emplace_back(surname, roomId, days);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }

    void showFreeRooms() const
    {
        cout << "\nСвободные номера:\n";
        for (const auto &r : rooms)
        {
            if (!r.isOccupied())
            {
                cout << "Комната " << r.getId()
                     << " — цена: " << r.getPrice() << " руб.\n";
            }
        }
    }

    bool showClientCost(const string &surname)
    {
        for (const auto &c : clients)
        {
            if (c.getSurname() == surname)
            {
                for (const auto &r : rooms)
                {
                    if (r.getId() == c.getRoomId())
                    {
                        cout << "Стоимость проживания: "
                             << c.calcCost(r.getPrice()) << " руб.\n";
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

class Menu
{
private:
    Hotel hotel;

public:
    void run()
    {
        while (true)
        {

            cout << "\n===== МЕНЮ =====\n"
                 << "1. Добавить номер\n"
                 << "2. Зарегистрировать клиента\n"
                 << "3. Показать свободные номера\n"
                 << "4. Узнать стоимость по фамилии\n"
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
                registerClientMenu();
            else if (choice == 3)
                hotel.showFreeRooms();
            else if (choice == 4)
                showCostMenu();
            else if (choice == 5)
                break;
        }
    }

private:
    void addRoomMenu()
    {
        int id;
        double price;
        int type;
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

        if (hotel.addRoom(Room(id, price, static_cast<RoomType>(type))))
            cout << "Комната добавлена.\n";
    }

    void registerClientMenu()
    {
        string surname;
        int id, days;
        string input;

        while (true)
        {
            cout << "Введите фамилию клиента (макс 50 символов, только буквы, '-' или '''): ";
            cin >> surname;

            if (surname.length() > 50)
            {
                cout << "Ошибка: фамилия слишком длинная.\n";
                continue;
            }

            bool valid = true;
            for (char c : surname)
            {
                if (!(isalpha(static_cast<unsigned char>(c)) || static_cast<unsigned char>(c) >= 128) && c != '-' && c != '\'')
                {
                    valid = false;
                    break;
                }
            }

            if (!valid)
            {
                cout << "Ошибка: используйте только буквы, '-' или '''.\n";
                continue;
            }

            break;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
            cout << "Введите количество дней: ";

            getline(cin, input);

            try
            {
                size_t pos;
                days = stoi(input, &pos);
                if (pos != input.size() || days < 1 || days > 365)
                    throw invalid_argument("неверный ввод");
                break;
            }
            catch (...)
            {
                cout << "Ошибка! Проживание ограничено 365 днями: \n";
            }
        }

        if (!hotel.registerClient(surname, id, days))
            cout << "Ошибка: номер занят или не существует.\n";
        else
            cout << "Клиент зарегистрирован.\n";
    }

    void showCostMenu()
    {
        string surname;
        cout << "Введите фамилию: ";
        cin >> surname;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!hotel.showClientCost(surname))
            cout << "Клиент не найден.\n";
    }
};

int main()
{
    setlocale(LC_ALL, "");
    Menu menu;
    menu.run();
    return 0;
}
