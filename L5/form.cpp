//form.cpp
#include "form.h"
#include "mainwindow.h"
#include <QPixmap>
#include <QDir>
#include <QRandomGenerator>
#include <QFile>
#define IMAGE_MODE 2

Form::Form(Person* p, int idx, QWidget* parent)
    : QWidget(parent), person(p), index(idx), mainWindow(parent)
{
    setupUI();
    setImage();
    setWindowTitle("Пропуск");
    setFixedSize(400, 300);
}

void Form::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // ФИО
    QLabel* nameLabel = new QLabel(this);
    nameLabel->setText("<h2>" + person->fullName() + "</h2>");
    nameLabel->setAlignment(Qt::AlignCenter);

    // Дата рождения
    QLabel* dateLabel = new QLabel(this);
    dateLabel->setText("<b>Дата рождения:</b> " + person->dateDMY());
    dateLabel->setAlignment(Qt::AlignCenter);

    // Картинка
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(150, 150);
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: 1px solid gray; background-color: #f0f0f0;");

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton("Отмена", this);
    QPushButton* printBtn = new QPushButton("Печать", this);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(printBtn);

    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(imageLabel, 0, Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &Form::onCancel);
    connect(printBtn, &QPushButton::clicked, this, &Form::onPrint);
}

void Form::setImage()
{
    QPixmap pix;

#if IMAGE_MODE == 1
    // Статичная картинка
    if (QFile::exists("default.jpg")) {
        pix.load("default.jpg");
    } else {
        pix = QPixmap(150, 150);
        pix.fill(Qt::lightGray);
    }

#elif IMAGE_MODE == 2
    // Рандомная из папки img_res
    QDir dir("img_res");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    QStringList files = dir.entryList(filters, QDir::Files);

    if (!files.isEmpty()) {
        int r = QRandomGenerator::global()->bounded(files.size());
        pix.load("img_res/" + files[r]);
    } else {
        pix = QPixmap(150, 150);
        pix.fill(Qt::lightGray);
    }

#elif IMAGE_MODE == 3
    // Уникальная для каждого
    QString path = QString("img_res/%1.jpg").arg(index);
    if (QFile::exists(path)) {
        pix.load(path);
    } else {
        pix = QPixmap(150, 150);
        pix.fill(Qt::lightGray);
    }
#endif

    imageLabel->setPixmap(pix);
}

void Form::onCancel()
{
    // Отмена – закрывает форму
    close();
}

void Form::onPrint()
{
    // Печать – закрывает форму и убирает человека из списка
    MainWindow* mw = qobject_cast<MainWindow*>(mainWindow);
    if (mw) {
        mw->deleteRow(index);
    }
    close();
}
