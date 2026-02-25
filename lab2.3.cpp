#ifndef INGREDIENT_HPP
#define INGREDIENT_HPP
#include <string>

struct Ingredient {
    std::string name;
    double volume;

    Ingredient() : name(""), volume(0.0) {}

    Ingredient(const std::string& n, double v) : name(n), volume(v) {}

    bool operator==(const Ingredient& other) const {
        return name == other.name;
    }
};

#endif 
//---------------------------------------------------------------------------------------------- 

#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <iostream>
#include <string>
#include <vector>


class Product {
private:
    std::string name;
    std::string description;
    double price;
    int shelfLife;
    std::string place;
    std::vector<Ingredient> ingredients;

    std::vector<Ingredient> removeDuplicates(const std::vector<Ingredient>& vec) const;

public:
    Product();
    Product(const std::string& n, const std::string& desc, double p,
        int life, const std::string& pl, const std::vector<Ingredient>& ing);
    Product(const Product& other);

    ~Product();

    // геттеры и сеттеры для всех полей
    std::string getName() const;
    void setName(const std::string& n);

    std::string getDescription() const;
    void setDescription(const std::string& desc);

    double getPrice() const;
    void setPrice(double p);

    int getShelfLife() const;
    void setShelfLife(int life);

    std::string getPlace() const;
    void setPlace(const std::string& pl);

    std::vector<Ingredient> getIngredients() const;
    void setIngredients(const std::vector<Ingredient>& ing);

    // перегрузка операторов
    Product operator+(const Product& other) const;
    Product operator-(const Product& other) const;
    Product& operator-=(const Product& other);

    void reduceShelfLife(int days);
    void printInfo() const;
    std::string getPlaceInfo() const;
};

std::ostream& operator<<(std::ostream& os, const Product& product);

#endif

//---------------------------------------------------------------------------------------------- 

#ifndef COMMISSION_HPP
#define COMMISSION_HPP

#include <string>
#include <vector>


class Commission {
private:
    std::string address; // адрес 
    std::vector<Ingredient> bannedIngredients; // список запрещенных ингредиентов
    int inspectionDate;   // срок проверки

public:
    Commission();
    Commission(const std::string& addr, const std::vector<Ingredient>& banned, int date);
    Commission(const Commission& other);

    // деструктор
    ~Commission();

    // геттеры и сеттеры 
    std::string getAddress() const;
    void setAddress(const std::string& addr);

    std::vector<Ingredient> getBannedIngredients() const;
    void setBannedIngredients(const std::vector<Ingredient>& banned);

    int getInspectionDate() const;
    void setInspectionDate(int date);

    bool checkProductLegality(const Product& product, int currentDate) const;

    void printInfo() const;
};

#endif

//---------------------------------------------------------------------------------------------- 

#include <algorithm>
#include <sstream>

Product::Product()
    : name("Неизвестный товар"), description("Нет описания"), price(0.0),
    shelfLife(0), place("Не указано") {
    std::cout << "Создан пустой товар" << std::endl;
}

Product::Product(const std::string& n, const std::string& desc, double p,
    int life, const std::string& pl, const std::vector<Ingredient>& ing) : name(n), description(desc), price(p), shelfLife(life), place(pl) {

    ingredients = removeDuplicates(ing);
    std::cout << "Создан товар: " << name << " (удалены повторы в ингредиентах)" << std::endl;
}

Product::Product(const Product& other)
    : name(other.name), description(other.description), price(other.price),
    shelfLife(other.shelfLife), place(other.place), ingredients(other.ingredients) {
    std::cout << "Скопирован товар: " << name << std::endl;
}

Product::~Product() {
    std::cout << "Уничтожается товар: " << name << std::endl;
    if (!ingredients.empty()) {
        ingredients.clear();
    }
}

std::vector<Ingredient> Product::removeDuplicates(const std::vector<Ingredient>& vec) const {
    std::vector<Ingredient> result;
    for (const auto& item : vec) {
        bool found = false;
        for (const auto& existing : result) {
            if (existing.name == item.name) {
                found = true;
                break;
            }
        }
        if (!found) {
            result.push_back(item);
        }
    }
    return result;
}

// геттеры и сеттеры
std::string Product::getName() const { return name; }
void Product::setName(const std::string& n) { name = n; }

std::string Product::getDescription() const { return description; }
void Product::setDescription(const std::string& desc) { description = desc; }

double Product::getPrice() const { return price; }
void Product::setPrice(double p) { price = p; }

int Product::getShelfLife() const { return shelfLife; }
void Product::setShelfLife(int life) { shelfLife = life; }

std::string Product::getPlace() const { return place; }
void Product::setPlace(const std::string& pl) { place = pl; }

std::vector<Ingredient> Product::getIngredients() const { return ingredients; }
void Product::setIngredients(const std::vector<Ingredient>& ing) {
    ingredients = removeDuplicates(ing);
}

// +
Product Product::operator+(const Product& other) const {
    Product result;

    result.name = "смесь " + this->name + " и " + other.name;
    result.description = "Смесь двух товаров";
    result.price = (this->price + other.price) * 0.9; // скидка 10%
    result.shelfLife = std::min(this->shelfLife, other.shelfLife);
    result.place = "Место смешивания";

    std::vector<Ingredient> thisClean = removeDuplicates(this->ingredients);
    std::vector<Ingredient> otherClean = removeDuplicates(other.ingredients);

    std::vector<Ingredient> newIngredients = thisClean;
    for (const auto& ing : otherClean) {
        bool found = false;
        for (const auto& existing : newIngredients) {
            if (existing.name == ing.name) {
                found = true;
                break;
            }
        }
        if (!found) {
            newIngredients.push_back(ing);
        }
    }
    result.ingredients = newIngredients;

    return result;
}

// -
Product Product::operator-(const Product& other) const {
    Product result(*this);
    result -= other;
    return result;
}

//  -=
Product& Product::operator-=(const Product& other) {
    this->name = "выбор " + this->name + " без " + other.name;
    this->description = "Товар после удаления ингредиентов";
    this->price = this->price * 0.9; // минус 10%
    this->shelfLife = this->shelfLife - 2; // минус 2 дня
    if (this->shelfLife < 0) this->shelfLife = 0;

    std::vector<Ingredient> thisClean = removeDuplicates(this->ingredients);
    std::vector<Ingredient> otherClean = removeDuplicates(other.ingredients);

    std::vector<Ingredient> newIngredients;
    for (const auto& ing : thisClean) {
        bool found = false;
        for (const auto& otherIng : otherClean) {
            if (ing.name == otherIng.name) {
                found = true;
                break;
            }
        }
        if (!found) {
            newIngredients.push_back(ing);
        }
    }

    newIngredients.push_back(Ingredient("консервант Т1000", 50.0));
    this->ingredients = newIngredients;

    return *this;
}

// уменьшение срока хранения
void Product::reduceShelfLife(int days) {
    if (days > 0) {
        shelfLife -= days;
        if (shelfLife < 0) shelfLife = 0;
        std::cout << "Срок годности товара \"" << name << "\" уменьшен на "
            << days << " дней. Теперь: " << shelfLife << std::endl;
    }
}

std::string Product::getPlaceInfo() const {
    return "Товар \"" + name + "\" можно получить по адресу: " + place;
}

void Product::printInfo() const {
    std::cout << "--- Информация о товаре ---" << std::endl;
    std::cout << "Название: " << name << std::endl;
    std::cout << "Описание: " << description << std::endl;
    std::cout << "Цена: " << price << " руб." << std::endl;
    std::cout << "Срок годности: " << shelfLife << " дн." << std::endl;
    std::cout << "Место получения: " << place << std::endl;
    std::cout << "Ингредиенты:" << std::endl;

    if (ingredients.empty()) {
        std::cout << "  Нет ингредиентов" << std::endl;
    }
    else {
        for (size_t i = 0; i < ingredients.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << ingredients[i].name
                << " (объем: " << ingredients[i].volume << ")" << std::endl;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Product& product) {
    os << "Товар \"" << product.getName() << "\", цена: " << product.getPrice()
        << " руб., срок: " << product.getShelfLife() << " дн.";
    return os;
}

//---------------------------------------------------------------------------------------------- 

#include <iostream>
#include <algorithm>

Commission::Commission() : address("Неизвестный адрес"), inspectionDate(0) {
    std::cout << "Комиссия создана (пустой конструктор)" << std::endl;
}

Commission::Commission(const std::string& addr, const std::vector<Ingredient>& banned, int date)
    : address(addr), bannedIngredients(banned), inspectionDate(date) {
    std::cout << "Комиссия создана по адресу: " << address << std::endl;
}

Commission::Commission(const Commission& other)
    : address(other.address), bannedIngredients(other.bannedIngredients), inspectionDate(other.inspectionDate) {
    std::cout << "Комиссия скопирована (конструктор копирования)" << std::endl;
}


// деструктор
Commission::~Commission() {
    std::cout << "Комиссия по адресу " << address << " уничтожена" << std::endl;
}

// геттеры и сеттеры 
std::string Commission::getAddress() const {
    return address;
}

void Commission::setAddress(const std::string& addr) {
    address = addr;
}

std::vector<Ingredient> Commission::getBannedIngredients() const {
    return bannedIngredients;
}

void Commission::setBannedIngredients(const std::vector<Ingredient>& banned) {
    bannedIngredients = banned;
}

int Commission::getInspectionDate() const {
    return inspectionDate;
}

void Commission::setInspectionDate(int date) {
    inspectionDate = date;
}


bool Commission::checkProductLegality(const Product& product, int currentDate) const {
    std::cout << "Комиссия проверяет товар \"" << product.getName() << "\"..." << std::endl;

    // проверка 1: на срок годности
    if (product.getShelfLife() < currentDate) {
        std::cout << "  Ошибка: срок годности истек! ("
            << product.getShelfLife() << " < " << currentDate << ")" << std::endl;
        return false;
    }

    // проверка 2: нет ли запрещенных ингредиентов
    std::vector<Ingredient> productIngredients = product.getIngredients();

    for (const auto& banned : bannedIngredients) {
        for (const auto& ingredient : productIngredients) {
            if (ingredient.name == banned.name) {
                std::cout << "  Ошибка: найден запрещенный ингредиент \""
                    << banned.name << "\"!" << std::endl;
                return false;
            }
        }
    }

    std::cout << "  Товар прошел проверку!" << std::endl;
    return true;
}

void Commission::printInfo() const {
    std::cout << "--- Информация о комиссии ---" << std::endl;
    std::cout << "Адрес: " << address << std::endl;
    std::cout << "Срок проверки: " << inspectionDate << std::endl;
    std::cout << "Запрещенные ингредиенты: ";

    if (bannedIngredients.empty()) {
        std::cout << "нет" << std::endl;
    }
    else {
        for (size_t i = 0; i < bannedIngredients.size(); ++i) {
            std::cout << bannedIngredients[i].name;
            if (i < bannedIngredients.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

//---------------------------------------------------------------------------------------------- 

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {


    std::vector<Ingredient> ing1;
    ing1.push_back(Ingredient("мука", 500.0));
    ing1.push_back(Ingredient("сахар", 200.0));
    ing1.push_back(Ingredient("сахар", 150.0));
    ing1.push_back(Ingredient("яйца", 100.0));
    ing1.push_back(Ingredient("масло", 50.0));
    ing1.push_back(Ingredient("масло", 30.0));  

    Product product1("Торт Наполеон", "Торт с кремом", 850.0,
        14, "ул. Ленина,134, магазин \"Вкусно\"", ing1);

    std::cout << "\nТовар 1 :" << std::endl;
    product1.printInfo();
    std::cout << std::endl;

    std::vector<Ingredient> ing2;
    ing2.push_back(Ingredient("мука", 300.0));
    ing2.push_back(Ingredient("сахар", 150.0));
    ing2.push_back(Ingredient("какао", 50.0));
    ing2.push_back(Ingredient("какао", 40.0));  
    ing2.push_back(Ingredient("молоко", 200.0));
    ing2.push_back(Ingredient("молоко", 180.0)); 

    Product product2("Шоколадный кекс", "Кекс с шоколадом", 450.0,
        12, "ул. Пушкина, 5, пекарня \"Сладкоежка\"", ing2);

    std::cout << "\nТовар 2 (после автоматического удаления повторов):" << std::endl;
    product2.printInfo();
    std::cout << std::endl;

    std::cout << "--- Оператор + (смесь товаров) ---" << std::endl;
    std::cout << "При смешивании повторы удаляются и внутри, и между товарами" << std::endl;
    Product product3 = product1 + product2;
    product3.printInfo();
    std::cout << std::endl;

    std::cout << "--- Оператор - (вычитание) ---" << std::endl;
    Product product4 = product1 - product2;
    product4.printInfo();
    std::cout << std::endl;

    std::cout << "--- Уменьшение срока годности ---" << std::endl;
    std::cout << "До: " << product1 << std::endl;
    product1.reduceShelfLife(3);
    std::cout << "После: " << product1 << std::endl;
    std::cout << std::endl;

    std::vector<Ingredient> banned;
    banned.push_back(Ingredient("пальмовое масло", 0));
    banned.push_back(Ingredient("какао", 0));
    banned.push_back(Ingredient("консервант Т1000", 0));

    Commission commission("г. Москва, ул. Космическая, 59", banned, 15);
    std::cout << "--- Создана комиссия ---" << std::endl;
    commission.printInfo();
    std::cout << std::endl;

    std::cout << "--- Проверка товаров комиссией ---" << std::endl;
    std::cout << "Текущая дата для проверки: 12" << std::endl;
    std::cout << std::endl;

    std::cout << "Проверка товара 1 (Торт Наполеон):" << std::endl;
    commission.checkProductLegality(product1, 12);
    std::cout << std::endl;

    std::cout << "Проверка товара 2 (Шоколадный кекс - есть какао):" << std::endl;
    commission.checkProductLegality(product2, 12);
    std::cout << std::endl;

    std::cout << "Проверка товара после вычитания (с консервантом Т1000):" << std::endl;
    commission.checkProductLegality(product4, 12);
    std::cout << std::endl;

    std::cout << "Проверка с истекшим сроком (дата 20 > срока 14):" << std::endl;
    commission.checkProductLegality(product1, 20);
    std::cout << std::endl;

    std::cout << "---------------- ПРОГРАММА ЗАВЕРШЕНА ------------" << std::endl;

    return 0;
}
