//person.h
#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QWidget>

struct Date {
    int d, m, y;
};

class Person
{
public:
    QString name;
    QString surname;
    Date dob;

    virtual ~Person() {}

    virtual void Craft(int idx, QWidget* parent) = 0;
    virtual QString fullName() const = 0;

    // дд.мм.гггг
    QString dateDMY() const {
        return QString("%1.%2.%3")
            .arg(dob.d, 2, 10, QChar('0'))
            .arg(dob.m, 2, 10, QChar('0'))
            .arg(dob.y);
    }

    // мм-дд-гггг
    QString dateMDY() const {
        return QString("%1-%2-%3")
            .arg(dob.m, 2, 10, QChar('0'))
            .arg(dob.d, 2, 10, QChar('0'))
            .arg(dob.y);
    }
};

// русский
class RuP : public Person
{
public:
    QString patr; // "-" если нет отч

    void Craft(int idx, QWidget* parent) override;

    QString fullName() const override {
        QString s = surname + " " + name;
        if (patr != "-") s += " " + patr;
        return s;
    }
};

// американский
class AmP : public Person
{
public:
    QString name2; // "-" если нет вт имя

    void Craft(int idx, QWidget* parent) override;

    QString fullName() const override {
        QString s = name;
        if (name2 != "-") s += " " + name2;
        s += " " + surname;
        return s;
    }
};

#endif
