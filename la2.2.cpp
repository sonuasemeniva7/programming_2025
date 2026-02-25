#define PRODUCT_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Product {
private:
    std::string name;
    double price;
    int shelfLife;
    std::vector<std::string> composition; // состав

public:
    Product();
    Product(const std::string& n, double p, int life, const std::vector<std::string>& comp);
    Product(const Product& other);

    ~Product();

    Product& operator=(const Product& other);

    // геттеры 
    std::string getName() const;
    double getPrice() const;
    int getShelfLife() const;
    std::vector<std::string> getComposition() const;

    // сеттеры
    void setName(const std::string& n);
    void setPrice(double p);
    void setShelfLife(int life);
    void setComposition(const std::vector<std::string>& comp);

    // перегрузка
    Product operator+(const Product& other) const; // +
    Product& operator-=(const Product& other); // -=
    Product operator-(const Product& other) const;

    void print() const;

    std::vector<std::string> removeDuplicates(const std::vector<std::string>& vec) const;
};

//--------------------------------------------------------------------------------------------------------------------
#include <algorithm>
#include <sstream>

Product::Product() : name("Unknown"), price(0.0), shelfLife(0) {}

// параметризированный конструктор
Product::Product(const std::string& n, double p, int life, const std::vector<std::string>& comp)
    : name(n), price(p), shelfLife(life), composition(removeDuplicates(comp)) { 
}

Product::Product(const Product& other)
    : name(other.name), price(other.price), shelfLife(other.shelfLife), composition(other.composition) {
    std::cout << "Constructor copy called for " << name << std::endl;
}

// деструктор
Product::~Product() {

    if (!composition.empty()) {
        std::cout << "Destructor called for " << name << ". Clearing composition..." << std::endl;
        composition.clear(); // очищаем 
        composition.shrink_to_fit(); // освобождаем память
    }
    else {
        std::cout << "Destructor called for " << name << std::endl;
    }
}

Product& Product::operator=(const Product& other) {
    std::cout << "Assignment operator called for " << name << " to " << other.name << std::endl;

    if (this != &other) { // проверка на самоприсваивание
        name = other.name;
        price = other.price;
        shelfLife = other.shelfLife;
        composition = other.composition;
    }
    return *this;
}

// геттеры
std::string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getShelfLife() const { return shelfLife; }
std::vector<std::string> Product::getComposition() const { return composition; }

// сеттеры
void Product::setName(const std::string& n) { name = n; }
void Product::setPrice(double p) { price = p; }
void Product::setShelfLife(int life) { shelfLife = life; }
void Product::setComposition(const std::vector<std::string>& comp) {
    composition = removeDuplicates(comp); // Очищаем повторы при установке
}

// удаление повторов из вектора
std::vector<std::string> Product::removeDuplicates(const std::vector<std::string>& vec) const {
    std::vector<std::string> result;
    for (const auto& item : vec) {
        // проверка
        if (std::find(result.begin(), result.end(), item) == result.end()) {
            result.push_back(item);
        }
    }
    return result;
}

// +
Product Product::operator+(const Product& other) const {
    Product result;

    result.name = "смесь " + this->name + " и " + other.name;
    // - 10%
    result.price = (this->price + other.price) * 0.9;
    // минимум из сроков
    result.shelfLife = std::min(this->shelfLife, other.shelfLife);

    std::vector<std::string> thisClean = removeDuplicates(this->composition);
    std::vector<std::string> otherClean = removeDuplicates(other.composition);

    std::vector<std::string> newComposition = thisClean;
    for (const auto& item : otherClean) {
        if (std::find(newComposition.begin(), newComposition.end(), item) == newComposition.end()) {
            newComposition.push_back(item);
        }
    }

    result.composition = newComposition;

    return result;
}

// -= 
Product& Product::operator-=(const Product& other) {
    this->name = "выбор " + this->name + " без " + other.name;
    // - 10%
    this->price = this->price * 0.9;

    // срок1 - 2 дня
    this->shelfLife = this->shelfLife - 2;
    if (this->shelfLife < 0) this->shelfLife = 0;

    std::vector<std::string> thisClean = removeDuplicates(this->composition);
    std::vector<std::string> otherClean = removeDuplicates(other.composition);

    // состав1 без состава2 + консервант Т1000
    std::vector<std::string> newComposition;

    for (const auto& item : thisClean) {
        if (std::find(otherClean.begin(), otherClean.end(), item) == otherClean.end()) {
            newComposition.push_back(item);
        }
    }
    // добавляем консервант
    newComposition.push_back("консервант Т1000");
    this->composition = newComposition;
    return *this;
}

// -  -=
Product Product::operator-(const Product& other) const {
    Product result(*this);
    result -= other;
    return result;
}

void Product::print() const {
    std::cout << "Товар: " << name << std::endl;
    std::cout << "Цена: " << price << " руб." << std::endl;
    std::cout << "Срок годности: " << shelfLife << " дн." << std::endl;
    std::cout << "Состав: ";
    if (composition.empty()) {
        std::cout << "пусто";
    }
    else {
        for (size_t i = 0; i < composition.size(); ++i) {
            std::cout << composition[i];
            if (i < composition.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Product& product) {
    os << "[\"" << product.getName() << "\", "
        << product.getPrice() << " руб., "
        << product.getShelfLife() << " дн., (";

    std::vector<std::string> comp = product.getComposition();
    for (size_t i = 0; i < comp.size(); ++i) {
        os << comp[i];
        if (i < comp.size() - 1) os << ", ";
    }
    os << ")]";

    return os;
}
//---------------------------------------------------------------------------------------------------------------------------
#include <iostream>

int main() {
    std::vector<std::string> comp1 = { "а", "а", "в", "в", "в", "г" };
    Product product1("А", 100, 50, comp1);
    product1.print();
    std::cout << std::endl;

    // Создаем второй товар
    std::vector<std::string> comp2 = { "а", "м", "н", "н" };
    Product product2("Б", 100, 30, comp2);
    product2.print();
    std::cout << std::endl;

    // +
    std::cout << "--- + смесь + ---" << std::endl;
    Product product3 = product1 + product2;
    std::cout << "Результат смеси:" << std::endl;
    product3.print();
    std::cout << product3 << std::endl;


    //  - и -=
    std::cout << "--- - вычитание - ---" << std::endl;
    Product product4 = product1 - product2;
    std::cout << "Результат вычитания:" << std::endl;
    product4.print();
    std::cout << std::endl;

    //  -=
    std::cout << "---   -=    ---" << std::endl;
    Product product5 = product1; 
    std::cout << "Исходный товар для -=: " << product5 << std::endl;
    product5 -= product2;
    std::cout << "После product5 -= product2:" << std::endl;
    product5.print();
    std::cout << std::endl;

    std::cout << "--- копирование ---" << std::endl;
    Product product6 = product3; 
    std::cout << "Скопированный товар: " << product6 << std::endl;
    std::cout << std::endl;

    std::cout << "--- Оператор присваивания ---" << std::endl;
    Product product7;
    product7 = product4; 
    std::cout << "Присвоенный товар: " << product7 << std::endl;
    std::cout << std::endl;

    std::cout << "---- Программа завершена, вызываются деструкторы ---" << std::endl;

    return 0;
}
