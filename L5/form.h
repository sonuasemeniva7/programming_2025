//form.h
#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class Person;
class MainWindow;

class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(Person* p, int idx, QWidget* parent = nullptr);

private slots:
    void onCancel();
    void onPrint();

private:
    Person* person;
    int index;
    QWidget* mainWindow;
    QLabel* imageLabel;

    void setupUI();
    void setImage();
};

#endif
